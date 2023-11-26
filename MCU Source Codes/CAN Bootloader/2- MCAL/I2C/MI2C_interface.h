/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : I2C                */
/* Layer     : MCAL   		      */
/* Version   : 1.0                */
/* Date      : November 6,2023    */
/* Last Edit : N/A                */
/**********************************/

#ifndef _I2C_INTERFACE_H_
#define _I2C_INTERFACE_H_

#define MI2C_WRITE 0
#define MI2C_READ  1

/*Status Flags*/
#define MI2C_START_CONDITION_SENT   0
#define MI2C_SLAVE_ADDR_SENT        1
#define MI2C_STOP_CONDITION_SENT    4
#define MI2C_DATA_BUFFER_NOT_EMPTY  6
#define MI2C_DATA_BUFFER_EMPTY      7
#define MI2C_ACK_FAIL               10
#define MI2C_BUSY                   12


STD_error_t MI2C_stderrInit(u32 ARG_u32Bitrate, u32 ARG_u32CPUFrequency);
void MI2C_voidEnable(void);
void MI2C_voidDisable(void);
void MI2C_voidEnableACK(void);
void MI2C_voidDisableACK(void);
void MI2C_voidFlagPolling(u8 ARG_u8Flag);
void MI2C_voidClearFlags(void);
void MI2C_voidSendStartCondition(void);
void MI2C_voidSendStopCondition(void);
void MI2C_voidTransmitSlaveAddress(u8 ARG_u8SlaveAddress, u8 ARG_u8ReadWrite);
void MI2C_voidTransmitByte(u8 ARG_u8Byte);
u8 MI2C_u8ReceiveByte(void);




#endif
