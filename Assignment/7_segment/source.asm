;---------------------
; Title: 7 Segment Display Counter using 2 Switches and Pic18
;---------------------
; Program Details:
; The purpose of this program is to generate a counter when a switch(es) is pressed. 
; When Pressing and holding Switch A, the counter is incrementing 
; When it Stop pressing A, it displays the last number
; When Pressing and holding Switch B, the couner is decrementing
; When it Stop pressing B, it displays the last number
; When A and B are pressed together ? Displaying 0 (reset)

    
    
; Inputs: Switch A and Switch B(input) => RD1 and RD2
; Outputs: PORTA1-A5 and PORTC6 are the outputs for the 7 segment display
; Setup: The Curiosity Board, Single 7 Segment Device, and 2 Switches
    
; Date: March 26, 2024
; File Dependencies / Libraries: It is required to include the 
;   AssemblyConfig.inc in the Header Folder
; Compiler: xc8, 2.4
; Author: Aaron John Estrada
; Versions:
;       V1.0: Draft Code
;	V2.0: Draft Code
;	V3.0: Revised Version with Loop Delay
;	V3.1: Revised Version with fixed 7 segment display
;	V3.2: Revised Version with fixed delay

;---------------------
; Initialization
;---------------------
#include "C:\Users\Owner\Downloads\FirstAssemblyMPLAB.X\AssemblyConfig.inc"
#include <xc.inc>
PSECT absdata,abs,ovrld        ; Do not change
inner_loop	equ 0xFF    ;inner loop value
outer_loop	equ 0xFF    ;outer loop value
loopvalue1	equ 32	;double inner loop value
loopvalue2	equ 33	;double outer loop value
ORG 0x20 ; when the program starts
CALL initialize_display ; calls the initialization function
LFSR 0,0x00 ;load FSR0 with 0x00.
MOVFF INDF0,PORTA ;it moves the contents from FSR0 to the PORTA register.
    ;then gets the data without changing the pointer.
MOVFF PORTA,PORTC ; it moves the contents from PORTA to PORTC
    
GOTO Program_Start ;go to program start function

/////////PORT SETUPS///////////
    
PortA_Setup:
    BANKSEL PORTA   ;Select bank PortA
    CLRF    PORTA   ;then clear portA
    BANKSEL LATA    ;Select bank LATA
    CLRF    LATA    ;then clear LATA
    BANKSEL ANSELA  ;Select bank ANSELA
    CLRF    ANSELA  ;then clear ANSELA
    BANKSEL TRISA   ;Select bank TRISA
    MOVLW   0b0000000 ;WREG = 0b0000000 
    MOVWF   TRISA   ;put the value to TRISA
    RETURN  ;return to previous function
    
    
PortC_Setup:
    BANKSEL PORTC   ;Select bank PortC
    CLRF    PORTC   ;then clear portC
    BANKSEL LATC    ;Select bank LATC
    CLRF    LATC    ;then clear LATC
    BANKSEL ANSELC  ;Select bank ANSELC
    CLRF    ANSELC  ;then clear ANSELC
    BANKSEL TRISC   ;Select bank TRISC
    MOVLW   0b0000000	;WREG = 0b0000000 
    MOVWF   TRISC   ;put the value to TRISC
    RETURN  ;return to previous function
    
 PortD_Setup:
    BANKSEL PORTD ;Select bank PortD
    CLRF    PORTD  ;then clear portD
    BANKSEL LATD ;Select bank LATD
    CLRF    LATD ;then clear LATD
    BANKSEL ANSELD  ;Select bank ANSELD
    CLRF    ANSELD ;then clear ANSELD
    BANKSEL TRISD ;Select bank TRISD
    MOVLW   0b1111111 ;WREG = ob1111111
    MOVWF   TRISD ;put the value to TRISD
    RETURN  ;return to previous function
    
///////LOOP DELAYs///////
loopDelay_initialization:
    MOVLW	inner_loop ;puts the designated loop value to wreg
    MOVWF	loopvalue1 ;puts the value to loopvalue1
    MOVLW	outer_loop ;puts the designated loop value to wreg
    MOVWF	loopvalue2 ;puts the value to loopvalue2
    
loopagain:
    DECF    loopvalue1,1 ;decrements the loopvalue1
    BNZ	    loopagain ;if loopvalue1 is not zero, go back to loopagain
    MOVLW   inner_loop ;wreg = inner_loop value
    MOVWF   loopvalue1 ;put the value into loopvalue1
    DECF    loopvalue2,1 ;;decrements the loopvalue1
    BNZ	    loopagain ;if loopvalue1 is not zero, go back to loopagain
    RETURN ; returns to the previous function
    
