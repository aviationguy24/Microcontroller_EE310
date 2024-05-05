
#define _XTAL_FREQ 8000000

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

#include <xc.h>
#include "lcd.h";
#include <pic16f877a.h>

// BEGIN CONFIG
#pragma config FOSC = HS   
#pragma config WDTE = OFF  
#pragma config PWRTE = OFF 
#pragma config BOREN = ON 
#pragma config LVP = OFF   
#pragma config CPD = OFF   
#pragma config WRT = OFF  
#pragma config CP = OFF    
//END CONFIG
void main()
{ 
  int a;

  TRISB = 0b00010000;         //RB4 as Input PIN (ECHO)
  TRISD = 0x00;               // LCD Pins as Output

  Lcd_Init();

  Lcd_Set_Cursor(1,1);
  Lcd_Write_String("Developed By");
  Lcd_Set_Cursor(2,1);
  Lcd_Write_String("electroSome");

  __delay_ms(3000);
  Lcd_Clear();

  T1CON = 0x10;               //Initialize Timer Module

  while(1)
  { 
    TMR1H = 0;                //Sets the Initial Value of Timer
    TMR1L = 0;                //Sets the Initial Value of Timer

    RB0 = 1;                  //TRIGGER HIGH
    __delay_us(10);           //10uS Delay 
    RB0 = 0;                  //TRIGGER LOW

    while(!RB4);              //Waiting for Echo
    TMR1ON = 1;               //Timer Starts
    while(RB4);               //Waiting for Echo goes LOW
    TMR1ON = 0;               //Timer Stops

    a = (TMR1L | (TMR1H<<8)); //Reads Timer Value
    a = a/58.82;              //Converts Time to Distance
    a = a + 1;                //Distance Calibration
    if(a>=2 && a<=400)        //Check whether the result is valid or not
    { 
      Lcd_Clear();
      Lcd_Set_Cursor(1,1);
      Lcd_Write_String("Distance = ");

      Lcd_Set_Cursor(1,14);
      Lcd_Write_Char(a%10 + 48);

      a = a/10;
      Lcd_Set_Cursor(1,13);
      Lcd_Write_Char(a%10 + 48);

      a = a/10;
      Lcd_Set_Cursor(1,12);
      Lcd_Write_Char(a%10 + 48);

      Lcd_Set_Cursor(1,15);
      Lcd_Write_String("cm");
    }  
    else
    {
      Lcd_Clear();
      Lcd_Set_Cursor(1,1);
      Lcd_Write_String("Out of Range");
    }
    __delay_ms(400);
  }
}
