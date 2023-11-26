/* ******************************************************** */
/* Author      	:	 Abd-alrahman Amin						*/
/* SWC         	:	 Flash Program and erase controller		*/
/* Layer       	:	 MCAL              						*/
/* MCu     		:	 stm32f103         						*/
/* Version     	:	 1.0               						*/
/* Date        	:	 September 23, 2023						*/
/* Last Edit   	:	 N/A 									*/
/* ******************************************************** */


/* Library Includes */
#include <LSTD_types.h>
#include <LBIT_math.h>
/* SWC includes */
#include <MFPEC_private.h>
#include <MFPEC_interface.h>
// #include "MFPEC_config.h"

void MFPEC_voidEraseAppArea(void)
{
	u8 L_u8i;
	/* flash start from page 0 -> 63 */
	for (L_u8i=4;L_u8i<64;L_u8i++)
	{
		MFPEC_voidFlashPageErase(L_u8i);
	}
	
}

void MFPEC_voidFlashPageErase(u8 ARG_u8PageNumber)
{
	/* Wait Busy Flag */
	while ( 1 == GET_BIT(FPEC->SR,BSY_FLAG) );

	/* Check if FPEC is locked or not */
	if ( 1 == GET_BIT(FPEC->CR,LOCK_BIT) )
	{
	/*	
	 *	The following values must be programmed consecutively to unlock the FLASH_CR register 
	 *	and allow programming/erasing it 
	 */
		FPEC -> KEYR = 0x45670123; /* 1st Key */
		FPEC -> KEYR = 0xCDEF89AB; /* 2nd Key */
	}
	
	/* Page Erase Operation */
	SET_BIT(FPEC->CR,PER_BIT);

	/* Write Page address */
	/* Page size 1K Flash + base address */
	FPEC->AR = (u32)(ARG_u8PageNumber * 1024) + 0x08000000 ;

	/* Start operation */
	SET_BIT(FPEC->CR,STRT_BIT);

	/* Wait Busy Flag */
	while ( 1 == GET_BIT(FPEC->SR,BSY_FLAG) );

	/* EOP */
	SET_BIT(FPEC->SR,EOP_FLAG);
	CLEAR_BIT(FPEC->CR,PER_BIT);
}

void MFPEC_voidFlashWrite(u32 ARG_u32Address, u16* ARG_pu16Data, u8 ARG_u8Length)
{
	u8 L_u8i;
	volatile u16 Temp;

	while ( 1 == GET_BIT(FPEC->SR,BSY_FLAG) );

	/* Check if FPEC is locked or not */
	if ( 1 == GET_BIT(FPEC->CR,LOCK_BIT) )
	{
		/*	
		 *	The following values must be programmed consecutively to unlock the FLASH_CR register 
		 *	and allow programming/erasing it 
		 */
		FPEC -> KEYR = 0x45670123; /* 1st Key */
		FPEC -> KEYR = 0xCDEF89AB; /* 2nd Key */ 
	}
	
	

	for (L_u8i = 0; L_u8i< ARG_u8Length; L_u8i++)
	{
		/* Write Flash Programming */
		SET_BIT(FPEC->CR,PG_BIT);

		/* Half word operation */

		Temp = ARG_pu16Data[L_u8i];
		*((volatile u16*)ARG_u32Address) = ARG_pu16Data[L_u8i];
		ARG_u32Address += 2 ; /* increment by 2byte */

		/* Wait Busy Flag */
		while (GET_BIT(FPEC->SR,BSY_FLAG) == 1);
	
		/* EOP */
		SET_BIT(FPEC->SR,EOP_FLAG);
		CLEAR_BIT(FPEC->CR,PG_BIT);
	}

}