///////PROGRAM INITIALIZATION/////////
Program_Start:
    CALL reset_all ;calls the 7 segment reset function
    BTFSC PORTD,1 ;it checks if PORTD1 is clear, skips if its clear
    CALL countup ;calls the count up function
    BTFSC PORTD,2 ;it checks if PORTD2 is clear, skips if its clear
    CALL countdown ;calls the count down function
    GOTO Program_Start ;loop back to the program_start function
    
reset_all:
    CALL loopDelay_initialization ; delays the function
    BTFSS PORTD,1 ;it checks if PORTD1 is set, skip if its set
    return ;return back to the previous function
    BTFSS PORTD,2  ;it checks if PORTD2 is set, skip if its set
    return ;return back to the previous function
    LFSR 0,0x00 ;load FSR0 with 0x00.
    MOVFF INDF0, PORTA ;it moves the contents from FSR0 to the PORTA register.
    ;then gets the data without changing the pointer.
    MOVFF INDF0, PORTC ;it moves the contents from FSR0 to the PORTC register.
    ;then gets the data without changing the pointer.
    GOTO reset_all ;go to reset function	
    
countup:
    CALL loopDelay_initialization;adds a delay for the countdown
    BTFSC   PORTD,2 ;it checks if PortD2 is set, if so, skip the next command
    return ;returns back to Program_start function
    MOVLW   0x10 ;wreg=10
    CPFSLT  FSR0    ; Compares the (FSR0) with (WREG) 
    ;and skips the next instruction if FSR0 < WREG
   LFSR    0,0x00 ;load FSR0 with 0x00.
    MOVFF   PREINC0,PORTA ;moves contents from FSR0 to PORTA
    ;and then increments FSR0.
    MOVFF INDF0,PORTC ;it moves the contents from FSR0 to the PORTC register.
    ;then gets the data without changing the pointer.
    BTFSS PORTD,1 ;it checks if PortD1 is set, if so, skip the next command
    return ;returns back to Program_start function
    GOTO countup ;goes to countup function
    
    
countdown:
    Call loopDelay_initialization ;adds a delay for the countdown
    BTFSC PORTD,1 ;it checks if PortD1 is set, if so, skip the next command
    return ;returns back to Program_start function
    MOVLW 0x10 ;wreg=10
    CPFSLT FSR0  ; Compares the (FSR0) with (WREG) 
    ;and skips the next instruction if FSR0 < WREG
    LFSR 0,0X0F ;load FSR0 with 0x0F.
    MOVFF INDF0,PORTC 
    MOVFF POSTDEC0, PORTA ;Moves the contents from FSR0 to the PORTA
    ;then decrements FSR0.
    BTFSS PORTD,2 ;it checks if PortD2 is set, if so, skip the next command
    return ;returns back to Program_start function
    GOTO countdown ;goes to countdown function
    
//////////INITIALIZE 7 SEGMENT LEDS/////////
initialize_display:
    
    MOVLW   0x3F ;it displays 0 for 7 segment
    MOVWF   0x00
    
    MOVLW   0x06 ;it displays 1 for 7 segment
    MOVWF   0x01 
    
    MOVLW   0x5b ;it displays 2 for 7 segment
    MOVWF   0x02 
    
    MOVLW   0x4F ;it displays 3 for 7 segment
    MOVWF   0x03 
    
    MOVLW   0x66 ;it displays 4 for 7 segment
    MOVWF   0x04
    
    MOVLW   0x6D ;it displays 5 for 7 segment
    MOVWF   0x05
    
    MOVLW   0x7D ;it displays 6 for 7 segment
    MOVWF   0x06
    
    MOVLW   0x07 ;it displays 7 for 7 segment
    MOVWF   0x07
    
    MOVLW   0x7F ;it displays 8 for 7 segment
    MOVWF   0x08
    
    MOVLW   0x6F ;it displays 9 for 7 segment
    MOVWF   0x09
    
    MOVLW   0x77 ;it displays A for 7 segment
    MOVWF   0X0A
    
    MOVLW   0X7C ;it displays B for 7 segment
    MOVWF   0X0B
    
    MOVLW   0X58 ;it displays C for 7 segment
    MOVWF   0X0C
    
    MOVLW   0X5E ;it displays D for 7 segment
    MOVWF   0X0D
    
    MOVLW   0x79 ;it displays E for 7 segment
    MOVWF   0x0E
    
    MOVLW   0x71 ;it displays F for 7 segment
    MOVWF   0x0F
    
    MOVLW   0x3F ;it displays 0 for 7 segment
    MOVWF   0x10
   
////////Calls to setup the input/output ports//////
    CALL    PortA_Setup //setup the porta output
    CALL    PortC_Setup //setup the portc output
    CALL    PortD_Setup //setup the portd input
    return ;go back to previous function
    

    
    


    
   

	
    

    
    
