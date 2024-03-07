//-----------------------------
// Title: Thermostat using Pic18 Microcontoller
//-----------------------------
// Purpose: Control cooling and heating systems based on temperature readings
// Dependencies: NONE
// Compiler: MPLAB X IDE v6.20
// Author: Aaron John Estrada
// OUTPUTS: PORTD.2 (Cooling), PORTD.1 (Heating)
// INPUTS: measuredTempInput, refTempInput
// Versions:
//  	V1.0: 3/2/2024 - Initial Version: Code Header, Inputs, Outputs,
//  	V1.1: 3/3/2024 - Main Code: Added Temperature Limit Check 
//  	V1.2: 3/4/2024 - Main Code: Added the Comparator and Conditions
//	V1.3: 3/5/2024 - Main Code: Added the Hex to Decimal Conversion
//	V2.0: 3/6/2024 - Main Code: Removed the Temperature Limit Check
//	V2.1: 3/7/2024 - Main Code: Added Comments and simplfied code
//-----------------------------
#include "C:\Users\Owner\Downloads\FirstAssemblyMPLAB.X\AssemblyConfig.inc"
#include <xc.inc> 
//PROGRAM INPUTS
#define  measuredTempInput 	0 ; this is the input value from the Temp Sensor
#define  refTempInput 		15 ; this is the input value from the Keypad

//REGISTERS
#define  measuredTempREG 	0x20 ; Designated Register address to store input value
				;of the Temp Sensor
#define  refTempREG 		0x21 ; Designeted Register address to store input value
				;of the Keypad
#define	    contREG		0x22 ; Designeted Register address to store
				;count reg
//REFERENCE VALUES 
#define  maxTEMP		61 ; Max Temp input of Temp Sensor in decimal!
//#define  leastTEMP		-20 ; Minimum Temp Input of Temp Sensor
//#define  max_ref_temp		50 ; Max Temp Input of Keypad
//#define  least_ref_temp	10 ; Minimum Temp Input of Keypad

//PROGRAM OUTPUTS
//#define  measuredTempDecimal 	0x70 ; NOT USED
//#define  refTempInputDecimal     ; NOT USED
#define   HEATER      PORTD,2
#define   COOLER  PORTD,1

 ;-----
PSECT absdata,abs,ovrld        ; Do not change
ORG 0X20			;Starts at 20
MOVLW	0X00		    ;WREG = 0
MOVWF	TRISD		    ;MOVE The value from the WREG to into the TRISD reg
MOVLW	measuredTempInput   ;WREG = MeasuredTempInput
MOVWF	measuredTempREG	    ;put the Input to its designated register (0x20)
MOVLW	refTempInput	    ;WREG = refTempInput
MOVWF	refTempREG	    ;put the Input to its designated register (0x21)

 ////NOT USED////
;least_ref_Temp_Check:
;    MOVLW   refTempInput
;    CPFSLT  least_ref_temp ;Check if least_ref_temp<refTempInput
;    SLEEP
;    GOTO    max_ref_Temp_Check		;go to next condition
;    
;    
;max_ref_Temp_Check:
;    MOVLW   refTempInput
;    CPFSGT  max_ref_temp		;Check if max_ref_temp>refTempInput
;    GOTO    least_measured_Temp_Check	    ;go to next condition
;    SLEEP
;    
;    
;    
;least_measured_Temp_Check:
;    MOVLW    measuredTempInput
;    CPFSLT  leastTEMP	;Check if leastTEMP<measuredTempInput
;    SLEEP
;    GOTO    max_measured_Temp_Check  ;go to next condition
;  
;    
;max_measured_Temp_Check:
 
 ///PART 1
 //////CONVERSION FROM HEX TO DECIMAL FOR 
 ///////(Temp Sensor Input)
NUME	EQU 0x43    ;RAM Location for NUME
QU	EQU 0x10    ;RAM Location for Quotient
RMND_L	EQU  0x70   ;First Digit
RMND_M	EQU 0X71    ;Second Digit
RMND_H	EQU 0X72    ;Third Digit
MYNUM EQU measuredTempInput	;Numerator (Temp Sensor Input)
MYDEN EQU 0x0A			;Denominator (10d)

MOVLW MYNUM	    ;WREG = Numerator Value
MOVWF NUME	    ;Put to the value from WREG to NUME reg
MOVLW MYDEN	    ;WREG = Denominator Value
 
