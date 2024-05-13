
 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
Â© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

//---------------------
// INITIALIZATION
//---------------------
#include "mcc_generated_files/system/system.h"
#include<stdio.h>
#include<stdlib.h>
#include <xc.h>
#include "LcdInterfaceConfig.h"


//#define PIC1
#ifndef PIC1
#define PIC2
#endif // PIC1


// Define macros for GPIO ports
#define BUTTON_PORT PORTA
#define BUTTON_TRIS TRISA

#define KEYPAD_PORT PORTB
#define KEYPAD_TRIS TRISB

#define MOTION_PORT PORTC
#define MOTION_TRIS TRISC


// Define macros for GPIO ports
#define SEGDIS_PORT  PORTA
#define SEGDIS_TRIS  TRISA
#define SEGDIS_LAT   LATA
#define SEGDIS_ANSEL ANSELA


// Define macros for keypad buttons
#define ROWS 4
#define COLS 4

//This is not used but it is a good visual of the keypad
const char keypad[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//This is equivalent to the one above but is in decimal.
//This is used
const int keypad_dec[ROWS][COLS] = {
    {1, 2, 3, 10},
    {4, 5, 6, 11},
    {7, 8, 9, 12},
    {14, 0, 15, 13}
};

//This is equivalent to the one above but is in hex-decimal.
//This is used
const int keypad_dis[ROWS][COLS] = {
    {0x06, 0x5B, 0x4F, 0x77},
    {0x66, 0x6D, 0x7D, 0x7C},
    {0x07, 0x7F, 0x67, 0x39},
    {0x79, 0x3f, 0x71, 0x5E}
};


//---------------------
// FUNCTIONS
//---------------------
int readKeypad();
int readButtons();
int readMotion();
void displayResult(int result);
void configure_ports();
void delay(int num1, int num2);
void motor(int num1, int num2);
void buzzer(int num1, int num2);
void melody(int num1, int num2);

// Global variables
volatile uint8_t X_Input_REG = 0; //first input
volatile uint8_t Y_Input_REG = 0; //second input
volatile char Operation_REG = ' '; // operand var
volatile int Display_Result_REG = 0; // Result value to be placed on LEDs
volatile int button_pressed = 0; // Used to wait for button to be depressed

int keypad_dec_to_seg(int input_val);
void updateSegDis(int input_val);

int reset = 0; // reset flag if "*" is pressed
int key1 = 0; // stores the first input
int op = 0; // stores operand
int key2 = 0; // stores the second input
int result = 0; // result flag if "#" is pressed
int button1 = 0; // stores the first input
int motion1 = 0; // stores the first input
int uart1_char = 0;
int uart2_char = 0;
char data[10];
int SEGDIS_bit7;
int SEGDIS_bit6;
int SEGDIS_bit5;
int SEGDIS_bit4;
int SEGDIS_bit3;
int SEGDIS_bit2;
int SEGDIS_bit1;
int SEGDIS_bit0;
int SEGDIS_val;

//Placeholder if we want to add delays
//Didn't need to use it
void delay(int num1, int num2) {
   int cnt1, cnt2;
   for (cnt1=0; cnt1<num1; cnt1++)
       for (cnt2=0; cnt2<num2; cnt2++)
       {}
}

//Reads the keypad
int readKeypad() {
    int row, col; 
    int keypad_val;
    
    //Cycle through the columns
    for (col = 0; col < COLS; col++) {
        //clear all columns
        //KEYPAD_PORT = ((0b00000000) & 0b11110000);
        PORTBbits.RB4 = 0;
        PORTBbits.RB5 = 0;
        PORTBbits.RB6 = 0;
        PORTBbits.RB7 = 0;
        delay(1, 10);
        
        //Set only 1 column based on COL variable
        //KEYPAD_PORT = ((0b00010000 << col) & 0b11110000);
        PORTBbits.RB4 = (col == 0);
        PORTBbits.RB5 = (col == 1);
        PORTBbits.RB6 = (col == 2);
        PORTBbits.RB7 = (col == 3);
        delay(1, 10);
        
        //Read keypad rows 
        keypad_val = KEYPAD_PORT & 0b00001111;
        
        //Cycle through the rows
        for (row = 0; row < ROWS; row++) {
            //check to see if row is detected
            if ((keypad_val) == (0b00000001 << row)) {
                //Detected button pressed
                //LED_PORT = ((keypad_dec[row][col]) & 0b00001111); // DEBUG
                button_pressed = 1;
                
                //KEYPAD_PORT = 0b11110000;
                delay(1,10);
                
                // Wait for button to be depressed
                while (button_pressed == 1) { 
                    keypad_val = KEYPAD_PORT & 0b00001111;
                    //if keypad is zero then the button was depressed
                    if (keypad_val == 0) {
                        button_pressed = 0;
                    }
                    delay(1,10);
                }
                
                delay(1,10);
                //returns keypad decimal value
                return keypad_dec[row][col];
            }
        }
    }
    //reserved 16 as no button pressed
    return 16; // No key pressed
}
//Reads the buttons
int readButtons() {

        if(PORTAbits.RA0 == 1){
            while (PORTAbits.RA0 == 1) {}
            return 20;
        }
        else if (PORTAbits.RA1 == 1){
            while (PORTAbits.RA1 == 1) {}
            return 21;
        }
        else if (PORTAbits.RA2 == 1){
            while (PORTAbits.RA2 == 1) {}
            return 22;
        }
        else if (PORTAbits.RA3 == 1){
            while (PORTAbits.RA3 == 1) {}
            return 23;
        }
        return 24;
}
    
//Reads the MotionDetector
int readMotion() {
        int cnt1;
        int cnt2;
        int cnt3;
        int cnt4;
        int motion1_cnt;
        int motion2_cnt;
        int motion3_cnt;
        int motion4_cnt;
        int diff_cnt;
        
        delay(1,10);
        PORTAbits.RA5 = 0;
        delay(1,10);
        PORTAbits.RA5 = 1;
        delay(1,10);
        PORTAbits.RA5 = 0;    
        cnt1 = 0;
        cnt2 = 0;
        while ((PORTAbits.RA4 == 0) && (cnt1 < 10000)) {
            cnt1++;
            //delay(1,10);
        }
        while ((PORTAbits.RA4 == 1) && (cnt2 < 10000)) {
            cnt2++;
            //delay(1,10);
        }
        //printf("cnt1 = %d \r\n", cnt1);
        //printf("cnt2 = %d \r\n", cnt2);
        
        delay(1,10);
        PORTAbits.RA5 = 0;
        delay(1,10);
        PORTAbits.RA5 = 1;
        delay(1,10);
        PORTAbits.RA5 = 0;    
        cnt3 = 0;
        cnt4 = 0;
        while ((PORTAbits.RA4 == 0) && (cnt3 < 10000)) {
            cnt3++;
            //delay(1,10);
        }
        while ((PORTAbits.RA4 == 1) && (cnt4 < 10000)) {
            cnt4++;
            //delay(1,10);
        }
        //printf("cnt3 = %d \r\n", cnt3);
        //printf("cnt4 = %d \r\n", cnt4);

        motion1_cnt = cnt1;
        motion2_cnt = cnt2;
        motion3_cnt = cnt3;
        motion4_cnt = cnt4;
       
        if (motion2_cnt == motion4_cnt) {
            return 31;
        }
        if (motion2_cnt > motion4_cnt) {
            if (motion2_cnt > motion4_cnt + 10) {
                return 30;
            }
        }
        if (motion4_cnt > motion2_cnt) {
            if (motion4_cnt > motion2_cnt + 10) {
                return 30;
            }
        }
        return 31;
}
    

void displayResult(int result) {
    // Convert result to binary and display on LEDs
    //LED_PORT = (unsigned char) result;
    //delay(1,10);
}

int keypad_dec_to_seg(int input_val) {
    if (input_val == 0)  { return 0x3f; }
    if (input_val == 1)  { return 0x06; }
    if (input_val == 2)  { return 0x5B; }
    if (input_val == 3)  { return 0x4F; }
    if (input_val == 4)  { return 0x66; }
    if (input_val == 5)  { return 0x6D; }
    if (input_val == 6)  { return 0x7D; }
    if (input_val == 7)  { return 0x07; }
    if (input_val == 8)  { return 0x7F; }
    if (input_val == 9)  { return 0x67; }
    if (input_val == 10) { return 0x77; }
    if (input_val == 11) { return 0x7C; }
    if (input_val == 12) { return 0x39; }
    if (input_val == 13) { return 0x5E; }
    if (input_val == 14) { return 0x79; }
    if (input_val == 15) { return 0x71; }
    return 0x00;
}

void updateSegDis(int input_val) {
    // Convert result to binary and display on LEDs
    //SEGDIS_PORT = (unsigned char) input_val;
    SEGDIS_bit7 = (((input_val >> 7) & 0x01) == 0);
    SEGDIS_bit6 = (((input_val >> 6) & 0x01) == 0);
    SEGDIS_bit5 = (((input_val >> 5) & 0x01) == 0);
    SEGDIS_bit4 = (((input_val >> 4) & 0x01) == 0);
    SEGDIS_bit3 = (((input_val >> 3) & 0x01) == 0);
    SEGDIS_bit2 = (((input_val >> 2) & 0x01) == 0);
    SEGDIS_bit1 = (((input_val >> 1) & 0x01) == 0);
    SEGDIS_bit0 = (((input_val >> 0) & 0x01) == 0);
    SEGDIS_PORT = ((SEGDIS_bit7 & 0x01) << 7) |
            ((SEGDIS_bit6 & 0x01) << 6) |
            ((SEGDIS_bit5 & 0x01) << 5) |
            ((SEGDIS_bit4 & 0x01) << 4) |
            ((SEGDIS_bit3 & 0x01) << 3) |
            ((SEGDIS_bit2 & 0x01) << 2) |
            ((SEGDIS_bit1 & 0x01) << 1) |
            ((SEGDIS_bit0 & 0x01) << 0) ;
    SEGDIS_LAT = ((SEGDIS_bit7 & 0x01) << 7) |
            ((SEGDIS_bit6 & 0x01) << 6) |
            ((SEGDIS_bit5 & 0x01) << 5) |
            ((SEGDIS_bit4 & 0x01) << 4) |
            ((SEGDIS_bit3 & 0x01) << 3) |
            ((SEGDIS_bit2 & 0x01) << 2) |
            ((SEGDIS_bit1 & 0x01) << 1) |
            ((SEGDIS_bit0 & 0x01) << 0) ;
    delay(1,10);
}

void motor(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1=0; cnt1<num1; cnt1++) {
       for (cnt2=0; cnt2<num2; cnt2++) {
           PORTCbits.RC0 = ((cnt2 < 1)) ;
       }
    }
}

