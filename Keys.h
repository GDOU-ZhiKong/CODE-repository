#ifndef __KEYS_H_
#define __KEYS_H_

struct keys
{
	
	unsigned char judge_sta;//Switch判断标志位
	unsigned char key_sta;//按键状态标志位
	int single_flag;//确认标志位，确定按键按下后置1
	//以下为长按和双击需要的参数
	unsigned int key_time;//按键按下时间
};

void KEY(void);
#endif