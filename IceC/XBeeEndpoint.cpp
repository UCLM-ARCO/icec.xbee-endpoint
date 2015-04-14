/* -*- mode: c; coding: utf-8 -*- */

#include <IceC/IceUtil.h>
#include <IceC/XBeeEndpoint.h>
#include <SoftwareSerial.h>

static IcePlugin_PluginListItem item;
static IcePlugin_EndpointObject endpoint;
static XBee_EndpointOptions ep_options;

static EasyXBee xbee;
static SoftwareSerial logger(2, 3); // Rx, Tx

void
logger_log(const char* s) {
    static bool logger_initialized = false;
    if (not logger_initialized)
	logger.begin(9600);

    logger.write(s);
    logger.flush();
}

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
    logger_log(__func__);
    logger_log("\n");
    _STOP_HERE_;

/*     uint32_t size = 0; */

/*     trace(); */

/*     if (type != Ice_EndpointTypeXBee) */
/* 	return false; */

/*     /\* first: read header, and get message size *\/ */
/*     self->size = XBee_read((byte*)self->data, 14); */
/*     size = (uint32_t)*(self->data + 10); */

/*     /\* then: read as size specifies *\/ */
/*     size -= 14; */
/*     assert(size <= MAX_MESSAGE_SIZE && "Message too big!"); */

/*     self->size += XBee_read((byte*)(self->data + 14), size); */
/*     return true; */
}

bool
IceC_XBee_ObjectAdapter_activate(Ice_ObjectAdapterPtr self) {

    logger_log(__func__);
    logger_log("\n");
    _STOP_HERE_;

/*     trace(); */

/*     if (self->epinfo.type != Ice_EndpointTypeXBee) */
/* 	return false; */

/*     self->connection.fd = XBee_init(); */
/*     return true; */
}

bool
IceC_XBee_EndpointInfo_init(Ice_EndpointInfoPtr self,
			    Ice_EndpointType type,
			    const char* endpoint) {
    ObjectPtr _ep_options_ptr;

    trace();
    if (type != Ice_EndpointTypeXBee)
    	return false;

    /* avoid annoying compiler messages */
    _ep_options_ptr = (ObjectPtr)&ep_options;
    Ptr_check(_ep_options_ptr);

    assert(! ep_options.used && "Only one instance of this endpoint allowed!");

    self->type = type;
    self->datagram = true;
    self->options = &ep_options;
    ep_options.used = true;

    IceUtil_getStringParam(endpoint, ep_options.panid, 'p');
    IceUtil_getStringParam(endpoint, ep_options.nodeid, 'n');
    Object_init((ObjectPtr)self);

    return true;
}

bool
IceC_XBee_ObjectPrx_connect(Ice_ObjectPrxPtr self) {
    trace();
    logger_log(__func__);
    logger_log("\n");

    if (self->epinfo.type != Ice_EndpointTypeXBee)
	return false;

    XBee_EndpointOptionsPtr opts;
    opts = (XBee_EndpointOptionsPtr)self->epinfo.options;

    // join a pan
    logger_log(" - try to set pan id\n");
    bool result = xbee.set_pan_id((byte*)opts->panid, strlen(opts->panid));
    // assert(result && "set panid failed");
    if (not result) {
	logger_log(" - error setting panid\n");
	_STOP_HERE_;
    }

    // wait until successfully connected to pan
    xbee.wait_until_ready();

    // resolve the name of destination node
    result = xbee.lookup((byte*)opts->nodeid, strlen(opts->nodeid),
			 &opts->node_addr64, &opts->node_addr16);
    // assert(result && "lookup node failed");
    if (not result) {
	logger_log(" - error looking node\n");
	_STOP_HERE_;
    }

    logger_log(" - connect done\n");
    return true;
}

bool
IceC_XBee_ObjectPrx_init(Ice_ObjectPrxPtr self,
			 Ice_EndpointType type,
			 const char* strprx) {

    trace();
    if (type != Ice_EndpointTypeXBee)
	return false;

    Ice_Connection_init(&(self->connection), type);
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

    logger_log(__func__);
    logger_log("\n");
    _STOP_HERE_;

/*     trace(); */
/*     if (self->type != Ice_EndpointTypeXBee) */
/* 	return false; */

/*     *size = XBee_write(data, *size); */
/*     return true; */
}

bool
IceC_XBee_Connection_dataReady(Ice_ConnectionPtr self,
			       bool* ready) {

    logger_log(__func__);
    logger_log("\n");
    _STOP_HERE_;

/*     trace(); */
/*     if (self->type != Ice_EndpointTypeXBee) */
/* 	return false; */

/*     *ready = XBee_available(); */
/*     return true; */
}

void
IceC_XBeeEndpoint_init(Ice_CommunicatorPtr ic) {
    trace();

    /* Note: is important to call IcePlugin_registerEndpoint after
       Ice_Communicator_initialize, check here that communicator is
       initialized. */
    Ptr_check(ic);

    xbee.begin(XBEE_SPEED);
    logger_log("xbee begin done\n");

    ep_options.used = false;
    Object_init((ObjectPtr)&ep_options);

    IcePlugin_EndpointObject_init(&endpoint);
    IcePlugin_PluginListItem_init(&item, (ObjectPtr)&endpoint);
    IcePlugin_registerEndpoint(&item);

    endpoint.getProtocolType = &IceC_XBee_getProtocolType;
    endpoint.InputStream_init = &IceC_XBee_InputStream_init;
    endpoint.ObjectAdapter_activate = &IceC_XBee_ObjectAdapter_activate;
    endpoint.EndpointInfo_init = &IceC_XBee_EndpointInfo_init;
    endpoint.ObjectPrx_init = &IceC_XBee_ObjectPrx_init;
    endpoint.ObjectPrx_connect = &IceC_XBee_ObjectPrx_connect;
    endpoint.Connection_accept = NULL;
    endpoint.Connection_send = &IceC_XBee_Connection_send;
    endpoint.Connection_close = NULL;
    endpoint.Connection_dataReady = &IceC_XBee_Connection_dataReady;
}
