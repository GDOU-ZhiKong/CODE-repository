#include "Keys.h"
#include "main.h"

volatile struct keys key[]={0};//��������


void KEY(void)
{
//******************************************//
		//��ȡ����״̬
		//******************************************//
		key[0].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1);
		//******************************************//
		//����״̬��
		//******************************************//
		switch (key[0].judge_sta)
		{
			case 0:
			{
				if(key[0].key_sta==0&&key[0].single_flag==0)//ȷ�ϱ�־λ�Ⱦ���
				{
					key[0].judge_sta=1;
					key[0].key_time=0;
				}
			}
			break;	
			case 1:
			{
				if(key[0].key_sta==0)//�����ж�
				{
					key[0].judge_sta=2;
				}
				else key[0].judge_sta=0;
			}
			break;			
			case 2:
			{
				if(key[0].key_sta==1)//�ɿ�ʱ
				{
					if(key[0].key_time<70)//�ж϶̰�
					{
						key[0].single_flag=1;
						key[0].judge_sta=0;
						key[0].key_time=0;
					}
				}
				else 
				{
					key[0].key_time+=1;//δ�ɿ��ͼ�����ʱ
					if(key[0].key_time>70)//һ��ʱ���ж�Ϊ������
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