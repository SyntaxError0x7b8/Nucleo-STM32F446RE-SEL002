/*
 * LCDbyte.c
 *
 *  Created on: Aug 2, 2024
 *      Author: syntaxerrorlab.net
 *
 *  This code will allow to use LCDdisplay with Nucleo-STM32F446RE (and possibly other STM32-F4xx)
 *  It is based on Shenzen Eone Electronics Co. Lts 1602A-1 (v1.2)
 *  This code does not make use of ST Microelectronics HAL, but it requires some of its definitions
 *
 *  Data ports: PC0 to PC7
 *  Control ports: PB5, PB6 and PB7
 *
 *  When display is ready to receive data will lit LD2
 *
 */

#include "LCDbyte.h"
#include "stm32f4xx.h"




extern void LCD_init(void){
	init_ports();

	delayLCD(35);
	LCD_cmd_strt(FUNC_SET_EIGHT);		//first time cannot read status
	delayLCD(15);
	LCD_cmd_strt(FUNC_SET_EIGHT);
	delayLCD(1);
	LCD_cmd_strt(FUNC_SET_EIGHT);		// busy flag ready

	LCD_cmd(EIGHT_BIT);
	LCD_cmd(SHIFT_CRS_RGHT);
	LCD_cmd(CLEAR_SCREEN);
	LCD_cmd(DISPLAY_ON_BLINK);
	GPIOA->ODR |= LED_PIN_SEL;			// LED2 ON, LCD ready to receive information

}


extern void init_ports(void){
	RCC->AHB1ENR |= (RCC_GPIOA_SEL | RCC_GPIOB_SEL | RCC_GPIOC_SEL);

	GPIOA->MODER &= CLEAR_LED_MODE;
	GPIOA->MODER |= SET_LED_OUTPUT_MODE;

    // PB5 is R/S
	// PB6 is R/W
	// PB7 is EN
	GPIOB->MODER &= CLEAR_CTRL_MODE;
	GPIOB->MODER |= SET_CTRL_OUTPUT;
	GPIOB->BSRR = ((EN | RW) << 16);				// turn off EN and RW

	/* Data D0-D7 ==> PC0-PC7 */
	GPIOC->MODER &= CLEAR_DATA_MODE;
	GPIOC->MODER |= SET_DATA_OUTPUT_MODE;


}

/* Function to wait for the flag (D7) to change from 1 to 0 before
 * data can be written again to the display controller.
 * Change PC7 to input. to read display. Change PC7 again to output
 * to write characters.
 * A mistake in input/output could destroy pin or microcontroller.
 */
extern void LCD_isready(void){

	volatile char flag = BUSY_FLAG;			//initialised to "busy" by default

	GPIOA->ODR &= ~LED_PIN_SEL;	// turn LED OFF

	/* change to poll status register**/
	GPIOC->MODER &= CLEAR_DATA_MODE;			// clear PC0-PC7 mode (input is reset state)
	GPIOB->BSRR = (RS << 16);					// set RS to 0
	GPIOB->BSRR = RW;							// set R/W to 1 (read)
	delayLCD(0);


	do{
		// stay in loop until flag changed to 0
		GPIOB->BSRR = EN;		//EN high
		delayLCD(0);
		flag = GPIOC->IDR;		// status register
		delayLCD(0);
		GPIOB->BSRR = (EN << 16);	//EN Low
		delayLCD(2);

	} while (flag & BUSY_FLAG);		// continue until bit flag = 0 in PC7

	/* IMPORTANT: return to write config */
	GPIOB->BSRR = (RW << 16);			// R/W = o is write to display
	GPIOC->MODER |= SET_DATA_OUTPUT_MODE;
	GPIOA->ODR |= LED_PIN_SEL;			// LED2 ON, LCD ready to receive information
}

extern void LCD_cmd(unsigned char cmd){
	LCD_isready();			// stuck in loop until ready
	GPIOB->BSRR = ((RS | RW) << 16);			// RS=0 (settings mode), RW=0 (write)
	GPIOB->BSRR = EN;							// EN high
	delayLCD(0);								// give it some time
	GPIOC->ODR = cmd;							// write instruction for LCD
	delayLCD(0);								// ensure data is in D0-D7
	GPIOB->BSRR = (EN << 16);					// EN to BRR
	delayLCD(0);
	GPIOC->ODR |= 0x0;							//clear instruction

}


extern void LCD_cmd_strt(unsigned char cmd){
	//LCD_isready();							// DO NOT USE LCD_isready first time
	GPIOB->BSRR = ((RS | RW) << 16);			// RS=0 (settings mode), RW=0 (write)
	GPIOB->BSRR = EN;							// EN high
	delayLCD(0);								// give it some time
	GPIOC->ODR = cmd;							// write instruction for LCD
	delayLCD(0);								// ensure data is in D0-D7
	GPIOB->BSRR = (EN << 16);					// EN to BRR
	delayLCD(0);
	GPIOC->ODR |= 0x0;							//clear instruction


}


extern void LCD_acsii(char data){

	LCD_isready();
	GPIOB->BSRR = RS;							// RS=1 (data mode)
	GPIOB->BSRR = (RW << 16);					// RW=0 (write mode)
	delayLCD(0);
	GPIOB->BSRR = EN;							// EN high
	delayLCD(1);
	GPIOC->ODR = data;							// write "data" to ODR of port C
	delayLCD(0);
	GPIOB->BSRR = (EN << 16);					//EN low, finished sending data to LCD
	delayLCD(1);
	GPIOC->ODR |= 0x0;							//clear data

}

/* APPROX> milliseconds delay for a 16MHz System Clock. Check with Oscilloscope
 * if accuracy is paramount.
 */
extern void delayLCD(unsigned int ms){

	int i = 0;
	for(; ms > 0; ms--){
		for(i = 0; i < 3200; i++);
	}
}


extern void refreshLCD(void){
	GPIOC->MODER &= BUTTON_INPUT_MODE;		// PC13 button as input
	while((GPIOC->IDR & BUTTON_IS_HIGH)){
		delayLCD(1);
	}
	LCD_cmd(CLEAR_SCREEN);
	delayLCD(500);

}
