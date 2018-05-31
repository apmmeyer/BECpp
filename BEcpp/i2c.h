#ifndef I2C_H
#define I2C_H

#include <iostream>
#include <string>
#include <stdint.h>
#include <list>
#include <mraa/i2c.h>
#include "except.h"

using namespace std;

class i2c_device{
  private:
	mraa_i2c_context i2c_device_ctrl;
	uint8_t device_address = 0;

  public:
	i2c_device(){}; // Don't use it
	i2c_device(int bus);
	i2c_device(int bus, uint8_t control_address);
	~i2c_device();

	bool writeData(uint8_t addr, uint8_t val);
};

#endif
