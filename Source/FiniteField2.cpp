/**********************************************************************************

 FiniteField2.cpp: implementation of the CFiniteField2 class.

**********************************************************************************/

#include "stdafx.h"
#include "FiniteField2.h"

/*******************************************************************************
Programmed by Xiao Ma, Sun Yat-sen University. 
If you have any suggestions, please contact me at maxiao@mail.sysu.edu.cn
The program can only be employed for academic research.
******************************************************************************/



/******************************************************************************
��ά���� ��PrimitivePolynomial[17][6]
���ܣ���ԭ����ʽ
˵����{5, 0, 2, 3, 4, 8} meaning: 5 nonzero terms--x^0 + x^2 + x^3 + x^4 + x^8
*****************************************************************************/

const int CFiniteField2::PrimitivePolynomial[17][6] = 
{
	{1, 0},
	{2, 0, 1},
	{3, 0, 1, 2},
	{3, 0, 1, 3},
	{3, 0, 1, 4},
	{3, 0, 2, 5},
	{3, 0, 1, 6},
	{3, 0, 3, 7},
	{5, 0, 2, 3, 4, 8},
	{3, 0, 4, 9},
	{3, 0, 3, 10},
	{3, 0, 2, 11},
	{5, 0, 1, 4, 6, 12},
	{5, 0, 1, 3, 4, 13},
	{5, 0, 1, 6, 10, 14},
	{3, 0, 1, 15},
	{5, 0, 1, 3, 12, 16}
};






/********************************************************************
 Construction/Destruction
*********************************************************************/

CFiniteField2::CFiniteField2()
{

}

CFiniteField2::~CFiniteField2()
{

}



/********************************************************************
���� �� Malloc(int m)
���� :  ��������
���������m ---- GF(2^m)
*********************************************************************/

void CFiniteField2::Malloc(int m)
{
	int i, j;
	int m_gx;

	m_len_element = m;
	m_gen_poly = new int[m_len_element+1];
	for (i = 0; i <= m_len_element; i++)
		m_gen_poly[i] = 0;

	for (i = 1; i <= PrimitivePolynomial[m][0]; i++)
		m_gen_poly[m-PrimitivePolynomial[m][i]] = 1;

	m_num_element = 1 << m_len_element;
	Exp = new int[m_num_element];
	Log = new int[m_num_element];

//the field elements are represented by integers 0, 1, 2, ..., m_num_element-1
//which correspond to the bianry vectors of length m_len_element
//the MSB is the first component (indexed by 0) of the binary vector
	m_gx = 0;
	for (i = 1; i <= m_len_element; i++)
		m_gx = 2*m_gx + m_gen_poly[i];

	Exp[0] = 1;
	Log[1] = 0;
	for (i = 1; i < m_num_element-1; i++){
		Exp[i] = 2*Exp[i-1];
		if (Exp[i] >= m_num_element){
			Exp[i] = Exp[i] - m_num_element;
			Exp[i] = Exp[i] ^ m_gx;
		}
		Log[Exp[i]] = i;
	}

	Vec = new int*[m_num_element];
	for (i = 0; i < m_num_element; i++)
		Vec[i] = new int[m_len_element];

//	cout << endl;
	for (i = 0; i < m_num_element; i++)
	{ 
		for (j = 0; j < m_len_element; j++)
		{ 
			Vec[i][j] = (i >> (m_len_element-1-j)) % 2;
//			cout << " " << Vec[i][j];
		}
//		cout << endl;
	}



//	fprintf(stdout, "\nExp:\n");
//	for (i = 0; i <= m_num_element-1; i++)
//		fprintf(stdout, "%d ", Exp[i]);
//	fprintf(stdout, "\n");
	
//	fprintf(stdout, "\nLog:\n");
//	for (i = 0; i <= m_num_element-1; i++)
//		fprintf(stdout, "\n%d", Log[i]);
//	fprintf(stdout, "\n");

	return;
}

/********************************************************************
���� �� Free()
���� :  �ͷſռ�
*********************************************************************/

void CFiniteField2::Free()
{
	int i;

	delete []m_gen_poly;
	delete []Exp;
	delete []Log;

	for (i = 0; i < m_num_element; i++)
		delete []Vec[i];

	delete []Vec;

	return;
}

/********************************************************************
���� �� Add(int alpha, int beta)
���� :  ��������ļӷ� alpha+beta
*********************************************************************/

int CFiniteField2::Add(int alpha, int beta)
{
	return alpha ^ beta;
}


