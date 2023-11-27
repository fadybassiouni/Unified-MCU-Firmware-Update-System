/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : GPIO               */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 16,2023  */
/* Last Edit : N/A                */
/**********************************/

#include <LBIT_math.h>
#include <LSTD_types.h>

#include <MGPIO_interface.h>
#include <MGPIO_private.h>

static volatile MGPIO_t *MGPIO_pmgpio_tRegisters[3] = {MGPIOA, MGPIOB, MGPIOC};

/*----------------------------------------------------------------------------------------------------------
 * General Purpose I/O Driver works by taking a pin ID as an input and analyzing the input number to get
 * the port that the pin is located in and the pin number out of the 16 pins in that desired port.
 * The Explanation is as follows:
 *
 * The user enters Pin number MGPIO_PIN_B12 which translates to predefined value 28 decimal 
 * and (00000000)(00000000)(00000000)(00<01><1100>) binary, and from that binary number
 * we can see that the 3rd and 4th bit contain the predefined port number 
 * for port B which is 1 in decimal and 00<01>1100 in binary.
 * Now we have the port number by which we can index the desired
 * register whether it is in port A, B, or C. to get the pin number we can see that the first four bits
 * represent the pin number (0001<1100>) which is 12 in decimal. from that we can conclude that a general rule
 *
 * Pin Number (AB) -> (00<AA><BBBB>) where A represents port Number and B represents Pin Number
 *
 * Now we can get the port number by Shifting the pin number to the right 4 times (AB) >> 4
 * and we can get the pin number by masking the pin number by the number 0x0000000F (AB) & 0x0000000F;
 ---------------------------------------------------------------------------------------------------------*/

/*GPIO Function To set the mode for a specific pin*/
STD_error_t MGPIO_stderrSetPinMode(u8 ARG_u8PinID, u8 ARG_u8PinMode)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	/*Obtain Port and Pin number from pin ID*/
	u8 pinNo  = ARG_u8PinID & MGPIO_PIN_NO_MASK;
	u8 portNo = ARG_u8PinID >> MGPIO_PORT_NO_SHIFT_FACTOR;
	
	/*Masking variable to increase code readability and separate masking and config steps*/
	u32 L_u32Mask = 0;

	/*Shifting factor to shift configuration bits according to their width*/
	u8 L_u8shiftFactor = pinNo * MGPIO_PIN_MODE_CONFIG_BITS_WIDTH;

	/*Check for valid user input*/
	if(ARG_u8PinID <= MGPIO_PIN_C15 && (ARG_u8PinMode <= MGPIO_MODE_AF_OPENDRAIN_50MHZ || ARG_u8PinMode == MGPIO_MODE_INPUT_PULLUP))
	{
		if(pinNo<MPGIO_CR_LOW_PINS)
		{
			/*Prepare Mask*/
			L_u32Mask = MGPIO_pmgpio_tRegisters[portNo]->CRL & (~(MGPIO_PIN_MODE_MASK<<L_u8shiftFactor));
			
			/*Configure Pin Mode*/
			MGPIO_pmgpio_tRegisters[portNo]->CRL = L_u32Mask | ((ARG_u8PinMode & MGPIO_PIN_MODE_MASK)<<L_u8shiftFactor);
			

			/*Set Pull State*/
			MGPIO_pmgpio_tRegisters[portNo]->ODR = (MGPIO_pmgpio_tRegisters[portNo]->ODR & (~(1<<pinNo))) | ((ARG_u8PinMode>>MGPIO_PIN_ODR_SHIFT_FACTOR)<<pinNo);
		}
		else
		{
			/*Subtract 32 bits from shift factor because this is a different register and the values are not concurrent*/
			L_u8shiftFactor-=32;

			/*Prepare Mask*/
			L_u32Mask = MGPIO_pmgpio_tRegisters[portNo]->CRH & (~(MGPIO_PIN_MODE_MASK<<L_u8shiftFactor));
			
			/*Configure Pin Mode*/
			MGPIO_pmgpio_tRegisters[portNo]->CRH = L_u32Mask | ((ARG_u8PinMode & MGPIO_PIN_MODE_MASK)<<L_u8shiftFactor);
		
			/*Set Pull State*/
			MGPIO_pmgpio_tRegisters[portNo]->ODR = (MGPIO_pmgpio_tRegisters[portNo]->ODR & (~(1<<pinNo))) | ((ARG_u8PinMode>>MGPIO_PIN_ODR_SHIFT_FACTOR)<<pinNo);
		}
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}



