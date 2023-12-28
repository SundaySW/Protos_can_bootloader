//010620201

#ifndef APP_I2C_HPP_
#define APP_I2C_HPP_

#include "stm32g4xx_hal.h"
#include "i2c_driver.h"

class I2C : public I2CDriver
{
public:
	enum
	{
		WRITE_BUFFER_SIZE = 8,
		READ_BUFFER_SIZE  = 8
	};

	I2C() = default;
	I2C (I2C_HandleTypeDef* _i2c)
		: i2c (_i2c)
    {}

	BUS_STATUS GetBusStatus() const
	{
		return Status;
	}

	char GetReadByte(int i) const
	{
		return ReadBuffer[i];
	}

	I2C_RESULT GetTransactionResult(bool acceptTransaction)
	{
		return Result;
	}

	bool HasPendingTransaction() const
	{
//		return PendingTransactionResult;
        return Status == BUS_STATUS_READY ? 1 : 0;
	}
//	void HandleInterrupt()
//	{
//		return;
//	}

	I2C_RESULT Read(uint8_t addr, uint8_t bytesToRead)
	{
		return WriteRead(addr, 0, 0, bytesToRead);
	}

	I2C_RESULT ReadSync(uint8_t addr, uint8_t* buffer, uint8_t bytesToRead)
	{
		HAL_StatusTypeDef HAL_ANSW = HAL_I2C_Master_Receive(i2c, (addr << 1), buffer, bytesToRead, 10);

		return convertHALtoI2CResult(HAL_ANSW);
	}

	I2C_RESULT Write(uint8_t addr, uint8_t* writeData, uint8_t bytesToWrite)
	{
		return WriteRead(addr, writeData, bytesToWrite, 0);
	}

	I2C_RESULT WriteReadSync(uint8_t addr, uint8_t *writeData, uint8_t bytesToWrite,uint8_t *readData, uint8_t bytesToRead, int pause)
	{
		HAL_StatusTypeDef HAL_ANSW = HAL_I2C_Master_Transmit(i2c, (addr << 1), writeData, bytesToWrite, 10);
		if (HAL_ANSW != HAL_OK)
		{
			convertHALtoI2CResult(HAL_ANSW);
		}

		if (pause)
			HAL_Delay(pause);

		HAL_ANSW = HAL_I2C_Master_Receive(i2c, (addr << 1), readData, bytesToRead, 10);

		return convertHALtoI2CResult(HAL_ANSW);
	}

	I2C_RESULT WriteSync(uint8_t addr, uint8_t* writeData, uint8_t bytesToWrite)
	{
		HAL_StatusTypeDef  HAL_ANSW =  HAL_I2C_Master_Transmit(i2c, (addr << 1), writeData, bytesToWrite, 10);
		return convertHALtoI2CResult(HAL_ANSW);
	}

	I2C_RESULT WriteRead(uint8_t addr, uint8_t *writeData, uint8_t bytesToWrite, uint8_t bytesToRead)
	{
		if (bytesToWrite > WRITE_BUFFER_SIZE) {
			return I2C_RESULT::I2C_RW_ERROR;
		}
		if (bytesToRead > READ_BUFFER_SIZE) {
			return I2C_RESULT::I2C_RW_ERROR;
		}

		if (Status == BUS_STATUS_READY)
		{
			Status = BUS_STATUS_BUSY;
			Result = I2C_PENDING;
			PendingTransactionResult = false;

			for (uint8_t bufferIndex = 0; bufferIndex < bytesToWrite; bufferIndex++)
			{
				WriteBuffer[bufferIndex] = writeData[bufferIndex];
			}

			SlaveAddress = addr << 1;
			WriteData = WriteBuffer;
			ReadData  = ReadBuffer;

			BytesToWrite = bytesToWrite;
			BytesToRead = bytesToRead;
			BytesWritten = 0;
			BytesRead = 0;

			if (bytesToWrite > 0)
			{
				HAL_I2C_Master_Transmit_IT(i2c, SlaveAddress, WriteData, BytesToWrite);
			}
			else if (bytesToRead > 0)
			{
				HAL_I2C_Master_Receive_IT(i2c, SlaveAddress, ReadData, BytesToRead);
			}
			return I2C_RESULT::I2C_OK;
		}

//		return I2C_RESULT::I2C_RW_ERROR;
		return I2C_RESULT::I2C_BUS_NOT_READY;
	}

