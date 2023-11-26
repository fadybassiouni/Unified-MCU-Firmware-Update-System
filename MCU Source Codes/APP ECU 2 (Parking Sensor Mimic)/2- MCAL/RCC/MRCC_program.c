/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : RCC                */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 12,2023  */
/* Last Edit : N/A                */
/**********************************/
#include <LBIT_math.h>
#include <LSTD_types.h>

#include <MRCC_private.h>
#include <MRCC_config.h>
#include <MRCC_interface.h>

static volatile u8 MRCC_u8PLLConfigured = 0;
static volatile u32 *MRCC_p32Buses[3] = {MRCC_AHBENR, MRCC_APB2ENR, MRCC_APB1ENR};

STD_error_t MRCC_stderrInitSystemClock(u8 ARG_u8SystemClockSource)
{
	/*State OK no Errors*/
	STD_error_t L_stderrState = E_OK;

	/*Switch According to Provided System Clock Source*/
	switch(ARG_u8SystemClockSource)
	{
		/*Using High Speed Internal Clock*/
		case MRCC_SYSCLOCK_HSI:
		{
			/*Enable The High Speed Internal Clock*/
	 		SET_BIT(MRCC->CR, MRCC_CR_HSION);

	 		/*Wait for the clock to be stable*/
	 		while(!GET_BIT(MRCC->CR, MRCC_CR_HSIRDY));

	 		break;	
		}

		/*Using External Crystal Oscillator*/
		case MRCC_SYSCLOCK_HSE_CRYSTAL: 
		{
			/*Enable The High Speed External Clock Source*/
			SET_BIT(MRCC->CR, MRCC_CR_HSEON); 

			/*Wait for the clock to be stable*/
			while(!GET_BIT(MRCC->CR, MRCC_CR_HSERDY));
			
			break;	
		}

		/*Using External RC or User Defined Clock*/
		case MRCC_SYSCLOCK_HSE_USER_DEF:
		{
			/*Disable The High Speed External Clock Source
			 *as the HSE Bypass bit cannot be written to 
			 * if the HSEON bit is set to 1*/
			CLEAR_BIT(MRCC->CR, MRCC_CR_HSEON);

			/*Enable HSE Bypass for external clock*/
			SET_BIT(MRCC->CR, MRCC_CR_HSEBYP);

			/*Enable The High Speed External Clock Source*/
			SET_BIT(MRCC->CR, MRCC_CR_HSEON);

			/*Wait for the clock to be stable*/
			while(!GET_BIT(MRCC->CR, MRCC_CR_HSERDY));

			break;
		}

		/*Using Phase Locked Loop*/
		case MRCC_SYSCLOCK_PLL: 		
		{
			/*Check if the PLL has already been configured before Enabling and using it*/
			if(MRCC_u8PLLConfigured == 1)
			{
				/*Enable The Phase Locked Loop Clock*/
				SET_BIT(MRCC->CR, MRCC_CR_PLLON);	

				/*Wait for the clock to be stable*/
				while(!GET_BIT(MRCC->CR, MRCC_CR_PLLRDY));
			}
			else
			{
				L_stderrState = E_NOK;
			}

			break;					
		}
		/*If a Wrong option has been selected Set state to Not OK*/
		default: L_stderrState = E_NOK;
	}

	if(L_stderrState == E_OK)
	{
		/*Select the System Clock*/
		MRCC->CFGR = (MRCC->CFGR & MRCC_SW_MASK) | (ARG_u8SystemClockSource);

		/*Check if it has been successfully selected without any problems*/
		L_stderrState = !(((MRCC->CFGR & MRCC_SWS_MASK) >> 2) == ARG_u8SystemClockSource);





	}
	else
	{
		/*MISRA*/
	}

	return L_stderrState;
}


