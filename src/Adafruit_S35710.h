/**
 * @file Adafruit_S35710.h
 * @brief Library for interfacing with the S-35710 Wake-Up Timer IC.
 * 
 * This library provides an interface for setting and getting values from the S-35710 Wake-Up Timer IC.
 * It uses the I2C protocol for communication and provides methods for reading and writing to the chip's registers.
 * 
 * @author Ladyada
 * 
 * @section LICENSE
 * 
 * MIT License
 * 
 * @section DEPENDENCIES
 * 
 * This library depends on the Adafruit_BusIO and Wire libraries.
 * 
 */

#ifndef ADAFRUIT_S35710_H
#define ADAFRUIT_S35710_H

#include <Adafruit_BusIO_Register.h>
#include <Wire.h>

#define S35710_I2C_ADDRESS 0x32 ///< I2C Address for S-35710


/**
 * @class Adafruit_S35710
 * @brief Main class for interfacing with the S-35710 Wake-Up Timer IC.
 */
class Adafruit_S35710 {
public:
  Adafruit_S35710(uint8_t reset_pin);

  // Initialization function
  bool begin(TwoWire *theWire = &Wire, uint8_t addr=S35710_I2C_ADDRESS);

  // Function to set the wake-up time register
  bool setWakeUpTimeRegister(uint32_t value);

  // Function to get the wake-up time register
  int32_t getWakeUpTimeRegister();

  // Function to get the time register
  int32_t getTimeRegister();

  bool reset(void);

private:
  Adafruit_I2CDevice *i2c_dev; // I2C device
  uint8_t _reset_pin;          // Reset pin
};

#endif // ADAFRUIT_S35710_H
