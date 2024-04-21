/*
 * ---------------------
 * Title: Simple Box Vault with Secret Code using Photoresistors
 * ---------------------
 * Program Details:
 * The purpose of this program is to make Box vault that uses photoresistors
 * ,relay, and implement an external interrupt
 * Inputs: PR1, PR2 (Photoresistors inputs)
 * Outputs: Buzzer
 * 
 * 
 * Setup: The Curiosity Board, Keypad Device, relays, and 6 LEDs
 * Date: Apr 21, 2024
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * Compiler: xc8, 2.6
 * Author: Aaron John Estrada
 * Versions:
 *      V1.0: First Draft
 */

#include <xc.h> // must have this
#include "C:\Program Files\Microchip\xc8\v2.46\pic\include\proc\pic18f47k42.h"
#include <stdio.h>

void MSDelay(unsigned int, unsigned int);
unsigned char hex2bcd (unsigned char);

void main(void){
    PORTB = 0x00;
    LATB = 0x00;
    ANSELB = 0x00;
    TRISB = 0x00;
    
    
    PORTD = 0x00;
    LATD = 0x00;
    ANSELD = 0x00;
    TRISD = 0x00; 
    
    TRISC = 0b1111111;
    ANSELC = 0x00;
    PORTC = 0x00;
    int secretCode1 = 0x3, secretCode2 = 0x3;
    unsigned char right = 0x0, left=0x0, counter=0x0, changed=0x0;
    PORTBbits.RB6 =1 ;
    while(1){
        if(PORTCbits.RC0){
            right++;
            MSDelay(25,160);
        }
        if (PORTCbits.RC1){
            left++;
            MSDelay (25, 160);
        }
        if (PORTCbits.RC2){
            left = right = 0;
            if((secretCode1==right) && (secretCode2 ==left)){
                PORTDbits.RD0 = !PORTDbits.RD0;
                MSDelay (25, 160);
                PORTDbits.RD0 = !PORTDbits.RD0;
                MSDelay (25, 160);
                PORTDbits.RD1 = !PORTDbits.RD1;
                MSDelay (25, 160);
                PORTDbits.RD1 = !PORTDbits.RD1;
            }
            else{
                for(char i =0; i<8; i++){
                    PORTBbits.RB6 = !PORTBbits.RB6;
                    MSDelay (25, 160);
                    PORTBbits.RB6 = !PORTBbits.RB6;
                    MSDelay (25, 160);
                }
            }
            counter++;
            changed = hex2bcd(counter);
            PORTB=changed;
            PORTBbits.RB6 = 1;
        }
    }
}


/*void PORT_Init(void) {

    // Initialization  
    // set port B and D as outputs 
    PORTB = 0x00;
    LATB = 0x00;
    ANSELB = 0x00;
    TRISB = 0x00;
    
    
    PORTD = 0x00;
    LATD = 0x00;
    ANSELD = 0x00;
    TRISD = 0x00; 
    
    TRISC = 0b1111111;
    ANSELC = 0x00;
    PORTC = 0x00;
   
    
}*/

void MSDelay(unsigned int itime1, unsigned int itime2){
    unsigned int i; unsigned int j;
    for (i=0; i < itime1; i++)
        for(j = 0; j< itime2; j++);
}

unsigned char hex2bcd (unsigned char x)
{
    unsigned char y;
    y = (x/10) << 4;
    y= y | (x % 10);
    return (y);
}

