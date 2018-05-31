#ifndef LCD_H
#define LCD_H

#include <iostream>
#include <string>
#include <stdint.h>
#include <list>
#include "i2c.h"
#include "except.h"

#define LCD_I2C_BUS			0
#define LCD_I2C_RGBaddr		0x62
#define LCD_I2C_LCDaddr		0x3E

#define LCD_CLEARDISPLAY	0x01
#define LCD_ENTRYMODESET	0x04
#define LCD_FUNCTIONSET 	0x20
#define LCD_DATA			0x40
#define LCD_CMD				0x80
#define LCD_DISPLAYCONTROL	0x08
#define LCD_DISPLAYON		0x04

// flags for display entry mode
#define LCD_ENTRYLEFT		0x02
#define LCD_ENTRYSHIFTDECREMENT	0x00

#define LCD_2LINE 0x08

using namespace std;


class lcd_device{
  private:
	mraa_i2c_context i2c_lcd_ctrl(LCD_I2C_BUS, LCD_I2C_LCDaddr);
	mraa_i2c_context i2c_rgb_ctrl(LCD_I2C_BUS, LCD_I2C_RGBaddr);

  public:
	lcd_device(){};
	~lcd_device(){};

	void setColor(uint8_t R, uint8_t G, uint8_t B);
	void goto_xy(uint8_t x, uint8_t y);
	void writeText(uint8_t[] text, int len);
};

#endif
