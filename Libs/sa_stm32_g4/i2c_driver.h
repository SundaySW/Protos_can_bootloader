#ifndef I2C_DRIVER
#define I2C_DRIVER

#include "i2c_result.h"

class I2CDriver
{
public:
	enum BUS_STATUS
	{
		BUS_STATUS_READY = 0,
		BUS_STATUS_BUSY  = 1
	};

	enum TRANSACTION_RESULT
	{
		TRANSACTION_RESULT_UNKNOWN		    = 0x00,
		TRANSACTION_RESULT_OK               = 0x01,
		TRANSACTION_RESULT_BUFFER_OVERFLOW  = 0x02,
		TRANSACTION_RESULT_ARBITRATION_LOST = 0x03,
		TRANSACTION_RESULT_BUS_ERROR        = 0x04,
		TRANSACTION_RESULT_NACK_RECEIVED    = 0x05,
		TRANSACTION_RESULT_FAIL             = 0x06,
	};

	virtual BUS_STATUS GetBusStatus() const = 0;
	virtual char GetReadByte(int i) const = 0;
	virtual I2C_RESULT GetTransactionResult(bool acceptTransaction) = 0;
	
	virtual bool HasPendingTransaction() const = 0;
	virtual void ReadHandler() = 0;
	virtual void WriteHandler() = 0;
	virtual void ErrorHandler() = 0;
	
	virtual I2C_RESULT Read(uint8_t addr, uint8_t bytesToRead) = 0;
	virtual I2C_RESULT ReadSync(uint8_t addr, uint8_t* buffer, uint8_t bytesToRead) = 0;

	virtual I2C_RESULT Write(uint8_t addr, uint8_t* writeData, uint8_t bytesToWrite) = 0;
	virtual I2C_RESULT WriteReadSync(uint8_t addr, uint8_t *writeData, uint8_t bytesToWrite,
		uint8_t *readData, uint8_t bytesToRead, int pause) = 0;

	virtual I2C_RESULT WriteSync(uint8_t addr, uint8_t* writeData, uint8_t bytesToWrite) = 0;
	virtual I2C_RESULT WriteRead(uint8_t addr, uint8_t *writeData,
			uint8_t bytesToWrite, uint8_t bytesToRead) = 0;

	volatile bool PendingTransactionResult;	 //added SA 08.2022
};

#endif
