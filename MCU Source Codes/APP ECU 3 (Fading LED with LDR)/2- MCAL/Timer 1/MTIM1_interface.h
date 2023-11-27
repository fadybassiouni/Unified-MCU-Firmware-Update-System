/**********************************/
/* Author    : Muhammad Reda      */
/* SWC       : Timer 	          */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : October 23,2023    */
/* Last Edit : N/A                */
/**********************************/

#ifndef MTIM1_INTERFACE_H
#define MTIM1_INTERFACE_H


STD_error_t MTIM1_stderrInit(u16 Copy_u16ARRValue,u16 Copy_u16Prescalar);
STD_error_t	MTIM1_stderrSetDutyCycle(u16 Copy_u16DCValue);
void MTIM1_voidEnable(void);
void MTIM1_voidDisable(void);
#endif
