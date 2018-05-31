/*
sites de recherche
https://www.arduino.cc/en/ArduinoCertified/IntelEdison
http://iotdk.intel.com/docs/master/mraa/dir_1bbdfdabbe53a0e81c14e2e2208abfc8.html
http://wiki.seeedstudio.com/Grove-125KHz_RFID_Reader/
*/

#include <mraa/i2c.h>
#include <mraa/uart.h>
#include <mraa/gpio.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "except.h"
#include "buzzer.h"
#include "RFID.h"
#include "tag_list.h"



#define LCD_CLEARDISPLAY 0x01
#define LCD_ENTRYMODESET 0x04
#define LCD_FUNCTIONSET 0x20
#define LCD_DATA 0x40
#define LCD_CMD 0x80
#define LCD_DISPLAYCONTROL 0x08
#define LCD_DISPLAYON 0x04

// flags for display entry mode
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTDECREMENT 0x00

#define LCD_2LINE 0x08


using namespace std;

int m_bus=0;
uint8_t m_lcd_control_address=0x3E;
uint8_t m_rgb_control_address=0x62;

mraa_result_t i2cData (mraa_i2c_context ctx, uint8_t value) {
    mraa_result_t error = MRAA_SUCCESS;

    uint8_t data[2] = { LCD_DATA, value };
    error = mraa_i2c_address (ctx, m_lcd_control_address);
    error = mraa_i2c_write (ctx, data, 2);

    return error;
}

mraa_result_t i2Cmd (mraa_i2c_context ctx, uint8_t value) {
    mraa_result_t error = MRAA_SUCCESS;

    uint8_t data[2] = { LCD_CMD, value };
    error = mraa_i2c_address (ctx, m_lcd_control_address);
    error = mraa_i2c_write (ctx, data, 2);

    return error;
}

mraa_result_t i2cReg (mraa_i2c_context ctx, uint8_t deviceAdress, uint8_t addr, uint8_t value) {
    mraa_result_t error = MRAA_SUCCESS;

    uint8_t data[2] = { addr, value };
    error = mraa_i2c_address (ctx, deviceAdress);
    error = mraa_i2c_write (ctx, data, 2);

    return error;
}

//mraa_result_t


int main(){

    mraa_i2c_context m_i2c_lcd_control, m_i2c_lcd_rgb;
	mraa_uart_context m_uart_RFID_Control;
	mraa_gpio_context m_gpio;
    //uint64_t tag;
	RFID_device RFID_control(12, 9600, 8, MRAA_UART_PARITY_NONE, 1);
	tag_list maliste;
	uint8_t tag[12];

    // initialisation
	mraa_init();
	m_gpio = mraa_gpio_init(2);
	mraa_gpio_dir(m_gpio, MRAA_GPIO_OUT);

	m_uart_RFID_Control = mraa_uart_init(0);
	mraa_uart_set_baudrate(m_uart_RFID_Control, 9600);
	mraa_uart_set_mode(m_uart_RFID_Control, 8,MRAA_UART_PARITY_NONE, 1);

    m_i2c_lcd_control = mraa_i2c_init(m_bus);
    mraa_i2c_address(m_i2c_lcd_control, m_lcd_control_address);

	m_i2c_lcd_rgb = mraa_i2c_init(m_bus);
	mraa_i2c_address(m_i2c_lcd_rgb, m_rgb_control_address);

	usleep(50000);
	i2Cmd (m_i2c_lcd_control, LCD_FUNCTIONSET | LCD_2LINE);
	usleep(4500);
	i2Cmd (m_i2c_lcd_control, LCD_FUNCTIONSET | LCD_2LINE);
	usleep(4500);
	i2Cmd (m_i2c_lcd_control, LCD_FUNCTIONSET | LCD_2LINE);
	usleep(4500);
	i2Cmd (m_i2c_lcd_control, LCD_FUNCTIONSET | LCD_2LINE);
	i2Cmd (m_i2c_lcd_control, LCD_DISPLAYCONTROL | LCD_DISPLAYON);
	i2Cmd (m_i2c_lcd_control, LCD_CLEARDISPLAY);
	usleep(4500);
	i2Cmd (m_i2c_lcd_control, LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
	i2cReg (m_i2c_lcd_rgb, m_rgb_control_address, 0, 0);
	i2cReg (m_i2c_lcd_rgb, m_rgb_control_address, 1, 0);
	i2cReg (m_i2c_lcd_rgb, m_rgb_control_address, 0x08, 0xAA);

	i2cReg (m_i2c_lcd_rgb, m_rgb_control_address, 0x04, 0);
	i2cReg (m_i2c_lcd_rgb, m_rgb_control_address, 0x03, 0);
	i2cReg (m_i2c_lcd_rgb, m_rgb_control_address, 0x02, 255);

	// Ajout barbare des tags. A améliorer
	tag[0] = '4';tag[1] = '2';tag[2] = '0';tag[3] = '0';
	tag[4] = '5';tag[5] = 'F';tag[6] = 'D';tag[7] = '4';tag[8] = 'C';tag[9] = 'B';
	tag[10] = '0';tag[11] = '2';
	maliste.addElement(tag);
	tag[0] = '4';tag[1] = '2';tag[2] = '0';tag[3] = '0';
	tag[4] = '2';tag[5] = 'C';tag[6] = '8';tag[7] = '6';tag[8] = '7';tag[9] = 'F';
	tag[10] = '9';tag[11] = '7';
	maliste.addElement(tag);
	tag[0] = '0';tag[1] = '9';tag[2] = '0';tag[3] = '0';
	tag[4] = '2';tag[5] = 'E';tag[6] = '3';tag[7] = '5';tag[8] = '9';tag[9] = '3';
	tag[10] = '8';tag[11] = '1';
	maliste.addElement(tag);
	tag[0] = 'A';tag[1] = '9';tag[2] = '0';tag[3] = '0';
	tag[4] = '5';tag[5] = '2';tag[6] = 'D';tag[7] = '2';tag[8] = '5';tag[9] = 'E';
	tag[10] = '7';tag[11] = '7';
	maliste.addElement(tag);

	maliste.print();

	while(1){
		if(RFID_control.isAvailable()){
			try{
				if(RFID_control.ExtractData(tag)){
					maliste.countElement(tag);
				}
				/*tag = RFID_control.getTag();
				if(tag != -1)
					cout<<"Tag received : "<<tag<<endl;
				else
					cout<<"Error in tag reception"<<endl;*/
			} catch(_exception e) {
				e.Error();
			}
		}
	}

    // fin
    sleep(3);
    mraa_i2c_stop(m_i2c_lcd_control);
    mraa_i2c_stop(m_i2c_lcd_rgb);
    mraa_uart_stop(m_uart_RFID_Control);
    return 0;
}
/*
		if(mraa_uart_data_available(m_uart_RFID_Control, 0)){
			i_len = mraa_uart_read(m_uart_RFID_Control, pc_buffer, 64);
			pc_buffer[i_len] = '\0';
			mraa_gpio_write(m_gpio, 1);
			cout<<"Reception de "<<i_len<<" caractères : "<<pc_buffer<<endl;
			cout<<"Start bit : "<<(int)pc_buffer[0]<<" | Stop bit : "<<(int)pc_buffer[i_len-1]<<endl;
			sleep(2);
			mraa_gpio_write(m_gpio, 0);
*/
