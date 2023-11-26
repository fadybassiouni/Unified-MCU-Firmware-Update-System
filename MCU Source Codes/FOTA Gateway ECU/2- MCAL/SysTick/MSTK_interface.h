/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : SysTick            */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 18,2023  */
/* Last Edit : N/A                */
/**********************************/
#ifndef _STK_INTERFACE_H
#define _STK_INTERFACE_H

#define MSTK_CLOCKSRC_AHB_8  0
#define MSTK_CLOCKSRC_AHB    1

#define MSTK_SINGLE_INTERVAL    0
#define MSTK_PERIOD_INTERVAL    1

STD_error_t MSTK_stderrInit(u8 ARG_u8ClockSource);
STD_error_t MSTK_stderrStartSingle(u32 ARG_u32Ticks, void (*ARG_pvoidfuncUserFunction)(void));
STD_error_t MSTK_stderrStartPeriodic(u32 ARG_u32Ticks, void (*ARG_pvoidfuncUserFunction)(void));
void MSTK_voidBusyWait(u32 ARG_u32Ticks);
void MSTK_voidStop(void);
u32 MSTK_u32GetElapsedTime(void);
u32 MSTK_u32GetRemainingTime(void);

#endif
