#ifndef __KEYS_H_
#define __KEYS_H_

struct keys
{
	
	unsigned char judge_sta;//Switch�жϱ�־λ
	unsigned char key_sta;//����״̬��־λ
	int single_flag;//ȷ�ϱ�־λ��ȷ���������º���1
	//����Ϊ������˫����Ҫ�Ĳ���
	unsigned int key_time;//��������ʱ��
};

void KEY(void);
#endif