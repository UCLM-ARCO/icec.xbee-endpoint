/* -*- mode: c; coding: utf-8 -*- */

#ifndef _ICEC_XBEE_ENDPOINT_H_
#define _ICEC_XBEE_ENDPOINT_H_

#include <IceC/IceC.h>
#include <EasyXBee.h>

#ifdef __cplusplus
extern "C" {
#endif

#define Ice_EndpointTypeXBee 13

/* XBEE_SPEED must match settings of XBee device */
#define XBEE_SPEED           57600

#define MAX_PANID_SIZE       8
#define MAX_NODEID_SIZE      10

typedef struct XBee_EndpointOptions {
    Object _base;

    char panid[MAX_PANID_SIZE];
    char nodeid[MAX_NODEID_SIZE];
    XBeeAddress64 node_addr64;
    uint16_t node_addr16;
    bool used;
} XBee_EndpointOptions;

typedef XBee_EndpointOptions* XBee_EndpointOptionsPtr;

void logger_log(const char* s);

bool IceC_Xbee_getProtocolType(const char* proto, Ice_EndpointType* result);
bool IceC_Xbee_InputStream_init(Ice_InputStreamPtr self, int fd, Ice_EndpointType type);
bool IceC_Xbee_ObjectAdapter_activate(Ice_ObjectAdapterPtr self);
bool IceC_Xbee_EndpointInfo_init(Ice_EndpointInfoPtr self,
				 Ice_EndpointType type,
				 const char* endpoint);

bool IceC_XBee_EndpointInfo_writeToOutputStream(Ice_EndpointInfoPtr self,
						Ice_OutputStreamPtr os);

bool IceC_Xbee_ObjectPrx_connect(Ice_ObjectPrxPtr self);
bool IceC_Xbee_ObjectPrx_init(Ice_ObjectPrxPtr self,
			      Ice_EndpointType type,
			      const char* strprx);

bool IceC_Xbee_Connection_send(Ice_ConnectionPtr self, int fd, byte* data,
			       uint16_t* size);

bool IceC_XBee_Connection_dataReady(Ice_ConnectionPtr self,
				    bool* ready);

/* plugin specific functions */
void IceC_XBeeEndpoint_init(Ice_CommunicatorPtr ic);

#ifdef __cplusplus
}
#endif

#endif /* _ICEC_XBEE_ENDPOINT_H_ */
