#ifndef APP_EEPROM_HPP_
#define APP_EEPROM_HPP_

#include "stm32g4xx_hal.h"

#define EEPROM_1WIRE_DATA_SIZE          320 //OneWire::ParamTable::MAX_ROWS_COUNT * OneWire::ParamTable::ROW_SIZE
#define EEPROM_BOARD_DATA_START_ADDR    (EEPROM_1WIRE_DATA_SIZE)
#define EEPROM_BOARD_DATA_SIZE          (sizeof(uint8_t) * 7) //UID(4byte) ADDR(1byte) + HWver(1byte) + SWver(1byte) + 1byte
#define EEPROM_PARAMS_START_ADDR        (EEPROM_BOARD_DATA_START_ADDR + EEPROM_BOARD_DATA_SIZE)
#define PARAM_NOFCALIB_FIELDS           2
#define EEPROM_CALIB_DATA_SIZE          (PARAM_NOFCALIB_FIELDS * sizeof(float))

bool eeprom_write_block(unsigned int offset, const char* buffer, unsigned int len);

bool eeprom_read_block(unsigned int offset, char* buffer, unsigned int len);

bool eeprom_write_byte(unsigned int offset, char id);

#endif /* APP_EEPROM_HPP_ */