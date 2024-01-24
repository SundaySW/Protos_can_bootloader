#pragma once

#include "sa_stm32_g4/eeprom_g4_map.hpp"

#define WAIT_TIME_IN_BOOT_SEC (7)

uint32_t EEPROM_START_ADDR = ADDR_FLASH_PAGE_63;

#define BLOCK_SIZE_FLASH        (FLASH_WRITE_BLOCK_SIZE)
#define BYTES_IN_PACKET         (8)
#define PACKETS_IN_BLOCK        (BLOCK_SIZE_FLASH/BYTES_IN_PACKET)
#define RESEND_PACKETS_N_TIMES  (3)
#define DEFAULT_DEVICE_ADDR     (0xFF)

#define EEPROM_I2C_ADDR         0x50

enum BOARD_ERROR{
    NO_ERROR,
    CAN_ERROR,
    EEPROM_ERROR,
    UNEXPECTED_PACKET_IN_BLOCK,
    LIMIT_SWITCH_ERROR,
    STANDBY_MOVEMENT_ERROR,
};

enum HEX_CMD_IN_BOOT_MSG{
    HEX_DATA = 0x00,
    HEX_LAST_DATA = 0x01,
    HEX_ADDRofMAIN = 0x05,
};

static inline void delay(uint32_t delayms){
    __HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE);
    TIM6->CNT = 0;
    HAL_TIM_Base_Start(&htim6);
    while(TIM6->CNT < delayms){}
    HAL_TIM_Base_Stop(&htim6);
    __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
}