void buzzer(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1=0; cnt1<num1; cnt1++) {
       for (cnt2=0; cnt2<num2; cnt2++) {
           PORTCbits.RC0 = cnt1 & 0b00000001;
       }
    }
}

void melody(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1=0; cnt1<num1; cnt1++) {
        buzzer(10,1000);
        delay (10,1000);
        buzzer(4,1000);
        delay (4,1000);
        buzzer(20,1000);
        delay (4,1000);
    }
}


/////////////////////////////////////////////
//CONFIGURE PORTS
void configure_ports_pic1() {
    // Initialize ports for keypad and LEDs
    PORTA = 0;
    LATA = 0;
    ANSELA = 0;
    BUTTON_TRIS = 0b00011111; // Set all pins as output
    
    PORTB = 0;
    LATB = 0;
    ANSELB = 0;
    KEYPAD_TRIS = 0b00001111; // Set first 4 pins as input
    
    PORTC = 0;
    LATC = 0;
    ANSELC = 0;
    MOTION_TRIS = 0b00001001; // Set all pins as output
}

void configure_ports_pic2() {
    // Initialize ports for 7-SEGMENT DISPLAY
    SEGDIS_PORT = 0;
    SEGDIS_LAT = 0;
    SEGDIS_ANSEL = 0;
    SEGDIS_TRIS = 0b00000000; // Outputs for 7 Segment Display
    
    PORTC = 0;
    LATC = 0;
    ANSELC = 0;
    TRISC = 0b00000000; // Set all pins as output
}

