/*
 * myTask.c
 *
 *  Created on: Nov 24, 2019
 *      Author: Andi
 */

#include "myTask.h"
#include "myLCD.h"

/* ------------------------------- */
enum state{start, startDelay, BuzzerOn, insertCoin, checkCoin, ready, drop, in500, in1000, out500, out1000, cancel}myState;
volatile uint16_t delay, delay1=0;
volatile uint16_t debounce1=0xFF;
volatile uint16_t debounce2=0xFF;
volatile uint16_t debounce3=0xFF;
volatile uint16_t debounce4=0xFF;
volatile uint16_t saldo=0;
volatile uint8_t i=0,j=0;
_Bool pressed=0;
_Bool toggle=0;

void myTask_init(void){
	myLCD_init();
	myLCD_Bkl(1);
	ledProcess(0);
	ledCancel(0);
	ledBack(0);
	ledDrop(0);
}

void myTask_Run(void){
	switch (myState){
	case start:{
		myLCD_setCursor(0, 0); myLCD_print("Vending Machine");
		myLCD_setCursor(0, 1); myLCD_print("Andi Mei");
		myState=startDelay;
	}break;

	case startDelay:{
		if(++delay1 > 1000){
			delay1=0;
			myLCD_clear();
			myLCD_setCursor(0, 0); myLCD_print("Insert Coin !");
			myState=checkCoin;
		}

	}break;

	case checkCoin:{
		if(PB_u500()){
			myLCD_clear();
			myLCD_setCursor(0, 0); myLCD_print("Success !");
			myLCD_setCursor(0, 1); myLCD_print("500 inserted");
			myState=in500;
		}
		if(PB_u1000()){
			myLCD_clear();
			myLCD_setCursor(0, 0); myLCD_print("Success !");
			myLCD_setCursor(0, 1); myLCD_print("1000 inserted");
			myState=in1000;
		}

		if(saldo==500){
			if(PB_Cancel()){
				saldo=0;
				myState=cancel;
			}
		}

		if(saldo==1000){
			myLCD_clear();
			myState=ready;
		}

		if(saldo==1500){
			myLCD_clear();
			myLCD_setCursor(0, 0); myLCD_print("Money Returned !");
			myLCD_setCursor(0, 1); myLCD_print("Rp.500");
			saldo-=500;		//Saldo dikurangi 500
			myState=out500;
		}
	}break;

	case ready:{
		myLCD_setCursor(0, 0); myLCD_print("Process? (Y/N)");
		ledProcess(1);
		if(PB_Process()){
			myLCD_clear();
			myLCD_setCursor(0, 0); myLCD_print("Success !");
			myLCD_setCursor(0, 1); myLCD_print("Candy Dropped");
			saldo=0;
			myState=drop;
		}

		if(PB_Cancel()){
			myLCD_clear();
			myLCD_setCursor(0, 0); myLCD_print("Transaction");
			myLCD_setCursor(0, 1); myLCD_print("Canceled");
			saldo=0;
			myState=cancel;

		}
	}break;

	case drop:{
		ledProcess(0);
		if(++delay1 > 80){
			delay1=0;
			toggle=!(toggle);
			i++;
			buzzer(toggle);
			ledDrop(toggle);
		}

		if(i>5) {
			i=0;
			myLCD_clear();
			myState=start;
		}

	}break;

	case cancel:{
		ledProcess(0);
		if((++delay > 80) && (i<6)){
			delay=0;
			toggle=!(toggle);
			buzzer(toggle);
			ledCancel(toggle);
			i++;
		}

		if(++delay1 > 500){
			i=0;
			delay1=0;
			myLCD_clear();
			myLCD_setCursor(0, 0); myLCD_print("Money Returned !");
			if(saldo==0){
				myLCD_setCursor(0, 1); myLCD_print("Rp.500");
			}
			else {
				myLCD_setCursor(0, 1); myLCD_print("Rp.1000");
			}
			myState=out1000;
		}
	}break;

	case in500:{
		buzzer(1);
		if(++delay1 > 500){
			delay1=0;
			buzzer(0);
			myLCD_clear();
			saldo+=500;
			if(saldo<1000){
				myLCD_setCursor(0, 0); myLCD_print("Insert Coin !");
			}
			myLCD_setCursor(0, 1); myLCD_print("Saldo: Rp."); myLCD_printNum(saldo);
			myState=checkCoin;
		}
	}break;

	case in1000:{
		buzzer(1);
		if(++delay1 > 500){
			delay1=0;
			buzzer(0);
			myLCD_clear();
			saldo+=1000;
			myState=checkCoin;
		}
	}break;

	case out500:{
		ledBack(1);
		if(++delay1 > 1000){
			delay1=0;
			ledBack(0);
			myLCD_clear();
			myState=checkCoin;
		}
	}break;

	case out1000:{
		saldo=0;
		ledBack(1);
		if(++delay1 > 1000){
			delay1=0;
			ledBack(0);
			myLCD_clear();
			myState=start;
		}
	}break;

	}

}

/* ---------------------------------------------------------------------------------------- */
_Bool PB_Process(void){
	_Bool flagDetect=0;
	if(HAL_GPIO_ReadPin(PB_PROCESS_GPIO_Port, PB_PROCESS_Pin)==0)
		debounce1 = debounce1<<1;
	else
		debounce1 = (debounce1<<1)|1;
	if(debounce1==3)
		flagDetect=1;
	return flagDetect;
}

_Bool PB_Cancel(void){
	_Bool flagDetect=0;
	if(HAL_GPIO_ReadPin(PB_CANCEL_GPIO_Port, PB_CANCEL_Pin)==0)
		debounce2 = debounce2<<1;
	else
		debounce2 = (debounce2<<1)|1;
	if(debounce2==3)
		flagDetect=1;
	return flagDetect;
}

_Bool PB_u500(void){
	_Bool flagDetect=0;
	if(HAL_GPIO_ReadPin(u500_GPIO_Port, u500_Pin)==0)
		debounce3 = debounce3<<1;
	else
		debounce3 = (debounce3<<1)|1;
	if(debounce3==3)
		flagDetect=1;
	return flagDetect;
}

_Bool PB_u1000(void){
	_Bool flagDetect=0;
	if(HAL_GPIO_ReadPin(u1000_GPIO_Port, u1000_Pin)==0)
		debounce4 = debounce4<<1;
	else
		debounce4 = (debounce4<<1)|1;
	if(debounce4==3)
		flagDetect=1;
	return flagDetect;
}

