#pragma once
#include "flash_driver.h"

static bool FlashWriteBlock(uint32_t addr, uint32_t len, uint8_t *data);
static bool FlashEraseSectors(uint8_t first_sector_idx, uint8_t last_sector_idx);
static bool FlashErasePages(uint32_t addr, uint32_t len = FLASH_ERASE_PAGE_SIZE);
static uint8_t getSectorIdx(uint32_t address);
static uint32_t getPage(uint32_t address);
static uint32_t getBank(uint32_t address);
static bool verifyBankMode();

typedef struct
{
    uint32_t sector_start;
    uint32_t sector_size;   //sector size in bytes
} tFlashSector;

uint8_t vectorAddrBlock[FLASH_WRITE_BLOCK_SIZE];

static const tFlashSector flashLayout[] =
{
  /* { 0x08000000, 0x00800 },              flash sector  0 - reserved for bootloader   */
  /* { 0x08000800, 0x00800 },              flash sector  1 - reserved for bootloader   */
  /* { 0x08001000, 0x00800 },              flash sector  2 - reserved for bootloader   */
  /* { 0x08001800, 0x00800 },              flash sector  3 - reserved for bootloader   */
  /* { 0x08002000, 0x00800 },              flash sector  4 - reserved for bootloader   */
  /* { 0x08002800, 0x00800 },              flash sector  5 - reserved for bootloader   */
  /* { 0x08003000, 0x00800 },              flash sector  6 - reserved for bootloader   */
  /* { 0x08003800, 0x00800 },              flash sector  7 - reserved for bootloader   */
  /* { 0x08004000, 0x00800 },              flash sector  8 - 2kb - reserved for bootloader+I2C   */
  /* { 0x08004800, 0x00800 },              flash sector  9 - 2kb - reserved for bootloader+I2C   */
  { 0x08005000, 0x00800 },              /* flash sector 10 - 2kb                       */
  { 0x08005800, 0x00800 },              /* flash sector 11 - 2kb                       */
  { 0x08006000, 0x00800 },              /* flash sector 12 - 2kb                       */
  { 0x08006800, 0x00800 },              /* flash sector 13 - 2kb                       */
  { 0x08007000, 0x00800 },              /* flash sector 14 - 2kb                       */
  { 0x08007800, 0x00800 },              /* flash sector 15 - 2kb                       */
#if (BOOT_FLASH_SIZE_KB > 32)
  { 0x08008000, 0x04000 },              /* flash sector 16 - 16kb                      */
  { 0x0800C000, 0x04000 },              /* flash sector 17 - 16kb                      */
#endif
#if (BOOT_FLASH_SIZE_KB > 64)
  { 0x08010000, 0x4000 },               /* flash sector 18 - 16kb                      */
  { 0x08014000, 0x4000 },               /* flash sector 19 - 16kb                      */
  { 0x08018000, 0x4000 },               /* flash sector 20 - 16kb                      */
  { 0x0801C000, 0x4000 },               /* flash sector 21 - 16kb                      */
#endif
#if (BOOT_FLASH_SIZE_KB > 128)
  { 0x08020000, 0x4000 },               /* flash sector 22 - 16kb                      */
  { 0x08024000, 0x4000 },               /* flash sector 23 - 16kb                      */
  { 0x08028000, 0x4000 },               /* flash sector 24 - 16kb                      */
  { 0x0802C000, 0x4000 },               /* flash sector 25 - 16kb                      */
  { 0x08030000, 0x4000 },               /* flash sector 26 - 16kb                      */
  { 0x08034000, 0x4000 },               /* flash sector 27 - 16kb                      */
  { 0x08038000, 0x4000 },               /* flash sector 28 - 16kb                      */
  { 0x0803C000, 0x4000 },               /* flash sector 29 - 16kb                      */
#endif
#if (BOOT_FLASH_SIZE_KB > 256)
  { 0x08040000, 0x4000 },               /* flash sector 30 - 16kb                      */
  { 0x08044000, 0x4000 },               /* flash sector 31 - 16kb                      */
  { 0x08048000, 0x4000 },               /* flash sector 32 - 16kb                      */
  { 0x0804C000, 0x4000 },               /* flash sector 33 - 16kb                      */
  { 0x08050000, 0x4000 },               /* flash sector 34 - 16kb                      */
  { 0x08054000, 0x4000 },               /* flash sector 35 - 16kb                      */
  { 0x08058000, 0x4000 },               /* flash sector 36 - 16kb                      */
  { 0x0805C000, 0x4000 },               /* flash sector 37 - 16kb                      */
  { 0x08060000, 0x4000 },               /* flash sector 38 - 16kb                      */
  { 0x08064000, 0x4000 },               /* flash sector 39 - 16kb                      */
  { 0x08068000, 0x4000 },               /* flash sector 40 - 16kb                      */
  { 0x0806C000, 0x4000 },               /* flash sector 41 - 16kb                      */
  { 0x08070000, 0x4000 },               /* flash sector 42 - 16kb                      */
  { 0x08074000, 0x4000 },               /* flash sector 43 - 16kb                      */
  { 0x08078000, 0x4000 },               /* flash sector 44 - 16kb                      */
  { 0x0807C000, 0x4000 },               /* flash sector 45 - 16kb                      */
#endif
#if (BOOT_FLASH_SIZE_KB > 512)
#error "BOOT_FLASH_SIZE_KB > 512 is currently not supported."
#endif
};

