#include "RFID.h"

int i_LookFor(const char* buff, int size, const char ToFind){
	int res = 0;
	while((buff[res] != ToFind) && (res < size))
		res ++;
	return res;
}

bool b_2charToHex(const char MSByte, const char LSByte, uint8_t* Result){
	bool b_res = true;
	if ((MSByte <= '9') && (MSByte >= '0')) {
		(*Result) = ((MSByte - '0')& 0x0F)<<4;
	} else {
		if ((MSByte <= 'F') && (MSByte >= 'A')) {
			(*Result) = ((MSByte - 'A' + 0x0A)& 0x0F)<<4;
		} else {
			b_res = false;
		}
	}
	if ((LSByte <= '9') && (LSByte >= '0')) {
		(*Result) |= ((LSByte - '0')& 0x0F);
	} else {
		if ((LSByte <= 'F') && (LSByte >= 'A')) {
			(*Result) |= ((LSByte - 'A' + 0x0A)& 0x0F);
		} else {
			b_res = false;
		}
	}
	return b_res;
}

/* ----- Constructeurs ----- */
RFID_device::RFID_device(){
	// Set the RFID Uart port with default parameters and key length of 12
	m_uart_RFID_Control = mraa_uart_init(0);
	i_keyLength = 12;
}

RFID_device::RFID_device(int i_Key){
	// Set the RFID Uart port with default frame type and i_Key key length
	m_uart_RFID_Control = mraa_uart_init(0);
	i_keyLength = i_Key;
}

RFID_device::RFID_device(int i_Key, int i_bds){
	// Set the RFID Uart port with default frame type, i_Key key length and i_bds speed
	m_uart_RFID_Control = mraa_uart_init(0);
	i_keyLength = i_Key;
	mraa_uart_set_baudrate(m_uart_RFID_Control, i_bds);
}

RFID_device::RFID_device(int i_Key, int i_bds, int i_bytesize, mraa_uart_parity_t i_parity, int i_stopbits){
	/* Set the RFID Uart port with :
	 * i_Key key length,
	 * i_bds speed,
	 * i_bytesize size of data,
	 * i_parity parity mode,
	 * i_stopbits number of stop bit(s)
	 */
	m_uart_RFID_Control = mraa_uart_init(0);
	i_keyLength = i_Key;
	mraa_uart_set_baudrate(m_uart_RFID_Control, i_bds);
	mraa_uart_set_mode(m_uart_RFID_Control, i_bytesize, i_parity, i_stopbits);
}

/* ------ Destructeur ------ */
RFID_device::~RFID_device(){
	// Close the UART port
	mraa_uart_stop(m_uart_RFID_Control);
}


/* ------- Methodes -------- */
bool RFID_device::isAvailable(){
	// Indicates if a RFID frame is available
	return mraa_uart_data_available(m_uart_RFID_Control, 0);
}

int RFID_device::readData(char* buff, int length){
	// Get the data by the uart bus.
	// Be careful, verify that data is available (BUS.isAvailable()) before !
	return mraa_uart_read(m_uart_RFID_Control, buff, length);
}

bool RFID_device::ExtractData(uint8_t key[12]){
	int i_readed, i_posS, i_posE, i_idx;
	bool result = false;
	if ((i_readed = mraa_uart_read(m_uart_RFID_Control, pc_buffer, 64)) >= i_keyLength){
		if ((i_posS = i_LookFor(pc_buffer, i_readed, STARTBYTETag)) < i_readed) {
			if ((i_posE = i_LookFor(pc_buffer, i_readed, ENDBYTETag)) < i_readed) {
				if ((i_posE - i_posS) == (i_keyLength+1)) {
					cout<<"Received frame : ";
					for(i_idx = 1; i_idx <= 12; i_idx ++){
						cout<<pc_buffer[i_posS+i_idx]<<" ";
						key[i_idx-1] = pc_buffer[i_posS+i_idx];
					}
					cout<<endl;
					result = true;
				}
			}
		}
	}
	return result;
}

uint64_t RFID_device::getTag(){
	int i_readed, i_posS, i_posE, i_idx;
	uint8_t hexaTag[6];
	uint8_t c_checksum;
	uint64_t u64_return = 0;
	if ((i_readed = mraa_uart_read(m_uart_RFID_Control, pc_buffer, 64)) >= i_keyLength){
		if ((i_posS = i_LookFor(pc_buffer, i_readed, STARTBYTETag)) < i_readed) {
			if ((i_posE = i_LookFor(pc_buffer, i_readed, ENDBYTETag)) < i_readed) {
				if ((i_posE - i_posS) == (i_keyLength+1)) {
					cout<<"Received frame : ";
					for(i_idx = 1; i_idx <= 12; i_idx ++)
						cout<<pc_buffer[i_posS+i_idx]<<" ";
					cout<<endl;
					for(i_idx = 0; i_idx < 6; i_idx ++) {
						if(!b_2charToHex(pc_buffer[i_posS+1+i_idx*2], pc_buffer[i_posS+1+(i_idx*2)+1], (hexaTag+i_idx))) {
							throw _exception("RFID.cpp:95 : uint64_t RFID_device::getTag() : Received frame isn't correct");
						}
					}
					cout<<"Hexa obtained : "<<hex<<(int)hexaTag[0]<<" ";
					cout<<hex<<(int)hexaTag[1]<<" ";
					cout<<hex<<(int)hexaTag[2]<<" ";
					cout<<hex<<(int)hexaTag[3]<<" ";
					cout<<hex<<(int)hexaTag[4]<<" ";
					cout<<hex<<(int)hexaTag[5]<<endl;
					c_checksum = hexaTag[0] ^ hexaTag[1] ^ hexaTag[2] ^ hexaTag[3] ^ hexaTag[4];
					if (c_checksum != hexaTag[5]) {
						throw _exception("RFID.cpp:95 : uint64_t RFID_device::getTag() : Bad checksum");
					}
					for(i_idx = 2; i_idx < 5; i_idx++)					
						u64_return |= ((uint64_t)hexaTag & 0xFF) << (8 * (4 - i_idx));
					return u64_return;
				}
			}
		}
	}
	return -1;
}


