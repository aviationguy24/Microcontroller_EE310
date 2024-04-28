#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 4000000
#define FCY     _XTAL_FREQ/4

const char seg_code[] __at(0x100) = {0x3f, 0x06, 0x5B, 0x4F};
//Input from the Photoresistors
int first_input = 0;
int second_input = 0;
//the secret code
int mycode = 0x11;
//used for differentiating the input to the secret code
int total_input = 0;
int count;
int s_button = 0;


//Implementing the interrupt
void __interrupt(irq(IRQ_INT0), base(0x4008)) INT0_ISR(void)
{
    if(PIR1bits.INT0IF == 1){
        PIR1bits.INT0IF = 1;
        // If the interrupt switch is enabled, it makes the buzzer beep with a
        //specific melody
        for (int i = 0; i < 2; i++){
            PORTDbits.RD7 = 1;
            __delay_ms(700);
            PORTDbits.RD7 = 0;
            __delay_ms(100);
            PORTDbits.RD7 = 1;
            __delay_ms(100);
            PORTDbits.RD7 = 0;
            __delay_ms(500);
        }
        
    }
   PIR1bits.INT0IF = 0;
}
void check(void){
    
    //For LED to indicate if pic is on 
    PORTBbits.RB7 = 1;      
    
    //initializing the relay to 0
    PORTBbits.RB5 = 0;      
    
    // the main while loop for the program to check the input for PR1 and PR2
    while(1){
        
        // Checks for PR1 circuit input
        if (s_button == 0)
        {

            PORTBbits.RB1 = 1;                  // Turns on input for PR1
            if(PORTBbits.RB2 == 1){
                PORTD = seg_code[count];
                count++;
                __delay_ms(1000);
            }
            if(count == 4)
                count = 0;
        }
        
        
        
        // This checks if the submit button has been pressed
        PORTCbits.RC2 = 1;
        if (PORTCbits.RC3 == 1){
            s_button++;
            if(s_button == 1){
                first_input = count - 1;
                __delay_ms(800);
                count = 0;
                PORTD = 0;
            }
            if (s_button == 2)
                second_input = count - 1;
            __delay_ms(2000);
        }
        PORTCbits.RC2 = 0;
        
        
        // Checks for PR2 circuit input
        if(s_button == 1)
        {
            
            PORTBbits.RB1 = 0;                  // Turn off input for PR1
            PORTBbits.RB4 = 1;                  // Turn on input for PR2
            
            // Checks if the PR2 is set
            if (PORTBbits.RB3 == 1){
                PORTD = seg_code[count];
                count++;
                __delay_ms(1000);
            }
            // Checks for overflow and resets to 0
            if (count == 4)
                count = 0;
        }
            
            // This is a function that Checks if the 2 inputs are equal to 
            //the Secret_Code. If true, the motor will activate. If not, goes
            //back to 0
   
        if (s_button == 2){
            
            PORTBbits.RB4 = 0;                  // Turn off input for PR2
            total_input = (second_input*16) | (first_input);
            
            if (mycode == total_input){
                PORTBbits.RB5 = 1;
                __delay_ms(250);
                PORTBbits.RB5 = 0;
                PORTD = 0;
                count = 0;
                s_button = 0;
            }
            
            // If the input is not the same then the buzzer will activate
            // then resets to 0
            else{
                PORTDbits.RD7 = 1;
                __delay_ms(2000);
                PORTDbits.RD7 = 0;
                PORTD = 0;
                count = 0;
                s_button = 0;
            }
        }
          
    }
}
