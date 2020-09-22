#pragma once

#include <vector>
#include <complex>
#define M_PI 3.1415926535
#define comjd complex<double>(0,1)
#define comjf complex<float>(0,1)

using namespace std;

class Signal
{
private:
	//возвращает ближайшее значение pow(2,n)
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
	//Функция нормировки фазы до +-2M_PI
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
	void GetData(vector<bool>& data)
	{
		data.resize(bits_size*2);
		for (int i = 0; i < data.size(); i++)
		{
			double kkk = 0. + 1000. * rand() / RAND_MAX;
			if (kkk > 500) data[i] = true;
			else data[i] = false;
		}
	}
	//Амплитудная мдуляция
	void GetSignals_AM()
	{
		Signal1.clear();
		Signal2.clear();
		vector<bool> data;
		GetData(data);
		bit_time = sampling / bitrate; //кол-во отчётов на 1 бит
		int N1 = bit_time * bits_size; //Signal1 size
		int N2 = N1 * 2; //Signal2 size
		int delay_size = delay * N1;
		vector<bool>obraz; obraz.resize(N2);
		///////////////////////////////////////////////////
		/// for b_bit
		int buf_ii = 0;
		bool bit_buf;
		int l = 0;
		bit_buf = data[l];
		for (int i = 0; i < obraz.size(); i++)
		{
			buf_ii++;
			obraz[i] = bit_buf;
			if (buf_ii == bit_time)
			{
				buf_ii = 0;
				l++; if (l == data.size())l--;
				bit_buf = data[l];
			}
		}
		//////////
		Signal1.resize(N1);
		Signal2.resize(N2);
		double Buffaza = 0;
		//////////
		for (int i = 0; i < obraz.size(); i++)
		{
			double AAA;
			Buffaza += (2 * M_PI * (f_0) / sampling);
			if (obraz[i])AAA = 1.;
			else AAA = 0.7;
			Signal2[i] = AAA * cos(Buffaza);
		}
		for (int i = 0; i < N1; i++) Signal1[i]= Signal2[i + delay_size];
	}
	//Фазовая модуляция (2)
	void GetSignals_FM2()
	{
		Signal1.clear();
		Signal2.clear();
		vector<bool> data;
		GetData(data);
		bit_time = sampling / bitrate; //кол-во отчётов на 1 бит
		int N1 = bit_time * bits_size; //Signal1 size
		int N2 = N1 * 2; //Signal2 size
		int delay_size = delay * N1;
		vector<bool>obraz; obraz.resize(N2);
		///////////////////////////////////////////////////
		/// for b_bit
		int buf_ii = 0;
		bool bit_buf;
		int l = 0;
		bit_buf = data[l];
		for (int i = 0; i < obraz.size(); i++)
		{
			buf_ii++;
			obraz[i] = bit_buf;
			if (buf_ii == bit_time)
			{
				buf_ii = 0;
				l++; if (l == data.size())l--;
				bit_buf = data[l];
			}
		}
		//////////
		Signal1.resize(N1);
		Signal2.resize(N2);
		double Buffaza = 0;
		bit_buf = obraz[0];
		//////////
		for (int i = 0; i < obraz.size(); i++)
		{
			if (obraz[i] == bit_buf)Buffaza += (2 * M_PI * (f_0) / sampling);
			else
			{
				Buffaza += (2 * M_PI * (f_0) / sampling) + M_PI;
				bit_buf = obraz[i];
			}
			NormalPhaza(Buffaza);
			Signal2[i] = cos(Buffaza);
		}
		for (int i = 0; i < N1; i++) Signal1[i] = Signal2[i + delay_size];
	}
	//MSK частотная модуляция
	void GetSignals_MSK()
	{
		Signal1.clear();
		Signal2.clear();
		vector<bool> data;
		GetData(data);
		bit_time = sampling / bitrate; //кол-во отчётов на 1 бит
		int N1 = bit_time * bits_size; //Signal1 size
		int N2 = N1 * 2; //Signal2 size
		int delay_size = delay * N1;
		vector<bool>obraz; obraz.resize(N2);
		///////////////////////////////////////////////////
		/// for b_bit
		int buf_ii = 0;
		bool bit_buf;
		int l = 0;
		bit_buf = data[l];
		for (int i = 0; i < obraz.size(); i++)
		{
			buf_ii++;
			obraz[i] = bit_buf;
			if (buf_ii == bit_time)
			{
				buf_ii = 0;
				l++; if (l == data.size())l--;
				bit_buf = data[l];
			}
		}
		//////////
		Signal1.resize(N1);
		Signal2.resize(N2);
		double Buffaza = 0;
		//////////
		double delta4astota = bitrate / 4;
		for (int i = 0; i < obraz.size(); i++)
		{
			if (obraz[i])Buffaza += (2 * M_PI * (f_0 + delta4astota) / sampling);
			else
			{
				Buffaza += (2 * M_PI * (f_0 - delta4astota) / sampling);
			}
			NormalPhaza(Buffaza);
			Signal2[i] = cos(Buffaza);
		}
		for (int i = 0; i < N1; i++) Signal1[i] = Signal2[i + delay_size];
	}
	void fur(vector <complex <double>>& data, int is)
	{
		int i, j, istep, n;
		n = data.size();
		int m, mmax;
		double r, r1, theta, w_r, w_i, temp_r, temp_i;
		double pi = 3.1415926f;
		r = pi * is;
		j = 0;
		for (i = 0; i < n; i++)
		{
			if (i < j)
			{
				temp_r = data[j].real();
				temp_i = data[j].imag();
				data[j] = data[i];
				data[i] = temp_r + complex <double>(0, 1) * temp_i;

			}
			m = n >> 1;
			while (j >= m) { j -= m; m = (m + 1) / 2; }
			j += m;
		}
		mmax = 1;
		while (mmax < n)
		{
			istep = mmax << 1;
			r1 = r / (double)mmax;
			for (m = 0; m < mmax; m++)
			{
				theta = r1 * m;
				w_r = (double)cos((double)theta);
				w_i = (double)sin((double)theta);
				for (i = m; i < n; i += istep)
				{
					j = i + mmax;
					temp_r = w_r * data[j].real() - w_i * data[j].imag();
					temp_i = w_r * data[j].imag() + w_i * data[j].real();
					data[j] = (data[i].real() - temp_r) + complex <double>(0, 1) * (data[i].imag() - temp_i);
					data[i] += (temp_r)+complex <double>(0, 1) * (temp_i);
				}
			}
			mmax = istep;
		}
		if (is > 0)
			//#pragma omp parallel for simd
			for (i = 0; i < n; i++)
			{
				data[i] /= (double)n;
			}
	}
/// <summary>
/// ////////////////////////////////////////PUBLIC////////////////////////////////////////////
/// </summary>
public:
	//кол-во отчётов на 1 бит
	int bit_time;
	// Частота дискретизации
	int sampling;
	// Несущая
	int f_0;
	// Частота модуляции
	int bitrate;
	// Число бит данных
	int bits_size;
	// ОСШ
	double SNR;
	// Тип модуляции (1-АМ, 2-ФМ2, 3-MSK)
	int mod_type;
	// Задержка (отсчёты)
	double delay;