void FlashInit()
{
  //be sure that the prefetch is enabled to prevent an occasional flash programming error at a random memory address.
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
}

bool FlashWrite(uint32_t addr, uint32_t len, uint8_t *data)
{
  bool result = true;
  if(!len) return result;
  if ((len - 1) > (FLASH_END_ADDRESS - addr))
      result = false;

  if (result)
  {
    if ((getSectorIdx(addr) == FLASH_INVALID_SECTOR_IDX) || \
        (getSectorIdx(addr + len - 1) == FLASH_INVALID_SECTOR_IDX))
    {
      result = false;
    }
  }

  if (result)
  {
      if (!FlashWriteBlock(addr, len, data))
          result = false;
  }
  return result;
}

bool FlashErase(uint32_t addr, uint32_t len)
{
  bool result = true;
  uint8_t first_sector_idx;
  uint8_t last_sector_idx;

  if ((len - 1) > (FLASH_END_ADDRESS - addr))
      result = false;

  if (result)
  {
    first_sector_idx = getSectorIdx(addr);
    last_sector_idx  = getSectorIdx(addr + len - 1);
    if ((first_sector_idx == FLASH_INVALID_SECTOR_IDX) ||
        (last_sector_idx == FLASH_INVALID_SECTOR_IDX))
    {
      result = false;
    }
  }
  if (result){
//      if(last_sector_idx - first_sector_idx) result = FlashEraseSectors(first_sector_idx, last_sector_idx);
//      else result = FlashErasePages(addr, len);
      result = FlashErasePages(addr, len);
  }
  return result;
}

static bool FlashErasePages(uint32_t addr, uint32_t len)
{
    bool result = true;
    //uint32_t pageTotal = len / FLASH_ERASE_PAGE_SIZE;
    uint32_t pageTotal = len / FLASH_ERASE_PAGE_SIZE + (len % FLASH_ERASE_PAGE_SIZE ? 1 : 0);
    uint32_t pageError = 0;
    FLASH_EraseInitTypeDef eraseInitStruct = { 0 };

    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); //Clear OPTVERR bit set on virgin samples
    CpuWatchDogUpdate();
    eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInitStruct.Banks = getBank(addr);
    eraseInitStruct.Page = getPage(addr);
    eraseInitStruct.NbPages = pageTotal;
    if (HAL_FLASHEx_Erase(&eraseInitStruct, (uint32_t *)&pageError) != HAL_OK)
        result = false;
    HAL_FLASH_Lock();
    return result;
}

bool FlashFinishWriteChecksum()
{
    uint32_t signature_checksum = 0;
    auto *dataPtr = (uint32_t *)vectorAddrBlock;
    for(int i=0; i<7; i++)
        signature_checksum += *(dataPtr++);
    signature_checksum = ~signature_checksum;
    signature_checksum += 1;
    vectorAddrBlock[BOOT_FLASH_VECTOR_TABLE_OFFSET] = signature_checksum & 0xff;
    vectorAddrBlock[BOOT_FLASH_VECTOR_TABLE_OFFSET + 1] = (signature_checksum >> 8) & 0xff;
    vectorAddrBlock[BOOT_FLASH_VECTOR_TABLE_OFFSET + 2] = (signature_checksum >> 16) & 0xff;
    vectorAddrBlock[BOOT_FLASH_VECTOR_TABLE_OFFSET + 3] = (signature_checksum >> 24) & 0xff;
    bool result = FlashWrite(getUserProgBaseAddress(), FLASH_WRITE_FIRST_BLOCK_SIZE, vectorAddrBlock);
    return result;
}

bool FlashVerifyChecksum()
{
    bool result = true;
    uint32_t signature_checksum = 0;
    auto *dataPtr = (uint32_t *)getUserProgBaseAddress();

    for(int i=0; i<7; i++)
        signature_checksum += *(dataPtr++);

    signature_checksum += *((unsigned int *)(flashLayout[0].sector_start + BOOT_FLASH_VECTOR_TABLE_OFFSET));
    if (signature_checksum != 0)
        result = false;

    return result;
}

uint32_t getUserProgBaseAddress()
{
  return flashLayout[0].sector_start;
}

