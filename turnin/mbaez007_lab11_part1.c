/*	Author: Marco Baez
 *  Partner(s) Name: NA
 *	Lab Section:021
 *	Assignment: Lab #11  Exercise #1
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

task tasks[2];

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
        for(i=0;i<2;i++){
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

void transmit_data(unsigned char data){
int i;
for(i=0;i<8;++i){
	PORTC=0x08;
	PORTC|=((data>>i)&0x01);
	PORTC|=0x02;	
 }
 PORTC|=0x04;
 PORTC=0x00;
}

unsigned char button = 0x00;
unsigned char button2 = 0x00;
unsigned char temp = 0x00;
enum buttonstates{init, incdec, b, b2, r};
int buttonSM(int state){
	button=~PINA&0x01;
	button2=(~PINA>>1)&0x01;
	switch(state){
		case init:
			temp=0x00;
			state=incdec;
			break;
		case incdec:
			if(button&button2){
				state=r;
				temp=0x00;
			}
			else if(button&(!button2)){
				state=b;
				if(temp<0xFF){++temp;}
			}
			else if((!button)&button2){
				state=b2;
				if(temp>0){--temp;}
			}
			else{
				state=incdec;
			}
			break;
		case b:
			if(button&button2){
				state=r;
				temp=0x00;
			}
			else if(button&(!button2)){
				state=button;
			}
			else if((!button)&(!button2)){
				state=incdec;
			}
			break;
		case b2:
			if(button&button2){
				state=r;
				temp=0;
			}
			else if(button2&(!button)){
				state=button2;
			}
			else if((!button)&(!button2)){
				state=incdec;
			}
			break;
		case r:
			if((!button)&(!button2)){
				state=incdec;
			}
			else{
				state=r;
			}
			break;
		default:
			break;
	}
	return state;
}

enum LEDsSmStates{LEDInit, LEDStart};
int LEDsSM(int state2){
	switch(state2){
		case LEDInit:
			state2=LEDStart;
			break;
		case LEDStart:
			state2=LEDStart;
			transmit_data(temp);
			//transmit_data(0);
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
    
    tasks[temp].state=init;
    tasks[temp].period=100;
    tasks[temp].elapsedtime=0;
    tasks[temp].TickFct=&buttonSM;
    temp++;
    tasks[temp].state=LEDInit;
    tasks[temp].period=100;
    tasks[temp].elapsedtime=0;
    tasks[temp].TickFct=&LEDsSM;
    TimerSet(100);
    TimerOn();
    while (1) {
	    //transmit_data(3);
    }
    return 1;
}