int main(void)
{
    SYSTEM_Initialize();
    
#ifdef PIC1
    configure_ports_pic1();
#endif // PIC1
    
#ifdef PIC2
    configure_ports_pic2();
#endif // PIC2
    
    UART2_Initialize();
    UART1_Initialize();
    
#ifdef PIC2
    LCD_Init();
    LCD_Clear();
    updateSegDis(0x00);
#endif // PIC2
    
    key1 = 0;
    button1 = 0;
    motion1 = 0;
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
    //INTERRUPT_GlobalInterruptEnable(); 

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 

    printf("\n\nStart Main Loop \r\n");
    
    while(1)
    {
#ifdef PIC1
        key1 = readKeypad();
        if (key1 < 16){
            printf("Read Keypad = %d \r\n", key1);
              
            if((UART1_IsTxReady())) {
                UART1_Write(key1);
                printf("Send to UART1_TX keypad =%d \r\n", key1);
            }
        }
        
        button1 = readButtons();
        if ((button1 >= 20) && (button1 < 24)) {
            printf("Read Buttons = %d \r\n", button1);
              
            if((UART1_IsTxReady())) {
                UART1_Write(button1);
                printf("Send to UART1_TX button =%d \r\n", button1);
            }
        }
        motion1 = readMotion();
        if ((motion1 >= 30) && (motion1 < 31)) {
            printf("Read Motion = %d \r\n", motion1);
                   
            if((UART1_IsTxReady())) {
                UART1_Write(motion1);
                printf("Send to UART1_TX motion =%d \r\n", motion1);
            }
        }
#endif // PIC1
        
        if((UART2_IsRxReady())) {
            uart2_char = UART2_Read();
            printf("uart2_char =%d \r\n", uart2_char);

            if((UART1_IsTxReady())) {
                UART1_Write(uart2_char);
                printf("Send to UART1_TX uart2_char =%d \r\n", uart2_char);
            }

#ifdef PIC2
            sprintf(data,"%d",uart2_char);
            LCD_String_xy(1,0,"UART2_CHAR:"); 
            LCD_String_xy(2,0,data);/*Send string data for printing*/
            //MSdelay(1000);  
#endif // PIC2  
        }
        
        if((UART1_IsRxReady())) {
            uart1_char = UART1_Read();
            printf("Receive from UART1_RX uart1_char =%d \r\n", uart1_char);
            
#ifdef PIC2
            //updateSegDis(keypad_dec_to_seg(0));
            if(uart1_char < 16) {
                SEGDIS_val = keypad_dec_to_seg(uart1_char);
                updateSegDis(SEGDIS_val);
            }
            
            if(uart1_char == 15 ) {
                melody(5,1000);
            }

#endif // PIC2
        }

    }
    
}
