//
// Created by outlaw on 08.11.2022.
//

#ifndef TIM_ICMODE_HPP
#define TIM_ICMODE_HPP


class ITim
{
public:
    virtual bool GetValue(float& value, int channel = 0) = 0;
};

class Tim_ICMode : public ITim
{
public:
    Tim_ICMode() = default;
    Tim_ICMode(TIM_HandleTypeDef* _htim,  int _channel)
            :htim(_htim),
             tim_channel(_channel)
    {}

    void Start()
    {
        timerFreq = SystemCoreClock / (htim->Instance->PSC);
        timerARR = __HAL_TIM_GET_AUTORELOAD(htim);
        HAL_TIM_IC_Start_DMA(htim, tim_channel, (uint32_t*)&capture_result, 1);
    }

    void OnCollBack(){
        if(capture_result > MIN_PULSE_WIDTH){
            sum += float(capture_result);
            count++;
        }
    }

    float calcAverage()
    {
        if(!count) return 0;
        float averageValue = sum / float(count);
        sum = 0, count = 0;
        return averageValue;
    }

    bool GetValue(float& value, int channel) override
    {
        auto calcValue = calcAverage();
        if(!calcValue or calcValue > timerFreq){
            value = 0;
            return true;
        }
        value = timerFreq/calcValue;
        return true;
    }
    TIM_HandleTypeDef* getHTim(){
        return htim;
    }

private:
    TIM_HandleTypeDef* htim;
    int tim_channel;
    int MIN_PULSE_WIDTH = 0;
    uint16_t capture_result = 0;
    int count = 0;
    float sum = 0;
    uint32_t timerFreq = 0;
    uint32_t timerARR = 0;
};

template<typename Func>
class Tim_ICmParam : public UpdateParam, public FloatParam, public CalibrParam
{
public:
    Tim_ICmParam() = default;
    Tim_ICmParam(ITim* _tim, Func f)
            :Tim(_tim), saveToEEPROM(std::move(f))
    {}

    bool QueryInterface(INTERFACE_ID iid, void*& p) override
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
        if (Tim->GetValue(value)){
            Value = Calibrate(value);
            return true;
        }
        return false;
    }

private:
    Func saveToEEPROM = nullptr;
    ITim* Tim = nullptr;
};

#endif //TIM_ICMODE_HPP
