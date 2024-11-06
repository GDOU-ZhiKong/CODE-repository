#include "SoftIIC.h"

void HAL_Delay_us(uint32_t udelay)
{
  uint32_t startval,tickn,delays,wait;
 
  startval = SysTick->VAL;
  tickn = HAL_GetTick();
  //sysc = 72000;  //SystemCoreClock / (1000U / uwTickFreq);
  delays =udelay * 64; //sysc / 1000 * udelay;
  if(delays > startval)
    {
      while(HAL_GetTick() == tickn)
        {
 
        }
      wait = 64000 + startval - delays;
      while(wait < SysTick->VAL)
        {
 
        }
    }
  else
    {
      wait = startval - delays;
      while(wait < SysTick->VAL && HAL_GetTick() == tickn)
        {
 
        }
    }
}
 
/*****************************************
  SDA����ת��Ϊ OUT���ģʽ(���ģʽ��ֹͣ ��ʼ�ź�) 
******************************************/
void IIC_SDA_Mode_OUT(void)
{
  GPIO_InitStruct.Pin = SDA_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SDA_Type, &GPIO_InitStruct);
}
/*****************************************
  SDA����ת��Ϊ ����ģʽ(����ģʽ������������) 
******************************************/
void IIC_SDA_Mode_IN(void)
{
  GPIO_InitStruct.Pin = SDA_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SDA_Type, &GPIO_InitStruct);
}
/*****************************************
  IIC��ʼ�ź�
******************************************/
void IIC_Start(void)//IIC��ʼ�ź�
{
	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT();
	
	//����״̬���������Ǹߵ�ƽ
	SDA_OUT(1);
	SCL_OUT(1) ;
	HAL_Delay_us(5);
	
	//����������
	SDA_OUT(0);
	HAL_Delay_us(5);
	
	//������ʱ����
	SCL_OUT(0) ;
	HAL_Delay_us(5);
}
//IICֹͣ�ź�
void IIC_Stop(void)
{
	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT();
 
	//����
	SDA_OUT(0);
	SCL_OUT(0);
	HAL_Delay_us(5);
	
	//ʱ����������
	SCL_OUT(1);
	HAL_Delay_us(5);
	
	//�ٰ�����������
	SDA_OUT(1);
	HAL_Delay_us(5);
}
void IIC_Send_Byte(uint8_t d)//��������8λ���ݸ��ӻ�MSB ��λ�ȷ�
{
    uint8_t i = 0;
    	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT();
	
	SDA_OUT(0);
	SCL_OUT(0);
	HAL_Delay_us(5);
	for(i=0;i<8;i++)
	{
		if(d & (0x1<<(7-i)))//��ʾ������1
			SDA_OUT(1)
		else
			SDA_OUT(0);
		
		HAL_Delay_us(5);
		SCL_OUT(1);//����ʱ���ߣ����߶Է�����Զ���
		
		HAL_Delay_us(5);
		SCL_OUT(0);//����ʱ���ߣ����߶Է�����ʱ���������׼������
	}
 
}
uint8_t IIC_Wait_Ack(void)//�ȴ��ӻ�������Ӧ����߲�Ӧ��
{
	uint8_t ack = 0;
	//����Ϊ����ģʽ
	IIC_SDA_Mode_IN();
	
	//ʱ��������,ʱ����Ϊ�ߵ�ƽ�ڼ䣬���������ݻ���ack������Ч��
	SCL_OUT(1);
	HAL_Delay_us(5);
	
	if( SDA_IN  == 1)
		ack = 1;//��ЧACK��������ЧӦ��
	else
		ack = 0;//��ЧACK��������ЧӦ��
	
	SCL_OUT(0);
    
	HAL_Delay_us(5);
	return ack;
}
void IIC_NAck(void) 
{
    // ��SCL����Ϊ�ߵ�ƽ���Կ�ʼI2Cʱ������
    SCL_OUT(1);

    // ��ʱ��ȷ��SCLΪ�ȶ��ĸߵ�ƽ
    HAL_Delay_us(5); // ��ʱ5΢�룬���ֵ������Ҫ��������I2Cʱ��Ƶ�ʵ���

    // ��SCL���ָߵ�ƽ��ͬʱ����SDA�����Է���NACK�ź�
    SDA_OUT(0);

    // ��ʱ��ȷ��SDA��SCLΪ��ʱ���ֵ͵�ƽ�������NACK�źŵķ���
    HAL_Delay_us(5); // ��ʱ5΢�룬���ֵ������Ҫ��������I2Cʱ��Ƶ�ʵ���

    // ��SCL�����Խ���I2Cʱ������
    SCL_OUT(0);
}
uint8_t  IIC_Read_Byte(void)//�ӻ�����8λ���ݸ�����
{
	uint8_t i =0;
	uint8_t data = 0;
	//����Ϊ����ģʽ
	IIC_SDA_Mode_IN();
	//������ʱ���ߣ�׼������
	SCL_OUT(0);
	HAL_Delay_us(5);
	
	for(i=0;i<8;i++)
	{
		SCL_OUT(1);//ʱ����Ϊ�ߵ�ƽ�ڼ����ݲ�����Ч��
		HAL_Delay_us(5);
		if(SDA_IN == 1)
			data |= (0x1<<(7-i));//���ݾ���1
		else
			data &= ~(0x1<<(7-i));//���ݾ���0
		
		SCL_OUT (0);//���߶Է���ʱ׼�����ݣ��ȱ��д
		HAL_Delay_us(5);
	}
	return data;
}
void IIC_Ack(uint8_t ack)//��������Ӧ����߲�Ӧ����ӻ�
{
	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT();
	
	SDA_OUT(0);
	SCL_OUT(0);
	HAL_Delay_us(5);
	
	SDA_OUT(ack) ;//���͸�/�͵�ƽ--->���Ͳ�Ӧ��/Ӧ��
	HAL_Delay_us(5);
	
	SCL_OUT(1);//���ߴӻ����Ѿ�׼�������ݣ�����Զ�ȡ��
	HAL_Delay_us(5);
	
	SCL_OUT (0);//����ʱ���ߣ�����ack����
	HAL_Delay_us(5);
}

