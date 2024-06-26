/*
 * ---------------------
 * Title: Simple Digital Calculator using Curiosity Board and LED Lights
 * ---------------------
 * Program Details:
 * The purpose of this program is to make a Digital Calculator using Pic18,
 * C-Programming, and Keypad.
 * Inputs: X_Input_REG and Y_Input_REG, 
 * each input can be a number between 1 - 9
 * Outputs: Display_Result_REG
 * 
 * 
 * Setup: The Curiosity Board, Keypad Device, and 8 LEDs
 * Date: Apr 06, 2024
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * Compiler: xc8, 2.6
 * Author: Aaron John Estrada
 * Versions:
 *      V1.0: First Draft
 *      V2.0: Second version with Time Delay
 *      V2.1: Updated with fixed operator function
 *      V2.2: Updated Output and Input ports
 *      V2.3: Updated Output and Input ports
 *      V2.4: Updated with Pragma Headers
 */

// CONFIG1L
#pragma config FEXTOSC = LP     // External Oscillator Selection (LP (crystal oscillator) optimized for 32.768 kHz; PFM set to low power)
#pragma config RSTOSC = EXTOSC  // Reset Oscillator Selection (EXTOSC operating per FEXTOSC bits (device manufacturing default))

// CONFIG1H
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = ON      // PRLOCKED One-Way Set Enable bit (PRLOCK bit can be cleared and set only once)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = ON      // Multi-vector enable bit (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCK bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (ULPBOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG2H
#pragma config BORV = VBOR_2P45 // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 2.45V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF      // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4L
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG4H
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not write-protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)

// CONFIG5L
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)


#include <xc.h> 
#include "C:\Program Files\Microchip\xc8\v2.46\pic\include\proc\pic18f47k42.h"


#define _XTAL_FREQ 4000000 /*Clock Frequency of 4Mhz */
#define FCY    _XTAL_FREQ/4


/* Declarations or Initializations*/
void Program_delay(unsigned int, unsigned int);
int X_Input_REG; /* First Input from the user */
int Y_Input_REG; /* Second Input from the user */
int Operation_Set; /* initiliazation to check if an operation is set */
char Operation_REG; /* Type of Operation saved in this reg */
int Display_Result_REG;
int Keypad_Input (int); /*Signals from the Keypad Device */
int Operations_check (int); /* check the Arithmetic Operations */
/*M D A S*/
int Multiplication(int, int); /* Multiplication Operation */
int Division(int, int); /* Division Operation */
int Addition(int, int); /* Addition Operation */
int Subtraction(int, int); /* Subtraction Operation */
/* -------------------  */



void main(void) {
   Operation_Set = 0;   /* The register to store the operation
                          * this is to know if there is an operation set
                          *  in the program */
    /* Required Core Design Variables */
    X_Input_REG = 0; // First User Input
    Y_Input_REG = 0; // Second User Input
    Operation_REG = 0; //Where selected Operation is stored
    Display_Result_REG = 0; //Where the result of the application
                            //will be stored
    /*  Didn't work
    //Initializing Outputs [Port B]
    PORTB = 0; 
    LATB = 0; 
    ANSELB = 0; 
    TRISB = 0b11110000;   //Initialize PORTB
    //Set bits [7,6,5,4] in PORTB as inputs
    */
    
    //Initializing inputs [Port B]
    PORTB = 0; 
    LATB = 0; 
    ANSELB = 0; 
    TRISB = 0b11011000;   //Initialize PORTB
    //Set bits [7,6,4,3] in PORTB as inputs
    
    /* Didn't work due to lack of voltage
    //Initializing Outputs [Port A]
    PORTA = 0;
    LATA = 0; 
    ANSELA = 0; 
    TRISA = 0; 
     */
    
    //Initializing Outputs [Port D]
    PORTD = 0;
    LATD = 0; 
    ANSELD = 0; 
    TRISD = 0;     
    
    
    while(1){ //While loop
        //Checks for the first User Input from Keypad
        if (PORTD == 0){
            X_Input_REG = Keypad_Input(X_Input_REG);
            if (X_Input_REG > 0){
                PORTD = 1;
                //Program_delay(100,100);
            }
        }
        
        
        //it verifies the operation input from the keypad
        if (PORTD == 1){
            Operation_REG = Operations_check(Operation_REG);//Assigns keypad op 
                                            //input to Operation_REG
            Operation_Set = Operation_REG; 
        }
        if (Operation_Set > 0){
            PORTD = 3;
            Operation_Set = 0;
            //Program_delay(100,100);
        }
        
        //Checks for the Second User Input from Keypad
        if (PORTD == 3){
            Y_Input_REG = Keypad_Input(Y_Input_REG); //Assigns keypad  
                                            //input to Y input
            if (Y_Input_REG > 0){ //check if it's a valid input
                PORTD = 2; //if so, set PortD = 2
               // Program_delay(100,100);
            }
            if (PORTD > 0){
                PORTBbits.RB1 = 1;  
                if (PORTBbits.RB6 == 1){ 
                    PORTD = 0;
                    X_Input_REG = 0;
                    Y_Input_REG = 0;
                    Operation_REG = 0;
                }
                PORTBbits.RB1 = 0;
            }
        }
        
        
        /* It Computes the operation using the X_Input and Y_Input values */
        if (PORTD == 2){
        
            if (Operation_REG == 1){ //goes to Addition function

                Display_Result_REG = Addition(X_Input_REG, Y_Input_REG);
            }
            else if (Operation_REG == 2){ //goes to subtraction function

                Display_Result_REG = Subtraction(X_Input_REG, Y_Input_REG);
            }
            else if (Operation_REG == 3){ //goes to Multiplication function

                Display_Result_REG = Multiplication(X_Input_REG, Y_Input_REG);
            }
            else if (Operation_REG == 4){ //goes to division function

                Display_Result_REG = Division(X_Input_REG, Y_Input_REG);
            }
            
            if (PORTD == 2){
                PORTBbits.RB2 = 1;
                if (PORTBbits.RB6 == 1){
                    PORTD = Display_Result_REG; //outputs the results
                    //Program_delay(100,100);
                }
                PORTBbits.RB2 = 0;
            }
        }
        
        
        // resets after the an operation
        if (PORTD == 2)
        {
            PORTBbits.RB1 = 1;
            if (PORTBbits.RB6 == 1){
                PORTD = 0; //resets
                Operation_REG = 0;
                Y_Input_REG = 0;
                X_Input_REG = 0;
            }
            PORTBbits.RB1 = 0;
        }
    }
        
        
} 


