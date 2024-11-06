#ifndef __SOFTIIC_H_
#define __SOFTIIC_H_


#include "gpio.h"
/***********************************************************************/
/*****************************ע������**********************************/
//�����豸������ʹ�����½ӿ���������ļ�
//����

/***********************************************************************/
//*handleΪiic���

//dev_adressΪ�豸7λ��ַ������Ҫ�������ƺͶ�дλ����

//reg_adressΪҪ��ȡ����д��ļĴ�����ַ

//*dataΪҪд������ݻ�洢��ȡ���ݵ�ָ��

//lenΪҪ��ȡ�ļĴ�������

//usΪ��ʱ��ʱ�䣬��λΪ΢�롣

/***********************************************************************/
//typedef struct {
//    void (*write_reg)(void *handle,uint8_t dev_adress,uint8_t reg_adress,uint8_t *data,uint8_t len);
//    uint8_t (*read_reg)(void *handle,uint8_t dev_address,uint8_t reg_address,uint8_t *data,uint8_t len);
//    void (*mdelay)(uint32_t us);
//    void *handle;  // ���ڴ洢�ض���ƽ̨����������Ϣ
//} bsp_�豸��_t;

/***********************************************************************/
//��ʼ���ӿ�ʾ��
//bsp_�豸��_t dev= {
//        .write_reg = WriteRegister,
//        .read_reg = ReadRegister,
//        .mdelay = HAL_Delay_us,
//        .handle = NULL,  // �������Ҫ�ض���ƽ̨����������Ϣ����ΪNULL
//    };
/***********************************************************************/


/**********************************************************
   1.IIC���ģ��   ʹ��HAL��ʱ
   2.��ҪSTM32CubeMX���ó�ʼ�����������ΪGPIOģʽ SDA SCL��ʼ״̬�¶������ ���� ����ģʽ 
   4.��ʼ״̬��SDA �� SCLҪ���ߵ�ƽ ʹ�øߵ͵�ƽת��ʱ֮��Ҫ�����Ե�us����ʱ
**********************************************************/
static GPIO_InitTypeDef GPIO_InitStruct;
/**********************************************************
1.�������� �궨����IF��� 
2.�����ŵ�ƽ����Ҫ�����ģʽ
3.SCLһֱ�������ģʽ(���ʱ�ӿ϶������ģʽ)
4.�궨�������SDA��SCL   SDA PB7    SCL PB6
**********************************************************/
#define SCL_Type     GPIOB
#define SDA_Type     GPIOB
 
#define SCL_GPIO    GPIO_PIN_9
#define SDA_GPIO    GPIO_PIN_8
//��������ߵ͵�ƽģʽ
#define SDA_OUT(X)   if(X) \
					 HAL_GPIO_WritePin(SDA_Type, SDA_GPIO, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(SDA_Type, SDA_GPIO, GPIO_PIN_RESET);
 
#define SCL_OUT(X)   if(X) \
					 HAL_GPIO_WritePin(SCL_Type, SCL_GPIO, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(SCL_Type, SCL_GPIO, GPIO_PIN_RESET);    
                                     
#define SDA_IN		 HAL_GPIO_ReadPin(SDA_Type,SDA_GPIO)//ֻ������ģʽ���ܶ�ȡ��ƽ״̬
						 
void HAL_Delay_us(uint32_t udelay);
void IIC_NAck(void);
int32_t ReadRegister(void *handle,uint8_t device_address,uint8_t register_address,uint8_t *data,uint16_t len);
int32_t WriteRegister(void *handle,uint8_t device_address,uint8_t register_address,const uint8_t *data,uint16_t len);
#endif