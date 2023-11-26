/************************************/
/*	Author	  :	Muhammad Reda		*/
/*	SWC		  : ADC					*/
/*  Layer	  : MCAL				*/
/*	Version	  :	1.0		  			*/
/*	Date	  :	October 9, 2023		*/
/*	Last Edit :	N/A					*/
/************************************/	

/********* Libraries Includes *********/
#include <LSTD_types.h>
#include <LBIT_math.h>

/********* ADC Header Files Includes *********/
#include <MADC_interface.h>
#include <MADC_private.h>
#include <MADC_config.h>

void	MADC_voidInit(void)
{

	/*Set Conversion Trigger to External Event*/
	MADC->CR2 |= (1<<20);

	/*External Event selection pointed to SWSTART bit*/
	MADC->CR2 |= (7<<17);

	/*Select Channel*/
	MADC->SQR3 |= (0<<0);  // SEQ1 for Channel 1


}

void		MADC_voidEnable(void)
{
	SET_BIT(MADC->CR2, 0);
}

void		MADC_voidDisable(void)
{

	CLEAR_BIT(MADC->CR2, 0);
}	
void		MADC_voidStartConversion(void)
{
	MADC->SR = 0;
	SET_BIT(MADC->CR2, 22);
}
STD_error_t	MADC_stderrReadAdc(u16 *ARG_pu16ADCRead)
{
	STD_error_t L_stderrState=E_OK;
	if(ARG_pu16ADCRead != NULL_POINTER)
	{
		*ARG_pu16ADCRead = MADC->DR;
	}
	else
	{
		L_stderrState=E_NULL_POINTER;
	}
	return L_stderrState;
}

void 		MADC_voidEOCFlagPolling(void)
{
	while(!GET_BIT(MADC->SR, 1));
}
