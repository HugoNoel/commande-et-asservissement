/*
 * fonctions.h
 *
 *  Created on: 20 oct. 2021
 *      Author: pensi
 */

#ifndef INC_FONCTIONS_H_
#define INC_FONCTIONS_H_

#endif /* INC_FONCTIONS_H_ */

#include "stm32g4xx_hal.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

static const char LIGNE[29] = "<Hugo/Romain>@Nucleo-G431 >>\0";

static const char START[6] = "start\0";
static const char SPEED[7] = "speed=\0";
static const char HELP[5] = "help\0";
static const char PINOUT[7] = "pinout\0";
static const char STOP[5] = "stop\0";
static const char GET_SPEED[10] = "get_speed\0";

extern char Commande[100];
extern TIM_HandleTypeDef htim1;
extern int ON;
extern int vitesse;
extern UART_HandleTypeDef huart2;

void Get_Speed();
void Ligne_Init(UART_HandleTypeDef *huart);
void Commande_exec(int i);
void Start();
void Stop();
void Set_SPEED(int i);