// Delay function of the Program
/*void Program_delay(unsigned int inner, unsigned int outer)
{
    
    unsigned int i; unsigned int j;
    for (i = 0; i <inner ;  i++) //Inner LOOP
        for (j = 0 ; j<outer ; j++); //Outer LOOP
}    
*/
// Multiplication Function
int Multiplication (int X3, int Y3)
{
    int Multiply;
    Multiply = X3 * Y3;
    return Multiply;
}

// Division Function
int Division (int X4, int Y4)
{
    int Divide;
    Divide = X4 / Y4;
    return Divide;
}

// Addition Function
int Addition (int X1, int Y1)
{
    int add;
    add = X1 + Y1;
    return add;
}

// Subtraction Function
int Subtraction (int X2, int Y2)
{
    int Subtract;
    Subtract = X2 - Y2;
    return Subtract;
}


// Operation check function
int Operations_check(int what_op){
    what_op = 0;
    if (what_op == 0){
        PORTBbits.RB5 = 1;
        if (PORTBbits.RB3 == 1)
            what_op = 1; //addition
        if (PORTBbits.RB4 == 1)
            what_op = 2; //subtraction
        if (PORTBbits.RB7 == 1)
            what_op = 3; //multiplication 
        if (PORTBbits.RB6 == 1)
            what_op = 4; //division
        PORTBbits.RB5 = 0;
    }
    return what_op;
}

// Input check function
int Keypad_Input(int what_button){
    what_button = 0;
    if (what_button == 0){
        PORTBbits.RB0 = 1;
        if (PORTBbits.RB3 == 1)
            what_button = 1; //1 is pressed
        else if (PORTBbits.RB4 == 1)
            what_button = 4; //4 is pressed
        else if (PORTBbits.RB7 == 1)
            what_button = 7; //7 is pressed
        PORTBbits.RB0 = 0;
        }
        
    if (what_button == 0){
        PORTBbits.RB1 = 1;
        if (PORTBbits.RB3 == 1)
            what_button = 2; //2 is pressed
        else if (PORTBbits.RB4 == 1)
            what_button = 5; //5 is pressed
        else if (PORTBbits.RB7 == 1)
            what_button = 8; //8 is pressed
        PORTBbits.RB1 = 0;
        }
        
    if (what_button == 0){
        PORTBbits.RB2 = 1;
        if (PORTBbits.RB3 == 1)
            what_button = 3; //3 is pressed
        else if (PORTBbits.RB4 == 1)
            what_button = 6; //6 is pressed 
        else if (PORTBbits.RB7 == 1)
            what_button= 9; //9 is pressed
        PORTBbits.RB2 = 0;
        }
    return what_button;
}


