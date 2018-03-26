/*Interfacing pic16f877a, and 2*16 lcd  in four bit mode , and ultra-sonic
sensor to display distance
*/


/*define oscilator speed*/
#define  _XTAL_FREQ 4000000

/*interface LCD in 4 bits mode */

#define RS    RD2
#define EN    RD3
#define D4    RD4
#define D5    RD5
#define D6    RD6
#define D7    RD7

#include<xc.h>
#include<lcd.h>
#include<pic16f877a.h>

//BEGIN CONFIG WORD
#pragama config FOSC  =HS
#pragama config WDTE  =OFF
#pragama config PWRTE =OFF
#pragama config BOREN =ON
#pragama config LVP   =OFF
#pragama config CPD   =OFF
#pragama config WRT   =OFF
#pragama config CP    =OFF
//END CONFIG
int distance=0,errorFactor=1;
void interrupt echo()
{
  if(RBIF==1)
  {
    RBIE=0;               //disable interrupt
    if(RB4==1)
    TMR1ON=1;
    if(RB4==0)
    {
      TMR1ON=0;
      distance=(TMR1L|(TMR1H<<8))/58.82;    //Calculate Distance

    }
  }
  RBIF=0;             //Clear PORTB On-Change Interrupt  flag.
  RBIE=1;             //Enable PORTB On-Change Interrupt
}
void main()
{
  TRISTB|=1<<RB4;           // RB4 IS ECHO PIN WILL BE INPUT
  TRISD=0x0;
  Lcd_Init();

  T1CON =0X10;              //Initialize Timer Module

  while(1)
  {
    TMR1H=0;                     //Sets the initial value of Timer
    TMR1L=0;                   //Sets the initial value of Timer
    RB0=1;                       //Trigger pin is high
    __delay_us(10);                 //10uS __delay_us
    RB0=0;                          //Trigger low
    __delay_ms(100);                //Waiting for ECHO
    distance+=errorFactor;        //Error Correction Constant
    if(distance!=0 && distance<=400)
    {
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Dsitance = ");
    Lcd_Set_Cursor(1,14);
    Lcd_Write_Char(distance%10+48);   //pass distance as character numbers
    distance/=10;                     //get the second number of value
    Lcd_Set_Cursor(1,13);
    Lcd_Write_Char(a%10+48);
    distance/=10;                     //get the second number of value
    Lcd_Set_Cursor(1,12);
    Lcd_Write_Char(a%10+48);
    Lcd_Set_Cursor(1,15);
    Lcd_Write_String("CM");
    }
    else
    {
      Lcd_Clear();
      Lcd_Set_Cursor(1,1);
      Lcd_Write_String("Out of range");
    }
    __delay_ms(400);
  }


}
