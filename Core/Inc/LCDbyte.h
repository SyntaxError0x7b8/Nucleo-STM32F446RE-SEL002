/*
 * LCDbyte.h
 *
 *  Created on: Aug 2, 2024
 *      Author: jtmen
 */

#ifndef INC_LCDBYTE_H_
#define INC_LCDBYTE_H_

//RCC
#define RCC_GPIOA_SEL	(1)
#define RCC_GPIOB_SEL	(2)
#define RCC_GPIOC_SEL	(4)

#define RS (0x20)				//Pin5 mask RS=0 command register, RS=1 data register
#define RW (0x40)				//Pin6 mask RW=0 write , RW=1 read
#define EN (0x80)				//Pin7 mask to enable data transfer

/* LD2 PA5 */
#define CLEAR_LED_MODE		(~0x00000C00)
#define SET_LED_OUTPUT_MODE	(0x00000400)
#define LED_PIN_SEL			(0x00000020)

// LCD  Instruction register
#define CLEAR_SCREEN		(0x01)
#define RETURN_CRS_HOME		(0x02)
#define SHIFT_CRS_RGHT		(0x06)
#define DISPLAY_ON_BLINK	(0x0F)
#define FORCE_FIRST_LINE	(0x80)
#define FORCE_SECOND_LINE	(0xC0)
#define EIGHT_BIT			(0x38)
#define FOUR_BIT			(0x28)
#define FUNC_SET_EIGHT		(0x30)

// data
#define CLEAR_DATA_MODE			(~0x0000FFFF)		// clear mode for all data pins
#define SET_DATA_OUTPUT_MODE	(0x00005555)		// all data pins as output
#define BUSY_FLAG				(0x80)				//D7=1 busy

// control
#define CLEAR_CTRL_MODE		(~0x0000FC00)
#define SET_CTRL_OUTPUT		(0x00005400)

// PC13 button
#define BUTTON_INPUT_MODE		(~0x0C000000)
#define BUTTON_IS_HIGH			(0x00002000)



//functions
extern void delayLCD(unsigned int ms);
extern void LCD_cmd(unsigned char comd);
extern void LCD_cmd_strt(unsigned char cmd);
extern void LCD_acsii(char data);
extern void LCD_init(void);
extern void LCD_isready(void);
extern void init_ports(void);
extern void refreshLCD(void);

#endif /* INC_LCDBYTE_H_ */