D_1:
    INCF QU,F	;Increment the Quotient by the value of F register
    SUBWF NUME	;Subtract the WREG by the number from Numerator
    BC	D_1	;if positive go back! (C=1 for Positive)
    ADDWF NUME	;Once too many, this is the first digit
    DECF QU,F	;Once too many for quotient
    MOVFF NUME, RMND_L ;Saving the first digit
    MOVFF QU, NUME ;Repeat the process one more time
    CLRF QU ;Clear Quotient
    
D_2:
    INCF QU,F	;Increment the Quotient by the value of F register
    SUBWF NUME	;Subtract the WREG by the number from Numerator
    BC D_2	;if positive go back! (C=1 for Positive)
    ADDWF NUME	;Once too many, this is the first digit
    DECF QU,F	;Once too many for quotient
    MOVFF NUME, RMND_M ;Saving the second digit
    MOVFF QU, RMND_H ;Saving the third digit
    
 
 ///PART 2
 //////CONVERSION FROM HEX TO DECIMAL FOR 
 ///////(Keypad Input)
CLRF QU	;Clear Quotient from Part 1
NUME	EQU 0x43 ;RAM Location for NUME
QU	EQU 0x10    ;RAM Location for Quotient
REF_RMND_L	EQU  0x60   ;First Digit
REF_RMND_M	EQU 0X61    ;Second Digits
REF_RMND_H	EQU 0X62    ;Third Digit
REF_MYNUM EQU refTempInput ;Numerator (Keypad Temp Input)

MOVLW REF_MYNUM	;WREG = Numerator Value
MOVWF NUME  ;Put to the value from WREG to NUME reg
MOVLW MYDEN ;WREG = Denominator Value
 
D_1_1:
    INCF QU,F	;Increment the Quotient by the value of F register
    SUBWF NUME	;Subtract the WREG by the number from Numerator
    BC	D_1_1	;if positive go back! (C=1 for Positive)
    ADDWF NUME	;Once too many, this is the first digit
    DECF QU,F	;Once too many for quotient
    MOVFF NUME, REF_RMND_L ;Saving the first digit
    MOVFF QU, NUME  ;Saving the second digit
    CLRF QU ;Clear Quotient
    
D_2_2:
    INCF QU,F	;Increment the Quotient by the value of F register
    SUBWF NUME	;Subtract the WREG by the number from Numerator
    BC D_2_2	;if positive go back! (C=1 for Positive)
    ADDWF NUME	;Once too many, this is the first digit
    DECF QU,F	;Once too many for quotient
    MOVFF NUME, REF_RMND_M  ;Saving the second digit
    MOVFF QU, REF_RMND_H    ;Saving the third digit
///////
MOVLW	measuredTempInput   ;WREG = Temp Sensor Input
CPFSLT	maxTEMP  ; if 61 < measuretemp
GOTO	If_Equal    ;if not, go to if equal condition
GOTO	LED_HEAT    ;if yes, go to LED heat condition

If_Equal:   ;Check if Temp sensor input = Keypad Input
    MOVLW   measuredTempInput ;WREG = Temp sensor input
    CPFSEQ  refTempREG	;If equal to to LED_OFF if not, go to heat
    GOTO If_HEAT
    GOTO LED_OFF
    
If_HEAT: ;Check if needs to turn on heating
    MOVLW   measuredTempInput ;WREG = Temp sensor input
    CPFSGT  refTempREG ; If WREG > value in refTempREG
    GOTO    if_COOL ;if no, go to cool condition
    GOTO    LED_HEAT ;if yes go to LED heat condition
    
if_COOL:  ;Check if needs to turn on cooling
    MOVLW   measuredTempInput ;WREG = Temp sensor input
    CPFSLT  refTempREG ; If WREG < value in refTempREG
    SLEEP ;if no, go to cool SLEEP!!
    GOTO LED_COOL ;if yes go to cool condition
    
LED_OFF: ;turn LED off
    MOVLW   0x00
    MOVWF   contREG ;none because there is no action required
    MOVWF   PORTD  ;no led
    SLEEP
    
LED_HEAT:
    MOVLW   0x1 ;WREG =1
    MOVWF   contREG ;Put WREG value to ContReg
    MOVLW   0b0000001
    MOVWF   PORTD,a ;Set a bit in portd into 1
    SLEEP
    
LED_COOL:
    MOVLW   0x1 ;WREG =1
    MOVWF   contREG ;Put WREG value to ContReg
    MOVLW   0b0000001
    MOVWF   PORTD,a ;Set a bit in portd into 1
    SLEEP
