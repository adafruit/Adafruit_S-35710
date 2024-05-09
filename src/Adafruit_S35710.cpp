/**
 * @file Adafruit_S35710.cpp
 * @brief Implementation for the Adafruit_S35710 library.
 * 
 * This file provides the function implementations for the Adafruit_S35710 
 * library, which interfaces with the S-35710 Wake-Up Timer IC.
 * 
 * @author Limor 'ladyada' Fried
 *
 * @section LICENSE
 * 
 * MIT License
 * 
 */

#include "Adafruit_S35710.h"

/**
 * @brief Constructor for the Adafruit_S35710 class.
 * 
 */
Adafruit_S35710::Adafruit_S35710(uint8_t reset_pin) 
  : _reset_pin(reset_pin), i2c_dev(nullptr) 
{}

/**
 * @brief Initialization function to verify S-35710 chip is found on I2C bus
 * 
 * @param theWire Pointer to the I2C interface. Default is &Wire.
 * @param addr The I2C address, defaults to S35710_I2C_ADDRESS
 * @return true if initialization was successful, false otherwise.
 */
bool Adafruit_S35710::begin(TwoWire *theWire, uint8_t addr) {
  if (i2c_dev) {
    delete i2c_dev;
    i2c_dev = nullptr;
  }

  i2c_dev = new Adafruit_I2CDevice(addr, theWire);
  return i2c_dev->begin();
}

/**
 * @brief Perform a hardware reset, by toggling the reset pin set
 * during device instantiation
 */
bool Adafruit_S35710::reset(void) {
  if (_reset_pin < 0) {
    return false;
  }

  pinMode(_reset_pin, OUTPUT);
  digitalWrite(_reset_pin, HIGH);
  delay(10);
  digitalWrite(_reset_pin, LOW);
  delay(10);
  digitalWrite(_reset_pin, HIGH);
  return true;
}

/**
 * @brief Function to set the wake-up time register. The wake-up time 
 * register is a 3-byte register that stores the wake-up time of the
 * microcontroller in seconds.
 * 
 * @param value The seconds 'value' set in the wake-up time register.
 * @return true if the i2c write operation was successful, false otherwise.
 */
bool Adafruit_S35710::setWakeUpTimeRegister(uint32_t value) {
  if (!i2c_dev) {
    return false;
  }

  uint8_t buffer[4];
  buffer[0] = 0x81; // Dummy byte
  buffer[1] = (value >> 16) & 0xFF; // MSB
  buffer[2] = (value >> 8) & 0xFF;
  buffer[3] = value & 0xFF;        // LSB

  return i2c_dev->write(buffer, 4);
}

/**
 * @brief Function to get the wake-up time register. Note this will set reset
 * pin to high in order to perform the read
 * 
 * @return The value from the wake-up time register, 
 * or -1 if there was an error.
 */
int32_t Adafruit_S35710::getWakeUpTimeRegister() {
  if (!i2c_dev) {
    return -1;
  }

  uint8_t dummy_byte = 0x01;
  uint8_t buffer[3];

  if (!i2c_dev->write_then_read(&dummy_byte, 1, buffer, 3, false)) {
    return -1;
  }

  uint32_t value = 0;
  value |= ((uint32_t)buffer[0] << 16); // MSB
  value |= ((uint32_t)buffer[1] << 8);
  value |= buffer[2];                   // LSB

  return value;
}

/**
 * @brief Function to get the time register. The time register is a 3-byte 
 * register that stores the timer value in seconds. The time register is 
 * read-only.
 * 
 * @return The value from the time register, or -1 if there was an error.
 */
int32_t Adafruit_S35710::getTimeRegister() {
  if (!i2c_dev) {
    return -1;
  }

  uint8_t buffer[3];

  if (!i2c_dev->read(buffer, 3, false)) {
    return -1;
  }

  uint32_t value = 0;
  value |= ((uint32_t)buffer[0] << 16); // MSB
  value |= ((uint32_t)buffer[1] << 8);
  value |= buffer[2];                   // LSB

  return value;
}
