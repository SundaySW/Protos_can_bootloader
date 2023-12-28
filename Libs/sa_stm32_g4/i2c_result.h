#pragma once

#ifndef PROTOCOL_I2C_I2C
    #define PROTOCOL_I2C 0x10
#endif

enum I2C_RESULT
{
	I2C_OK				   	= PROTOCOL_I2C|0x00,
    I2C_BUFFER_OVERFLOW    	= PROTOCOL_I2C|0x01, ///< I2C buffer overflow
	I2C_ARBITRATION_LOST   	= PROTOCOL_I2C|0x03, ///< I2C arbitration lost
	I2C_BUS_NOT_READY      	= PROTOCOL_I2C|0x04, ///< Invalidate bus state
	I2C_NACK_RECEIVED      	= PROTOCOL_I2C|0x05, ///< NACK received from slave
	I2C_UNEXPECTED_STATE   	= PROTOCOL_I2C|0x06,
	I2C_RW_TIMEOUT		   	= PROTOCOL_I2C|0x07, ///< transaction timeout
	I2C_PENDING			   	= PROTOCOL_I2C|0x08, ///< transaction pending
	I2C_RW_ERROR		   	= PROTOCOL_I2C|0x09, ///< common read/write error
	I2C_INVALID_BUFFER_SIZE = PROTOCOL_I2C|0x0A, ///< invalid read/write buffer size 

	I2C_HAL_OK       = 0x0B,
	I2C_HAL_ERROR    = 0x0C,
	I2C_HAL_BUSY     = 0x0D,
	I2C_HAL_TIMEOUT  = 0x0E,
};