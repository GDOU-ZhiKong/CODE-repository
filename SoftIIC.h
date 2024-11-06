#ifndef __SOFTIIC_H_
#define __SOFTIIC_H_


#include "gpio.h"
/***********************************************************************/
/*****************************注意事项**********************************/
//建议设备驱动层使用以下接口来适配该文件
//其中

/***********************************************************************/
//*handle为iic句柄

//dev_adress为设备7位地址，不需要进行左移和读写位操作

//reg_adress为要读取或者写入的寄存器地址

//*data为要写入的数据或存储读取数据的指针

//len为要读取的寄存器个数

//us为延时的时间，单位为微秒。

/***********************************************************************/
//typedef struct {
//    void (*write_reg)(void *handle,uint8_t dev_adress,uint8_t reg_adress,uint8_t *data,uint8_t len);
//    uint8_t (*read_reg)(void *handle,uint8_t dev_address,uint8_t reg_address,uint8_t *data,uint8_t len);
//    void (*mdelay)(uint32_t us);
//    void *handle;  // 用于存储特定于平台的上下文信息
//} bsp_设备名_t;

/***********************************************************************/
//初始化接口示例
//bsp_设备名_t dev= {
//        .write_reg = WriteRegister,
//        .read_reg = ReadRegister,
//        .mdelay = HAL_Delay_us,
//        .handle = NULL,  // 如果不需要特定于平台的上下文信息，则为NULL
//    };
/***********************************************************************/


/**********************************************************
   1.IIC软件模拟   使用HAL库时
   2.需要STM32CubeMX配置初始化的相关引脚为GPIO模式 SDA SCL初始状态下都是输出 推挽 上拉模式 
   4.初始状态下SDA 与 SCL要给高电平 使用高低电平转换时之间要有明显的us级延时
**********************************************************/
static GPIO_InitTypeDef GPIO_InitStruct;
/**********************************************************
1.引脚配置 宏定义用IF语句 
2.给引脚电平必须要给输出模式
3.SCL一直都是输出模式(输出时钟肯定是输出模式)
4.宏定义绑定引脚SDA与SCL   SDA PB7    SCL PB6
**********************************************************/
#define SCL_Type     GPIOB
#define SDA_Type     GPIOB
 
#define SCL_GPIO    GPIO_PIN_9
#define SDA_GPIO    GPIO_PIN_8
//设置输出高低电平模式
#define SDA_OUT(X)   if(X) \
					 HAL_GPIO_WritePin(SDA_Type, SDA_GPIO, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(SDA_Type, SDA_GPIO, GPIO_PIN_RESET);
 
#define SCL_OUT(X)   if(X) \
					 HAL_GPIO_WritePin(SCL_Type, SCL_GPIO, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(SCL_Type, SCL_GPIO, GPIO_PIN_RESET);    
                                     
#define SDA_IN		 HAL_GPIO_ReadPin(SDA_Type,SDA_GPIO)//只有输入模式才能读取电平状态
						 
void HAL_Delay_us(uint32_t udelay);
void IIC_NAck(void);
int32_t ReadRegister(void *handle,uint8_t device_address,uint8_t register_address,uint8_t *data,uint16_t len);
int32_t WriteRegister(void *handle,uint8_t device_address,uint8_t register_address,const uint8_t *data,uint16_t len);
#endif