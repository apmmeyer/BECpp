#ifndef RFID_H
#define RFID_H

#include <mraa/uart.h>
#include "except.h"

#define STARTBYTETag 2
#define ENDBYTETag   3


class RFID_device{
  private:
	mraa_uart_context m_uart_RFID_Control;
	int i_keyLength;
	char pc_buffer[64];

  public:
	RFID_device();
	RFID_device(int i_Key);
	RFID_device(int i_Key, int i_bds);
	RFID_device(int i_Key, int i_bds, int i_bytesize, mraa_uart_parity_t i_parity, int i_stopbits);

	~RFID_device();

	bool isAvailable();
	int readData(char* buff, int length);
	bool ExtractData(uint8_t key[12]);
	
	uint64_t getTag();// Non fonctionnel du fait des erreurs de communication UART


};



#endif
