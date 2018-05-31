#include "lcd.h"

using namespace std;
/*
mraa_i2c_context i2c_lcd_ctrl(LCD_I2C_BUS, LCD_I2C_LCDaddr);
	mraa_i2c_context i2c_rgb_ctrl(LCD_I2C_BUS, LCD_I2C_RGBaddr);*/

void i2cData (mraa_i2c_context ctx, uint8_t value) {
    uint8_t data[2] = { LCD_DATA, value };
    mraa_i2c_address (ctx, m_lcd_control_address);
    mraa_i2c_write (ctx, data, 2);
}

void i2Cmd (mraa_i2c_context ctx, uint8_t value) {
    uint8_t data[2] = { LCD_CMD, value };
	mraa_i2c_address (ctx, m_lcd_control_address);
	mraa_i2c_write (ctx, data, 2);
}

void i2cReg (mraa_i2c_context ctx, uint8_t deviceAdress, uint8_t addr, uint8_t value) {
    uint8_t data[2] = { addr, value };
    mraa_i2c_address (ctx, deviceAdress);
    mraa_i2c_write (ctx, data, 2);
}

void lcd_device::setColor(uint8_t R, uint8_t G, uint8_t B){
	i2cReg(i2c_rgb_ctrl, LCD_I2C_RGBaddr, 0x04, R);
	i2cReg(i2c_rgb_ctrl, LCD_I2C_RGBaddr, 0x03, G);
	i2cReg(i2c_rgb_ctrl, LCD_I2C_RGBaddr, 0x02, B);
}

void lcd_device::goto_xy(uint8_t x, uint8_t y){
	x = (y == 0 ? x|0x80 : x|0xc0);
    i2cCmd(i2c_lcd_ctrl, x);
}

void lcd_device::writeText(uint8_t[] text, int len){
	int i;
	for(i = 0; i < len; i++){
		i2cData(i2c_lcd_ctrl, text[i]);
	}
}
/*
i2cReg(m_i2c_lcd_rgb, m_rgb_control_address, 0, 0);
i2cReg(m_i2c_lcd_rgb, m_rgb_control_address, 1, 0);
i2cReg(m_i2c_lcd_rgb, m_rgb_control_address, 0x08, 0xAA);
*/
