/*
 * myTask.c
 *
 *  Created on: Nov 24, 2019
 *      Author: Andi Mei Prasetyo
 *
 */

#include "myTask.h"
#include "myLCD.h"

/* ------------------------------- */
enum state{start, startDelay, insertCoin1, insertCoin2, ready, cancel, drop, k500, k1000, }myState;
enum flag{noCoin, in500, in1000}myFlag;
volatile uint16_t delay, delay1=0;
volatile uint16_t debounce1=0xFF;
volatile uint16_t debounce2=0xFF;
volatile uint16_t debounce3=0xFF;
volatile uint16_t debounce4=0xFF;
volatile uint16_t money=0;
volatile uint8_t i=0,j=0;
_Bool pressed=0;
_Bool toggle=0;
_Bool printEn=0;

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
		myLCD_setCursor(0, 1); myLCD_print("   Andi Mei");
		myState=startDelay;
	}break;

	case startDelay:{
		if(++delay1 > 1000){
			delay1=0;
			myLCD_clear();
			myLCD_setCursor(0, 0); myLCD_print("Insert Coin !");
			printEn=1;
			myState=insertCoin1;
		}

	}break;

	case insertCoin1:{
		/*	Printed once in loop	*/
		if(printEn){
			printEn=0;
			myLCD_setCursor(0, 1); myLCD_print("Money: "); myLCD_printNum(money);
		}

		/*	Check type of coin when sensor detected, and give appropriate flag	*/
		if(PB_u500()){
			buzzer(1);
			myLCD_clear();
			myLCD_setCursor(0, 0); myLCD_print("Rp.500 inserted !");
			myFlag=in500;
		}

		if(PB_u1000()){
			buzzer(1);
			myLCD_clear();
			myLCD_setCursor(0, 0); myLCD_print("Rp.1000 inserted");
			myFlag=in1000;
		}

		/*	Check flag and goto next state	*/
		if(myFlag==in500){
			if(++delay > 500){
				delay=0;
				buzzer(0);
				money+=500;
				myLCD_clear();
				myFlag=noCoin;
				myLCD_setCursor(0, 0); myLCD_print("Insert Coin !");
				printEn=1;
				myState=insertCoin2;
			}
		}

		if(myFlag==in1000){
			if(++delay > 500){
				delay=0;
				buzzer(0);
				money+=1000;
				myLCD_clear();
				myFlag=noCoin;
				printEn=1;
				myState=ready;
			}
		}
	}break;

	case insertCoin2:{
		/*	Printed once in loop	*/
		if(printEn){
			printEn=0;
			myLCD_setCursor(0, 1); myLCD_print("Money: "); myLCD_printNum(money);
		}

		/*	Check type of coin when sensor detected, and give appropriate flag	*/
		if(PB_u500()){
			buzzer(1);
			myLCD_clear();
			myLCD_setCursor(0, 0); myLCD_print("Rp.500 inserted");
			myFlag=in500;
		}

		if(PB_u1000()){
			buzzer(1);
			myLCD_clear();
			myLCD_setCursor(0, 0); myLCD_print("Rp.1000 inserted");
			myFlag=in1000;
		}
		if(PB_Cancel()){
			myLCD_clear();
			printEn=1;
			myState=cancel;
		}

		/*	Check flag and goto next state	*/
		if(myFlag==in500){
			if(++delay > 500){
				delay=0;
				buzzer(0);
				money+=500;
				myLCD_clear();
				printEn=1;
				myFlag=noCoin;
				myState=ready;
			}
		}

		if(myFlag==in1000){
			if(++delay > 500){
				delay=0;
				buzzer(0);
				money+=1000;
				myLCD_clear();
				myFlag=noCoin;
				myState=k500;
			}
		}
	}break;

	case ready:{
		if(printEn){	//printed once
			printEn=0;
			myLCD_setCursor(0, 0); myLCD_print("Process? (Y/N)");
			myLCD_setCursor(0, 1); myLCD_print("Money: "); myLCD_printNum(money);
		}

		if(PB_Process()){
			money-=1000;			//money is taken to pay candy
			myLCD_clear();
			printEn=1;
			myState=drop;
		}
		if(PB_Cancel()){
			myLCD_clear();
			printEn=1;
			myState=cancel;
		}
		if(PB_u500()){
			money+=500;
			myLCD_clear();
			myState=k500;
		}
		if(PB_u1000()){
			money+=1000;
			myLCD_clear();
			myState=k1000;
		}
	}break;

	case cancel:{
		/* check money in machine	*/
		if(money==500){
			/* Printed once in loop	*/
			if(printEn){
				printEn=0;
				myLCD_setCursor(0, 0); myLCD_print("Money Refunded !");
				myLCD_setCursor(0, 1); myLCD_print("Rp.500");
			}

			/* Blinky LED and buzzer beep	*/
			if((++delay > 80) && (i<=9)){
				delay=0;
				toggle=!(toggle);
				buzzer(toggle);
				ledLoading(toggle);
				i++;
			}

			/* goto next state after blinky	*/
			if(i>=8){
				led500(1);
				buzzer(1);
				if(++delay > 500){
					i=0;
					led500(0);
					buzzer(0);
					delay=0;
					myLCD_clear();
					printEn=1;
					money-=500;		//500 refunded
					myState=start;
				}
			}
		}

		/* check money in machine	*/
		else if(money==1000){
			/* Printed once in loop	*/
			if(printEn){
				printEn=0;
				myLCD_setCursor(0, 0); myLCD_print("Money Refunded !");
				myLCD_setCursor(0, 1); myLCD_print("Rp.1000");
			}
			/* Blinky LED and buzzer beep	*/
			if((++delay > 80) && (i<=9)){
				delay=0;
				toggle=!(toggle);
				buzzer(toggle);
				ledLoading(toggle);
				i++;
			}

			/* goto next state after blinky	*/
			if(i>=8){
				led1000(1);
				buzzer(1);
				if(++delay > 500){
					i=0;
					led1000(0);
					buzzer(0);
					delay=0;
					myLCD_clear();
					printEn=1;
					money-=1000;	//1000 refunded
					myState=start;
				}
			}
		}

	}break;

	case drop:{
		/*	printed once in loop	*/
		if(printEn){
			printEn=0;
			myLCD_setCursor(0, 0); myLCD_print("Success !");
			myLCD_setCursor(0, 1); myLCD_print("Candy Dropped");
		}

		/* Blinky LED and buzzer beep	*/
		if((++delay > 80) && (i<=9)){
			delay=0;
			toggle=!(toggle);
			buzzer(toggle);
			ledLoading(toggle);
			i++;
		}

		/* goto next state after blinky	*/
		if(i>=8){
			ledDrop(1);
			buzzer(1);
			if(++delay > 500){
				i=0;
				ledDrop(0);
				buzzer(0);
				delay=0;
				myLCD_clear();
				printEn=1;
				myState=ready;
			}
		}
	}break;

	case k500:{
		/* Money overflow	*/
		if(money==1500){
			money-=500;		//500 refunded
			myLCD_setCursor(0, 0); myLCD_print("Money Refunded !");
			myLCD_setCursor(0, 1); myLCD_print("Rp.500");
		}

		/* Blinky LED and buzzer beep	*/
		if((++delay > 80) && (i<=9)){
			delay=0;
			toggle=!(toggle);
			buzzer(toggle);
			ledLoading(toggle);
			i++;
		}

		/* goto next state after blinky	*/
		if(i>=8){
			led500(1);
			buzzer(1);
			if(++delay > 500){
				i=0;
				led500(0);
				buzzer(0);
				delay=0;
				myLCD_clear();
				printEn=1;
				myState=ready;
			}
		}
	}break;

	case k1000:{
		/* Money overflow	*/
		if(money==2000){
			money-=1000;		//1000 refunded
			myLCD_setCursor(0, 0); myLCD_print("Money Refunded !");
			myLCD_setCursor(0, 1); myLCD_print("Rp.1000");
		}

		/* Blinky LED and buzzer beep	*/
		if((++delay > 80) && (i<=9)){
			delay=0;
			toggle=!(toggle);
			buzzer(toggle);
			ledLoading(toggle);
			i++;
		}

		/* goto next state after blinky	*/
		if(i>=8){
			led1000(1);
			buzzer(1);
			if(++delay > 500){
				i=0;
				led1000(0);
				buzzer(0);
				delay=0;
				myLCD_clear();
				printEn=1;
				myState=ready;
			}
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

