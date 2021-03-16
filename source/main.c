/*	Author: Marco Baez
 *  Partner(s) Name: NA
 *	Lab Section:021
 *	Assignment: Lab #11  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link: https://drive.google.com/drive/folders/1-1_Zf0EL01XEZT5VoUOW-SXjQH8O5fE9?usp=sharing
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

typedef struct task{
	int state;
	unsigned long period;
	unsigned long elapsedtime;
	int(*TickFct)(int);
} task;

task tasks[5];

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){

        TCCR1B = 0x0B;

        OCR1A = 125;

        TIMSK1 = 0x02;

        TCNT1 = 0x02;

        _avr_timer_cntcurr = _avr_timer_M;

        SREG |= 0x80;
}

void TimerOff(){

        TCCR1B = 0x00;

}

void TimerISR(){
        //TimerFlag = 1;
        unsigned char i;
        for(i=0;i<6;i++){
                if(tasks[i].elapsedtime>=tasks[i].period){
                        tasks[i].state=tasks[i].TickFct(tasks[i].state);
                        tasks[i].elapsedtime=0;
                }
                tasks[i].elapsedtime+=100;
        }
}

ISR(TIMER1_COMPA_vect){
        _avr_timer_cntcurr--;
        if(_avr_timer_cntcurr == 0){
                TimerISR();
                _avr_timer_cntcurr = _avr_timer_M;
        }
}

void TimerSet(unsigned long M){
        _avr_timer_M = M;
        _avr_timer_cntcurr = _avr_timer_M;
}

void transmit_data(unsigned char data, unsigned char port){
if(port==1){	
	int i;
	for(i=0;i<8;++i){
		PORTC=0x08;
		PORTC|=((data>>i)&0x01);
		PORTC|=0x02;	
 	}
 	PORTC|=0x04;
 	PORTC=0x00;}
else if(port==2){
	int j;
	for(j=0;j<8;++j){
        	PORTD=0x08;
       	        PORTD|=((data>>j)&0x01);
        	PORTD|=0x02;
 	}
 PORTD|=0x04;
 PORTD=0x00;


}

}

#define button (~PINA&0x01)
#define button2 ((~PINA>>1)&0x01)
#define button3 ((~PINA>>2)&0x01)
#define button4 ((~PINA>>3)&0x01)
//unsigned char button = 0x00;
//unsigned char button2 = 0x00;
unsigned char go = 0x00;
unsigned char go2 = 0x00;

unsigned char led = 0x00;
unsigned char count = 0x00;
enum states{InitFest, StartFest};
int LEDSM(int states){
	switch(states){
		case InitFest:
			led=0;
			if((go==1)||(go2==1)){
				states=StartFest;
				count=0;
			}
			break;
		case StartFest:
			if((go==1)||(go2==1)){states=StartFest;}
			else{states=InitFest;}
			break;
		default:
			break;
	}
	switch(states){
		case InitFest:
			break;
		case StartFest:
			if(count==0){led=0x00;}
			if(count==1){led=0x01;}
			if(count==2){led=0x03;}
			if(count==3){led=0x07;}
			if(count==4){led=0x0F;}
			if(count==5){led=0x1F;}
			if(count==6){led=0x3F;}
			if(count==7){led=0x7F;}
			if(count==8){led=0xFF;}
			if(count==9){led=0x00;}
			if(count==10){led=0xFF;}
			if(count==11){led=0x00;}
			if(count<11){count++;}
			else{count=0;}
			break;
		default:
			break;
	}
   return states;	
}

enum states2{InitFest2, StartFest2};
unsigned char led2 = 0x00;
unsigned char count2 = 0x00;
int LEDSM2(int states2){
	switch(states2){
		case InitFest2:
			if((go==2)||(go2==2)){
				count2=0x00;
				states2=StartFest2;		
			}
			break;
		case StartFest2:
			if((go==2)||(go2==2)){states2=StartFest2;}
			else{states2=InitFest2;}
			break;
		default:
			break;
	}	
	switch(states2){
		case InitFest2:
			break;
		case StartFest2:
			if(count2==0){led2=0x00;}
			if(count2==1){led2=0x18;}
			if(count2==3){led2=0x3C;}
			if(count2==4){led2=0x7E;}
			if(count2==5){led2=0xFF;}
			if(count2==6){led2=0x7E;}
			if(count2==7){led2=0x3C;}
			if(count2==8){led2=0x18;}
			if(count2==9){led2=0x00;}
			if(count2<9){count2++;}
			else{count2=0;}
			break;
		default:
			break;
	}
    return states2;	
}

enum states3{InitFest3, StartFest3};
unsigned char led3 = 0x55;
unsigned char count3 = 0;
int LEDSM3(int states3){
	switch(states3){
		case InitFest3:
			led3=0;
			if((go==3)||(go2==3)){count3=0;states3=StartFest3;}
			else{states3=InitFest3;}
			break;
		case StartFest3:
			if((go==3)||(go2==3)){states3=StartFest3;}
			else{states3=InitFest3;}
			break;
		default:
			break;
	}
	switch(states3){
		case InitFest3:
			break;
		case StartFest3:
			if(count3==0){led3=0x01;}
			if(count3==1){led3=0x80;}
			if(count3==2){led3=0x02;}
			if(count3==3){led3=0x40;}
			if(count3==4){led3=0x04;}
			if(count3==5){led3=0x20;}
			if(count3==6){led3=0x08;}
			if(count3==7){led3=0x10;}
			if(count3==8){led3=0x18;}
			if(count3==9){led3=0x00;}
			if(count3==10){led3=0x18;}
			if(count3==11){led3=0x00;}
			if(count3==12){led3=0x18;}
			if(count3<12){count3++;}
			else{count3=0;}
			break;
		default:
			break;
	}
   return states3;	
}

//unsigned char button = 0x00;
//unsigned char button2 = 0x00;
//unsigned char temp = 0x00;
enum buttonstates{InitB, OffB, OffWait, On, OnWait,  B, B2};
int buttonSM(int bstate){
//	button=~PINA&0x01;
//	button2=(~PINA>>1)&0x01;
	switch(bstate){
		case InitB:
			go=0;
			bstate=OffB;
			break;
		case OffB:
			if((!button)&&(!button2)){bstate=OffB;}
			else if(button&&button2){bstate=OffWait;}
			break;
		case OffWait:
			if(button&&button2){bstate=OffWait;}
			else if((!button)&&(!button2)){bstate=On;}
			break;
		case On:
			if(button&&button2){bstate=OnWait;}
			else if((!button)&&(!button2)){bstate=On;}
			else if((!button)&&button2){bstate=B2;}
			else if(button&&(!button2)){bstate=B;}
			break;
		case OnWait:
			if(button&&button2){bstate=OnWait;}
			else{bstate=OffB;go=0;}
			break;
		case B:
			if(button&&button2){bstate=OnWait;go=0;}
			else if(button&&(!button2)){bstate=B;}
			else if((!button)&&(!button2)){
				bstate=On;
				if(go<3){go++;}
				else{go=1;}
			}
			break;
		case B2:
			if(button&&button2){bstate=OnWait;go=0;}
			else if((!button)&&button2){bstate=B2;}
			else if((!button)&&(!button2)){
			bstate=On;	
			if(go>1){go--;}
			else{go=3;}
			}
			break;
		default:
			break;

	}

	switch(bstate){
		case OffB:
			go=0;
			break;
		default:
			break;
	}
    return bstate;	
}

enum buttonstates2{InitB2, OffB2, OffWait2, On2, OnWait2,  Bv2, B2v2};
int buttonSM2(int bstate2){
//      button=~PINA&0x01;
//      button2=(~PINA>>1)&0x01;
        switch(bstate2){
                case InitB2:
                        go2=0;
                        bstate2=OffB2;
                        break;
                case OffB2:
                        if((!button3)&&(!button4)){bstate2=OffB2;}
                        else if(button3&&button4){bstate2=OffWait2;}
                        break;
                case OffWait2:
                        if(button3&&button4){bstate2=OffWait2;}
                        else if((!button3)&&(!button4)){bstate2=On2;}
                        break;
                case On2:
                        if(button3&&button4){bstate2=OnWait2;}
                        else if((!button3)&&(!button4)){bstate2=On2;}
                        else if((!button3)&&button4){bstate2=B2v2;}
                        else if(button3&&(!button4)){bstate2=Bv2;}
                        break;
                case OnWait2:
                        if(button3&&button4){bstate2=OnWait2;}
                        else{bstate2=OffB2;go2=0;}
                        break;
                case Bv2:
                        if(button3&&button4){bstate2=OnWait2;go2=0;}
                        else if(button3&&(!button4)){bstate2=Bv2;}
                        else if((!button3)&&(!button4)){
                                bstate2=On2;
                                if(go2<3){go2++;}
                                else{go2=1;}
                        }
                        break;
		case B2v2:
                        if(button3&&button4){bstate2=OnWait2;go2=0;}
                        else if((!button3)&&button4){bstate2=B2v2;}
                        else if((!button3)&&(!button4)){
                        bstate2=On2;
                        if(go2>1){go2--;}
                        else{go2=3;}
                        }
                        break;
                default:
                        break;

        }

        switch(bstate2){
                case OffB2:
                        go2=0;
                        break;
                default:
                        break;
        }
    return bstate2;
}

enum LEDsSmStates{LEDInit, LEDStart};
int LEDsSM(int state2){
	switch(state2){
		case LEDInit:
			state2=LEDStart;
			break;
		case LEDStart:
			state2=LEDStart;
			//transmit_data(temp);
			//transmit_data(0);
			break;
		default:
			break;
	}
	switch(state2){
		case LEDInit:
			break;
		case LEDStart:
			if(go==1){transmit_data(led, 1);}
			else if(go==2){transmit_data(led2, 1);}
			else if (go==3){transmit_data(led3, 1);}
			else if(go==0){transmit_data(0, 1);}
			if(go2==1){transmit_data(led, 2);}
			if(go2==2){transmit_data(led2, 2);}
			if(go2==3){transmit_data(led3, 2);}
			else if(go2==0){transmit_data(0, 2);}
			break;
		default:
			break;
	}
	return state2;
}


int main(void) {
	
    unsigned char temp = 0;
    DDRA=0x00; PORTA=0xFF;
    DDRC=0xFF; PORTC=0x00;
    DDRD=0xFF; PORTD=0x00;
   
    tasks[temp].state=InitFest;
    tasks[temp].period=300;
    tasks[temp].elapsedtime=0;
    tasks[temp].TickFct=&LEDSM;
    temp++;
    tasks[temp].state=InitFest2;
    tasks[temp].period=100;
    tasks[temp].elapsedtime=0;
    tasks[temp].TickFct=&LEDSM2;
    temp++;
    tasks[temp].state=InitFest3;
    tasks[temp].period=300;
    tasks[temp].elapsedtime=0;
    tasks[temp].TickFct=&LEDSM3;
    temp++;
    tasks[temp].state=InitB;
    tasks[temp].period=100;
    tasks[temp].elapsedtime=0;
    tasks[temp].TickFct=&buttonSM;
    temp++;
    tasks[temp].state=InitB2;
    tasks[temp].period=100;
    tasks[temp].elapsedtime=0;
    tasks[temp].TickFct=buttonSM2;
    temp++;
    tasks[temp].state=LEDInit;
    tasks[temp].period=100;
    tasks[temp].elapsedtime=0;
    tasks[temp].TickFct=&LEDsSM;    
    TimerSet(100);
    TimerOn();
    while (1) {
	   // transmit_data(3);
    }
    return 1;
}
