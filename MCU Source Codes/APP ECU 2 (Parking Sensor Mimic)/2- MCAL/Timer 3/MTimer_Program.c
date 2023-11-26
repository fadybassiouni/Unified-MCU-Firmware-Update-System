/**
 * @file MSTK_program.c
 * @author Abdelrahman Amin
 * 				Amr Khaled
 * 				ElAmir Galal
 * 				Fady Taha
 * 				Mohammed Hassan
 * 				Mohammed Reda
 * @brief
 * @version 0.3
 * @date 2023-11-8
 * @copyright Copyright (c) 2023
 **/


#include "LBIT_Math.h"
#include "LSTD_Types.h"
#include "MTimer_Interface.h"
#include "MTimer_Private.h"

void (*MTIM3_GlobalPtrTFunction)(void)  ;
/**
 * @brief Initializes Timer 3 (TIM3) for capturing input signals.
 * @param ARG_u8MTIMChannel The Timer channel to configure for capture (MTIM_CHANNEL1, MTIM_CHANNEL2, MTIM_CHANNEL3, MTIM_CHANNEL4).
 * @param ARG_u16Prescaler The prescaler value to set for TIM3.
 * @return Error status.
 */
STD_error_t MTIM3_stderrTimerInit(u8 ARG_u8MTIMChannel, u16 ARG_u16Prescaler)
{
	STD_error_t L_stderrStatus = E_OK ;
	// Edge-aligned mode. The counter counts up or down depending on the direction bit
	CLEAR_BIT(TIM3->CR1,CMS0);
	CLEAR_BIT(TIM3->CR1,CMS1);
	//Counter used as upcounter
	CLEAR_BIT(TIM3->CR1,DIR);
	switch(ARG_u8MTIMChannel)
	{
		case MTIM_CHANNEL1:  
		//01: CC1 channel is configured as input, IC1 is mapped on TI1
			SET_BIT(TIM3->CCMR1,CC1S0);
			CLEAR_BIT(TIM3->CCMR1,CC1S1);
			/*00: no prescaler, capture is done each time an edge is detected on the  
			capture inpuT*/		
			CLEAR_BIT(TIM3->CCMR1,IC1PSC0);
			CLEAR_BIT(TIM3->CCMR1,IC1PSC1);
			//11: noninverted/both edges
			// SET_BIT(TIMER2->CCER,CC1P);
			// SET_BIT(TIMER2->CCER,CC1NP);
			// //Enable Capture
			// SET_BIT(TIMER2->CCER,CC1E);
		break ; 
		case MTIM_CHANNEL2:  
		//01: CC1 channel is configured as input, IC1 is mapped on TI1
			SET_BIT(TIM3->CCMR1,CC2S0);
			CLEAR_BIT(TIM3->CCMR1,CC2S1);
			/*00: no prescaler, capture is done each time an edge is detected on the  
			capture inpuT*/		
			CLEAR_BIT(TIM3->CCMR1,IC2PSC0);
			CLEAR_BIT(TIM3->CCMR1,IC2PSC1);
		break ; 
		case MTIM_CHANNEL3:
			//01: CC1 channel is configured as input, IC1 is mapped on TI1
			SET_BIT(TIM3->CCMR2,CC3S0);
			CLEAR_BIT(TIM3->CCMR2,CC3S1);
			/*00: no prescaler, capture is done each time an edge is detected on the  
			capture inpuT*/		
			CLEAR_BIT(TIM3->CCMR2,IC3PSC0);
			CLEAR_BIT(TIM3->CCMR2,IC3PSC1);
		break ; 
		case MTIM_CHANNEL4:  
			//01: CC1 channel is configured as input, IC1 is mapped on TI1
			SET_BIT(TIM3->CCMR2,CC4S0);
			CLEAR_BIT(TIM3->CCMR2,CC4S1);
			/*00: no prescaler, capture is done each time an edge is detected on the  
			capture inpuT*/		
			CLEAR_BIT(TIM3->CCMR2,IC4PSC0);
			CLEAR_BIT(TIM3->CCMR2,IC4PSC1);
		break ; 
		default : L_stderrStatus = E_NOK ; 
	}
	TIM3->PSC = ARG_u16Prescaler;

	TIM3->CNT = 0 ;
	return L_stderrStatus;
}
/**
 * @brief Starts Timer 3 (TIM3).
 */
void        MTIM3_voidTimerStart(void)
{
	u32 volatile temp ;
	/*Enable Counter*/
	SET_BIT(TIM3->CR1,CEN);
	temp = TIM3->CNT ;

}
/**
 * @brief Stops Timer 3 (TIM3).
 */
void        MTIM3_voidTimerStop(void)
{
	/*Disable Counter*/
	CLEAR_BIT(TIM3->CR1,CEN);
}
/**
 * @brief Retrieves the captured value from Timer 3 (TIM3) channels.
 * @param ARG_u8MTIMChannel The Timer channel to get the captured value (MTIM_CHANNEL1, MTIM_CHANNEL2, MTIM_CHANNEL3, MTIM_CHANNEL4).
 * @return The captured value.
 */
