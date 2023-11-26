/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : NVIC               */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 18,2023  */
/* Last Edit : N/A                */
/**********************************/
#ifndef _MNVIC_PRIVATE_H_
#define _MNVIC_PRIVATE_H_

/*NVIC Base Address*/
#define NVIC_BASE_ADDRESS ((volatile u32 *)0xE000E100)

/*Registers and Reserved Locations Count in Memory*/
#define MNVIC_ISER_COUNT                3
#define RESERVED0_LOCATIONS_COUNT       29
#define MNVIC_ICER_COUNT                3
#define RESERVED1_LOCATIONS_COUNT       29
#define MNVIC_ISPR_COUNT                3
#define RESERVED2_LOCATIONS_COUNT       29
#define MNVIC_ICPR_COUNT                3
#define RESERVED3_LOCATIONS_COUNT       29
#define MNVIC_IABR_COUNT                3
#define RESERVED4_LOCATIONS_COUNT       61
#define MNVIC_IPR_COUNT                 80
#define RESERVED5_LOCATIONS_COUNT       277

/*Masks and Shift Factors*/
#define MNVIC_REGISTER_SHIFT_FACTOR 	5
#define MNVIC_BIT_NO_MASK           	0x0000001F
#define MNVIC_PRIORITY_SHIFT_FACTOR     4
#define MNVIC_AIRCR_GROUP_MASK          0x0000F8FF
#define MNVIC_AIRCR_VECTKEY0            16
#define MNVIC_AIRCR_PRIGROUP0           8

/*Vector State Key*/
#define MNVIC_VECTKEY                         0x5FA

/*NVIC Registers Structure*/
typedef struct
{
	volatile u32 ISER[MNVIC_ISER_COUNT]; 			    /*From 0xE000E100 to 0xE000E108*/
	volatile u32 RESERVED0[RESERVED0_LOCATIONS_COUNT];  /*From 0xE000E10C to 0xE000E17C*/
	volatile u32 ICER[MNVIC_ICER_COUNT];			    /*From 0xE000E180 to 0xE000E188*/
	volatile u32 RESERVED1[RESERVED1_LOCATIONS_COUNT];  /*From 0xE000E18C to 0xE000E1FC*/
	volatile u32 ISPR[MNVIC_ISPR_COUNT];			    /*From 0xE000E200 to 0xE000E208*/
	volatile u32 RESERVED2[RESERVED2_LOCATIONS_COUNT];  /*From 0xE000E20C to 0xE000E27C*/
	volatile u32 ICPR[MNVIC_ICPR_COUNT];				/*From 0xE000E280 to 0xE000E288*/
	volatile u32 RESERVED3[RESERVED3_LOCATIONS_COUNT];  /*From 0xE000E28C to 0xE000E2FC*/
	volatile const u32 IABR[MNVIC_IABR_COUNT];			/*From 0xE000E300 to 0xE000E308*/
	volatile u32 RESERVED4[RESERVED4_LOCATIONS_COUNT];  /*From 0xE000E30C to 0xE000E3FC*/
	volatile u8  IPR[MNVIC_IPR_COUNT];					/*From 0xE000E400 to 0xE000E420*/
	volatile u32 RESERVED5[RESERVED5_LOCATIONS_COUNT];	/*From 0xE000E424 to 0xE000EDFC*/
	volatile u32 STIR;
}MNVIC_t;

/*Pointers to structure for NVIC Peripheral*/
#define MNVIC ((volatile MNVIC_t *)NVIC_BASE_ADDRESS)

/*SCB Application interrupt and reset control Register*/
#define AIRCR (*((volatile u32 *)0xE000ED0C))


#endif