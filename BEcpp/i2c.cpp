#include "i2c.h"

using namespace std;

i2c_device::i2c_device(int bus){
    i2c_device_ctrl = mraa_i2c_init(bus);
}

i2c_device::i2c_device(int bus, uint8_t control_address){
    i2c_device_ctrl = mraa_i2c_init(bus);
    mraa_i2c_address(i2c_device_ctrl, control_address);
	device_address = control_address;
}

i2c_device::~i2c_device(){
    mraa_i2c_stop(i2c_device_ctrl);
}

bool i2c_device::writeData(uint8_t addr, uint8_t val){
	bool result = false;
	mraa_result_t error = MRAA_SUCCESS;

	uint8_t data[2] = {addr, val};
    error += mraa_i2c_address (i2c_device_ctrl, device_address);
    error += mraa_i2c_write (i2c_device_ctrl, data, 2);
	
	if(error == MRAA_SUCCESS) result = true;
	return result;
}


