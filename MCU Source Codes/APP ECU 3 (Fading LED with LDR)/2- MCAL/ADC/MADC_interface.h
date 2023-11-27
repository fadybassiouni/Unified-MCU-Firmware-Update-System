/************************************/
/*	Author	  :	Muhammad Reda		*/
/*	SWC		  : ADC					*/
/*  Layer	  : MCAL				*/
/*	Version	  :	1.0		  			*/
/*	Date	  :	October 9, 2023		*/
/*	Last Edit :	N/A					*/
/************************************/	
#ifndef	MADC_INTERFACE_H
#define	MADC_INTERFACE_H


#define 	MADC_RESOLUTION_12_BIT	0
#define 	MADC_RESOLUTION_10_BIT	1
#define 	MADC_RESOLUTION_8_BIT	2
#define 	MADC_RESOLUTION_6_BIT	3


#define 	MADC_PRESCALER_DIV_2	0	
#define 	MADC_PRESCALER_DIV_4	1
#define 	MADC_PRESCALER_DIV_6	2
#define 	MADC_PRESCALER_DIV_8	3


void	MADC_voidInit(void);
void		MADC_voidEnable(void);
void		MADC_voidDisable(void);
void		MADC_voidStartConversion(void);
void 		MADC_voidEOCFlagPolling(void);
STD_error_t	MADC_stderrReadAdc(u16 *ARG_pu16ADCRead);

#endif
