#pragma once
class CAWGN
{
public:
	CAWGN(void);
	~CAWGN(void);

	int m_dim_in_signal;//�����ŵ�ά��
	int m_dim_out_signal;//����ŵ�ά��
	double m_var;//��������
	double m_sigma;//������׼��


	void Malloc();
	void Init(double var);
	void Free();
	void InputOutput(double *xx, double *yy, int channeluse);

};

