/*
 * myTask.h
 *
 *  Created on: Nov 24, 2019
 *      Author: Andi
 */

#ifndef INC_MYTASK_H_
#define INC_MYTASK_H_

#include "main.h"
#define ledCancel(state) { HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, (state)? GPIO_PIN_RESET:GPIO_PIN_SET); }
#define ledProcess(state) { HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, (state)? GPIO_PIN_RESET:GPIO_PIN_SET); }
#define ledDrop(state) { HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, (state)? GPIO_PIN_RESET:GPIO_PIN_SET); }
#define ledBack(state) { HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, (state)? GPIO_PIN_RESET:GPIO_PIN_SET); }
#define buzzer(state) { HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, (state)? GPIO_PIN_SET:GPIO_PIN_RESET); }

#define ledLoading(state)	{ HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, (state)? GPIO_PIN_RESET:GPIO_PIN_SET);}
#define led500(state)		{ HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, (state)? GPIO_PIN_RESET:GPIO_PIN_SET);}
#define led1000(state)		{ HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, (state)? GPIO_PIN_RESET:GPIO_PIN_SET);}

void myTask_init(void);
void myTask_Run(void);
_Bool PB_Process(void);
_Bool PB_Cancel(void);
_Bool PB_u500(void);
_Bool PB_u1000(void);


#endif /* INC_MYTASK_H_ */
