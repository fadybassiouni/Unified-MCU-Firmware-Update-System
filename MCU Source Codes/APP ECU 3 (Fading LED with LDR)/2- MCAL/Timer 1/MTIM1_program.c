/**********************************/
/* Author    : Muhammad Reda      */
/* SWC       : Timer 	          */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : October 23,2023    */
/* Last Edit : N/A                */
/**********************************/

#include <LSTD_types.h>
#include <LBIT_math.h>

#include <MTIM1_interface.h>
#include <MTIM1_private.h>


STD_error_t MTIM1_stderrInit(u16 Copy_u16ARRValue,u16 Copy_u16Prescalar)
{
	STD_error_t L_stderrState=E_OK;
	if((Copy_u16ARRValue < 65536) && (Copy_u16Prescalar < 65536))
	{
	// Configure Timer 1 for PWM generation
	TIM1_CR1 = 0; // Clear control register 1
	TIM1_CR1 = 0; // Clear control register 2
	TIM1_CCMR1 |= (6 << 4) | (1 << 3); // PWM mode
	TIM1_CCER |= (1 << 0); // Enable capture/compare channel 1 output
	TIM1_BDTR |= (1 << 15);
	TIM1_PSC = Copy_u16Prescalar; // prescaler Value
	TIM1_ARR = Copy_u16ARRValue; // Auto-reload value
	TIM1_CCR1 = 999; // Initial duty cycle value
	}
	else
	{
		L_stderrState=E_NOK;
	}
	return L_stderrState;
}
void 	MTIM1_voidEnable(void)
{
	// Enable Timer 1 Enable
	TIM1_CR1 |= (1 << 0);
}
void 	MTIM1_voidDisable(void)
{
	// Disable Timer 1 Enable
	CLEAR_BIT(TIM1_CR1,0);
}
STD_error_t	MTIM1_stderrSetDutyCycle(u16 Copy_u16DCValue)
{
	STD_error_t L_stderrState=E_OK;
	if(Copy_u16DCValue<65536)
	{
	TIM1_CCR1 = Copy_u16DCValue;
	}
	else
	{
		//Return Error
		L_stderrState=E_NOK;
	}
	return L_stderrState;
}