// ��ȡ�Ĵ���
int32_t ReadRegister(void *handle,uint8_t device_address,uint8_t register_address,uint8_t *data,uint16_t len)
{
    //uint8_t data = 0;
    IIC_Start();// ����I2C��ʼ�ź�
    IIC_Send_Byte(device_address<<1);// �����豸��ַ��д����λ
    IIC_Wait_Ack();// �ȴ��ӻ�Ӧ��
    IIC_Send_Byte(register_address);// ���ͼĴ�����ַ
    IIC_Wait_Ack();// �ٴεȴ��ӻ�Ӧ��
    
    IIC_Start();// ����I2C�ظ���ʼ�ź�
    IIC_Send_Byte((device_address<<1)|1);// �����豸��ַ�Ͷ�����λ
    IIC_Wait_Ack();// �ȴ��ӻ�Ӧ��
		for(int i=0;i<len;i++)
		{
			data[i]= IIC_Read_Byte();// ��ȡ����  
			if(i<len-1){IIC_Ack(0);}
		}		
    IIC_Ack(1);// ����I2C��Ӧ���ź�    
    IIC_Stop();// ����I2Cֹͣ�ź�    
    return 0;
}

// д��Ĵ���
int32_t WriteRegister(void *handle,uint8_t device_address,uint8_t register_address,const uint8_t *data,uint16_t len)
{
    IIC_Start();// ����I2C��ʼ�ź�
    IIC_Send_Byte(device_address<<1);// �����豸��ַ��д����λ
    IIC_Wait_Ack();// �ȴ��ӻ�Ӧ��
    IIC_Send_Byte(register_address);// ���ͼĴ�����ַ
    IIC_Wait_Ack();// �ٴεȴ��ӻ�Ӧ��
for(int i=0;i<len;i++)
		{
			IIC_Send_Byte(data[i]);// ��������
			IIC_Wait_Ack();// �ȴ��ӻ�Ӧ��
		}
    
    IIC_Stop();// ����I2Cֹͣ�ź�
		return 0;
}