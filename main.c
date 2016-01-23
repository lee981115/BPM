#include <mega128.h>
#include <lcd.h>
#include <delay.h>
#include <stdio.h>
#asm(".equ __lcd_port = 0x12")
#define CHACK_TIME 10

int puls_cnt,cnt;
int sec,iscomplete;
char str[16];


void main()
{
    DDRC=0xFF;
    EIMSK = 0x10;
    EICRB = 0x03; 
    TCCR0 = 0x07;
    lcd_init(16);
    #asm("sei")
    while(1)
    {
        if(iscomplete)
        {                
            lcd_gotoxy(0,0);      
            sprintf(str,"%d BPM        ",puls_cnt*6);
            lcd_puts(str);  
            iscomplete = 0; 
            puls_cnt = 0; 
            delay_ms(500);
            #asm("sei")        
        }                         
    }
}
interrupt [EXT_INT4]void ext4()
{
    PORTC = 0xFF;
    TIMSK=0x01;
    puls_cnt++;
    
}
interrupt [TIM0_OVF]void tim0()
{
    cnt++;  
    lcd_gotoxy(0,0);      
    sprintf(str,"scanning %d sec ",CHACK_TIME - sec);
    lcd_puts(str); 
    if(cnt==61)
    {
         sec++;
         cnt = 0; 
         if(sec==CHACK_TIME)
         {
            PORTC = 0x0;
            sec = 0;    
            iscomplete = 1;
            TIMSK=0x00;
            #asm("cli")
         }
    }
}
