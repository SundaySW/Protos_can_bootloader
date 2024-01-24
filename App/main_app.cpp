#include "flash_driver/flash_driver.h"
#include "Bootloader.hpp"

extern "C"
{
    void AppInit(void) {
        FlashInit();
        if(HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
            Error_Handler();
        if(HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
            Error_Handler();
        BootLoader::getRef().init();
    }

    void OnSysTickTimer()
    {
        BootLoader::getRef().OnTimerINT(1);
    }

    void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
    {
        if (htim->Instance == TIM1)
            BootLoader::getRef().stayInBootTimHandler();
        if (htim->Instance == TIM3)
            BootLoader::getRef().requestDataPackets();
    }

    FDCAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];
    void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
    {
        if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
        {
            if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
                Error_Handler();
            else
                BootLoader::getRef().OnCanRX(RxHeader, RxData);
        }
    }

    void AppLoop(){
        BootLoader::getRef().Poll();
    }
}