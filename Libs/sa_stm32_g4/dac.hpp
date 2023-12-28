#ifndef APP_DAC_HPP_
#define APP_DAC_HPP_

#include "protos_core/base_param.h"

class DacParam : public CtrlParam, public ShortParam
{
public:
    DacParam() = default;
	DacParam(DAC_HandleTypeDef* _hdac, int _channel)
		:hdac(_hdac), channel(_channel)
	{}

    DacParam& operator=(DacParam&& dacParam)  noexcept {
	    channel = dacParam.channel;
	    hdac = dacParam.hdac;
        static_cast<CtrlParam&>(*this) = static_cast<CtrlParam&&>(dacParam);
        static_cast<ShortParam&>(*this) = static_cast<ShortParam&&>(dacParam);
        return *this;
    }

	bool Start()
	{
		return HAL_DAC_Start(hdac, channel);
	}

	bool PerformCtrl() override
	{
		return (HAL_DAC_SetValue(hdac,channel, DAC_ALIGN_12B_R, GetValue()) == HAL_OK);
	}

private:
	DAC_HandleTypeDef* hdac{};
	int channel{};
};
#endif /* APP_DAC_HPP_ */
