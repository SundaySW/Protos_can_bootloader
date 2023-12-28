#ifndef APP_SHT31_HPP_
#define APP_SHT31_HPP_

#include "i2c.hpp"

#define SHT31_ADDRESS_0 0x44
#define SHT31_ADDRESS_1 0x45


class Sht31
{
public:
	Sht31(I2CDriver* _i2c, uint8_t _addr)
		: i2cMaster(_i2c)
		, addr(_addr)
	{	}

	bool SingleShotMeasure()
	{
		uint8_t w_data[2] = {0x24, 0x16};
		uint8_t r_data[6] = {0};

		i2cMaster->WriteReadSync(addr, w_data, 2, r_data, 6, 2);

		float T = ((r_data[0] << 8) + r_data[1])/374.4857 - 45;
		float Hu = ((r_data[3] << 8) + r_data[4])/655.35;

		if (Crc8(r_data, 2) != r_data[2])
			return false;

		Summ_T += T;
		counter_T++;

		if (Crc8(r_data + 3, 2) != r_data[5])
			return false;

		Summ_Hu += Hu;
		counter_Hu++;

		return true;
	}

	void OnTimer(int ms)
	{
		if (shtRequestCounter)
			shtRequestCounter--;
	}

	void OnPoll()
	{
		if (!shtRequestCounter)
		{
			shtRequestCounter = 10;

			SingleShotMeasure();
		}
	}

	bool GetAvgT(float* res)
	{
		if (!counter_T)
			return false;
		*res = Summ_T / counter_T;
		Summ_T = 0;
		counter_T = 0;
		return true;
	}

	bool GetAvgHu(float* res)
	{
		if (!counter_Hu)
			return false;
		*res = Summ_Hu / counter_Hu;
		Summ_Hu = 0;
		counter_Hu = 0;
		return true;
	}

    inline uint8_t Crc8(uint8_t *pcBlock, uint8_t len)
    {
        uint8_t crc = 0xFF;
        uint8_t i;
        while (len--)
        {
            crc ^= *pcBlock++;
            for (i = 0; i < 8; i++)
                crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
        }
        return crc;
    }

	float Summ_T = 0, Summ_Hu = 0;
	int counter_T = 0, counter_Hu = 0;

private:
	I2CDriver* i2cMaster;
	uint8_t addr = 0;
	uint16_t shtRequestCounter = 100;


};

class Sht_T_Param : public UpdateParam, public FloatParam
{
public:
	Sht_T_Param(Sht31* _sht31)
	{
		sht31 = _sht31;
	}

	bool UpdateValue ()
	{

		if (sht31->GetAvgT(&Value))
		{
			Value = (Value + calibOffset) * calibMult;
			return true;
		}

		return false;
	}
	float calibOffset = 0, calibMult = 1;
private:
	Sht31* sht31;


};

class Sht_Hu_Param : public UpdateParam, public FloatParam
{
public:
	Sht_Hu_Param(Sht31* _sht31)
	{
		sht31 = _sht31;
	}

	bool UpdateValue ()
	{

		if (sht31->GetAvgHu(&Value))
		{
			Value = (Value + calibOffset) * calibMult;
			return true;
		}

		return false;
	}

	float calibOffset = 0, calibMult = 1;
private:
	Sht31* sht31;


};


#endif /* APP_SHT31_HPP_ */