/********************************************************************
���� ��Mult(int alpha, int beta)
���� :  ��������ĳ˷� alpha*beta
*********************************************************************/

int CFiniteField2::Mult(int alpha, int beta)
{
	int temp;

	if (alpha == 0 || beta == 0)
		return 0;
	else {
		temp = Log[alpha] + Log[beta];
		if (temp < m_num_element-1)
			return Exp[temp];
		else
			return Exp[temp - m_num_element+1];
	}
	
}


/********************************************************************
���� �� Div(int alpha, int beta)
���� :  ��������ĳ��� alpha / beta
*********************************************************************/

int CFiniteField2::Div(int alpha, int beta)
{
	int temp;

	if (beta == 0){
		fprintf(stderr, "\n The divisor can not be zero!");
		exit(3);
	}

	if (alpha == 0)
		return 0;
	else {
		temp = Log[alpha] - Log[beta];
		if (temp >= 0)
			return Exp[temp];
		else
			return Exp[temp + m_num_element-1];
	}
}

/********************************************************************
���� �� Pow(int alpha, int exp)
���� :  ��������ĳ˷� alpha^exp
*********************************************************************/

int CFiniteField2::Pow(int alpha, int exp)
{
	if (alpha == 0)
		return 0;
	else 
		return Exp[exp * Log[alpha] % (m_num_element - 1)];
}


/********************************************************************
���� �� VecAdd(int *alpha, int *beta, int *gamma, int len)
���� :  �������������ӷ�
���������*alpha ---- ����1
          *beta  ---- ����2
		  len    ---- ��������
���������*gamma ---- ������ӽ��
*********************************************************************/

void CFiniteField2::VecAdd(int *alpha, int *beta, int *gamma, int len)
{
	int i;

    for (i = 0; i < len; i++)
		gamma[i] = Add(alpha[i], beta[i]);

	return;
}


/********************************************************************
���� �� Mult(int *alpha, int *beta, int *gamma, int len)
���� :  �������������˷�
���������*alpha ---- ����1
          *beta  ---- ����2
		  len    ---- ��������
���������*gamma ---- ������˽��
*********************************************************************/

void CFiniteField2::VecMult(int *alpha, int *beta, int *gamma, int len)
{
	int i;

    for (i = 0; i < len; i++)
		gamma[i] = Mult(alpha[i], beta[i]);

	return;
}


/********************************************************************
���� �� ScalarMult(int alpha, int *beta, int *gamma, int len)
���� :  �������������������
���������alpha ---- ����
          *beta  ---- ����
		  len    ---- ��������
���������*gamma ---- ��˽��
*********************************************************************/

void CFiniteField2::ScalarMult(int alpha, int *beta, int *gamma, int len)
{
	int i;
	
	for (i = 0; i < len; i++)
		gamma[i] = Mult(alpha, beta[i]);

	return;
}


/********************************************************************
���� �� PolyEvaluate(int x, int *poly, int deg)
���� :  �����������ʽ��ֵ
���������x      ----  �Ա���
          *poly  ----  ����ʽϵ��
		  deg    ----  ����ʽ���ϵ��
����ֵ������ʽ��ֵ���
*********************************************************************/

int CFiniteField2::PolyEvaluate(int x, int *poly, int deg)
{
	int i;
	int y;

	y = 0;
	for (i = 0; i <= deg; i++)
		y = Add(y, Mult(poly[i], Pow(x, i)));
	
	return y;
}


/********************************************************************
���� �� PolyMult(int *A, int deg_A, int *B, int deg_B, int *C)
���� :  �����������ʽ���
���������*A     ----  ����ʽA
          deg_A  ----  ����ʽ��ߴ���
		  *B     ----  ����ʽB
		  deg_B  ----  ����ʽ��ߴ���
���������*C     ----  ����ʽ��˽��
����ֵ��         ----  ����ʽ��˽����ߴ���
*********************************************************************/

int CFiniteField2::PolyMult(int *A, int deg_A, int *B, int deg_B, int *C)
{
	int i, j;
	int deg_C;
	
	if ((deg_A == 0 && A[deg_A] == 0) || (deg_B == 0 && B[deg_B] == 0)){
		deg_C = 0;
		C[0] = 0;
		return deg_C;
	}
	
	if (A[deg_A] == 0 || B[deg_B] == 0)
		fprintf(stderr, "\nThe highest coefficient of poly is zero!");

	deg_C = deg_A + deg_B;

	for (i = 0; i <= deg_C; i++)
		C[i] = 0;
	for (i = 0; i <= deg_A; i++)
		for (j = 0; j <= deg_B; j++)
			C[i+j] = Add(C[i+j], Mult(A[i], B[j]));


	return deg_C;
}


