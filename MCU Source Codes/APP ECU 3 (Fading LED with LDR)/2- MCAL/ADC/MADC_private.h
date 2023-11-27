/************************************/
/*	Author	  :	Muhammad Reda		*/
/*	SWC		  : ADC					*/
/*  Layer	  : MCAL				*/
/*	Version	  :	1.0		  			*/
/*	Date	  :	October 9, 2023		*/
/*	Last Edit :	N/A					*/
/************************************/	
#ifndef	MADC_PRIVATE_H
#define	MADC_PRIVATE_H


#define ADC_BASE_ADDRESS ((volatile u32 *)0x40012400)

typedef struct
{
	volatile u32 SR;
	volatile u32 CR1;
	volatile u32 CR2;
	volatile u32 SMPR1;
	volatile u32 SMPR2;
	volatile u32 JOFR1;
	volatile u32 JOFR2;
	volatile u32 JOFR3;
	volatile u32 JOFR4;
	volatile u32 HTR;
	volatile u32 LTR;
	volatile u32 SQR1;
	volatile u32 SQR2;
	volatile u32 SQR3;
	volatile u32 JSQR;
	volatile u32 JDR1;
	volatile u32 JDR2;
	volatile u32 JDR3;
	volatile u32 JDR4;
	volatile u32 DR;

}MADC_t;

#define MADC ((volatile MADC_t *)ADC_BASE_ADDRESS)



#endif
