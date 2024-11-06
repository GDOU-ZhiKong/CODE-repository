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
  SDA引脚转变为 OUT输出模式(输出模式给停止 开始信号) 
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
  SDA引脚转变为 输入模式(输入模式传输具体的数据) 
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
  IIC开始信号
******************************************/
void IIC_Start(void)//IIC开始信号
{
	//设置为输出模式
	IIC_SDA_Mode_OUT();
	
	//空闲状态两个引脚是高电平
	SDA_OUT(1);
	SCL_OUT(1) ;
	HAL_Delay_us(5);
	
	//拉低数据线
	SDA_OUT(0);
	HAL_Delay_us(5);
	
	//再拉低时钟线
	SCL_OUT(0) ;
	HAL_Delay_us(5);
}
//IIC停止信号
void IIC_Stop(void)
{
	//设置为输出模式
	IIC_SDA_Mode_OUT();
 
	//拉低
	SDA_OUT(0);
	SCL_OUT(0);
	HAL_Delay_us(5);
	
	//时钟线先拉高
	SCL_OUT(1);
	HAL_Delay_us(5);
	
	//再把数据线拉高
	SDA_OUT(1);
	HAL_Delay_us(5);
}
void IIC_Send_Byte(uint8_t d)//主机发送8位数据给从机MSB 高位先发
{
    uint8_t i = 0;
    	//设置为输出模式
	IIC_SDA_Mode_OUT();
	
	SDA_OUT(0);
	SCL_OUT(0);
	HAL_Delay_us(5);
	for(i=0;i<8;i++)
	{
		if(d & (0x1<<(7-i)))//表示数据是1
			SDA_OUT(1)
		else
			SDA_OUT(0);
		
		HAL_Delay_us(5);
		SCL_OUT(1);//拉高时钟线，告诉对方你可以读了
		
		HAL_Delay_us(5);
		SCL_OUT(0);//拉低时钟线，告诉对方你暂时别读，我在准备数据
	}
 
}
uint8_t IIC_Wait_Ack(void)//等待从机给主机应答或者不应答
{
	uint8_t ack = 0;
	//设置为输入模式
	IIC_SDA_Mode_IN();
	
	//时钟线拉高,时钟线为高电平期间，不管是数据还是ack都是有效的
	SCL_OUT(1);
	HAL_Delay_us(5);
	
	if( SDA_IN  == 1)
		ack = 1;//无效ACK，就是无效应答
	else
		ack = 0;//有效ACK，就是有效应答
	
	SCL_OUT(0);
    
	HAL_Delay_us(5);
	return ack;
}
void IIC_NAck(void) 
{
    // 将SCL设置为高电平，以开始I2C时钟周期
    SCL_OUT(1);

    // 延时，确保SCL为稳定的高电平
    HAL_Delay_us(5); // 延时5微秒，这个值可能需要根据您的I2C时钟频率调整

    // 在SCL保持高电平的同时，将SDA拉低以发送NACK信号
    SDA_OUT(0);

    // 延时，确保SDA在SCL为高时保持低电平，以完成NACK信号的发送
    HAL_Delay_us(5); // 延时5微秒，这个值可能需要根据您的I2C时钟频率调整

    // 将SCL拉低以结束I2C时钟周期
    SCL_OUT(0);
}
uint8_t  IIC_Read_Byte(void)//从机发送8位数据给主机
{
	uint8_t i =0;
	uint8_t data = 0;
	//设置为输入模式
	IIC_SDA_Mode_IN();
	//先拉低时钟线，准备数据
	SCL_OUT(0);
	HAL_Delay_us(5);
	
	for(i=0;i<8;i++)
	{
		SCL_OUT(1);//时钟线为高电平期间数据才是有效的
		HAL_Delay_us(5);
		if(SDA_IN == 1)
			data |= (0x1<<(7-i));//数据就是1
		else
			data &= ~(0x1<<(7-i));//数据就是0
		
		SCL_OUT (0);//告诉对方此时准备数据，先别读写
		HAL_Delay_us(5);
	}
	return data;
}
void IIC_Ack(uint8_t ack)//主机发送应答或者不应答给从机
{
	//设置为输出模式
	IIC_SDA_Mode_OUT();
	
	SDA_OUT(0);
	SCL_OUT(0);
	HAL_Delay_us(5);
	
	SDA_OUT(ack) ;//发送高/低电平--->发送不应答/应答
	HAL_Delay_us(5);
	
	SCL_OUT(1);//告诉从机我已经准备好数据，你可以读取了
	HAL_Delay_us(5);
	
	SCL_OUT (0);//拉低时钟线，发送ack结束
	HAL_Delay_us(5);
}

// 读取寄存器
int32_t ReadRegister(void *handle,uint8_t device_address,uint8_t register_address,uint8_t *data,uint16_t len)
{
    //uint8_t data = 0;
    IIC_Start();// 发送I2C开始信号
    IIC_Send_Byte(device_address<<1);// 发送设备地址和写操作位
    IIC_Wait_Ack();// 等待从机应答
    IIC_Send_Byte(register_address);// 发送寄存器地址
    IIC_Wait_Ack();// 再次等待从机应答
    
    IIC_Start();// 发送I2C重复开始信号
    IIC_Send_Byte((device_address<<1)|1);// 发送设备地址和读操作位
    IIC_Wait_Ack();// 等待从机应答
		for(int i=0;i<len;i++)
		{
			data[i]= IIC_Read_Byte();// 读取数据  
			if(i<len-1){IIC_Ack(0);}
		}		
    IIC_Ack(1);// 发送I2C非应答信号    
    IIC_Stop();// 发送I2C停止信号    
    return 0;
}

// 写入寄存器
int32_t WriteRegister(void *handle,uint8_t device_address,uint8_t register_address,const uint8_t *data,uint16_t len)
{
    IIC_Start();// 发送I2C开始信号
    IIC_Send_Byte(device_address<<1);// 发送设备地址和写操作位
    IIC_Wait_Ack();// 等待从机应答
    IIC_Send_Byte(register_address);// 发送寄存器地址
    IIC_Wait_Ack();// 再次等待从机应答
for(int i=0;i<len;i++)
		{
			IIC_Send_Byte(data[i]);// 发送数据
			IIC_Wait_Ack();// 等待从机应答
		}
    
    IIC_Stop();// 发送I2C停止信号
		return 0;
}