	void ReadHandler()
	{
//		/* Fetch data if bytes to be read. */
//		if (BytesRead < READ_BUFFER_SIZE)
//		{
//			ReadData[BytesRead] = I2C->DATA.reg;
//			BytesRead++;
//		}
//		/* If buffer overflow, issue STOP and BUFFER_OVERFLOW condition. */
//		else
//		{
//			I2C->CTRLB.bit.ACKACT = 0;
//			I2C->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);  //send STOP
//			TransactionFinished(TRANSACTION_RESULT_BUFFER_OVERFLOW);
//		}
//		/* If more bytes to read, issue ACK and start a byte read. */
//		if (BytesRead < BytesToRead)
//		{
//			I2C->CTRLB.bit.ACKACT = 0;
//			I2C->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(2); // issue ACK
//			// Wait synchronization
//			while (I2C->SYNCBUSY.bit.SYSOP);
//		}
//		/* If transaction finished, issue NACK and STOP condition. */
//		else
//		{
//			/* Send NACK and STOP */
//			I2C->CTRLB.bit.ACKACT = 1;
//			I2C->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
//			// Wait synchronization
//			while (I2C->SYNCBUSY.bit.SYSOP);
//			TransactionFinished(TRANSACTION_RESULT_OK);
//		}
		BytesRead += BytesToRead;
		TransactionFinished(I2C_OK);
	}

	void WriteHandler()
	{
		/* If NOT acknowledged (NACK) by slave cancel the transaction. */
//		if (I2C->STATUS.reg & SERCOM_I2CM_STATUS_RXNACK)
//		{
//			//send STOP
//			I2C->CTRLB.bit.ACKACT = 0;
//			I2C->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
//			// Wait synchronization
//			while (I2C->SYNCBUSY.bit.SYSOP);
//			Result = TRANSACTION_RESULT_NACK_RECEIVED;
//			Status = BUS_STATUS_READY;
//		}
//		/* If more bytes to write, send data. */
//		else if (BytesWritten < BytesToWrite)
//		{
//			uint8_t data = WriteData[BytesWritten];
//			I2C->DATA.reg = data;
//			// Wait synchronization
//			while (I2C->SYNCBUSY.bit.SYSOP);
//			++BytesWritten;
//		}
		/* If bytes to read, send repeated START condition + Address +
		 * 'R/_W = 1'
		 */
		BytesWritten += BytesToWrite;

		if (BytesRead < BytesToRead)
		{
			HAL_I2C_Master_Receive_IT(i2c, SlaveAddress, ReadData, BytesToRead);
		}
		/* If transaction finished, send STOP condition and set RESULT OK. */
		else
		{
			TransactionFinished(I2C_OK);
		}
	}

	void ErrorHandler()
	{
        __disable_irq();
        while (1)
        {

        }
	}

	void TransactionFinished(I2C_RESULT result)
	{
		Result = result;
		Status = BUS_STATUS_READY;
		PendingTransactionResult = true;
	}

//	volatile bool PendingTransactionResult;	 //added SA 08.2022
protected:
	I2C_RESULT convertHALtoI2CResult(HAL_StatusTypeDef HAL_ANSW)
	{
		switch (HAL_ANSW)
		{
		case HAL_OK:
			return I2C_OK;
		case HAL_ERROR:
			return I2C_HAL_ERROR;
		case HAL_BUSY:
			return I2C_HAL_BUSY;
		case HAL_TIMEOUT:
			return I2C_HAL_TIMEOUT;
		}
		return I2C_HAL_ERROR;
	}

private:
	volatile char SlaveAddress;						/*!< Slave address with one bit offset	*/
	uint8_t WriteBuffer[WRITE_BUFFER_SIZE];			/*!< Buffer for asynchronous write		*/
	uint8_t ReadBuffer [READ_BUFFER_SIZE];			/*!< Buffer for asynchronous read		*/
	volatile uint8_t BytesToWrite;					/*!< Number of bytes to write			*/
	volatile uint8_t BytesToRead;					/*!< Number of bytes to read			*/
	volatile uint8_t BytesWritten;					/*!< Number of bytes written			*/
	volatile uint8_t BytesRead;						/*!< Number of bytes read				*/
	volatile BUS_STATUS Status;						/*!< Status of transaction				*/
//	volatile TRANSACTION_RESULT Result;				/*!< Result of transaction				*/
	volatile I2C_RESULT Result;

	uint8_t* WriteData;								/*!< Data to write						*/
	uint8_t* ReadData;								/*!< Read data							*/
//	volatile SercomI2cm* I2C;
	I2C_HandleTypeDef* i2c;
};



#endif /* APP_I2C_HPP_ */