static bool FlashWriteBlock(uint32_t addr, uint32_t len, uint8_t *data)
{
  bool result = true;
  uint32_t p_addr;
  uint64_t p_data;
  uint32_t dword_cnt;

  if (getSectorIdx(addr) == FLASH_INVALID_SECTOR_IDX)
      result = false;

  if (!verifyBankMode())
      result = false;

  if (result)
  {
    HAL_FLASH_Unlock();
    for (dword_cnt = 0; dword_cnt < (len/sizeof(uint64_t)); dword_cnt++)
    {
        uint64_t addrOffset = dword_cnt * sizeof(uint64_t);
        p_addr = addr + addrOffset;
        p_data = *(volatile uint64_t*)(data + addrOffset);
        CpuWatchDogUpdate();
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, p_addr, p_data) != HAL_OK)
        {
            result = false;
            break;
        }
        if (*(volatile uint64_t *)p_addr != p_data)
        {
            result = false;
            break;
        }
    }
    HAL_FLASH_Lock();
  }
  return result;
}

static bool FlashEraseSectors(uint8_t first_sector_idx, uint8_t last_sector_idx)
{
  bool result = true;
  uint8_t sectorIdx;
  uint32_t sectorBaseAddr;
  uint32_t sectorSize;
  uint32_t pageTotal;
  uint32_t pageError = 0;
  FLASH_EraseInitTypeDef eraseInitStruct = { 0 };

  if (first_sector_idx > last_sector_idx)
      result = false;

  if (result)
  {
    if (last_sector_idx > (FLASH_TOTAL_SECTORS-1))
        result = false;
  }

  if (result)
  {
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); //Clear OPTVERR bit set on virgin samples
    for (sectorIdx = first_sector_idx; sectorIdx <= last_sector_idx; sectorIdx++)
    {
      CpuWatchDogUpdate();
      sectorBaseAddr = flashLayout[sectorIdx].sector_start;
      sectorSize = flashLayout[sectorIdx].sector_size;
      if ( (sectorBaseAddr == FLASH_INVALID_ADDRESS) || (sectorSize == 0) ||
           ((sectorSize % FLASH_ERASE_PAGE_SIZE) != 0) )
      {
        result = false;
        break;
      }
      pageTotal = sectorSize / FLASH_ERASE_PAGE_SIZE;
      eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
      eraseInitStruct.Banks = getBank(sectorBaseAddr);
      eraseInitStruct.Page = getPage(sectorBaseAddr);
      eraseInitStruct.NbPages= pageTotal;

      if (HAL_FLASHEx_Erase(&eraseInitStruct, (uint32_t *)&pageError) != HAL_OK)
      {
        result = false;
        break;
      }
    }
    HAL_FLASH_Lock();
  }
  return result;
}

static uint8_t getSectorIdx(uint32_t address)
{
  uint8_t result = FLASH_INVALID_SECTOR_IDX;
  uint8_t sectorIdx;

  for (sectorIdx = 0; sectorIdx < FLASH_TOTAL_SECTORS; sectorIdx++)
  {
    CpuWatchDogUpdate();
    if ((address >= flashLayout[sectorIdx].sector_start) && \
        (address < (flashLayout[sectorIdx].sector_start + \
                    flashLayout[sectorIdx].sector_size)))
    {
      result = sectorIdx;
      break;
    }
  }
  return result;
}

static uint32_t getPage(uint32_t address)
{
    uint32_t result;
    if (getBank(address) == FLASH_BANK_1)
    {
        if(address >= FLASH_BASE){}//TODO check addr assert
        result = (address - FLASH_BASE) / FLASH_ERASE_PAGE_SIZE;
    }
    else
    {
        if(address >= (FLASH_BASE + FLASH_BANK_SIZE)){}//TODO check addr assert
        result = (address - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_ERASE_PAGE_SIZE;
    }
    return result;
}

static uint32_t getBank(uint32_t address)
{
    uint32_t result = FLASH_BANK_1;

    #if defined (FLASH_OPTR_DBANK)
    if (address >= (FLASH_BASE + FLASH_BANK_SIZE))
      result = FLASH_BANK_2;
    #endif

    return result;
}

static bool verifyBankMode()
{
    bool result = true;
    #if defined (FLASH_OPTR_DBANK)
        /* is the flash device configured for single bank mode? */
      if ((FLASH->OPTR & FLASH_OPTR_DBANK) == 0U)
      {
        /* single bank mode is not supported. update the result accordingly. */
        result = false;
      }
    #endif
    return result;
}

void FlashCopyToVectorBlock(uint32_t src)
{
    CpuMemCopy((uint32_t)vectorAddrBlock, (uint32_t)src, FLASH_WRITE_FIRST_BLOCK_SIZE);
}