/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : EEPROM             */
/* Layer     : HAL  		      */
/* Version   : 1.0                */
/* Date      : November 6,2023    */
/* Last Edit : N/A                */
/**********************************/

#ifndef _HNVM_INTERFACE_H_
#define _HNVM_INTERFACE_H_

#define HNVM_SLAVE_ADDRESS 0xA0

STD_error_t HNVM_stderrInit(void);
void HNVM_voidWriteByte(u32 ARG_u32Address, u8 ARG_u8Data);
u8 HNVM_u8ReadByte(u32 ARG_u32Address);
STD_error_t HNVM_stderrBurstWrite(u32 ARG_u32StartAddress, u8 *ARG_pu8Data, u8 ARG_u8ByteCount);
STD_error_t HNVM_stderrBurstRead(u32 ARG_u32StartAddress, u8 *ARG_pu8Data, u32 ARG_u32ByteCount);


#endif
