/************************************/
/*	Author	  :	Elamir Galal		*/
/*	SWC		  : TIMER				*/
/*  Layer	  : MCAL				*/
/*	Version	  :	1.0		  			*/
/*	Date	  :	November 5, 2023	*/
/*	Last Edit :						*/
/************************************/


#ifndef _MTIMER_PRIVATE_H_
#define _MTIMER_PRIVATE_H_


/*************Timer Base Address************/
#define TIM1_BASE_ADDRESS  0x40010000
#define TIM2_BASE_ADDRESS  0x40000000
#define TIM3_BASE_ADDRESS  0x40000400
#define TIM4_BASE_ADDRESS  0x40000800
#define TIM5_BASE_ADDRESS  0x40000C00
#define TIM9_BASE_ADDRESS  0x40014000 
#define TIM10_BASE_ADDRESS 0x40014400
#define TIM11_BASE_ADDRESS 0x40014800


/*************	Register struct	 *************/
typedef struct 
{
	volatile u32 CR1	  	  ;
	volatile u32 CR2          ;
	volatile u32 SMCR         ;
	volatile u32 DIER         ;
	volatile u32 SR           ;
	volatile u32 EGR          ;
	volatile u32 CCMR1        ;
	volatile u32 CCMR2        ;
	volatile u32 CCER         ;
	volatile u32 CNT          ;
	volatile u32 PSC          ;
	volatile u32 ARR          ;
	volatile u32 Reserved     ;
	volatile u32 CCR1         ;
	volatile u32 CCR2         ;
	volatile u32 CCR3         ;
	volatile u32 CCR4         ;
	volatile u32 Reserved1    ;
	volatile u32 DCR	      ;
	volatile u32 DMAR         ;
	volatile u32 OR_TIM2_TIM5 ;		
}MTIM_t; 


#define TIM3 ((volatile MTIM_t*)0x40000400)

/*************	CR1 Bits	 *************/
#define CEN			0  // Counter Enable bit 
#define UDIS		1  // Update Disable 
#define URS			2  // Update request source
#define OPM			3  // One-pulse mode
#define DIR			4  // Direction
#define CMS0		5  // Center-aligned mode selection
#define CMS1		6  // Center-aligned mode selection
#define ARPE		7  // Auto-reload preload enable
#define CKD0		8  // Clock division
#define CKD1		9  // Clock division

/*************	CR2 Bits	 *************/

#define CCDS		3  // Capture/compare DMA selection
#define MMS0		4  // Master mode selection
#define MMS1		5  // Master mode selection
#define MMS2		6  // Master mode selection
#define TI1S		7  // TI1 selection

/*************	SMCR Bits	 *************/

#define SMS0		0
#define SMS1		1
#define SMS2		2
#define TS0			4
#define TS1			5
#define TS2			6
#define MSM			7
#define ETF0		8
#define ETF1		9
#define ETF2		10
#define ETF3		11
#define ETPS0		12
#define ETPS1		13
#define ECE			14
#define ETP			15
/*************	DIER Bits	 *************/

#define UIE        0  // Update Interrupt Enable
#define CC1IE      1  // Capture/Compare 1 Interrupt Enable
#define CC2IE      2  // Capture/Compare 2 Interrupt Enable
#define CC3IE      3  // Capture/Compare 3 Interrupt Enable
#define CC4IE      4  // Capture/Compare 4 Interrupt Enable
#define TIE        6  // Trigger Interrupt Enable
#define UDE        8  // Update DMA request Enable
#define CC1DE      9  // Capture/Compare 1 DMA request Enable
#define CC2DE      10 // Capture/Compare 2 DMA request Enable
#define CC3DE      11 // Capture/Compare 3 DMA request Enable
#define CC4DE      12 // Capture/Compare 4 DMA request Enable
#define TDE        14 // Trigger DMA request Enable


/*************	SR Bits	 *************/

#define UIF       0  // Update Interrupt Flag
#define CC1IF     1  // Capture/Compare 1 Interrupt Flag
#define CC2IF     2  // Capture/Compare 2 Interrupt Flag
#define CC3IF     3  // Capture/Compare 3 Interrupt Flag
#define CC4IF     4  // Capture/Compare 4 Interrupt Flag
#define TIF       6  // Trigger Interrupt Flag
#define CC1OF     9  // Capture/Compare 1 Overcapture Flag
#define CC2OF     10 // Capture/Compare 2 Overcapture Flag
#define CC3OF     11 // Capture/Compare 3 Overcapture Flag
#define CC4OF     12 // Capture/Compare 4 Overcapture Flag

/*************	EGR Bits	 *************/
#define UG        0  // Update Generation
#define CC1G      1  // Capture/Compare 1 Generation
#define CC2G      2  // Capture/Compare 2 Generation
#define CC3G      3  // Capture/Compare 3 Generation
#define CC4G      4  // Capture/Compare 4 Generation
#define TG        6  // Trigger Generation
#define BG        7  // Break Generation

/*************	CCMR1 Bits	 *************/
#define CC1S0      0  // Capture/Compare 1 selection
#define CC1S1      1  // Capture/Compare 1 selection
#define IC1PSC0	   2
#define IC1PSC1	   3
#define OC1FE	   2
#define OC1PE	   3
#define IC1F0	   4
#define IC1F1      5
#define IC1F2      6
#define IC1F3      7
#define OC1M0      4
#define OC1M1      5
#define OC1M2      6
#define OC1CE      7
#define CC2S0      8
#define CC2S1      9
#define IC2PSC0	   10
#define IC2PSC1	   11
#define OC2FE	   10
#define OC2PE	   11
#define IC2F0	   12	
#define IC2F1	   13
#define IC2F2	   14	
#define IC2F3	   15	
#define OC2M0	   12
#define OC2M1	   13
#define OC2M2	   14
#define OC2CE	   15
/*************	CCMR2 Bits	 *************/
#define CC3S0      0  // Capture/Compare 1 selection
#define CC3S1      1  // Capture/Compare 1 selection
#define IC3PSC0	   2
#define IC3PSC1	   3
#define OC3FE	   2
#define OC3PE	   3
#define IC3F0	   4
#define IC3F1      5
#define IC3F2      6
#define IC3F3      7
#define OC3M0      4
#define OC3M1      5
#define OC3M2      6
#define OC3CE      7
#define CC4S0      8
#define CC4S1      9
#define IC4PSC0	   10
#define IC4PSC1	   11
#define OC4FE	   10
#define OC4PE	   11
#define IC4F0	   12	
#define IC4F1	   13
#define IC4F2	   14	
#define IC4F3	   15	
#define OC4M0	   12
#define OC4M1	   13
#define OC4M2	   14
#define OC4CE	   15
/*************	CCER Bits	 *************/
#define CC1E       0  
#define CC1P       1  
#define CC1NP	   3
#define CC2E	   4
#define CC2P	   5
#define CC2NP      7
#define CC3E       8 
#define CC3P       9
#define CC3NP      11
#define CC4E       12
#define CC4P       13
#define CC4NP      15


#endif
