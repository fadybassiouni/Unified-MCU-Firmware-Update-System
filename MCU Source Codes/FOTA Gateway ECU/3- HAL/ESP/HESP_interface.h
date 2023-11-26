#ifndef _HESP_INTERFACE_H_
#define _HESP_INTERFACE_H_

#define HESP_OP_MODE_STATION   1
#define HESP_OP_MODE_AP        2
#define HESP_OP_MODE_BOTH      3
#define HESP_OP_MODE_INVALID   4

#define HESP_CONNECTION_MODE_SINGLE   0
#define HESP_CONNECTION_MODE_MULTIPLE 1


#define HESP_SERVER_PROTOCOL_TCP 0
#define HESP_SERVER_PROTOCOL_UDP 1
#define HESP_SERVER_PROTOCOL_SSL 2

#define HESP_SERVER_MAX_CONNECTIONS   5
#define HESP_SERVER_MAX_PORT_NO       65535
#define HESP_SERVER_MAX_ALIVE_SECONDS 7200

#define HESP_RESPONSE_TYPE_SYSTEM 0
#define HESP_RESPONSE_TYPE_WIFI   1
#define HESP_RESPONSE_TYPE_SERVER 2

typedef enum
{
	HESP_RESPONSE_WAITING,
	HESP_RESPONSE_RECIEVED,
	HESP_RESPONSE_TIMEOUT,
	HESP_RESPONSE_EXPECTED,
	HESP_RESPONSE_UNEXPECTED,
	HESP_RESPONSE_UNKOWN,
	HESP_STATE_ACTIVE,
	HESP_STATE_INACTIVE,
	HESP_VERSION_AVAILABLE,
	HESP_VERSION_UNAVAILABLE,
	HESP_OP_MODE_SET,
	HESP_OP_MODE_NOT_SET,
	HESP_WIFI_CONNECTED,
	HESP_WIFI_NOT_CONNECTED,
	HESP_WIFI_AP_NOT_FOUND,
	HESP_WIFI_FAILED_TO_CONNECT,
	HESP_CONN_MODE_SET,
	HESP_CONN_MODE_NOT_SET,
	HESP_SERVER_CONNECTED,
	HESP_SERVER_DISCONNECTED,
	HESP_SERVER_INVALID_PROTOCOL,
	HESP_SERVER_INVALID_CONFIGURATION,
	HESP_SERVER_FAILED_TO_CONNECT,
	HESP_SERVER_SEND_SUCCESS,
	HESP_SERVER_SEND_FAIL,
	HESP_BUFFER_FULL,
	HESP_BUFFER_EMPTY,
	HESP_BUFFER_NOT_EMPTY,

}HESP_status;

/*ESP Prototypes*/
STD_error_t HESP_stderrInit(u8 ARG_u8UARTID, u32 ARG_u32BaudRate);
HESP_status HESP_hespstatusCheckActive(void);
HESP_status HESP_hespstatusGetVersion(char *ARG_pcharVersion);
HESP_status HESP_hespstatusSetOperationMode(u8 ARG_u8OperationMode);
HESP_status HESP_hespstatusConnectToWIFI(const char *ARG_pccharssid, const char *ARG_pccharPassword);
HESP_status HESP_hespstatusSetConnectionMode(u8 ARG_u8ConnectionMode);
HESP_status HESP_hespstatusConnectToServer(u8 ARG_u8LinkID,u8 ARG_u8Protocol, const char *ARG_pccharHost, u16 ARG_u8Port);
HESP_status HESP_hespstatusDisconnectServer(void);
HESP_status HESP_hespstatusSendToServer(const char *ARG_pccharCommand, u16 ARG_u16CommandTimeout);
void HESP_voidGetServerResponse(char *ARG_pccharResponse);
void HESP_voidFlushBuffer(void);
HESP_status HESP_stderrCheckIfBufferIsEmpty(void);
HESP_status HESP_hespstatusQueryState(void);

#endif