STD_error_t MRCC_stderrConfigurePLL(MRCC_PLLconfig_t *ARG_pmrcc_pllconfig_tConfig)
{
	STD_error_t L_stderrState = E_OK;
	MRCC_u8PLLConfigured = 0;

	/*Check if input config is valid in terms of min and max values*/
	/*NOTE: This doesn't check for wrong logical calculations this is the user's task to calculate valid Frequencies*/
	if((ARG_pmrcc_pllconfig_tConfig->PLLClockSource           <= MRCC_PLL_SRC_HSE)
		&&(ARG_pmrcc_pllconfig_tConfig->HSEPrescalar        <= MRCC_PLL_SRC_HSE_2)
		&&((ARG_pmrcc_pllconfig_tConfig->MultiplicationFactor >= MRCC_PLL_MIN_MUL_FACT)           && (ARG_pmrcc_pllconfig_tConfig->MultiplicationFactor <= MRCC_PLL_MAX_MUL_FACT))
		&&((ARG_pmrcc_pllconfig_tConfig->USBClockPrescalar    >= MRCC_PLL_MIN_USBCLOCK_PRESCALER) && (ARG_pmrcc_pllconfig_tConfig->USBClockPrescalar    <= MRCC_PLL_MAX_USBCLOCK_PRESCALER)))	
	{
		/*Must Disable PLL and PLL2S Before Configuring PLL*/
		CLEAR_BIT(MRCC->CR, MRCC_CR_PLLON);

		/*Configure PLL Settings*/
		MRCC->CFGR = (MRCC->CFGR & MRCC_CFGR_PLL_MASK) | (ARG_pmrcc_pllconfig_tConfig->HSEPrescalar << MRCC_CFGR_PLLXTPRE) |(ARG_pmrcc_pllconfig_tConfig->PLLClockSource << MRCC_CFGR_PLLSRC) | (ARG_pmrcc_pllconfig_tConfig->MultiplicationFactor << MRCC_CFGR_PLLMUL0) | (ARG_pmrcc_pllconfig_tConfig->USBClockPrescalar << MRCC_CFGR_USBPRE);

		MRCC_u8PLLConfigured = 1;
	}
	else
	{
		/*State is Error Not OK if any of the passed configs is invalid*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}


STD_error_t MRCC_stderrInitBusClock(u8 ARG_u8Bus, u8 ARG_u8ClockPrescalar)
{
	STD_error_t L_stderrState = E_OK;

	/*Switch According to selected Bus*/
	switch(ARG_u8Bus)
	{
		case MRCC_BUS_AHB:
		{
			/*check if passed prescaler is valid*/
			if(ARG_u8ClockPrescalar >= MRCC_AHB_PRESCALAR_2 && ARG_u8ClockPrescalar <= MRCC_AHB_PRESCALAR_512)
			{
				/*Configure bus prescaler*/
				MRCC->CFGR = (MRCC->CFGR & MRCC_HPRE_MASK) | (ARG_u8ClockPrescalar << MRCC_CFGR_HPRE0); 
			}
			else
			{
				/*State is Not OK*/
				L_stderrState = E_NOK;
			}

			break;
		}
		case MRCC_BUS_APB1:
		{
			/*check if passed prescaler is valid*/
			if(ARG_u8ClockPrescalar >= MRCC_APB1_NO_PRESCALAR && ARG_u8ClockPrescalar <= MRCC_APB1_PRESCALAR_16)
			{
				/*Configure bus prescaler*/
				MRCC->CFGR = (MRCC->CFGR & MRCC_PPRE1_MASK) | (ARG_u8ClockPrescalar << MRCC_CFGR_PPRE10);	
			}
			else
			{
				/*State is Not OK*/
				L_stderrState = E_NOK;
			}

			break;
		}
		case MRCC_BUS_APB2:
		{
			/*check if passed prescaler is valid*/
			if(ARG_u8ClockPrescalar >= MRCC_APB2_NO_PRESCALAR && ARG_u8ClockPrescalar <= MRCC_APB2_PRESCALAR_16)
			{
				/*Configure bus prescaler*/
				MRCC->CFGR = (MRCC->CFGR & MRCC_PPRE2_MASK) | (ARG_u8ClockPrescalar << MRCC_CFGR_PPRE20);	
			}
			else
			{
				/*State is Not OK*/
				L_stderrState = E_NOK;
			}

			break;
		}

		/*State is Error Not OK if bus number is invalid*/
		default: L_stderrState = E_NOK; break; 
	}

	/*Return Error State*/
	return L_stderrState;
}


/*----------------------------------------------------------------------------------------------------------------
 * The Function works by taking a peripheral id as an input and analyzing the input number to get
 * the bus that the peripheral is on and the bit number out of the 32 bits in that desired bus.
 * The Explanation is as follows:
 *
 * The user enters peripheral id MRCC_PERIPHERAL_TIM4 which translates to predefined value 66 decimal and 
 * 0b(0000 0000)(0000 0000)(0000 0000)(0<10><00010>) and from that
 * binary number we can see that the 6th and 7th bit contain the predefined bus number for APB1 which is
 * 2 in decimal and (0<10>00010) in binary. Now we have the bus number by which we can index the desired
 * register whether it is in AHB, APB2, or APB1. to get the bit number we can see that the first five bits
 * represent the bit number (010<00010>) which is 2 in decimal. from that we can conclude that a general rule
 *
 * Peripheral ID (AB) -> 0x000000(0b0AABBBBB) where A represents bus Number and B represents bit Number
 *
 * Now we can get the bus number by Shifting the peripheral ID to the right 5 times (AB) >> 5
 * and we can get the bit number by masking the peripheral ID by the mask 0x0000001F (AB) & 0x0000001F
 ------------------------------------------------------------------------------------------------------------------*/

STD_error_t MRCC_stderrEnablePeripheralClock(u8 ARG_u8PeripheralID)
{

	STD_error_t L_stderrState = E_OK;

	/*Check For Valid Peripheral ID*/
	if(ARG_u8PeripheralID < MRCC_MAX_PERIPHERALS)
	{
		SET_BIT(*MRCC_p32Buses[ARG_u8PeripheralID>>MRCC_PERIPHERAL_PORT_SHIFT_FACTOR], ARG_u8PeripheralID & MRCC_PERIPHERAL_NUM_MASK);
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}
	
	/*Return Error State*/
	return L_stderrState;
}


STD_error_t MRCC_stderrDisablePeripheralClock(u8 ARG_u8PeripheralID)
{
	STD_error_t L_stderrState = E_OK;

	/*Check For Valid Peripheral ID*/
	if(ARG_u8PeripheralID < MRCC_MAX_PERIPHERALS)
	{
		CLEAR_BIT(*MRCC_p32Buses[ARG_u8PeripheralID>>MRCC_PERIPHERAL_PORT_SHIFT_FACTOR], ARG_u8PeripheralID & MRCC_PERIPHERAL_NUM_MASK);
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}
	/*Return Error State*/
	return L_stderrState;
}


void MRCC_voidEnableCSS(void)
{
	SET_BIT(MRCC->CR, MRCC_CR_CSSON);
}

void MRCC_voidDisableCSS(void)
{
	CLEAR_BIT(MRCC->CR, MRCC_CR_CSSON);
}
