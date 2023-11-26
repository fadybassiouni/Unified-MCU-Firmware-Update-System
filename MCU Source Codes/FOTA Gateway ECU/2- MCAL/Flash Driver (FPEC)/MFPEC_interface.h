/* ******************************************************** */
/* Author      	:	 Abd-alrahman Amin						*/
/* SWC         	:	 Flash Program and erase controller		*/
/* Layer       	:	 MCAL              						*/
/* MCu     		:	 stm32f103         						*/
/* Version     	:	 1.0               						*/
/* Date        	:	 September 23, 2023						*/
/* Last Edit   	:	 N/A 									*/
/* ******************************************************** */


#ifndef _MFPEC_INTERFACE_H_
#define _MFPEC_INTERFACE_H_

void MFPEC_voidEraseAppArea(void);
	
void MFPEC_voidFlashPageErase(u8 ARG_u8PageNumber);

void MFPEC_voidFlashWrite(u32 ARG_u32Address, u16* ARG_pu16Data, u8 ARG_u8Length);

#endif
