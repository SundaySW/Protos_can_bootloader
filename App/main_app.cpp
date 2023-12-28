#include "flash_driver/flash_driver.h"
#include "Bootloader.hpp"

extern "C"
{
    BootLoader bootLoader = BootLoader(DeviceUID::TYPE_MICROCHIP, 0x1, 0x0, &hfdcan1);

    void AppInit(void) {
        FlashInit();
        if(HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
            Error_Handler();
        if(HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
            Error_Handler();
        bootLoader.init();
    }

    void OnSysTickTimer()
    {
        bootLoader.OnTimerINT(1);
    }

    void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
    {
        if (htim->Instance == TIM1)
            bootLoader.stayInBootTimHandler();
        if (htim->Instance == TIM3)
            bootLoader.requestDataPackets();
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
                bootLoader.OnCanRX(RxHeader, RxData);
        }
    }

    void AppLoop(){
        bootLoader.Poll();
    }
}