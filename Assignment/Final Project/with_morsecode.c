#include "mcc_generated_files/system/system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//---------------------
// INITIALIZATION
//---------------------
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

const int keypad_dec[ROWS][COLS] = {
    {1, 2, 3, 10},
    {4, 5, 6, 11},
    {7, 8, 9, 12},
    {14, 0, 15, 13}
};

const int morseToKey[26][5] = {
    {1,2},       // A
    {2,1,1,1},   // B
    {2,1,2,1},   // C
    {2,1,1},     // D
    {1},         // E
    {1,1,2,1},   // F
    {2,2,1},     // G
    {1,1,1,1},   // H
    {1,1},       // I
    {1,2,2,2},   // J
    {2,1,2},     // K
    {1,2,1,1},   // L
    {2,2},       // M
    {2,1},       // N
    {2,2,2},     // O
    {1,2,2,1},   // P
    {2,2,1,2},   // Q
    {1,2,1},     // R
    {1,1,1},     // S
    {2},         // T
    {1,1,2},     // U
    {1,1,1,2},   // V
    {1,2,2},     // W
    {2,1,1,2},   // X
    {2,1,2,2},   // Y
    {2,2,1,1}    // Z
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
void textToMorse(const char* text, char* morse);
void morseToText(const char* morse, char* text);

volatile uint8_t X_Input_REG = 0;
volatile uint8_t Y_Input_REG = 0;
volatile char Operation_REG = ' ';
volatile int Display_Result_REG = 0;
volatile int button_pressed = 0;

int reset = 0;
int key1 = 0;
int op = 0;
int key2 = 0;
int result = 0;
int button1 = 0;
int motion1 = 0;
int uart1_char = 0;
int uart2_char = 0;
char data[100];
int SEGDIS_bit7;
int SEGDIS_bit6;
int SEGDIS_bit5;
int SEGDIS_bit4;
int SEGDIS_bit3;
int SEGDIS_bit2;
int SEGDIS_bit1;
int SEGDIS_bit0;
int SEGDIS_val;

void delay(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1 = 0; cnt1 < num1; cnt1++)
        for (cnt2 = 0; cnt2 < num2; cnt2++) {}
}

int readKeypad() {
    int row, col;
    int keypad_val;

    for (col = 0; col < COLS; col++) {
        PORTB &= 0b00001111; // Clear all columns
        PORTB |= (1 << col); // Set only 1 column based on COL variable
        delay(1, 10);
        keypad_val = KEYPAD_PORT & 0b00001111;

        for (row = 0; row < ROWS; row++) {
            if (keypad_val == (1 << row)) {
                button_pressed = 1;
                delay(1, 10);

                while (button_pressed == 1) {
                    keypad_val = KEYPAD_PORT & 0b00001111;
                    if (keypad_val == 0) {
                        button_pressed = 0;
                    }
                    delay(1, 10);
                }

                delay(1, 10);
                return keypad_dec[row][col];
            }
        }
    }
    return 16;
}

int readButtons() {
    if (PORTAbits.RA0 == 1) {
        while (PORTAbits.RA0 == 1) {}
        return 20;
    } else if (PORTAbits.RA1 == 1) {
        while (PORTAbits.RA1 == 1) {}
        return 21;
    } else if (PORTAbits.RA2 == 1) {
        while (PORTAbits.RA2 == 1) {}
        return 22;
    } else if (PORTAbits.RA3 == 1) {
        while (PORTAbits.RA3 == 1) {}
        return 23;
    }
    return 24;
}

int readMotion() {
    int cnt1, cnt2, cnt3, cnt4, motion1_cnt, motion2_cnt, motion3_cnt, motion4_cnt, diff_cnt;

    delay(1, 10);
    PORTAbits.RA5 = 0;
    delay(1, 10);
    PORTAbits.RA5 = 1;
    delay(1, 10);
    PORTAbits.RA5 = 0;
    cnt1 = 0;
    cnt2 = 0;

    while ((PORTAbits.RA4 == 0) && (cnt1 < 10000)) {
        cnt1++;
    }

    while ((PORTAbits.RA4 == 1) && (cnt2 < 10000)) {
        cnt2++;
    }

    delay(1, 10);
    PORTAbits.RA5 = 0;
    delay(1, 10);
    PORTAbits.RA5 = 1;
    delay(1, 10);
    PORTAbits.RA5 = 0;
    cnt3 = 0;
    cnt4 = 0;

    while ((PORTAbits.RA4 == 0) && (cnt3 < 10000)) {
        cnt3++;
    }

    while ((PORTAbits.RA4 == 1) && (cnt4 < 10000)) {
        cnt4++;
    }

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

void displayResult(int result) {}

int keypad_dec_to_seg(int input_val) {
    if (input_val == 0) {
        return 0x3f;
    }

    if (input_val == 1) {
        return 0x06;
    }

    if (input_val == 2) {
        return 0x5B;
    }

    if (input_val == 3) {
        return 0x4F;
    }

    if (input_val == 4) {
        return 0x66;
    }

    if (input_val == 5) {
        return 0x6D;
    }

    if (input_val == 6) {
        return 0x7D;
    }

    if (input_val == 7) {
        return 0x07;
    }

    if (input_val == 8) {
        return 0x7F;
    }

    if (input_val == 9) {
        return 0x67;
    }

    if (input_val == 10) {
        return 0x77;
    }

    if (input_val == 11) {
        return 0x7C;
    }

    if (input_val == 12) {
        return 0x39;
    }

    if (input_val == 13) {
        return 0x5E;
    }

    if (input_val == 14) {
        return 0x79;
    }

    if (input_val == 15) {
        return 0x71;
    }
    return 0x00;
}

void updateSegDis(int input_val) {
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
            ((SEGDIS_bit0 & 0x01) << 0);
    SEGDIS_LAT = ((SEGDIS_bit7 & 0x01) << 7) |
            ((SEGDIS_bit6 & 0x01) << 6) |
            ((SEGDIS_bit5 & 0x01) << 5) |
            ((SEGDIS_bit4 & 0x01) << 4) |
            ((SEGDIS_bit3 & 0x01) << 3) |
            ((SEGDIS_bit2 & 0x01) << 2) |
            ((SEGDIS_bit1 & 0x01) << 1) |
            ((SEGDIS_bit0 & 0x01) << 0);
    delay(1, 10);
}

void motor(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1 = 0; cnt1 < num1; cnt1++) {
        for (cnt2 = 0; cnt2 < num2; cnt2++) {
            PORTCbits.RC0 = ((cnt2 < 1));
        }
    }
}

