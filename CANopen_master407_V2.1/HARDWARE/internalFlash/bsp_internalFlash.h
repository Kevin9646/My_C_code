#ifndef __INTERNAL_FLASH_H
#define	__INTERNAL_FLASH_H

#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* 要擦除内部FLASH的起始地址 */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_3   
/* 要擦除内部FLASH的结束地址 */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_4


/* Base address of the Flash sectors */ 
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base address of Sector 1, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base address of Sector 2, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base address of Sector 3, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base address of Sector 4, 64 Kbytes   */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base address of Sector 5, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base address of Sector 6, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base address of Sector 7, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base address of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base address of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base address of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base address of Sector 11, 128 Kbytes */

#define ADDR_FLASH_SECTOR_12     ((uint32_t)0x08100000) /* Base address of Sector 12, 16 Kbytes  */

#define ADDR_CodeWriteFlag      ((u32)0x0807FFF0)	//IAP程序更新标志位存放地址，如果是0x55,说明已经有应用程序; 若是0x00，说明需要更新程序;

#define ADDR_ConfigData          ((u32)0x0800C000)  //从改地址开始存储持久化数据

#define IAP_ADDRESS   (uint32_t)0x08000000 

/* Private variables ---------------------------------------------------------*/
typedef  void (*pFunction)(void);

int InternalFlash_Test(void);
int EraseSector_Flash(void);
int WriteConfig_Flash(uint32_t data, uint32_t Start_address, uint32_t End_address);
u16 ReadConfig_Flash_16Bit(uint32_t Start_address, uint32_t End_address);
int WriteConfig_Flash_16Bit(u16 data, uint32_t Start_address, uint32_t End_address);
u8 ReadConfig_Flash_8Bit(uint32_t Start_address, uint32_t End_address);
void Robot_ConfigInit_FromFlash(void);

void Init_IAP_Flash_Flag(void);
void Jump_To_IAP_Program(void);

#endif /* __INTERNAL_FLASH_H */

