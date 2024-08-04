/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "LCDbyte.h"




int main(void){

	LCD_init();

	    while(1){

	    	const char message[32] = "SyntaxErrorLab\0";
	    	/* debug pins one by one
	    	LCD_acsii(0b01000001); //A
	    	LCD_acsii(0b01000010); //B
	    	LCD_acsii(0b01000100); //D
	    	LCD_acsii(0b01001000); //H
	    	LCD_acsii(0b01010000); //P
	    	LCD_acsii(0b01100001); //a
	    	LCD_acsii(0b01100010); //b
	    	*/


	    	short c = 0;
	    	while (message[c] != '\0'){
	    		LCD_acsii(message[c]);
	    		c++;
	    		if(c > 32) { return 1;}
	    	}


	    	/* clear LCD */
	    	refreshLCD();
	    }

}