u16 		MTIM3_u16GetCapturedValue(u8 ARG_u8MTIMChannel)
{
	u16 L_u16CaputerdValue = 0 ; 
	switch(ARG_u8MTIMChannel)
	{
		case MTIM_CHANNEL1: L_u16CaputerdValue = TIM3->CCR1 ; break ; 
		case MTIM_CHANNEL2: L_u16CaputerdValue = TIM3->CCR2 ; break ; 
		case MTIM_CHANNEL3: L_u16CaputerdValue = TIM3->CCR3 ; break ; 
		case MTIM_CHANNEL4: L_u16CaputerdValue = TIM3->CCR4 ; break ; 
		
	}
	return L_u16CaputerdValue ;
}
/**
 * @brief Enables interrupts for Timer 3 (TIM3).
 * @param ARG_u8Interrupt The interrupt to enable (MTIM_INT_UPDATE_ENABLE, MTIM_INT_CC1, MTIM_INT_CC2, MTIM_INT_CC3, MTIM_INT_CC4).
 * @return Error status.
 */
STD_error_t MTIM3_stderrInterruptEnable(u8 ARG_u8Interrupt)
{
	STD_error_t L_stderrStatus = E_OK ; 
	switch(ARG_u8Interrupt)
	{
		case MTIM_INT_UPDATE_ENABLE	: SET_BIT(TIM3->DIER,UIE)  ; break ;
		case MTIM_INT_CC1			: SET_BIT(TIM3->DIER,CC1IE); break ;			
		case MTIM_INT_CC2			: SET_BIT(TIM3->DIER,CC2IE); break ;			
		case MTIM_INT_CC3			: SET_BIT(TIM3->DIER,CC3IE); break ;			
		case MTIM_INT_CC4			: SET_BIT(TIM3->DIER,CC4IE); break ;
		default: 	L_stderrStatus = E_NOK ; break ;	
		
	}

	return L_stderrStatus; 
}
/**
 * @brief Disables interrupts for Timer 3 (TIM3).
 * @param ARG_u8Interrupt The interrupt to disable (MTIM_INT_UPDATE_ENABLE, MTIM_INT_CC1, MTIM_INT_CC2, MTIM_INT_CC3, MTIM_INT_CC4).
 * @return Error status.
 */
STD_error_t MTIM3_stderrInterruptDisable(u8 ARG_u8Interrupt)
{
	STD_error_t L_stderrStatus = E_OK ; 
	switch(ARG_u8Interrupt)
	{
		case MTIM_INT_UPDATE_ENABLE	: CLEAR_BIT(TIM3->DIER,UIE)  ; break ;
		case MTIM_INT_CC1			: CLEAR_BIT(TIM3->DIER,CC1IE); break ;			
		case MTIM_INT_CC2			: CLEAR_BIT(TIM3->DIER,CC2IE); break ;			
		case MTIM_INT_CC3			: CLEAR_BIT(TIM3->DIER,CC3IE); break ;			
		case MTIM_INT_CC4			: CLEAR_BIT(TIM3->DIER,CC4IE); break ;
		default: 	L_stderrStatus = E_NOK ; break ;	
		
	}
	return L_stderrStatus; 
}



/**
 * @brief Enables Input Capture Unit (ICU) for Timer 3 (TIM3) channels.
 * @param ARG_u8MTIMChannel The Timer channel to enable ICU (MTIM_CHANNEL1, MTIM_CHANNEL2, MTIM_CHANNEL3, MTIM_CHANNEL4).
 * @return Error status.
 */

STD_error_t MTIM3_stderrEnableICU(u8 ARG_u8MTIMChannel)
{
	STD_error_t L_stderrStatus = E_OK;
	switch(ARG_u8MTIMChannel)
	{
		case MTIM_CHANNEL1:SET_BIT(TIM3->CCER,CC1E); break ; 
		case MTIM_CHANNEL2:SET_BIT(TIM3->CCER,CC2E); break ; 
		case MTIM_CHANNEL3:SET_BIT(TIM3->CCER,CC3E); break ; 
		case MTIM_CHANNEL4:SET_BIT(TIM3->CCER,CC4E); break ; 
		default : L_stderrStatus = E_NOK;
	}
	return L_stderrStatus ;
}


/**
 * @brief Sets the callback function for Timer 3 (TIM3) interrupts.
 * @param PtrtoFunction Pointer to the callback function.
 * @return Error status.
 */
STD_error_t MTIM3_stderrSetCallBack(void(*PtrtoFunction)(void))
{
	STD_error_t L_stderrStatus = E_OK;
	if(PtrtoFunction!=NULL_POINTER)
	{
		MTIM3_GlobalPtrTFunction=PtrtoFunction;
	}
	else 
	{
		L_stderrStatus = E_NULL_POINTER;
	}
	return L_stderrStatus;
}

