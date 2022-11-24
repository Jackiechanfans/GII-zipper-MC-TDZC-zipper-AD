#pragma once
class CBPSKModem
{
public:
	CBPSKModem();
	~CBPSKModem();

	int m_num_signal;//��������
	int m_dim_signal;//����ά��

	void Malloc();//��ʼ����������ȡ�ļ�����
	void Free();//�ͷſռ�
	void Modulation(int *uu, double *xx, int len_uu);//����
	void Hard_DeModulation(double *yy, int *uu_hat, int len_yy);//�����Ӳ�о�
	void Soft_DeModulation(double *yy, double var, double *p0_cc, int len_yy);//��������о�

private:
	double *m_constellation;//����
};