void buzzer(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1 = 0; cnt1 < num1; cnt1++) {
        for (cnt2 = 0; cnt2 < num2; cnt2++) {
            PORTCbits.RC0 = cnt1 & 0b00000001;
        }
    }
}

void melody(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1 = 0; cnt1 < num1; cnt1++) {
        for (cnt2 = 0; cnt2 < num2; cnt2++) {
            PORTCbits.RC0 = cnt1 & 0b00000001;
            PORTCbits.RC1 = (cnt1 >> 1) & 0b00000001;
            PORTCbits.RC2 = (cnt1 >> 2) & 0b00000001;
        }
    }
}

void textToMorse(const char* text, char* morse) {
    int i, j;
    char c;

    for (i = 0; text[i] != '\0'; i++) {
        c = toupper(text[i]);
        if (c >= 'A' && c <= 'Z') {
            for (j = 0; j < 5; j++) {
                if (morseToKey[c - 'A'][j] == 1) {
                    strcat(morse, ".");
                } else if (morseToKey[c - 'A'][j] == 2) {
                    strcat(morse, "-");
                }
            }
            strcat(morse, " ");
        } else if (c == ' ') {
            strcat(morse, "  ");
        }
    }
}

void morseToText(const char* morse, char* text) {
    int i, j;
    char* token;
    char delimiter[] = " ";
    token = strtok(morse, delimiter);

    while (token != NULL) {
        if (strcmp(token, "") == 0) {
            strcat(text, " ");
        } else {
            for (i = 0; i < 26; i++) {
                char morseChar[6] = "";
                for (j = 0; j < 5; j++) {
                    if (morseToKey[i][j] == 1) {
                        strcat(morseChar, ".");
                    } else if (morseToKey[i][j] == 2) {
                        strcat(morseChar, "-");
                    }
                }
                if (strcmp(token, morseChar) == 0) {
                    char letter[2];
                    letter[0] = 'A' + i;
                    letter[1] = '\0';
                    strcat(text, letter);
                    break;
                }
            }
        }
        token = strtok(NULL, delimiter);
    }
}

//---------------------
// MAIN FUNCTION
//---------------------
int main(void) {
    SYSTEM_Initialize();
    configure_ports();
    while (1) {
        // Check for button press
        int button_val = readButtons();
        if (button_val < 20) {
            char text[100] = "";
            char morse[200] = "";
            // Read text from keypad
            while (1) {
                int key = readKeypad();
                if (key == 16) break; // Exit loop if # key is pressed
                if (key < 10) {
                    char num_char[2];
                    sprintf(num_char, "%d", key);
                    strcat(text, num_char);
                } else {
                    switch (key) {
                        case 10: strcat(text, "A"); break;
                        case 11: strcat(text, "B"); break;
                        case 12: strcat(text, "C"); break;
                        case 13: strcat(text, "D"); break;
                        case 14: strcat(text, "E"); break;
                        case 15: strcat(text, "F"); break;
                    }
                }
                delay(1, 500); // Delay to prevent multiple inputs from one press
            }
            // Translate text to Morse code
            textToMorse(text, morse);
            // Display Morse code on 7-segment display
            for (int i = 0; i < strlen(morse); i++) {
                updateSegDis(keypad_dec_to_seg(morse[i] - '0'));
                delay(1, 500); // Delay to display each Morse code character
            }
        }
    }
    return 0;
}

void configure_ports() {
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;

    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA5 = 0;

    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;

    TRISB = 0b00001111;
    PORTB = 0x00;

    PORTC = 0x00;
    PORTAbits.RA5 = 0;
}
