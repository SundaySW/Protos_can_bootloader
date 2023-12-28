#ifndef APP_EEPROM_24AA02UID_HPP_
#define APP_EEPROM_24AA02UID_HPP_

#include "i2c.hpp"

#define UID_ADDRESS 0x50

class Eeprom24AAUID
{
public:
	Eeprom24AAUID(I2CDriver* _i2c, uint8_t _addr)
		: i2cMaster(_i2c)
		, addr(_addr)
    {}

	bool readUID(uint8_t* r_data)
	{
		uint8_t w_data[2] = {0xFC,0};
		return (i2cMaster->WriteReadSync(UID_ADDRESS, w_data, 1, r_data, 4, 0) == I2C_OK);
	}

//	bool readUIDasync(uint8_t* r_data)
//	{
//		uint8_t w_data[2] = {0xFC,0};
//		i2cMaster->WriteRead(UID_ADDRESS, w_data, 1, r_data, 4);
//	}
//
//	bool Poll(){
//		if (i2cMaster.HasPendingTransaction())
//		{
//			auto result = i2cMaster.GetTransactionResult(true);
//
//			if (result == I2C_PENDING)
//				return;
//
//			OnI2CTransactionComplete(result);
//		};
//
//	}
//
//	void OnI2CTransactionComplete(result)
//	{
//
//	}

	bool writeByte(uint8_t addr, uint8_t data)
	{
		uint8_t w_data[2] = {addr, data};
		return (i2cMaster->WriteSync(UID_ADDRESS, w_data, 2) == I2C_OK);
	}

	bool writeBlock(uint8_t addr, uint8_t* data, uint8_t len)
	{
		if (len > 254)
			return false;
		uint8_t w_data[256];
		w_data[0] = addr;
		for (int i = 0; i < len; i++)
		    w_data[i] = data[i];
		return (i2cMaster->WriteSync(UID_ADDRESS, w_data, len + 1) == I2C_OK);
	}

	bool readByte(uint8_t addr, uint8_t* r_data)
	{
		uint8_t w_data[2] = {addr,0};
		return (i2cMaster->WriteReadSync(UID_ADDRESS, w_data, 1, r_data, 1, 0) == I2C_OK);
	}

	bool readBlock(uint8_t addr, uint8_t* r_data, uint8_t len)
	{
		uint8_t w_data[2] = {addr, 0};
		return (i2cMaster->WriteReadSync(UID_ADDRESS, w_data, 1, r_data, len, 0) == I2C_OK);
	}

private:
	I2CDriver* i2cMaster;
	uint8_t addr = 0;
};


#endif /* APP_EEPROM_24AA02UID_HPP_ */
