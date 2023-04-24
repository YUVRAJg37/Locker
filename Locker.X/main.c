#pragma config WDTE = OFF 

#include <xc.h>

#define _XTAL_FREQ 4000000

#define C1 RB0
#define C2 RB1
#define C3 RB2
#define R1 RB4
#define R2 RB5
#define R3 RB6
#define R4 RB7

#define RS RC0
#define RW RC1
#define EN RC2

unsigned char defaultPassword[] = "1234";
unsigned char currentPassword[] = "0000";

int passwordLength = 0;
int passwordMaxLength = 0;

void lcd_init();
void lock_init();
void lcd_instruction(unsigned char);
void lcd_data(unsigned char);
void lcd_string(unsigned char*, unsigned int);
void keypad();
_Bool checkPass();
int getSize(unsigned char*);
void resetLock();
void tryAgain();

void main() 
{
    TRISC = 0;
    TRISD = 0;
    TRISB = 0xF0;
    
    lock_init();

    passwordMaxLength = getSize(&defaultPassword);
    
    while(1)
    {          
        if(checkPass())
        {
            lcd_init();
            lcd_instruction(0x80);
            lcd_string("   UNLOCKED", 11);

            __delay_ms(10000);

            resetLock();
        }
        else
        {
            keypad();
            if(passwordLength > passwordMaxLength)
            {
                passwordLength = 0;
                tryAgain();
            } 
        }
    }
    
    return;
}

void lcd_init()
{
    lcd_instruction(0x38); // set to 16x2
    lcd_instruction(0x0C); //Cursor off, display on
    lcd_instruction(0x06); //Auto increment cursor
    lcd_instruction(0x01); // Clear screen
}

void lcd_instruction(unsigned char data)
{
    PORTD = data;
    RS = 0;
    RW = 0;
    EN = 1;
    
    __delay_ms(5);
    
    EN = 0;
}

void lcd_data(unsigned char data)
{
    PORTD = data;
    RS = 1;
    RW = 0;
    EN = 1;
    
    __delay_ms(5);
    
    EN = 0;
}

void lcd_string(unsigned char* string, unsigned int size)
{
    for(unsigned int i=0; i<size; i++)
    {
        lcd_data(string[i]);
    }
}

void keypad()
{ 
    C1 = 1; C2 = 0; C3 = 0;
    
    if(R1 == 1)
    {
        lcd_data('1');
        while(R1 == 1);
        currentPassword[passwordLength] = '1';
        passwordLength++;
    }
    if(R2 == 1)
    {
        lcd_data('4');
        while(R2 == 1);
        currentPassword[passwordLength] = '4';
        passwordLength++;
    }
    if(R3 == 1)
    {
        lcd_data('7');
        while(R3 == 1);
        currentPassword[passwordLength] = '7';
        passwordLength++;
    }
    if(R4 == 1)
    {
        lcd_data('*');
        while(R4 == 1);
        currentPassword[passwordLength] = '*';
        passwordLength++;
    }
    
    C1 = 0; C2 = 1; C3 = 0;
    
    if(R1 == 1)
    {
        lcd_data('2');
        while(R1 == 1);
        currentPassword[passwordLength] = '2';
        passwordLength++;
    }
    if(R2 == 1)
    {
        lcd_data('5');
        while(R2 == 1);
        currentPassword[passwordLength] = '5';
        passwordLength++;
    }
    if(R3 == 1)
    {
        lcd_data('8');
        while(R3 == 1);
        currentPassword[passwordLength] = '8';
        passwordLength++;
    }
    if(R4 == 1)
    {
        lcd_data('0');
        while(R4 == 1);
        currentPassword[passwordLength] = '0';
        passwordLength++;
    }
    
    C1 = 0; C2 = 0; C3 = 1;
    
    if(R1 == 1)
    {
        lcd_data('3');
        while(R1 == 1);
        currentPassword[passwordLength] = '3';
        passwordLength++;
    }
    if(R2 == 1)
    {
        lcd_data('6');
        while(R2 == 1);
        currentPassword[passwordLength] = '6';
        passwordLength++;
    }
    if(R3 == 1)
    {
        lcd_data('9');
        while(R3 == 1);
        currentPassword[passwordLength] = '9';
        passwordLength++;
    }
    if(R4 == 1)
    {
        lcd_data('#');
        while(R4 == 1);
        currentPassword[passwordLength] = '#';
        passwordLength++;
    }  
}

int getSize(unsigned char* string)
{
    int i=0;
    for(i=0; i<string[i]!='\0'; i++);
    
    return i;
}

_Bool checkPass()
{
    if(passwordLength == 0)
        return 0;
        
    for(int i=0; i<passwordMaxLength; i++)
    {
        if(currentPassword[i] != defaultPassword[i])
            return 0;
    }
    return 1;
}

void resetLock()
{
    passwordLength = 0;
    lock_init();
    for(int i=0; i<passwordMaxLength; i++)
    {
        currentPassword[i] = '0';
    }  
}

void lock_init()
{      
    lcd_init();
    lcd_instruction(0x80);
    lcd_string("    Welcome", 11);
    
    __delay_ms(10000);
    
    lcd_init();
    lcd_instruction(0x80);
    lcd_string(" Enter Password", 15);
   
    lcd_instruction(0xC0);
    lcd_string("     ", 5);  
}

void tryAgain()
{    
    lcd_init();
    lcd_instruction(0x80);
    lcd_string("   TRY AGAIN", 12);
    
    __delay_ms(10000);
    
    lcd_init();
    lcd_instruction(0x80);
    lcd_string(" Enter Password", 15);
     
    lcd_instruction(0xC0);
    lcd_string("     ", 5);   
}