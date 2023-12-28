#include "eeprom.hpp"

extern uint32_t EEPROM_START_ADDR;

bool eeprom_write_block(unsigned int offset, const char* buffer, unsigned int len)
{
	if (len > 2048)
		return false;

	//find page by offset
    uint32_t pageNum = (EEPROM_START_ADDR + offset - 0x08000000) / 2048;
	unsigned int offsetOnPage = offset % 2048;

	//read page to buffer
	uint32_t PageAddr = pageNum * 2048 + 0x08000000;
	uint64_t page_buffer[256];

	for (int i = 0; i < 256; i++)
	{
		page_buffer[i] = *(volatile uint64_t *)(PageAddr + i * 8);
	}

	//modify buffer
	auto* pBuf = (uint8_t*)page_buffer;

	for (int i = 0; i < len; i++)
	{
		pBuf[offsetOnPage + i] = buffer[i];
	}

	//erase page
	static FLASH_EraseInitTypeDef EraseInitStruct;

	HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks = FLASH_BANK_1; // ????
	EraseInitStruct.NbPages = 1;
	EraseInitStruct.Page = pageNum;

	uint32_t PAGEError;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
        return false;

	//write page
	int flashWriteCounter = 0;

	for (int i = 0; i < 256; i++)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, PageAddr + i * 8, page_buffer[i]) == HAL_OK)
			flashWriteCounter++;
	}

//	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);
//
//	HAL_StatusTypeDef ret =  HAL_FLASH_Program(FLASH_TYPEPROGRAM_FAST_AND_LAST, PageAddr, (uint64_t)(uintptr_t)page_buffer);
//	if (ret == HAL_OK)
//	{
//		HAL_FLASH_Lock();
//		return true;
//	}

	HAL_FLASH_Lock();

	if (flashWriteCounter == 256)
		return true;

	return false;
}

bool eeprom_read_block(unsigned int offset, char* buffer, unsigned int len)
{
	if (len > 2048)
		return false;

	uint32_t startAddr = EEPROM_START_ADDR + offset;

	for (int i = 0; i < len; i++)
	{
		buffer[i] = *(volatile uint8_t *)(startAddr + i);
	}
    return true;
}

bool eeprom_write_byte(unsigned int offset, char id)
{
	char buf[1] = {id};
	return eeprom_write_block(offset, buf, 1);
}