STD_error_t MGPIO_stderrGetPinState(u8 *ARG_pu8PinState, u8 ARG_u8PinID)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	/*Obtain Port and Pin number from pin ID*/
	u8 pinNo  = ARG_u8PinID & MGPIO_PIN_NO_MASK ;
	u8 portNo = ARG_u8PinID >> MGPIO_PORT_NO_SHIFT_FACTOR;

	/*Check for valid user input*/
	if(ARG_u8PinID <= MGPIO_PIN_C15)
	{
		*ARG_pu8PinState = (MGPIO_pmgpio_tRegisters[portNo]->IDR >> pinNo) & MGPIO_PIN_STATE_MASK;
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}

STD_error_t MGPIO_stderrSetPinState(u8 ARG_u8PinID, u8 ARG_u8PinState)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	/*Obtain Port and Pin number from pin ID*/
	u8 pinNo  = ARG_u8PinID & MGPIO_PIN_NO_MASK ;
	u8 portNo = ARG_u8PinID >> MGPIO_PORT_NO_SHIFT_FACTOR;

	/*Check for valid user input*/
	if(ARG_u8PinID <= MGPIO_PIN_C15)
	{
		switch(ARG_u8PinState)
		{
			case MGPIO_LOW : CLEAR_BIT(MGPIO_pmgpio_tRegisters[portNo]->ODR, pinNo); break;
			case MGPIO_HIGH: SET_BIT(MGPIO_pmgpio_tRegisters[portNo]->ODR, pinNo); break;
			default        : L_stderrState = E_NOK; break;
		}
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}

STD_error_t MGPIO_stderrAtomicSetPinState(u8 ARG_u8PinID, u8 ARG_u8PinState)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	/*Obtain Port and Pin number from pin ID*/
	u8 pinNo  = ARG_u8PinID & MGPIO_PIN_NO_MASK ;
	u8 portNo = ARG_u8PinID >> MGPIO_PORT_NO_SHIFT_FACTOR;

	/*Check for valid user input*/
	if(ARG_u8PinID <= MGPIO_PIN_C15)
	{
		switch(ARG_u8PinState)
		{
			case MGPIO_ATOMIC_LOW : SET_BIT(MGPIO_pmgpio_tRegisters[portNo]->BSRR, pinNo); break;
			case MGPIO_ATOMIC_HIGH: SET_BIT(MGPIO_pmgpio_tRegisters[portNo]->BRR,  pinNo); break;
			default               : L_stderrState = E_NOK; break;
		}
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}

STD_error_t MGPIO_stderrLockPortConfig(u8 ARG_u8PortID,u8 ARG_u8LockConfig)
{
	/*State Variable for indication of error state*/
	STD_error_t L_stderrState = E_OK;

	/*Read Variable required for locking sequence*/
	u32 L_u32Reading = 0;

	/*Check for valid user input*/
	if(ARG_u8PortID <= MGPIO_PORTC)
	{
		/*Perform Lock Sequence*/
		MGPIO_pmgpio_tRegisters[ARG_u8PortID]->LCKR = MGPIO_CONFIG_LOCK_MASK | ARG_u8LockConfig;
		MGPIO_pmgpio_tRegisters[ARG_u8PortID]->LCKR = ARG_u8LockConfig;
		MGPIO_pmgpio_tRegisters[ARG_u8PortID]->LCKR = MGPIO_CONFIG_LOCK_MASK | ARG_u8LockConfig;
		L_u32Reading = MGPIO_pmgpio_tRegisters[ARG_u8PortID]->LCKR;

		/*Set State According to Lock bit status*/
		L_stderrState = !(GET_BIT(MGPIO_pmgpio_tRegisters[ARG_u8PortID]->LCKR, MGPIOx_LCKR_LCKK));
	}
	else
	{
		/*State is Not OK*/
		L_stderrState = E_NOK;
	}

	/*Return Error State*/
	return L_stderrState;
}

