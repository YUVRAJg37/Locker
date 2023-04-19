#include <pic.h>
#define _XTAL_FREQ 20000000

#define RS RC0
#define RW RC1
#define EN RC2

#define C1 RB0
#define C2 RB1
#define C3 RB2
#define R1 RB4
#define R2 RB5
#define R3 RB6
#define R4 RB7

void lcd_data(unsigned char data)
{
    PORTD= data;
    RS = 1;
    RW = 0;
    EN = 1;
    
    __delay_ms(2);
    EN = 0;
}

void lcd_command(unsigned char command)
{
    PORTD = command;
    RS = 0;
    RW = 0;
    EN = 1;
    
    __delay_ms(5);
    EN = 0;
}

void lcd_string(unsigned char* string, unsigned char length)
{
    unsigned char i;
    for(i=0; i<length; i++)
    {
        lcd_data(string[i]);
    }
}

void lcd_init()
{
    lcd_command(0x38); // set to 16x2
    lcd_command(0x06); //auto increment cursor
    lcd_command(0x0C); //Display on cursor off 
    lcd_command(0x01); //clear screen
}

void KeyPad()
{
    C1 = 1; C2=0; C3=0;
    
    if(R1 == 1)
    {
        lcd_data('1');
        while(R1 == 1);
    }
    if(R2 == 1)
    {
        lcd_data('4');
        while(R2 == 1);
    }
    if(R3 == 1)
    {
        lcd_data('7');
        while(R3 == 1);
    }
    if(R4 == 1)
    {
        lcd_data('*');
        while(R4 == 1);
    }
    
    C1 = 0; C2=1; C3=0;
    
    if(R1 == 1)
    {
        lcd_data('2');
        while(R1 == 1);
    }
    if(R2 == 1)
    {
        lcd_data('5');
        while(R2 == 1);
    }
    if(R3 == 1)
    {
        lcd_data('8');
        while(R3 == 1);
    }
    if(R4 == 1)
    {
        lcd_data('0');
        while(R4 == 1);
    }
    
    C1 = 0; C2=0; C3=1;
    
    if(R1 == 1)
    {
        lcd_data('3');
        while(R1 == 1);
    }
    if(R2 == 1)
    {
        lcd_data('6');
        while(R2 == 1);
    }
    if(R3 == 1)
    {
        lcd_data('9');
        while(R3 == 1);
    }
    if(R4 == 1)
    {
        lcd_data('#');
        while(R4 == 1);
    }
}

void main() 
{
    TRISB = 0xF0;
    TRISC = 0x00;
    TRISD = 0x00;
    
    lcd_init();
    lcd_command(0x80); // 1st row 1st column
    lcd_string("Keypad", 7);
    lcd_command(0xC0); // 2nd row 1st column
    
    while(1)
    {
        KeyPad();
    }
    
    return;
}
