/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : GPIO               */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 16,2023  */
/* Last Edit : N/A                */
/**********************************/
#ifndef _MGPIO_PRIVATE_H_
#define _MGPIO_PRIVATE_H_

/*GPIO Ports Base Addresses*/
#define GPIOA_BASE_ADDRESS ((volatile u32 *)0x40010800)
#define GPIOB_BASE_ADDRESS ((volatile u32 *)0x40010C00)
#define GPIOC_BASE_ADDRESS ((volatile u32 *)0x40011000)

/*GPIO Structure tyedef*/
typedef struct 
{
	volatile u32 CRL;
	volatile u32 CRH;
	volatile const u32 IDR;
	volatile u32 ODR;
	volatile u32 BSRR;
	volatile u32 BRR;
	volatile u32 LCKR;
}MGPIO_t;

/*Pointers to structures for every GPIO Peripheral*/
#define MGPIOA ((MGPIO_t *)GPIOA_BASE_ADDRESS)
#define MGPIOB ((MGPIO_t *)GPIOB_BASE_ADDRESS)
#define MGPIOC ((MGPIO_t *)GPIOC_BASE_ADDRESS)

/*GPIO Masks*/

/*Pin ID Masks*/
#define MGPIO_PIN_NO_MASK   		         0x0000000F
#define MGPIO_PORT_NO_SHIFT_FACTOR           4

/*Pin Modes Masks*/
#define MGPIO_PIN_MODE_MASK                  0xF
#define MGPIO_PIN_MODE_CONFIG_BITS_WIDTH     4
#define MPGIO_CR_LOW_PINS                    8
#define MGPIO_PIN_ODR_SHIFT_FACTOR           5

/*Pin State mask*/
#define MGPIO_PIN_STATE_MASK                 0x00000001

/*Port COnfiguration Lock Masks*/
#define MGPIO_CONFIG_LOCK_MASK               0x00010000
#define MGPIOx_LCKR_LCKK                     16


#endif
