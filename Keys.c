#include "Keys.h"
#include "main.h"

volatile struct keys key[]={0};//按键变量


void KEY(void)
{
//******************************************//
		//获取按键状态
		//******************************************//
		key[0].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1);
		//******************************************//
		//按键状态机
		//******************************************//
		switch (key[0].judge_sta)
		{
			case 0:
			{
				if(key[0].key_sta==0&&key[0].single_flag==0)//确认标志位等均空
				{
					key[0].judge_sta=1;
					key[0].key_time=0;
				}
			}
			break;	
			case 1:
			{
				if(key[0].key_sta==0)//消抖判断
				{
					key[0].judge_sta=2;
				}
				else key[0].judge_sta=0;
			}
			break;			
			case 2:
			{
				if(key[0].key_sta==1)//松开时
				{
					if(key[0].key_time<70)//判断短按
					{
						key[0].single_flag=1;
						key[0].judge_sta=0;
						key[0].key_time=0;
					}
				}
				else 
				{
					key[0].key_time+=1;//未松开就继续计时
					if(key[0].key_time>70)//一定时间判断为长按键
					{
						key[0].single_flag=2;
						key[0].judge_sta=0;
						key[0].key_time=0;
					}
				}
			}
			break;	
		}

}