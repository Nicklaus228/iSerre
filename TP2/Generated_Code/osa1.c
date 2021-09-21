/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : osa1.c
**     Project     : TP2
**     Processor   : MKL26Z128VLH4
**     Component   : fsl_os_abstraction
**     Version     : Component 1.3.0, Driver 01.00, CPU db: 3.00.000
**     Repository  : KSDK 1.3.0
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-10-26, 17:20, # CodeGen: 0
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file osa1.c
** @version 01.00
*/         
/*!
**  @addtogroup osa1_module osa1 module documentation
**  @{
*/         

/* MODULE osa1. */

#include "osa1.h"

/* Timer period */
#define OSA1_TIMER_PERIOD_US           1000U
/* Software ISR counter */
static volatile uint16_t SwTimerIsrCounter = 0U;

/*
** ===================================================================
**     Method      :  HWTIMER_SYS_TimerIsr (component fsl_os_abstraction)
**
**     Description :
**         Interrupt service routine.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void SysTick_Handler(void)
{
  SwTimerIsrCounter++;
}

/*
** ===================================================================
**     Method      :  OSA_TimeInit (component fsl_os_abstraction)
**
**     Description :
**         This function initializes the timer used in BM OSA, the 
**         functions such as OSA_TimeDelay, OSA_TimeGetMsec, and the 
**         timeout are all based on this timer.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void OSA_TimeInit(void)
{
  uint64_t divider;
  
  /* Disable timer and interrupt */
  SysTick->CTRL = 0U;
  /* A write of any value to current value register clears the field to 0, and also clears the SYST_CSR COUNTFLAG bit to 0. */
  SysTick->VAL = 0U;    
#if FSL_FEATURE_SYSTICK_HAS_EXT_REF
    /* Set the clock source back to core freq */
    CLOCK_SYS_SetSystickSrc(kClockSystickSrcCore);
#endif  
  /* Get SysTick counter input frequency and compute divider value */  
  divider = ((((uint64_t)CLOCK_SYS_GetSystickFreq() * OSA1_TIMER_PERIOD_US)) / 1000000U);
  assert(divider != 0U);
  /* Set divide input clock of systick timer */
  SysTick->LOAD = (uint32_t)(divider - 1U);
  /* Set interrupt priority and enable interrupt */  
  NVIC_SetPriority(SysTick_IRQn, 1U);
  /* Run timer and enable interrupt */
  SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);  
}

/*
** ===================================================================
**     Method      :  OSA_TimeDiff (component fsl_os_abstraction)
**
**     Description :
**         This function gets the difference between two time stamp, time 
**         overflow is considered.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
uint32_t OSA_TimeDiff(uint32_t time_start, uint32_t time_end)
{
  if (time_end >= time_start) {
    return time_end - time_start;
  } else {
    /* Sw ISR counter is 16 bits. */
    return 0xFFFFUL - time_start + time_end + 1;
  }
}

/*
** ===================================================================
**     Method      :  OSA_TimeGetMsec (component fsl_os_abstraction)
**
**     Description :
**         This function gets current time in milliseconds.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
uint32_t OSA_TimeGetMsec(void)
{
  return (SwTimerIsrCounter);
}

/* END osa1. */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
