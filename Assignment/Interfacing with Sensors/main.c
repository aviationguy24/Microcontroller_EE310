/*
 * ---------------------
 * Title: Simple Box Vault using Pic18, photoresistors, with the
 * implementation of interrupts.
 * ---------------------
 * Program Details:
 * The purpose of this program is to make a simple vault using pic 18,
 * photoresistors and a buzzer. If the user failed to input the passcode,
 * the buzzer will be activated, if the input is correct, the motor will activate.
 * This program also implements interrupt on a pic18 module.
 * C-Programming, and Keypad.
 * Inputs: RB0 (Interrupt Input), RC2-C3 (Submit Button), 
 * RB1-B4 are the Photoresistors.
 * Outputs: PORTD0-D6: 7 Segment Display
 * PORTD7 - Buzzer
 * 
 * 
 * Setup: The Curiosity Board, Keypad Device, and 8 LEDs
 * Date: Apr 24, 2024
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File
 * Initialization file and the
 * config file 
 * Compiler: xc8, 2.6
 * Author: Aaron John Estrada
 * Versions:
 *      V1.0: First Draft
 *      V1.1: Second Draft with better interrupt
 *      V1.2: Third Draft with fixed photoresistor function
 *      V2.0: Trying to fix the motor issues
 *      V3.0: Fixed Circuit / Final
 */

#include "config.h"
#include "initialization.h"
#include "functions.h"
#include <xc.h>
#include "C:\Program Files\Microchip\xc8\v2.46\pic\include\proc\pic18f47k42.h"

#define _XRAL_FREQ 400000
#define FCY     _XTAL_FREQ/4

void initialize(void);
void check(void);

void main(void) {
    initialize();
    check();
    return;
}

