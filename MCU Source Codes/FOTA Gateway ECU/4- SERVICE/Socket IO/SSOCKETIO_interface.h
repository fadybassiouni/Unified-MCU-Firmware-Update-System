#ifndef __SSOCKETIO_INTERFACE_H_
#define __SSOCKETIO_INTERFACE_H

/*Socket IO Frame Types*/
#define SSOCKETIO_FRAME_CONNECT    0
#define SSOCKETIO_FRAME_DISCONNECT 1
#define SSOCKETIO_FRAME_EVENT      2

/*Socket IO States*/
typedef enum
{
	SSOCKETIO_WAITING,
	SSOCKETIO_HANDSHAKE_SUCCESS,
	SSOCKETIO_HANDSHAKE_FAIL=30,
	SSOCKETIO_TCP_CONNECT_FAIL,
	SSOCKETIO_CONNECT_SUCCESS,
	SSOCKETIO_CONNECT_FAIL,
	SSOCKETIO_EMIT_SUCCESS,
	SSOCKETIO_EMIT_FAIL,
	SSOCKETIO_HAS_EVENT,
	SSOCKETIO_NO_EVENT,
	SSOCKETIO_EVENT_ADD_SUCCESS,
	SSOCKETIO_EVENT_ADD_FAIL,
	SSOCKETIO_EVENTS_FULL,
	SSOCKETIO_INVALID_CONFIG,
	SSOCKETIO_SERVER_LISTENING=23
}SSOCKETIO_status;

/*Initialize Socket IO Hardware Dependencies*/
void SSOCKETIO_voidInit(void);

/*Connects to a Socket IO Server*/
SSOCKETIO_status SSOCKETIO_socketiostatusConnect(const char *ARG_pcchHostName, u16 ARG_u16Port);

/*Emits a message over a certain Event*/
SSOCKETIO_status SSOCKETIO_socketiostatusEmit(const char *ARG_pcchEventName, const char *ARG_pcchEventData);

/*Set Event Call Back Function*/
SSOCKETIO_status SSOCKETIO_socketiostatusOnEvent(const char *ARG_pcchEventName, void (*ARG_pvoidfEventFunction)(const char *ARG_pccharEventData));

/*Start Listening For Events*/
void SSOCKETIO_voidListen(void);

/*Heartbeat function to check liveness of the connection*/
SSOCKETIO_status SSOCKETIO_socketiostatusHeartbeat(void);

#endif