/**
 * @brief Configures edge triggers for Timer 3 (TIM3) channels.
 * @param ARG_u8MTIMChannel The Timer channel to configure the trigger (MTIM_CHANNEL1, MTIM_CHANNEL2, MTIM_CHANNEL3, MTIM_CHANNEL4).
 * @param ARG_u8Trigger The trigger type (MTIM_EDGE_TRIGGER_FALLING, MTIM_EDGE_TRIGGER_RISING, MTIM_EDGE_TRIGGER_BOTH).
 * @return Error status.
 */
STD_error_t MTIM3_stderrEdgeTrigger(u8 ARG_u8MTIMChannel, u8 ARG_u8Trigger)
{
	STD_error_t L_stderrStatus = E_OK;
	switch(ARG_u8MTIMChannel)
	{
		case MTIM_CHANNEL1	:
		TIM3->CCER &= ~(0x000A);
		if(MTIM_EDGE_TRIGGER_FALLING ==ARG_u8Trigger)
		{
			SET_BIT(TIM3->CCER,CC1P);
			//CLEAR_BIT(TIM3->CCER,CC1NP);
		}
		else if (MTIM_EDGE_TRIGGER_RISING ==ARG_u8Trigger)
		{
			CLEAR_BIT(TIM3->CCER,CC1P);
			//CLEAR_BIT(TIM3->CCER,CC1NP);
		}
		else if (MTIM_EDGE_TRIGGER_BOTH ==ARG_u8Trigger)
		{
		//	SET_BIT(TIM3->CCER,CC1P);
		//	SET_BIT(TIM3->CCER,CC1NP);
		}
		else 
		{
			L_stderrStatus = E_NOK; 
		}
		break ; 
		case MTIM_CHANNEL2	:
		TIM3->CCER &= ~(0x00A0);
		if(MTIM_EDGE_TRIGGER_FALLING ==ARG_u8Trigger )
		{
			SET_BIT(TIM3->CCER,CC2P);
			CLEAR_BIT(TIM3->CCER,CC2NP);
		}
		else if (MTIM_EDGE_TRIGGER_RISING ==ARG_u8Trigger)
		{
			CLEAR_BIT(TIM3->CCER,CC2P);
			CLEAR_BIT(TIM3->CCER,CC2NP);
		}
		else if (MTIM_EDGE_TRIGGER_BOTH ==ARG_u8Trigger)
		{
			SET_BIT(TIM3->CCER,CC2P);
			SET_BIT(TIM3->CCER,CC2NP);
		}
		else 
		{
			L_stderrStatus = E_NOK; 
		}
		break ; 
		case MTIM_CHANNEL3		:
		TIM3->CCER &= ~(0x0A00);
		if(MTIM_EDGE_TRIGGER_FALLING ==ARG_u8Trigger )
		{
			SET_BIT(TIM3->CCER,CC3P);
			CLEAR_BIT(TIM3->CCER,CC3NP);
		}
		else if (MTIM_EDGE_TRIGGER_RISING ==ARG_u8Trigger)
		{
			CLEAR_BIT(TIM3->CCER,CC3P);
			CLEAR_BIT(TIM3->CCER,CC3NP);
		}
		else if (MTIM_EDGE_TRIGGER_BOTH ==ARG_u8Trigger)
		{
			SET_BIT(TIM3->CCER,CC3P);
			SET_BIT(TIM3->CCER,CC3NP);
		}
		else 
		{
			L_stderrStatus = E_NOK; 
		}
		break ; 
		case MTIM_CHANNEL4		:
		TIM3->CCER &= ~(0xA000);
		if(MTIM_EDGE_TRIGGER_FALLING ==ARG_u8Trigger )
		{
			SET_BIT(TIM3->CCER,CC4P);
			CLEAR_BIT(TIM3->CCER,CC4NP);
		}
		else if (MTIM_EDGE_TRIGGER_RISING ==ARG_u8Trigger)
		{
			CLEAR_BIT(TIM3->CCER,CC4P);
			CLEAR_BIT(TIM3->CCER,CC4NP);
		}
		else if (MTIM_EDGE_TRIGGER_BOTH ==ARG_u8Trigger)
		{
			SET_BIT(TIM3->CCER,CC4P);
			SET_BIT(TIM3->CCER,CC4NP);
		}
		else 
		{
			L_stderrStatus = E_NOK; 
		}
		break ; 
		default : L_stderrStatus = E_NOK; 
		break ; 
	}
	return L_stderrStatus ;
}


/**
 * @brief Resets the counter value of Timer 3 (TIM3) to zero.
 */
void MTIM3_voidResetCounter(void)
{
	TIM3->CNT = 0;
}

/**
 * @brief Interrupt handler for Timer 3 (TIM3) IRQ.
 * Invokes the global callback function if assigned.
 */
void TIM3_IRQHandler(void)
{
	if(MTIM3_GlobalPtrTFunction!=NULL_POINTER)
	{
		MTIM3_GlobalPtrTFunction();
	}
	else 
	{
		
	}
}
