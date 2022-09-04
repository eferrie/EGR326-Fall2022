#include "msp.h"
#include <stdio.h>
#include <math.h>

#define RED 0
#define GREEN 1
#define BLUE 2

//define the debounce time
#define DEBOUNCE_TIME 30

void inital_setup(void);
void delay_ms(uint8_t delay_time);//delay for ms time
void delay_micro(uint8_t delay_time);//delay for microsecond time
void turn_RGB_on(uint8_t color);
uint8_t debounceSwitch(uint8_t pinNum);
enum mode{
    start,
    red,
    green,
    blue
} RGB = start;

//defining a button pressed state
uint8_t buttonPressed = 0;

int main(void){
    inital_setup();
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    //setup interrupts for buttons on port 3
    NVIC_EnableIRQ(PORT3_IRQn);//using pins on port 3 only
    __enable_interrupt();//starting to write interrupts now


    while(1){
            P2->OUT &= ~(BIT0|BIT1|BIT2);
            P2->OUT |= BIT(RGB);//put any of the colors as debounced color
        //for the other button is pressed
    }
    //need to implement some type of debouncing method to the button inputs
}

void inital_setup(void){
    //two button setup
    P3->SEL0 &= ~(BIT5|BIT6);//set as GPIO
    P3->SEL1 &= ~(BIT5|BIT6);//set as GPIO
    P3->DIR &= ~(BIT5|BIT6);//configure to input
    P3->REN |= (BIT5|BIT6);//enable resistor
    P3->OUT |= (BIT5|BIT6);//set initial value to 1

    //button interrupt
    P3 -> IE |= (BIT5|BIT6);  //interrupt enabled
    P3 -> IES |= (BIT5|BIT6); //interrupt high to low
    P3 -> IFG &= ~(BIT5|BIT6); //clear the interrupt flag
    //RGB setup  R=0 G=1 B=2
    P2->SEL0 &= ~(BIT0|BIT1|BIT2);//gpio mode
    P2->SEL1 &= ~(BIT0|BIT1|BIT2);
    P2->DIR  |= (BIT0|BIT1|BIT2);//configure to output
    P2->REN |= ~(BIT0|BIT1|BIT2);//enable resistor
    P2->OUT &= ~(BIT0|BIT1|BIT2);//initial value to zero
}
void turn_RGB_on(uint8_t color){
    P4->OUT |= 0b00000001 << color;
}


extern void PORT3_IRQHandler(){

    //add button debounce sequence here? location of is still pretty funky

    if(P3->IFG & BIT5){//IF BUTTON BLUE IS PRESSSED
        if(debounceSwitch(5)){
            if(RGB != blue){
                RGB++;
            }
            else{
                RGB = red;
            }
        }
    }
    if(P3->IFG & BIT6){//IF BUTTON BLACK IS PRESSSED
        if(debounceSwitch(6)){
            if(RGB != red){
                RGB--;
            }
            else{
                RGB = blue;
            }
        }
    }
    P3-> IFG = 0;

}

uint8_t debounceSwitch(uint8_t pinNum){
      static uint16_t State = 0;             // Current debounce status

      State=(State<<1) | (P3->IN &BIT(pinNum))>>1 | 0xfc00;
      __delay_cycles(DEBOUNCE_TIME*100);
      if(State==0xfc00)return 1;            // indicates 0 level is stable for 10 consecutive calls
    return 0;
}


