#include <Inc/i2c.h>
#include "Bootloader.hpp"

void fdcan_init(){
    if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK){Error_Handler();}
    if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK){Error_Handler();}
}

extern "C"
{
    BootLoader bootLoader = BootLoader(DeviceUID::TYPE_MICROCHIP, 0x1, 0x0, &hfdcan1);

    inline uint32_t calcUID(){
        //TODO calc 32bit UID with STM 96 bit UID
        return 0xFFFFFF;
    }

    void AppInit(void) {
//        CpuInit();
        FlashInit();
        fdcan_init();
        bootLoader.init(&hi2c2);
        HAL_I2C_DeInit(&hi2c2);
    }

    void OnCANRx(FDCAN_RxHeaderTypeDef header, uint8_t* data)
    {
        bootLoader.OnCanRX(header, data);
    }

    void OnSysTickTimer()
    {
        bootLoader.OnTimerINT(1);
    }

    void OnTimer_TIM1()
    {
        bootLoader.stayInBootTimHandler();
    }

    void OnTimer_TIM3(){
        bootLoader.requestDataPackets();
    }

    void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
    {
        if (htim->Instance == TIM1) OnTimer_TIM1();
        if (htim->Instance == TIM3) OnTimer_TIM3();
    }

    FDCAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];
    void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
    {
        if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
        {
            if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
                Error_Handler();
            else OnCANRx(RxHeader, RxData);
        }
    }

    void while1_in_mainCpp(){
        bootLoader.Poll();
    }
}