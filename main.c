// ******************************************************************************************* //
//
// File:         lab1p1.c
// Date:         
// Authors:      
//
// Description: Part 1 for lab 1
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "leds.h"
#include "interrupt.h"
#include "switch.h"
#include "timer.h"

#define LEDRUN LATGbits.LATG12  
#define LEDSTOP LATGbits.LATG14
#define EXTERNAL LATAbits.LATA7
#define ON 1
#define OFF 0
#define PRESSED 0

typedef enum stateTypeEnum{
    ledrun,ledstop, wait, wait2, debouncePress, debounceRelease, debounceRelease2
} stateType;

volatile int switchFlag=0;

/* Please note that the configuration file has changed from lab 0.
 * the oscillator is now of a different frequency.
 */
int main(void)
{
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimer1();
    initLEDs();
    initSW2();
    
    
    int PrevState=0; 
    int state=ledrun;
    while(1)
    {
        switch(state){                          //LED 1 is on
              case  ledrun:
                    LEDRUN=ON;
                    LEDSTOP=OFF;
                    PrevState=1;
                    if(switchFlag==1)              //if switch is pressed
                  {
                        switchFlag=0;
                      state=debouncePress;
                  }
                    break;
               case ledstop:                       //LED 2 is on
                    LEDRUN=OFF;
                    LEDSTOP=ON;
                    PrevState=2;
                     if(switchFlag==1)              //if switch is pressed
                  {
                      switchFlag=0;
                      state=debouncePress;
                  }
                    break;    
              case debouncePress:               //calls delay and moves to wait state
                  delayMs(5);
                  state=wait;
                  break;
    
              case debounceRelease:             //calls delay and moves to wait 2 aka logic state
                  delayMs(5);
                  state=wait2;
                  break;
   
                case wait:                      //waits until switch is released
                    if(switchFlag==0)
                    {
                        state=wait;
                    }
                    else
                    {
                        switchFlag=0;
                        state=debounceRelease;
                    }
                    break;
                case wait2:                     //uses flag that is triggerd if switch is held for longer than
                   if(PrevState==1)
                   {
                       state=ledstop;
                   }
                   else if(PrevState==2)
                   {
                       state=ledrun;
                   }
                   break;
                }
    
    }
    
    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL3SRS) _CNInterrupt(void){
    //TODO: Implement the interrupt to capture the press of the button
    PORTA;
    IFS1bits.CNAIF=0;
    switchFlag=1;
}