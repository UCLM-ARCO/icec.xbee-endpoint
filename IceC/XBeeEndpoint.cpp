/* -*- mode: c++; coding: utf-8 -*- */

#include <IceC/IceUtil.h>
#include <XBeeEndpoint.h>

#ifndef MAX_XBEE_ENDPOINT_INSTANCES
#define MAX_XBEE_ENDPOINT_INSTANCES 3
#endif

static IcePlugin_PluginListItem item;
static IcePlugin_EndpointObject endpoint;
static XBee_EndpointOptions ep_options[MAX_XBEE_ENDPOINT_INSTANCES];

static EasyXBee xbee;

bool
IceC_XBee_getProtocolType(const char* proto, Ice_EndpointType* result) {
    trace();

    if (!streq(proto, "xbee"))
    	return false;

    *result = Ice_EndpointTypeXBee;
    return true;
}

bool
IceC_XBee_InputStream_init(Ice_InputStreamPtr self,
			   int fd,
			   Ice_EndpointType type) {

    trace();
    if (type != Ice_EndpointTypeXBee)
	return false;

    byte* data;
    byte size;
    if (!xbee.receive(&data, &size))
	return false;

    assert (size <= MAX_MESSAGE_SIZE && "Too big message");
    memcpy(self->data, data, size);
    self->size = size;
    return true;
}

bool
IceC_XBee_ObjectAdapter_activate(Ice_ObjectAdapterPtr self) {

    trace();
    if (self->epinfo.type != Ice_EndpointTypeXBee)
	return false;

    XBee_EndpointOptionsPtr opts;
    opts = (XBee_EndpointOptionsPtr)self->epinfo.options;
    Ptr_check(opts);

    bool result;
    result = xbee.set_name((byte*)opts->nodeid, strlen(opts->nodeid));
    assert(result && "set name failed");

    result = xbee.set_pan_id((byte*)opts->panid, strlen(opts->panid));
    assert(result && "set panid failed");
    xbee.wait_until_ready();

    return true;
}

bool
IceC_XBee_EndpointInfo_init(Ice_EndpointInfoPtr self,
			    Ice_EndpointType type,
			    const char* endpoint) {
    trace();
    if (type != Ice_EndpointTypeXBee)
    	return false;


    XBee_EndpointOptions* options = NULL;
    byte i;
    for (i=0; i<MAX_XBEE_ENDPOINT_INSTANCES; i++) {
    	if (ep_options[i].used)
	    continue;
    	options = &ep_options[i];
	break;
    }

    assert(options != NULL && "Max number of endpoints reached!");
    Ptr_check(options);

    self->type = type;
    self->datagram = true;
    self->options = options;
    options->used = true;

    IceUtil_getStringParam(endpoint, options->panid, 'p');
    IceUtil_getStringParam(endpoint, options->nodeid, 'n');
    Object_init((ObjectPtr)self);

    return true;
}

bool
IceC_XBee_EndpointInfo_writeToOutputStream(Ice_EndpointInfoPtr self,
					   Ice_OutputStreamPtr os) {
    trace();

    if (self->type != Ice_EndpointTypeXBee)
	return false;

    XBee_EndpointOptions* options = (XBee_EndpointOptions*)self->options;

    Ice_OutputStream_startWriteEncaps(os);
    Ice_OutputStream_writeString(os, options->panid);
    Ice_OutputStream_writeString(os, options->nodeid);
    Ice_OutputStream_endWriteEncaps(os);

    return true;
}

bool
IceC_XBee_ObjectPrx_connect(Ice_ObjectPrxPtr self) {
    trace();

    if (self->epinfo.type != Ice_EndpointTypeXBee)
	return false;

    XBee_EndpointOptionsPtr opts;
    opts = (XBee_EndpointOptionsPtr)self->epinfo.options;

    bool result = xbee.set_pan_id((byte*)opts->panid, strlen(opts->panid));
    assert(result && "set panid failed");
    xbee.wait_until_ready();

    result = xbee.lookup((byte*)opts->nodeid, strlen(opts->nodeid),
			 &opts->node_addr64, &opts->node_addr16);
    assert(result && "lookup node failed");
    self->connection.fd = 0;
    return true;
}

bool
IceC_XBee_ObjectPrx_init(Ice_ObjectPrxPtr self,
			 Ice_EndpointType type,
			 const char* strprx) {

    trace();
    if (type != Ice_EndpointTypeXBee)
	return false;

    Ice_Connection_init(&(self->connection), &(self->epinfo));
    Ice_OutputStream_init(&(self->stream));

    IceC_XBee_EndpointInfo_init(&(self->epinfo), type, strprx);

    Object_init((ObjectPtr)self);
    return true;
}

bool
IceC_XBee_Connection_send(Ice_ConnectionPtr self,
			  int fd,
			  byte* data,
			  uint16_t* size) {

    trace();
    if (self->epinfo->type != Ice_EndpointTypeXBee)
	return false;

    XBee_EndpointOptionsPtr opts;
    opts = (XBee_EndpointOptionsPtr)self->epinfo->options;

    assert(*size <= 255 && "MTU excedded");

    xbee.send(opts->node_addr64, 0, data, (uint8_t)*size);
    return true;
}

bool
IceC_XBee_Connection_dataReady(Ice_ConnectionPtr self,
			       bool* ready) {

    trace();
    if (self->epinfo->type != Ice_EndpointTypeXBee)
	return false;

    *ready = xbee.available();
    return true;
}

void
IceC_XBeeEndpoint_init(Ice_CommunicatorPtr ic) {
    trace();

    /* Note: is important to call IcePlugin_registerEndpoint after
       Ice_Communicator_initialize, check here that communicator is
       initialized. */
    Ptr_check(ic);

    xbee.begin(XBEE_SPEED);

    for (byte i=0; i<MAX_XBEE_ENDPOINT_INSTANCES; i++) {
	ep_options[i].used = false;
	Object_init((ObjectPtr)&ep_options[i]);
    }

    IcePlugin_EndpointObject_init(&endpoint);
    IcePlugin_PluginListItem_init(&item, (ObjectPtr)&endpoint);
    IcePlugin_registerEndpoint(&item);

    endpoint.getProtocolType = &IceC_XBee_getProtocolType;
    endpoint.InputStream_init = &IceC_XBee_InputStream_init;
    endpoint.ObjectAdapter_activate = &IceC_XBee_ObjectAdapter_activate;
    endpoint.EndpointInfo_init = &IceC_XBee_EndpointInfo_init;
    endpoint.EndpointInfo_writeToOutputStream = &IceC_XBee_EndpointInfo_writeToOutputStream;
    endpoint.ObjectPrx_init = &IceC_XBee_ObjectPrx_init;
    endpoint.ObjectPrx_connect = &IceC_XBee_ObjectPrx_connect;
    endpoint.Connection_accept = NULL;
    endpoint.Connection_send = &IceC_XBee_Connection_send;
    endpoint.Connection_close = NULL;
    endpoint.Connection_dataReady = &IceC_XBee_Connection_dataReady;
}
