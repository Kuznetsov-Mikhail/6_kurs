#pragma once

#include <vector>
#define M_PI 3.1415926535
#define comjd complex<double>(0,1)
#define comjf complex<float>(0,1)

using namespace std;

class Signal
{
private:
	//���������� ��������� �������� pow(2,n)
	int step2(int sizein)
	{
		int i = 0;
		double S = sizein;
		for (;;)
		{
			if (S > 1)
			{
				i++;
				S /= 2;
			}
			else break;
		}
		return pow(2, i);
	}
	//������� ���������� ���� �� +-2M_PI
	void NormalPhaza(double& faza)
	{
		while (1)
		{
			if (faza > 0)
			{
				if (faza > (2 * M_PI))  faza -= 2 * M_PI;
				else break;
			}
			else
			{
				if (faza < (-2 * M_PI)) faza += 2 * M_PI;
				else break;
			}
		}
	}
	//����������� ��������
	void GetSignals_AM()
	{

	}
	//������� ��������� (2)
	void GetSignals_FM2()
	{

	}
	//MSK ��������� ���������
	void GetSignals_MSK()
	{

	}
public:
	// ������� �������������
	int sampling;
	// �������
	int f_0;
	// ������� ���������
	int bitrate;
	// ����� ��� ������
	int bits_size;
	// ��� ��� �������� �������
	double SNR_1;
	// ��� ��� ����������� �������
	double SNR_2;
	// ��� ��������� (1-��, 2-��2, 3-MSK)
	int mod_type;
	// �������� (�������)
	int delay;

	vector<double> Signal1, Signal2;

	Signal() {}
	virtual ~Signal() {}
	void Init(int _sampling, int _f_0, int _bitrate, int _bits_size, double _SNR_1, double _SNR_2, int _mod_type, int _delay)
	{
		sampling = _sampling;
		f_0 = _f_0;
		bitrate = _bitrate;
		bits_size = _bits_size;
		SNR_1 = _SNR_1;
		SNR_2 = _SNR_2;
		mod_type = _mod_type;
		delay = _delay;
	}
	//��������� �������
	void GetSignals()
	{
		switch (mod_type)
		{
		case 1:
			GetSignals_AM();
			break;
		case 2:
			GetSignals_FM2();
			break;
		case 3:
			GetSignals_MSK();
			break;
		default:
			break;
		}
	}
};