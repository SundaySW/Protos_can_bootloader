
#ifndef FLASH_DRIVER_H
#define FLASH_DRIVER_H

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_flash.h"
#include <cstdint>

#define BOOT_FLASH_SIZE_KB                  (128)
#define FLASH_INVALID_SECTOR_IDX            (0xff)
#define FLASH_INVALID_ADDRESS               (0xffffffff)
#define FLASH_WRITE_BLOCK_SIZE              (4096)
#define FLASH_WRITE_FIRST_BLOCK_SIZE        (512)
#define FLASH_ERASE_PAGE_SIZE               (FLASH_PAGE_SIZE)
#define FLASH_TOTAL_SECTORS                 (sizeof(flashLayout)/sizeof(flashLayout[0]))
#define FLASH_END_ADDRESS                   (flashLayout[FLASH_TOTAL_SECTORS-1].sector_start + \
                                                flashLayout[FLASH_TOTAL_SECTORS-1].sector_size - 1)

/** \brief calc in startup file ( g_pfnVectors ) also need to add space in table in startup file(	.word	0).*/
#define BOOT_FLASH_VECTOR_TABLE_OFFSET      (0x1D8)
#if (BOOT_FLASH_VECTOR_TABLE_CS_OFFSET >= FLASH_WRITE_FIRST_BLOCK_SIZE)
#error "BOOT_FLASH_VECTOR_TABLE_CS_OFFSET is set too high. It must be located in the first writable block."
#endif

/** \brief Pointer to the user program's reset vector. */
#define CPU_USER_PROGRAM_STARTADDR_PTR      ((uint32_t)(getUserProgBaseAddress() + 0x00000004))
/** \brief Pointer to the user program's vector table. */
#define CPU_USER_PROGRAM_VECTABLE_OFFSET    ((uint32_t)getUserProgBaseAddress())

void CpuInit();
void CpuStartUserProgram();
void CpuMemCopy(uint32_t dest, uint32_t src, uint16_t len);
void CpuMemSet(uint32_t dest, uint8_t value, uint16_t len);
void CpuIrqDisable();
void CpuIrqEnable();
void CpuWatchogConf();
void CpuWatchDogUpdate();

void FlashInit();
bool FlashWrite(uint32_t addr, uint32_t len, uint8_t *data);
bool FlashErase(uint32_t addr, uint32_t len = FLASH_ERASE_PAGE_SIZE);
bool FlashFinishWriteChecksum();
bool FlashVerifyChecksum();
uint32_t getUserProgBaseAddress();
void FlashCopyToVectorBlock(uint32_t src);
void FlashCopyToVectorBlockStd(uint32_t src_addr);

#endif /* FLASH_DRIVER_H */