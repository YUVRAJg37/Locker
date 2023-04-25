#pragma config WDTE = OFF //Turning off Watchdog Timer

#include <xc.h>

#define _XTAL_FREQ 4000000 

//Defining the alias for PIC ports
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

unsigned char defaultPassword[] = "1234"; //Default Password for the Lock
unsigned char currentPassword[] = "0000"; //Password typed by the user

int passwordCurrentLength = 0; //User typed password length
int passwordMaxLength = 0; //Length of the default password

void lcd_init(); //Initialize the lcd
void lock_init(); //Initialize the lock
void lcd_instruction(unsigned char); //Function to write lcd instruction
void lcd_data(unsigned char); //Function to write Char data on lcd
void lcd_string(unsigned char*, unsigned int); //Function to display String of LCD
void keypad(); //Keeping Track of the keys pressed on keypad
_Bool checkPass(); //Checking if Typed Password is equal to Deafult Password or not
int getSize(unsigned char*); //Calculation the size of the string
void resetLock(); //Reseting the lock
void tryAgain(); //Reseting the lock when password fails

void main() 
{
    TRISA = 0; //Setting Port A to output mode
    TRISB = 0xF0; //Setting half Port B values to 0 and half to 1
    TRISC = 0; //Setting Port C to output mode
    TRISD = 0; //Setting Port D to output mode
    
    
    lock_init();

    passwordMaxLength = getSize(&defaultPassword); //Storing the Default password length
    
    while(1)
    {          
        if(checkPass())
        {
            lcd_init();
            lcd_instruction(0x80);
            lcd_string("   UNLOCKED", 11);
            PORTA = 0xFF;
            __delay_ms(10000);
            
            PORTA = 0x00;
            resetLock();
        }
        else
        {
            keypad();
            if(passwordCurrentLength > passwordMaxLength)
            {
                passwordCurrentLength = 0;
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
    PORTD = data; //Setting PORTD value to data
    RS = 0; //Setting Mode to instruction read
    RW = 0; //Setting Read Write 0
    EN = 1; //Setting Enable pin value
    
    __delay_ms(5);
    
    EN = 0; //Setting Enable value
}

void lcd_data(unsigned char data)
{
    PORTD = data;//Setting PORTD value to data
    RS = 1;//Setting Mode to instruction read
    RW = 0;//Setting Read Write 0
    EN = 1;//Setting Enable pin value
    
    __delay_ms(5);
    
    EN = 0;//Setting Enable value
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
    C1 = 1; C2 = 0; C3 = 0; //Setting Column of Keypad values
    
    if(R1 == 1)
    {
        lcd_data('1');
        while(R1 == 1);
        currentPassword[passwordCurrentLength] = '1';
        passwordCurrentLength++;
    }
    if(R2 == 1)
    {
        lcd_data('4');
        while(R2 == 1);
        currentPassword[passwordCurrentLength] = '4';
        passwordCurrentLength++;
    }
    if(R3 == 1)
    {
        lcd_data('7');
        while(R3 == 1);
        currentPassword[passwordCurrentLength] = '7';
        passwordCurrentLength++;
    }
    if(R4 == 1)
    {
        lcd_data('*');
        while(R4 == 1);
        currentPassword[passwordCurrentLength] = '*';
        passwordCurrentLength++;
    }
    
    C1 = 0; C2 = 1; C3 = 0; //Setting Column of Keypad values
    
    if(R1 == 1)
    {
        lcd_data('2');
        while(R1 == 1);
        currentPassword[passwordCurrentLength] = '2';
        passwordCurrentLength++;
    }
    if(R2 == 1)
    {
        lcd_data('5');
        while(R2 == 1);
        currentPassword[passwordCurrentLength] = '5';
        passwordCurrentLength++;
    }
    if(R3 == 1)
    {
        lcd_data('8');
        while(R3 == 1);
        currentPassword[passwordCurrentLength] = '8';
        passwordCurrentLength++;
    }
    if(R4 == 1)
    {
        lcd_data('0');
        while(R4 == 1);
        currentPassword[passwordCurrentLength] = '0';
        passwordCurrentLength++;
    }
    
    C1 = 0; C2 = 0; C3 = 1; //Setting Column of Keypad values
    
    if(R1 == 1)
    {
        lcd_data('3');
        while(R1 == 1);
        currentPassword[passwordCurrentLength] = '3';
        passwordCurrentLength++;
    }
    if(R2 == 1)
    {
        lcd_data('6');
        while(R2 == 1);
        currentPassword[passwordCurrentLength] = '6';
        passwordCurrentLength++;
    }
    if(R3 == 1)
    {
        lcd_data('9');
        while(R3 == 1);
        currentPassword[passwordCurrentLength] = '9';
        passwordCurrentLength++;
    }
    if(R4 == 1)
    {
        lcd_data('#');
        while(R4 == 1);
        currentPassword[passwordCurrentLength] = '#';
        passwordCurrentLength++;
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
    if(passwordCurrentLength == 0)
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
    passwordCurrentLength = 0;
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