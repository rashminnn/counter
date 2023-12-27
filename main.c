#include <xc.h>
#include <pic16f887.h>
#include "header.h"
#define dig_ON 0
#define dig_OFF 1

#define _XTAL_FREQ 8000000                                                          // Oscillator frequency

unsigned char dig[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0xf7f,0x6f};         // Hex characters for 0-9 
unsigned char num=00;                                                               //Making a variable to set number  
volatile unsigned char tens,ones;                                                   //Making a varibale for get tens value and ones value                
unsigned char count=0;                                                              //Make a new variable for count  

 
void __interrupt() timer(void);                                                     //call interrupt function
void __interrupt() timer(void)                                                      //Interrupt function
{
     static unsigned char count = 0;                                                //Making count variable as stactic in interrupt function
     TMR0 -= 78;                                                                    //Loading Timer0 load value
         INTCONbits.T0IF = 0;                                                       //Checking Timer0 interrupt flag condition
         
         if (count & 1)                                                             //Check count variable's LSB is 1
         {                                                           
         PORTA = (uint8_t)((dig_OFF << 1) | (dig_ON << 0));                         //Turning on the 2nd display  (10)
         PORTD = dig[tens];                                                         //Showing tens value in PORT D
         }
         else
        {
        PORTA = (uint8_t)((dig_ON << 1) | (dig_OFF << 0));                          //Turning on the second display (01)
        PORTD = dig[ones];                                                          //Showing ones value in PORT D
        }
         count++;                                                                   //Incrementing the count
}


void main(){
            ANSEL=0x00;                                                             //Making digital I/O in ANSEL register [AN0-AN7]
            ANSELH=0x00;                                                            //making Digital I/O in higher order ANSELH register [AN8-AN15]
            OPTION_REGbits.T0CS=0;                                                  //Timer0 clock source use internal instruction cycle clock
            OPTION_REGbits.PSA=0;                                                   //Prescaler assigning to TMR0 
            OPTION_REGbits.PS=7;                                                    //Prescaler set to 7
            INTCONbits.GIE=1;                                                       //Enable Global Interrupt Enabler
            INTCONbits.PEIE=1;                                                      //Peripheral Interrupt enabler
            INTCONbits.T0IE=1;                                                      //Timer0 interrupt enable
            INTCONbits.T0IF=0;                                                      //Timer0 Interrupt flag
            
            OPTION_REG=0x07;                                                      
            TMR0=178;                                                               //Load Timer0 load value 
            PORTA=0x00;                                                             //Clear Ports A & D
            PORTD=0x00;
            TRISA =0x00;                                                            //set Ports A&D as outputs
            TRISD=0x00;                                                          
    while(1){
        
        tens=num/10;                                                                //calculating the digit for tens    
        ones=num%10;                                                                //calculating the digit for ones 
        num++;                                                                      //increment the number
        if(num==100)
        {
            num=0;                                                                  //if number reach to 100 reset the counter value
        }
        __delay_ms(1000);                                                           //1000ms delay
     
    }
}