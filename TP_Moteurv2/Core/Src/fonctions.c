/*
 * fonctions.c
 *
 *  Created on: 20 oct. 2021
 *      Author: pensi
 */
int V=50;

#include "fonctions.h"

void Ligne_Init(UART_HandleTypeDef *huart){
	HAL_UART_Transmit(huart, (uint8_t *)&LIGNE, strlen(LIGNE), 0xFFFF);
}

void Commande_exec(int i){
	int commande_existe=0;
	if ((strncmp(Commande,PINOUT,i)==0) && (i==strlen(PINOUT))){
		printf("\r\nLes broches connectées sont:\r\n");
		printf("    - start: carte: A2 - hacheur: pin 33\r\n");
		printf("    - PWM Red phase: carte: CH1:A5, CH1N:D11 - hacheur CH1:pin 13, CH1N:pin 31\r\n");
		printf("    - PWM Yellow phase: carte: CH2:A4, CH2N:A3 - hacheur CH1:pin 12, CH1N:pin 30\r\n");
		printf("    - Encodeur: A: A1 B: A0 \r\n");
		commande_existe=1;
	}
	if (strncmp(Commande,SPEED,6)==0){
		Set_SPEED(i);
		commande_existe=1;
	}
	if ((strncmp(Commande,START,i)==0) && (i==strlen(START))){
		printf("\r\nPower ON\r\n");
		Start();
		commande_existe=1;
	}
	if ((strncmp(Commande,STOP,i)==0) && (i==strlen(STOP))){
		printf("\r\nPower OFF\r\n");
		Stop();
		commande_existe=1;
	}
	if ((strncmp(Commande,HELP,i)==0) && (i==strlen(HELP))){
		printf("\r\nLes commandes disponibles sont:\r\n");
		printf("    - help\r\n");
		printf("    - pinout\r\n");
		printf("    - start\r\n");
		printf("    - stop\r\n");
		printf("    - speed=xx\r\n");
		printf("    - get_speed\r\n");
		commande_existe=1;
	}
	if ((strncmp(Commande,GET_SPEED,i)==0) && (i==strlen(GET_SPEED))){
		Get_Speed();
		commande_existe=1;
	}
	if(commande_existe==0){
		printf("\r\nCette commande n'existe pas\r\n");
	}
}

void Get_Speed(){
	int deb= (TIM2->ARR)/2;
	TIM2->CNT=deb;
	HAL_Delay(250);
	vitesse=((int)(TIM2->CNT)-deb)/256;

	printf("La vitesse est %d tour/sec\r\n", vitesse);
	//Ligne_Init(&huart2);
}

void Set_SPEED(int i){
	int length = i - strlen(SPEED);

	char valeur[length];
	for (int j=0; j<length; j++){
		valeur[j]=Commande[6+j];
	}

	int a = atoi(valeur);

	if(a>0 || a<100){

		if(a-V>5){
		 int n = (a-V)/5;
		 int inter=V;


		 for (int j=0; j<n+1; j++){
		   inter=inter+5;
		   TIM1->CCR1 = (int) ((TIM1->ARR)*inter)/100;
		   TIM1->CCR2 = (TIM1->ARR) - TIM1->CCR1;

		   HAL_Delay(150);
		   }
		}

		else if(a-V<5){
			int n = (a-V)/5;
			int inter=V;
			for (int j=0; j<n+1; j++){
					   inter=inter-5;
					   TIM1->CCR1 = (int) ((TIM1->ARR)*inter)/100;
					   TIM1->CCR2 = (TIM1->ARR) - TIM1->CCR1;

					   HAL_Delay(150);
					   }
		}


		TIM1->CCR1 = (int) ((TIM1->ARR)*a)/100;
		TIM1->CCR2 = (TIM1->ARR) - TIM1->CCR1;
		printf("\r\nCCR1=%d\r\n",(int)TIM1->CCR1);
		printf("\r\nCCR2=%d\r\n",(int)TIM1->CCR2);

		V=a;

	}
	else{
		printf("La valeur de vitesse n'est pas valable. Elle doit être comprise entre 1 et 99.=%d\r\n", a);
	}
}

void Start(){
	TIM1->CCR1 = (int) ((TIM1->ARR)*50)/100;
	TIM1->CCR2 = (TIM1->ARR) - TIM1->CCR1;
	printf("\r\nARR=%d\r\n",(int)TIM1->ARR);
	printf("\r\nCCR1=%d\r\n",(int)TIM1->CCR1);
	printf("\r\nCCR2=%d\r\n",(int)TIM1->CCR2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	ON=1;
}

void Stop(){
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
	ON=0;
}
