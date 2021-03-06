#include "main.h"
#include "function.c"
#include <math.h>
#rom 0x2100={0x31,0x30,0x30,0x30,0x30}
#rom 0x2110={0x30,0x31,0x36,0x36,0x33,0x39,0x31,0x35,0x33,0x30,0x38,0x20,0x20,0x20,0x20,0x20}
#rom 0x2160={0x31,0x31,0x31,0x31,0x31,0x31}
#define GSM_PW PIN_E2
#define GSM_SENSE PIN_E1
#define TEXTMODE 5
#define PDUMODE 0

int delay_100;
//**********************************************************
//      USART Interrupt service
//**********************************************************

#int_TIMER0
TIMER0_isr() 
{
   if(delay_100 < 40)
      delay_100++;
   else
      {
         delay_100=0;
         Led2=!led2;
      }
  
   return 0;
}

#INT_RDA
void rda()
{
   UART_ISR();
}

void main()
{      
   set_tris_b(0xf0);
   trisc1=0;
   trisc2=0;
   trisc3=0;
   portb=0x0f;
   B1=0;
   C1=0;
   C2=1;
   C3=1;
   
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   rcie=1;

   GSM_sync();
   Echo();
   GSM_sel_mess_mode(1);
   delay_ms(1000);
      printf("AT+CMGDA=\"DEL READ\"\r\n");
   delay_ms(800);
    while(1){
    delay_ms(1000);
    if(calling!=1){
    if(readsms())    
    if(admin==1){
      switch(admin1()){
      case 1:themsdt();
      break;
      case 2:xoasdt();
      break;
      case 3:doimktk();
      break;
      case 4:kttkchinh();
      break;
      case 5:kttkphu();
      break;
      default:break;
      }      
    }
    else if(admin>1){
      switch(admin2()){
      case 1:kttkchinh();
      break;
      case 2:kttkphu();
      break;
      default:break;
      }
    }
    else if(admin==0)
      admin0();
    smsInit();
    sendCommand(cmgd); 
    calling=1;
    numsms++;
    if(numsms==10){
    sendCommand(cmgdall);
    }
    }else{
    if(B4==0)
    if(checkcall())
    turndevice();
    }
   }
}

