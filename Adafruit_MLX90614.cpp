#include "Adafruit_MLX90614.h"

Adafruit_MLX90614::~Adafruit_MLX90614() {
  if (i2c_dev)
    delete i2c_dev;
}


bool Adafruit_MLX90614::begin(uint8_t addr, TwoWire *wire) {
  _addr = addr; 
  if (i2c_dev)
    delete i2c_dev;
  i2c_dev = new Adafruit_I2CDevice(addr, wire);
  return i2c_dev->begin();
}


uint16_t Adafruit_MLX90614::readEmissivityReg(void) {
  return read16(MLX90614_EMISS);
}

void Adafruit_MLX90614::writeEmissivityReg(uint16_t ereg) {
  write16(MLX90614_EMISS, 0); // erase
  delay(10);
  write16(MLX90614_EMISS, ereg);
  delay(10);
}

double Adafruit_MLX90614::readEmissivity(void) {
  uint16_t ereg = read16(MLX90614_EMISS);
  if (ereg == 0)
    return NAN;
  return ((double)ereg) / 65535.0;
}

void Adafruit_MLX90614::writeEmissivity(double emissivity) {
  uint16_t ereg = (uint16_t)(0xffff * emissivity);

  writeEmissivityReg(ereg);
}


double Adafruit_MLX90614::readObjectTempF(void) {
  return (readTemp(MLX90614_TOBJ1) * 9 / 5) + 32;
}

double Adafruit_MLX90614::readAmbientTempF(void) {
  return (readTemp(MLX90614_TA) * 9 / 5) + 32;
}


double Adafruit_MLX90614::readObjectTempC(void) {
  return readTemp(MLX90614_TOBJ1);
}


double Adafruit_MLX90614::readAmbientTempC(void) {
  return readTemp(MLX90614_TA);
}

float Adafruit_MLX90614::readTemp(uint8_t reg) {
  float temp;

  temp = read16(reg);
  if (temp == 0)
    return NAN;
  temp *= .02;
  temp -= 273.15;
  return temp;
}


uint16_t Adafruit_MLX90614::read16(uint8_t a) {
  uint8_t buffer[3];
  buffer[0] = a;
  
  bool status = i2c_dev->write_then_read(buffer, 1, buffer, 3);
  if (!status)
    return 0;
  
  return uint16_t(buffer[0]) | (uint16_t(buffer[1]) << 8);
}

byte Adafruit_MLX90614::crc8(byte *addr, byte len)

{
  byte crc = 0;
  while (len--) {
    byte inbyte = *addr++;
    for (byte i = 8; i; i--) {
      byte carry = (crc ^ inbyte) & 0x80;
      crc <<= 1;
      if (carry)
        crc ^= 0x7;
      inbyte <<= 1;
    }
  }
  return crc;
}

void Adafruit_MLX90614::write16(uint8_t a, uint16_t v) {
  uint8_t buffer[4];

  buffer[0] = _addr << 1;
  buffer[1] = a;
  buffer[2] = v & 0xff;
  buffer[3] = v >> 8;

  uint8_t pec = crc8(buffer, 4);

  buffer[0] = buffer[1];
  buffer[1] = buffer[2];
  buffer[2] = buffer[3];
  buffer[3] = pec;

  i2c_dev->write(buffer, 4);
}
