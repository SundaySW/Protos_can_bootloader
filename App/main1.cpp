#include "Bootloader.hpp"

void fdcan_init(){
    if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK){Error_Handler();}
    if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK){Error_Handler();}
}

extern "C"
{
    uint8_t Address = 0x0;
    uint8_t Family = 0x1;
    uint8_t HWVer = 0x1;
    uint8_t FWVer = 0x1;
    BootLoader bootLoader = BootLoader(DeviceUID::TYPE_MICROCHIP, Family, Address, &hfdcan1);

    void readEEPROM(){
        //TODO EEPROM init
        //TODO read Addr and etc from EEPROM
        //TODO fill vars with data
    }
    inline uint32_t calcUID(){
        //TODO calc 32bit UID with STM 96 bit UID
        return 0xFFFFFF;
    }

    void AppInit(void) {
//        CpuInit();
        FlashInit();
        fdcan_init();
        bootLoader.init(FWVer, HWVer);
        readEEPROM();
    }

    void OnCANRx(FDCAN_RxHeaderTypeDef header, uint8_t* data)
    {
        bootLoader.OnCanRX(header, data);
    }

    void OnSysTickTimer()
    {
        bootLoader.OnTimerINT(1);
    }

    uint8_t secToJumpToMain = WAIT_TIME_IN_BOOT_SEC;
    void OnTimer_TIM1()
    {
        HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
        if(secToJumpToMain == (WAIT_TIME_IN_BOOT_SEC - 1)) bootLoader.sendHello();
//        if(secToJumpToMain-- <= 0) {
//            HAL_TIM_Base_Stop_IT(&htim1);
//            bootLoader.jumpToMainProg();
//        }
    }

    void OnTimer_TIM3(){
        bootLoader.requestDataPackets();
    }

    void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
    {
        if (htim->Instance == TIM1) {
            OnTimer_TIM1();
        }
        if (htim->Instance == TIM3) {
            OnTimer_TIM3();
        }
    }

    FDCAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];
    void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
    {
        if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
        {
            /* Retrieve Rx messages from RX FIFO0 */
            if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
            {
                Error_Handler();
            }
            OnCANRx(RxHeader, RxData);
        }
    }

    void while1_in_mainCpp(){
        bootLoader.Poll();
    }
}