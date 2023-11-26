#ifndef _HESP_PRIVATE_H_
#define _HESP_PRIVATE_H_

static void HESP_voidSendCommand(const char *ARG_pccharCommand);
static void HESP_voidTimeout(void);
void HESP_writeToBuffer(void);
static HESP_status HESP_hespstatusGetResponse(u16 ARG_u16Timeout, u8 ARG_u8ResponseType);
static HESP_status HESP_hespstatusSendAndExpectResponse(const char *ARG_pccharCommand, const char *ARG_pccharExpectedResponse, u16 ARG_u16Timeout, u8 ARG_u8ResponseType);


#endif
