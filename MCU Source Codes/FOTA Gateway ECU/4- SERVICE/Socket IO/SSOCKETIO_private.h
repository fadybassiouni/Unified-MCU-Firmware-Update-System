#ifndef __SSOCKETIO_PRIVATE_H_
#define __SSOCKETIO_PRIVATE_H

#define SSOCKETIO_MAX_NUMBER_OF_EVENTS	     8
#define SSOCKETIO_MAX_EVENT_NAME_LENGTH      20
#define SSOCKETIO_MAX_EVENT_DATA_SIZE        200


static STD_error_t SSOCKETIO_stderrGetEventName(const char *ARG_pccharBuffer, char *ARG_pcharEventName);
static STD_error_t SSOCKETIO_stderrGetEventData(const char *ARG_pccharBuffer, char *ARG_pccharEvenData);
static STD_error_t SSOCKETIO_stderrEncodeSocketIOFrame(u8 *ARG_pu8frame, u16 *ARG_pu16frameSize, const char *ARG_pcchEventName, const char *ARG_pcchEventData, u8 ARG_u8FrameType);
static SSOCKETIO_status SSOCKETIO_socketiostatusHandshake(void);
static SSOCKETIO_status SSOCKETIO_socketiostatusCheckInBufferForEvent(const char *ARG_pccharBuffer);
static void SSOCKETIO_voidExecuteEvents(void);
static s8 SSOCKETIO_s8SearchForEvent(const char* ARG_pccharEventName, const char ARG_charEventNames[][20]);


#endif
