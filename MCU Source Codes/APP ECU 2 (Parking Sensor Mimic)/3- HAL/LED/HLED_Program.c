/**
 * @file HLED_program.c
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
#include "LSTD_Types.h"
#include "LBIT_Math.h"
#include "MGPIO_Interface.h"
#include "MRCC_interface.h"
#include "HLED_Interface.h"






/**
 * @brief Initializes the specified LED pin.
 * @param ARG_u8Pin The pin number to initialize as an LED.
 */
void HLED_voidLedInit(u8 ARG_u8Pin)
{
	MGPIO_stderrSetPinMode(ARG_u8Pin, MGPIO_MODE_OUTPUT_PUSHPULL_2MHZ);
}
/**
 * @brief Turns on the specified LED pin.
 * @param ARG_u8Pin The pin number of the LED to turn on.
 */
void HLED_voidLedOn(u8 ARG_u8Pin)
{
	MGPIO_stderrSetPinState(ARG_u8Pin, MGPIO_HIGH);
}
/**
 * @brief Turns off the specified LED pin.
 * @param ARG_u8Pin The pin number of the LED to turn off.
 */
void HLED_voidLedOff(u8 ARG_u8Pin)
{
	MGPIO_stderrSetPinState(ARG_u8Pin, MGPIO_LOW);
}
/**
 * @brief Toggles the state of the specified LED pin.
 * @param ARG_u8Pin The pin number of the LED to toggle.
 */
void HLED_voidLedToggle(u8 ARG_u8Pin)
{
	MGPIO_stderrSetPinState(ARG_u8Pin, MGPIO_TOGGLE);
}