/********************************************************************
���� �� PolyAdd(int *A, int deg_A, int *B, int deg_B, int *C)
���� :  �����������ʽ���
���������*A     ----  ����ʽA
          deg_A  ----  ����ʽ��ߴ���
		  *B     ----  ����ʽB
		  deg_B  ----  ����ʽ��ߴ���
���������*C     ----  ����ʽ��ӽ��
����ֵ��         ----  ����ʽ��ӽ����ߴ���
*********************************************************************/

int CFiniteField2::PolyAdd(int *A, int deg_A, int *B, int deg_B, int *C)
{
	int i, j;
	int deg_C;
	
	deg_C = deg_A;
	if (deg_B > deg_A)
		deg_C = deg_B;

	for (i = 0; i <= deg_C; i++)
		C[i] = 0;
	for (i = 0; i <= deg_A; i++)
		C[i] = Add(C[i], A[i]);
	for (j = 0; j <= deg_B; j++)
		C[j] = Add(C[j], B[j]);

	while (deg_C >0 && C[deg_C] == 0)
		deg_C--;

	return deg_C;
}


/********************************************************************
���� �� PolyDiv(int *A, int deg_A, int *B, int deg_B, int *C, int *D)
���� :  �����������ʽ���
���������*A     ----  ����ʽA
		  deg_A  ----  ����ʽ��ߴ���
		  *B     ----  ����ʽB
		  deg_B  ----  ����ʽ��ߴ���
���������*C     ----  ����ʽ�����ʽ
		  *D	 ----  ����ʽ�����ʽ
����ֵ��         ----  ����ʽ��������ߴ���
*********************************************************************/

int CFiniteField2::PolyDiv(int *A, int deg_A, int *B, int deg_B, int *C, int *D)
{
	int i;
	int deg;
	int degc;

	if (B[deg_B] == 0)
		fprintf(stderr, "\nThe highest coefficient of poly is zero!");
	else if ((deg_A == 0 && A[deg_A] == 0))
	{
		degc = 0;
		C[0] = 0;
		return degc;
	}
	else if (A[deg_A] != 0 && B[deg_B] != 0 && deg_A < deg_B)
	{
		degc = deg_A;
		for (i = 0; i < degc; i++)
			C[i] = A[i];
		return degc;
	}

	deg = deg_A;
	while (deg >= deg_B)
	{
		D[deg - deg_B] = 0;
		if (A[deg] > 0)
		{
			D[deg - deg_B] = Div(A[deg], B[deg_B]);
			for (i = 0; i < deg_B; i++)
				A[deg - 1 - i] = Add(A[deg - 1 - i], Mult(D[deg - deg_B], B[deg_B - 1 - i]));//multiplication, substraction
			A[deg] = 0;
		}
		deg--;
	}

	degc = deg_B - 1;
	for (i = 0; i <= degc; i++)
		C[i] = A[i];
	while (degc > 0 && C[degc] == 0)
		degc--;

	return degc;
}


/********************************************************************
���� �� LagrangeInterpolation(int k, int *alpha, int *beta, int *poly)
���� :  ��������Lagrange��ֵ
���������k      ----  ��ֵ����
          *alpha ----  ��ֵ���Ա���
          *beta  ----  ��ֵ�������
���������*poly  ----  ��ֵ����ʽ
*********************************************************************/

void CFiniteField2::LagrangeInterpolation(int k, int *alpha, int *beta, int *poly){
	int i, j;
	int li;
	int gi;
	int di;
	int *tempg;
	tempg = new int[k + 1];
	for (i = 1; i < k + 1; i++)
		tempg[i] = 0;
	tempg[0] = 1;
	for (i = 0; i < k; i++)
		poly[i] = 0;
	for (i = 0; i<k; i++) {
		if (i == 0) {
			poly[0] = beta[0];
			tempg[0] = alpha[0];
			tempg[1] = 1;
		}
		else {

			li = PolyEvaluate(alpha[i], poly, i - 1);
			gi = PolyEvaluate(alpha[i], tempg, i);
			di = Add(li, beta[i]);
			if (di != 0)
				for (j = 0; j < k; j++)
					poly[j] = Add(poly[j], Mult(Div(di, gi), tempg[j]));
			for (j = k; j > 0; j--)
				tempg[j] = Add(tempg[j - 1], Mult(tempg[j], alpha[i]));
			tempg[0] = Mult(tempg[j], alpha[i]);
		}
	}
	delete[]tempg;
}