	vector<double> Signal1, Signal2;

	Signal() {}
	virtual ~Signal() {}
	void Init(int _sampling, int _f_0, int _bitrate, int _bits_size, double _SNR, int _mod_type, double _delay)
	{
		sampling = _sampling;
		f_0 = _f_0;
		bitrate = _bitrate;
		bits_size = _bits_size;
		SNR = _SNR;
		mod_type = _mod_type;
		delay = _delay;
	}
	//Получение сигнала
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
	void addNoize(vector<double>& mass, double NoizeV)
	{
		double* shum_n = new double[mass.size()];
		double alfa;
		for (int i = 0; i < mass.size(); i++)
		{
			shum_n[i] = 0;
		}
		double sum_signal = 0;
		double sum_shum = 0;
		for (int i = 0; i < mass.size(); i++)
		{
			sum_signal += mass[i] * mass[i];
		}
		for (int i = 0; i < mass.size(); i++)
		{
			double M, ksi;
			M = rand() % 9 + 12;
			ksi = 0;
			for (int k = 1; k <= M; k++)
			{
				ksi += (double)((rand() % 21 - 10) / 10.);
			}
			shum_n[i] = ksi / M;
		}
		for (int i = 0; i < mass.size(); i++)
		{
			sum_shum += shum_n[i] * shum_n[i];
		}
		alfa = sqrt(sum_signal / (sum_shum * pow(10., 0.1 * NoizeV)));
		for (int i = 0; i < mass.size(); i++)
		{
			mass[i] = mass[i] + alfa * shum_n[i];
		}
		delete[]shum_n;
	}
	void Get_MMP(vector<double>& MMP)
	{
		if (Signal1.empty() || Signal2.empty()) return;
		int N1 = Signal1.size();
		int N2 = Signal2.size();
		if (N2 != (2 * N1)) return;
		MMP.resize(Signal1.size());
		for (int i = 0; i < N1; i++)
		{
			double buffer = 0;
			for (int j = 0; j < N1; j++)
				buffer += Signal1[j] * Signal2[i + j];
			MMP[i] = abs(buffer);
		}		
	}
	double GetMax(const vector<double>& data, int& number)
	{
		if (data.empty()) {
			return -1; number = -1;
		}
		number = 0;
		double Buf = data[0];
		for (int i = 1; i < data.size(); i++)
		{
			if (data[i] > Buf)
			{
				Buf = data[i];
				number = i;
			}
		}
		return Buf;
	}
	void FAST_FUR(vector <complex<double>> Signal, vector <complex<double>>& Spectr, int is)
	{
		Spectr.clear();
		int k = step2(Signal.size());
		Signal.resize(k);
		Spectr = Signal;
		fur(Spectr, is);
	}
	void spVertex(vector <complex<double>>& Spectr)
	{
		vector <complex<double>> first, second;
		int middle = Spectr.size() / 2;
		for (int i = 0; i < middle; i++)first.push_back(Spectr[i]);
		for (int i = middle; i < Spectr.size(); i++)second.push_back(Spectr[i]);
		Spectr.clear();
		for (int i = 0; i < second.size(); i++)Spectr.push_back(second[i]); second.clear();
		for (int i = 0; i < first.size(); i++)Spectr.push_back(first[i]);	first.clear();
	}
	void spCleaner(vector <complex<double>>& Spectr)
	{
		int size = Spectr.size();
		int start1 = (((double)f_0 - (double)f_0*0.7) / (double)sampling) * size;
		int finish1 = (((double)f_0 + (double)f_0 * 0.7) / (double)sampling) * size;
		if (start1 < 0)start1 = 0; if (start1 > size)start1 = size;
		if (finish1 < 0)finish1 = 0; if (finish1 > size)finish1 = size;
		int start2 = Spectr.size() - finish1;
		int finish2 = Spectr.size() - start1;
		for (int i = 0; i < start1; i++)Spectr[i] = 0;
		for (int i = finish1; i < start2; i++)Spectr[i] = 0;
		for (int i = finish2; i < Spectr.size(); i++)Spectr[i] = 0;		

	/*	int dot1 = (((double)f_0) / (double)sampling) * size; dot1 *= 4;
		int dot2 = Spectr.size() - dot1;
		for (int i = dot1; i < dot2; i++)Spectr[i] = 0;*/
	}
};