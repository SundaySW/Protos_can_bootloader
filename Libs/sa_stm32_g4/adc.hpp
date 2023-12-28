#ifndef APP_ADC_HPP_
#define APP_ADC_HPP_

#include "stm32g4xx_hal.h"
#include "protos_core/base_param.h"

class IAdc
{
public:
	virtual bool GetValue(int channel, float& value) = 0;
};

template<std::size_t CHANNEL_COUNT>
class Adc : public IAdc
{
public:
    Adc() = default;
	explicit Adc(ADC_HandleTypeDef* _hadc)
	    :hadc(_hadc)
	{}

	void Start()
    {
         HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED);
         HAL_ADC_Start_DMA(hadc, (uint32_t*)&adc_result, CHANNEL_COUNT);
    }

    //use in main: void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){adc....OnCallback()}
    void OnCallback()
    {
        for (int i = 0; i < CHANNEL_COUNT; i++)
        {
            Summ[i] += adc_result[i];
            count[i]++;
        }
    }

    bool GetValue(int channel, float& value) override
    {
        if(!count[channel]){
            value = 0;
            return false;
        }
        value = (float)(Summ[channel])/(count[channel]);
        Summ[channel] = 0; count[channel] = 0;
        return true;
    }

    ADC_HandleTypeDef* getHandler(){
        return hadc;
    }

private:
	ADC_HandleTypeDef* hadc{};
	uint16_t adc_result[CHANNEL_COUNT] = {0};
	int Summ[CHANNEL_COUNT] = {0};
	int count[CHANNEL_COUNT] = {0};
};

template<typename Func>
class AdcParam : public UpdateParam, public FloatParam, CalibrParam
{
public:
    AdcParam() = delete;
	AdcParam(IAdc* _adc, int _channel, Func f)
	    :Adc(_adc), channel(_channel), saveToEEPROM(std::move(f))
	{}

    bool QueryInterface(INTERFACE_ID iid, void*& p)
    {
        switch (iid)
        {
            case IID_UPDATABLE:
                p = (UpdateParam*)this;
                return true;
            case IID_CALIBRATEABLE:
                p = (CalibrParam*)this;
                return true;
            default:
                return false;
        }
        return false;
    }

    void SaveToEEPROM() override {
        float data[2] = {Offset, Mult};
        saveToEEPROM(Id, data);
    }

	bool UpdateValue() override
	{
		float value;
		if(Adc->GetValue(channel, value)){
            Value = Calibrate(value);
            return true;
		}
		return false;
	}

private:
    Func saveToEEPROM;
	int channel;
	IAdc* Adc;
};

#endif /* APP_ADC_HPP_ */
