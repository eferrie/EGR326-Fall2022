#include "msp.h"
#include <stdio.h>
#include <math.h>

//define the debounce time
#define DEBOUNCE_TIME 30

void inital_setup(void);
void delay_ms(uint8_t delay_time);//delay for ms time
void delay_micro(uint8_t delay_time);//delay for microsecond time

int main(void){

    //need to implement some type of debouncing method to the button inputs
    inital_setup();
    //checking for the first button
    if(!(P3->IN & BIT5)){
                //Wait before checking again
                delay_ms(DEBOUNCE_TIME);
                if(!(P3->IN & BIT5)){
                delay_ms(DEBOUNCE_TIME);
                while(!(P3->IN & BIT5));//wait til button is released
                }
    }
    return 0;
}

void inital_setup(void){
    //two button setup
    P3->SEL0 &= ~(BIT5|BIT6);//set as GPIO
    P3->SEL1 &= ~(BIT5|BIT6);//set as GPIO
    P3->DIR &= ~(BIT5|BIT6);//configure to input
    P3->REN |= (BIT5|BIT6);//enable resistor
    P3->OUT |= (BIT5|BIT6);//set initial value to 1
}

void delay_ms(uint8_t delay_time){
        SysTick->LOAD = (delay_time*3000) - 1; //to get ms
        SysTick->VAL = 'q'; //STCVR - Any value clears
        SysTick->CTRL = 0b1; //STCSR - Enable SysTick
        while(!(SysTick->CTRL&BIT(16))); //0b10000000000000000
}

void delay_micro(uint8_t delay_time){
        SysTick->LOAD = (delay_time*3) - 1; //to get us
        SysTick->VAL = 'a'; //STCVR - Any value clears
        SysTick->CTRL = 0b1; //STCSR - Enable SysTick
        while(!(SysTick->CTRL&BIT(16))); //0b10000000000000000
}
