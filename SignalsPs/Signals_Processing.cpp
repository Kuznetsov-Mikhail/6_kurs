#pragma once
#include "pch.h"
#include "Signals_Processing.h"
#include "cubic.h"

Signals_Processing::Signals_Processing()
{
	ippInit();
	Buffaza = 0;
	V = 0;
	delataW = 0;
	sampling = 0;
	BrV = 0;
	bit_time = 0;
}
Signals_Processing::~Signals_Processing()
{
}

void Signals_Processing::addNoize(vector<double>& mass, double NoizeV)
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

void Signals_Processing::addNoize(vector < complex<double> >& mass, double NoizeV)
{
	vector<double> shum_ampl;
	shum_ampl.resize(mass.size());
	for (int i = 0; i < shum_ampl.size(); i++)
	{
		shum_ampl[i] = 0;
	}
	double sum_signal = 0;
	double sum_shum = 0;
	for (int i = 0; i < mass.size(); i++)
	{
		sum_signal += mass[i].real() * mass[i].real() + mass[i].imag() * mass[i].imag();
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
		shum_ampl[i] = ksi / M;
	}
	vector<complex<double>> shum_c(shum_ampl.size());
	for (int i = 0; i < shum_c.size(); i++)
	{
		double r_phi = (rand() / RAND_MAX) * 2 * M_PI;
		shum_c[i] = shum_ampl[i] * cos(r_phi) + comjd * sin(r_phi);
	}
	for (int i = 0; i < mass.size(); i++)
	{
		sum_shum += shum_c[i].real() * shum_c[i].real() + \
			shum_c[i].imag() * shum_c[i].imag();
	}
	sum_signal = sqrt(sum_signal);
	sum_shum = sqrt(sum_shum);

	double alfa = sum_signal / (sum_shum * (pow(10, NoizeV / 20.)));
	//alfa = sqrt(sum_signal / (sum_shum * pow(10., 0.1 * NoizeV)));
	for (int i = 0; i < mass.size(); i++)
	{
		mass[i] += alfa * shum_c[i];
	}
}

void Signals_Processing::NormalPhaza(double& phaza)
{
	while (1)
	{
		if (phaza > 0)
		{
			if (phaza > (2 * M_PI))  phaza -= 2 * M_PI;
			else break;
		}
		else
		{
			if (phaza < (-2 * M_PI)) phaza += 2 * M_PI;
			else break;
		}
	}
}

void Signals_Processing::SignalFill(vector<double>& mass, vector<bool> data, bool type)
{
	if (type)
	{
		for (int i = 0; i < data.size(); i++)
		{
			if (data[i])
			{
				Buffaza = 2 * M_PI * (V + delataW) * i / sampling + Buffaza;
				NormalPhaza(Buffaza);
				mass[i] = sin(Buffaza);
				Buffaza = 2 * M_PI * (V + delataW) * i / sampling;

			}
			else
			{
				Buffaza = 2 * M_PI * (V - delataW) * i / sampling + Buffaza;
				NormalPhaza(Buffaza);
				mass[i] = sin(Buffaza);
				Buffaza = 2 * M_PI * (V - delataW) * i / sampling;
			}
		}
	}
	else
	{
		for (int i = 0; i < data.size(); i++)
		{
			Buffaza = 2 * M_PI * V * i / sampling + M_PI * data[i];
			NormalPhaza(Buffaza);
			mass[i] = sin(Buffaza);
			Buffaza = 2 * M_PI * V * i / sampling + M_PI * data[i];
		}
	}

}

void Signals_Processing::SignalFill(vector<complex<double>>& mass, vector<bool> data, bool type)
{

	if (type)
	{
		for (int i = 0; i < data.size(); i++)
		{//частотная
			if (data[i])
			{
				Buffaza += 2 * M_PI * (delataW) / sampling;
				NormalPhaza(Buffaza);
				mass[i] = cos(Buffaza) + comjd * sin(Buffaza);
			}
			else
			{
				Buffaza += 2 * M_PI * (-delataW) / sampling;
				NormalPhaza(Buffaza);
				mass[i] = cos(Buffaza) + comjd * sin(Buffaza);
			}
		}
	}
	else
	{//фазовая
		for (int i = 0; i < data.size(); i++)
		{
			Buffaza = M_PI * data[i];
			NormalPhaza(Buffaza);
			mass[i] = cos(Buffaza) + comjd * sin(Buffaza);
		}
	}

}

void Signals_Processing::DataFill(vector <bool>& Data1, vector <bool>& Data2, int Signal_size, int Data_size, int delay)
{
	vector <bool> Data;
	vector <bool> BufferDataRand;
	double SH = sampling / BrV;
	Data.resize(Data_size);
	for (int i = 0; i < Data_size; i++)
	{
		Data[i] = rand() % 2 == 0;
	}
	Data1.resize(Signal_size);
	for (int i = 0; i < Data_size; i++)
	{
		for (int j = 0; j < SH; j++)
		{
			Data1[i * SH + j] = Data[i];
		}
	}
	//DATA2
	int DELAY = delay;
	BufferDataRand.resize(2 * Data_size);
	for (int i = 0; i < BufferDataRand.size(); i++)
	{
		BufferDataRand[i] = rand() % 2 == 0;
	}
	Data2.resize(2 * Signal_size);
	for (int i = 0; i < BufferDataRand.size(); i++)
	{
		for (int j = 0; j < SH; j++)
		{
			Data2[i * SH + j] = BufferDataRand[i];
		}
	}
	for (int i = DELAY; i < Signal_size + DELAY; i++)
	{
		Data2[i] = Data1[i - DELAY];
	}
}

int Signals_Processing::Correlation(vector<double>& mass, vector<double> Signal1, vector<double> Signal2)
{
	int MMPdelay = 0;
	/*double SH = sampling / BrV;*/
	mass.resize(Signal1.size());
	for (int i = 0; i < mass.size(); i++)
	{
		mass[i] = 0;
	}

	for (int i = 0; i < mass.size(); i++)
	{
		for (int j = 0; j < Signal1.size(); j++)
		{
			mass[i] += (Signal2[i + j] * Signal1[j]);
		}
		mass[i] = abs(mass[i]) / Signal1.size();
	}
	double max = 0;
	for (int i = 0; i < mass.size(); i++)
	{
		if (mass[i] > max)
		{
			max = mass[i];
			MMPdelay = i;
		}
	}
	return ((double)MMPdelay/* / SH*/);


}

int Signals_Processing::Correlation(vector<double>& mass, vector<complex<double>> Signal1, vector<complex<double>> Signal2)
{
	int MMPdelay = 0;
	//double SH = sampling / BrV;
	vector <double> RrrReal, RrrImage;
	RrrReal.resize(Signal1.size());
	RrrImage.resize(Signal1.size());
	mass.resize(Signal1.size());
	for (int i = 0; i < mass.size(); i++)
	{
		mass[i] = 0;
		RrrReal[i] = 0;
		RrrImage[i] = 0;
	}
	for (int i = 0; i < mass.size(); i++)
	{
		for (int j = 0; j < Signal1.size(); j++)
		{

			RrrReal[i] += (Signal1[j].real() * Signal2[j + i].real()) + (Signal1[j].imag() * Signal2[j + i].imag());
			RrrImage[i] += (Signal1[j].imag() * Signal2[j + i].real()) - (Signal1[j].real() * Signal2[j + i].imag());
		}
		mass[i] = sqrt(pow(RrrReal[i], 2) + pow(RrrImage[i], 2));
		mass[i] /= Signal1.size();
	}
	double max = 0;
	for (int i = 0; i < mass.size(); i++)
	{
		if (mass[i] > max)
		{
			max = mass[i];
			MMPdelay = i;
		}
	}
	return ((double)MMPdelay /*/ SH*/);
}

void Signals_Processing::Uncertainty(vector<double>& mass, vector<complex<double>> Signal1, vector<complex<double>> Signal2, int ksum)
{
	if (ksum == 0)
	{
		ksum = 1;
	}
	double localMax;
	vector <complex<double>> Rrr; //вектор произведения С1 и С2
	vector <complex<double>> RrrK;// суммирование Rrr по блокам ksum
	int k = step2(Signal1.size());
	mass.resize(0);

	for (int i = 0; i < Signal1.size(); i++)
	{
		Rrr.resize(0);
		Rrr.resize(k);
		localMax = 0;
		int group = k / ksum;
		for (int j = 0; j < Signal1.size(); j++)
		{
			if ((i + j) == Signal2.size())
			{
				break;
			}
			Rrr[j] = (Signal1[j] * conj(Signal2[j + i]));
		}
		RrrK.resize(0);
		for (int j = 0; j < group; j++)
		{
			complex <double> bufferSum = 0;
			for (int k = 0; k < ksum; k++)
			{
				bufferSum += Rrr[j * ksum + k];
			}
			RrrK.push_back(bufferSum);
		};
		fur(RrrK, -1);
		for (int j = 0; j < RrrK.size(); j++)
		{
			if (abs(RrrK[j]) > localMax)localMax = abs(RrrK[j]);
		}
		mass.push_back(localMax);
	}
}

void Signals_Processing::Uncertainty_ipp(vector<float>& mass, const signal_buf& Signal1, const signal_buf& Signal2, int ksum)
{
	if (ksum == 0)
	{
		ksum = 1;
	}
	double localMax;
	int local_signal_size = Signal1.size();
	int k = step2(local_signal_size);
	mass.resize(local_signal_size);
	int group = k / ksum;

	Ipp64fc* pVec1 = ippsMalloc_64fc(local_signal_size);
#pragma omp parallel for
	for (int j = 0; j < local_signal_size; j++)
	{
		pVec1[j].re = Signal1[j].real();
		pVec1[j].im = Signal1[j].imag();
	}
	Ipp64fc* pVec2 = ippsMalloc_64fc(local_signal_size);
	Ipp64fc* correlation_Kgroup = ippsMalloc_64fc(group);// суммирование correlation по блокам ksum
	Ipp64f* correlation_Kgroup_abs = ippsMalloc_64f(group);// суммирование correlation по блокам ksum
	Ipp64fc* correlation = ippsMalloc_64fc(local_signal_size); //вектор произведения С1 и С2

	//for fft
	// Query to get buffer sizes
	int sizeFFTSpec, sizeFFTInitBuf, sizeFFTWorkBuf;
	int order = (int)(log((double)group) / log(2.0));
	ippsFFTGetSize_C_64fc(order, IPP_FFT_NODIV_BY_ANY,
		ippAlgHintAccurate, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
	// Alloc FFT buffers
	IppsFFTSpec_C_64fc* pFFTSpec = 0;
	Ipp8u* pFFTSpecBuf, * pFFTInitBuf, * pFFTWorkBuf;
	pFFTSpecBuf = ippsMalloc_8u(sizeFFTSpec);
	pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
	pFFTWorkBuf = ippsMalloc_8u(sizeFFTWorkBuf);
	// Initialize FFT
	ippsFFTInit_C_64fc(&pFFTSpec, order, IPP_FFT_NODIV_BY_ANY,
		ippAlgHintAccurate, pFFTSpecBuf, pFFTInitBuf);
	////
//#pragma omp parallel for
	for (int i = 0; i < local_signal_size; i++)
	{
#pragma omp parallel for
		for (int j = 0; j < local_signal_size; j++)
		{
			pVec2[j].re = Signal2[j + i].real();
			pVec2[j].im = Signal2[j + i].imag();
		}
		ippsConj_64fc_I(pVec2, local_signal_size);
		ippsMul_64fc(pVec1, pVec2, correlation, local_signal_size);

#pragma omp parallel for
		for (int j = 0; j < group; j++)
		{
			Ipp64fc bufferSum{ 0, 0 };
			for (int k = 0; k < ksum; k++)
			{
				if (j * ksum + k >= local_signal_size)break;
				bufferSum.re = bufferSum.re + correlation[j * ksum + k].re;
				bufferSum.im = bufferSum.im + correlation[j * ksum + k].im;
			}
			correlation_Kgroup[j] = bufferSum;
		}
		// Do the FFT
		ippsFFTFwd_CToC_64fc(correlation_Kgroup, correlation_Kgroup, pFFTSpec, pFFTWorkBuf);
		ippsMagnitude_64fc(correlation_Kgroup, correlation_Kgroup_abs, group);
		mass[i] = 0;
		Ipp64f imax;
		ippsMax_64f(correlation_Kgroup_abs, group, &imax);
		mass[i] = imax;
	}
	if (pFFTInitBuf) ippFree(pFFTInitBuf);
	if (pFFTWorkBuf) ippFree(pFFTWorkBuf);
	if (pFFTSpecBuf) ippFree(pFFTSpecBuf);
	ippFree(correlation);
	ippFree(correlation_Kgroup);
	ippFree(correlation_Kgroup_abs);
	ippFree(pVec2);
	ippFree(pVec1);
}

void Signals_Processing::Uncertainty_ipp(vector<double>& mass, const vector<complex<double>>& Signal1, const vector<complex<double>>& Signal2, int ksum)
{
	if (ksum == 0)
	{
		ksum = 1;
	}
	double localMax;
	int local_signal_size = Signal1.size();
	int k = step2(local_signal_size);
	mass.resize(local_signal_size);
	int group = k / ksum;

	Ipp64fc* pVec1 = ippsMalloc_64fc(local_signal_size);
#pragma omp parallel for
	for (int j = 0; j < local_signal_size; j++)
	{
		pVec1[j].re = Signal1[j].real();
		pVec1[j].im = Signal1[j].imag();
	}
	Ipp64fc* pVec2 = ippsMalloc_64fc(local_signal_size);
	Ipp64fc* correlation_Kgroup = ippsMalloc_64fc(group);// суммирование correlation по блокам ksum
	Ipp64f* correlation_Kgroup_abs = ippsMalloc_64f(group);// суммирование correlation по блокам ksum
	Ipp64fc* correlation = ippsMalloc_64fc(local_signal_size); //вектор произведения С1 и С2

	//for fft
	// Query to get buffer sizes
	int sizeFFTSpec, sizeFFTInitBuf, sizeFFTWorkBuf;
	int order = (int)(log((double)group) / log(2.0));
	ippsFFTGetSize_C_64fc(order, IPP_FFT_NODIV_BY_ANY,
		ippAlgHintAccurate, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
	// Alloc FFT buffers
	IppsFFTSpec_C_64fc* pFFTSpec = 0;
	Ipp8u* pFFTSpecBuf, * pFFTInitBuf, * pFFTWorkBuf;
	pFFTSpecBuf = ippsMalloc_8u(sizeFFTSpec);
	pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
	pFFTWorkBuf = ippsMalloc_8u(sizeFFTWorkBuf);
	// Initialize FFT
	ippsFFTInit_C_64fc(&pFFTSpec, order, IPP_FFT_NODIV_BY_ANY,
		ippAlgHintAccurate, pFFTSpecBuf, pFFTInitBuf);
	////
//#pragma omp parallel for
	for (int i = 0; i < local_signal_size; i++)
	{
#pragma omp parallel for
		for (int j = 0; j < local_signal_size; j++)
		{
			pVec2[j].re = Signal2[j + i].real();
			pVec2[j].im = Signal2[j + i].imag();
		}
		ippsConj_64fc_I(pVec2, local_signal_size);
		ippsMul_64fc(pVec1, pVec2, correlation, local_signal_size);

#pragma omp parallel for
		for (int j = 0; j < group; j++)
		{
			Ipp64fc bufferSum{ 0, 0 };
			for (int k = 0; k < ksum; k++)
			{
				if (j * ksum + k >= local_signal_size)break;
				bufferSum.re = bufferSum.re + correlation[j * ksum + k].re;
				bufferSum.im = bufferSum.im + correlation[j * ksum + k].im;
			}
			correlation_Kgroup[j] = bufferSum;
		}
		// Do the FFT
		ippsFFTFwd_CToC_64fc(correlation_Kgroup, correlation_Kgroup, pFFTSpec, pFFTWorkBuf);
		ippsMagnitude_64fc(correlation_Kgroup, correlation_Kgroup_abs, group);
		mass[i] = 0;
		Ipp64f imax;
		ippsMax_64f(correlation_Kgroup_abs, group, &imax);
		mass[i] = imax;
	}
	if (pFFTInitBuf) ippFree(pFFTInitBuf);
	if (pFFTWorkBuf) ippFree(pFFTWorkBuf);
	if (pFFTSpecBuf) ippFree(pFFTSpecBuf);
	ippFree(correlation);
	ippFree(correlation_Kgroup);
	ippFree(correlation_Kgroup_abs);
	ippFree(pVec2);
	ippFree(pVec1);
}

double  Signals_Processing::Uncertainty_ipp_jtids(int delay_size, const vector<complex<double>>& ImSignal1, const vector<complex<double>>& ImSignal2, int ksum, vector <double>& ResearchRrr, int& found_delay, int& delay_lama)
{
	//////////////////// Подготовка входных сигналов
	signal_buf signal1;
	signal_buf signal2;
	for (int i = 0; i < ImSignal1.size(); i++)
	{
		complex<float> buf; buf = ImSignal1[i].real() + comjd * ImSignal1[i].imag();
		signal1.push_back(buf);
	}
	for (int i = 0; i < ImSignal2.size(); i++)
	{
		complex<float> buf; buf = ImSignal2[i].real() + comjd * ImSignal2[i].imag();
		signal2.push_back(buf);
	}
	//////////////////// fast_convolution
	fast_convolution(signal1, this->fir_s, this->FHSS_Signals_initial_fl, GPU_FD);
	fast_convolution(signal2, this->fir_s, this->FHSS_Signals_fl, GPU_FD);

	ResearchRrr.clear();
	ResearchRrr.resize(this->FHSS_Signals_initial_fl[0].size());
	for (int i = 0; i < this->operating_frequencies.size(); i++)
	{
		vector<float>buffer;
		this->Uncertainty_ipp(buffer, this->FHSS_Signals_initial_fl[i], this->FHSS_Signals_fl[i], ksum);
#pragma omp parallel for
		for (int j = 0; j < ResearchRrr.size(); j++)
		{
			ResearchRrr[j] += buffer[j];
		}
	}
	if (ResearchRrr.size() != NULL)
	{
		double buff_ResearchRrr = 0;
		for (int i = 0; i < ResearchRrr.size(); i++)
		{
			if (ResearchRrr[i] > buff_ResearchRrr)
			{
				buff_ResearchRrr = ResearchRrr[i]; delay_lama = i;
			}
		}
	}
	this->FHSS_Signals_initial_fl.clear();
	this->FHSS_Signals_fl.clear();
	found_delay = delay_lama; //in counts 
	delay_lama = int((double)delay_lama / this->bit_time);// in bits
	int expected_delay = delay_size * this->bit_time;
	int delta_error = abs(expected_delay - found_delay);
	double pi = peak_intensity(ResearchRrr);
	return pi;
}

void Signals_Processing::Uncertainty_omp(vector<double>& mass, vector<complex<double>> Signal1, vector<complex<double>> Signal2, int ksum)
{
	if (ksum == 0)
	{
		ksum = 1;
	}
	double localMax;
	int local_signal_size = Signal1.size();
	int k = step2(Signal1.size());
	mass.resize(Signal1.size());
	int group = k / ksum;
#pragma omp parallel for
	for (int i = 0; i < local_signal_size; i++)
	{
		vector <complex<double>> correlation_Kgroup;// суммирование correlation по блокам ksum
		vector <complex<double>> correlation; //вектор произведения С1 и С2	
		correlation.resize(k);
		correlation_Kgroup.resize(group);
		for (int j = 0; j < local_signal_size; j++)
		{
			correlation[j] = (Signal1[j] * conj(Signal2[j + i]));
			//correlation[i] += (Signal1[j] * conj(Signal2[j + i]));
		}
#pragma omp parallel for
		for (int j = 0; j < group; j++)
		{
			complex <double> bufferSum = 0;
			for (int k = 0; k < ksum; k++)
			{
				bufferSum += correlation[j * ksum + k];
			}
			correlation_Kgroup[j] = bufferSum;
		}
		fur(correlation_Kgroup, -1);
		mass[i] = 0;
		for (int j = 0; j < correlation_Kgroup.size(); j++)
		{
			if (abs(correlation_Kgroup[j]) > mass[i])mass[i] = abs(correlation_Kgroup[j]);
		}
	}
}

void Signals_Processing::Uncertainty_omp(vector<float>& mass, signal_buf& Signal1, signal_buf& Signal2, int ksum)
{
	if (ksum == 0)
	{
		ksum = 1;
	}
	float localMax;
	int local_signal_size = Signal1.size();
	int k = step2(Signal1.size());
	mass.resize(Signal1.size());

#pragma omp parallel for
	for (int i = 0; i < local_signal_size; i++)
	{
		vector <complex<float>> correlation; //вектор произведения С1 и С2		
		vector <complex<float>> correlation_Kgroup;// суммирование correlation по блокам ksum
		correlation.resize(k);
		for (int j = 0; j < local_signal_size; j++)
		{
			correlation[i] += (Signal1[j] * conj(Signal2[j + i]));
		}
		int group = k / ksum;
		correlation_Kgroup.clear();
		correlation_Kgroup.resize(group);
#pragma omp parallel for
		for (int j = 0; j < group; j++)
		{
			complex <float> bufferSum = 0;
			for (int k = 0; k < ksum; k++)
			{
				bufferSum += correlation[j * ksum + k];
			}
			correlation_Kgroup[j] = bufferSum;
		}
		fur(correlation_Kgroup, -1);
		mass[i] = 0;
		for (int j = 0; j < correlation_Kgroup.size(); j++)
		{
			if (abs(correlation_Kgroup[j]) > mass[i])mass[i] = abs(correlation_Kgroup[j]);
		}
	}
}

void Signals_Processing::Uncertainty(vector<float>& mass, signal_buf& Signal1, signal_buf& Signal2, int ksum)
{
	if (ksum == 0)
	{
		ksum = 1;
	}

	float localMax;
	vector <complex<float>> Rrr; //вектор произведения С1 и С2
	vector <complex<double>> RrrK;// суммирование Rrr по блокам ksum
	int k = step2(Signal1.size());
	mass.resize(0);
	for (int i = 0; i < Signal1.size(); i++)
	{
		Rrr.resize(0);
		Rrr.resize(k);
		localMax = 0;
		int group = k / ksum;
		for (int j = 0; j < Signal1.size(); j++)
		{
			if ((i + j) == Signal2.size())
			{
				break;
			}
			Rrr[j] = (Signal1[j] * conj(Signal2[j + i]));
		}
		RrrK.resize(0);
		for (int j = 0; j < group; j++)
		{
			complex <float> bufferSum = 0;
			for (int k = 0; k < ksum; k++)
			{
				bufferSum += Rrr[j * ksum + k];
			}
			RrrK.push_back(bufferSum);
		}
		fur(RrrK, -1);
		for (int j = 0; j < RrrK.size(); j++)
		{
			if (abs(RrrK[j]) > localMax)localMax = abs(RrrK[j]);
		}
		mass.push_back(localMax);
	}
}

void Signals_Processing::FAST_FUR(vector <complex<double>> Signal, vector <complex<double>>& Spectr, bool is)
{
	Spectr.clear();
	int k = step2(Signal.size());
	Signal.resize(k);
	Spectr = Signal;
	fur(Spectr, is);
}

void Signals_Processing::spVertex(vector <complex<double>>& Spectr)
{
	vector <complex<double>> first, second;
	int middle = Spectr.size() / 2;
	for (int i = 0; i < middle; i++)first.push_back(Spectr[i]);
	for (int i = middle; i < Spectr.size(); i++)second.push_back(Spectr[i]);
	Spectr.clear();
	for (int i = 0; i < second.size(); i++)Spectr.push_back(second[i]); second.clear();
	for (int i = 0; i < first.size(); i++)Spectr.push_back(first[i]);	first.clear();
}

void Signals_Processing::fur(vector <complex <double>>& data, int is)
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

void Signals_Processing::fur(vector <complex <float>>& data, int is)
{
	int i, j, istep, n;
	n = data.size();
	int m, mmax;
	float r, r1, theta, w_r, w_i, temp_r, temp_i;
	float pi = 3.1415926f;
	r = pi * is;
	j = 0;
	for (i = 0; i < n; i++)
	{
		if (i < j)
		{
			temp_r = data[j].real();
			temp_i = data[j].imag();
			data[j] = data[i];
			data[i] = temp_r + complex <float>(0, 1) * temp_i;

		}
		m = n >> 1;
		while (j >= m) { j -= m; m = (m + 1) / 2; }
		j += m;
	}
	mmax = 1;
	while (mmax < n)
	{
		istep = mmax << 1;
		r1 = r / (float)mmax;
		for (m = 0; m < mmax; m++)
		{
			theta = r1 * m;
			w_r = (float)cos((float)theta);
			w_i = (float)sin((float)theta);
			for (i = m; i < n; i += istep)
			{
				j = i + mmax;
				temp_r = w_r * data[j].real() - w_i * data[j].imag();
				temp_i = w_r * data[j].imag() + w_i * data[j].real();
				data[j] = (data[i].real() - temp_r) + complex <float>(0, 1) * (data[i].imag() - temp_i);
				data[i] += (temp_r)+complex <float>(0, 1) * (temp_i);
			}
		}
		mmax = istep;
	}
	if (is > 0)
		//#pragma omp parallel for simd
		for (i = 0; i < n; i++)
		{
			data[i] /= (float)n;
		}
}

double Signals_Processing::Max(const vector<double>& Mass)
{
	double ymax = 0;
	for (auto J : Mass) if (J > ymax) ymax = J;
	return ymax;
}

double Signals_Processing::Min(const vector<double>& Mass)
{
	double ymin = 0;
	for (auto J : Mass) if (J < ymin) ymin = J;
	return ymin;
}

int Signals_Processing::step2(int sizein)
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

void Signals_Processing::Dopler(vector <complex<double>>& Signal, double shift, double center_frequency)
{
	double alfa = shift / center_frequency;
	Dopler_shift(Signal, shift);
	Dopler_scaling(Signal, alfa);
}

void Signals_Processing::Dopler_shift(vector<complex<double>>& mass, double PhiDopler)
{
	for (int i = 0; i < mass.size(); i++)
	{
		mass[i] *= exp(comjd * 2. * M_PI * PhiDopler * (double)i / sampling);
	}
}

void Signals_Processing::Dopler_scaling(vector <complex<double>>& Signal, double koeff)
{
	vector <complex<double>> BufSignal = Signal;
	vector<double> BufSignalR; BufSignalR.resize(BufSignal.size());
	vector<double> BufSignalI; BufSignalI.resize(BufSignal.size());
	vector <complex<double>> NewSignal;

	double alfa = 1 + koeff;
	for (int i = 0; i < BufSignal.size(); i++)
	{
		BufSignalR[i] = (BufSignal[i].real());
		BufSignalI[i] = (BufSignal[i].imag());
	}
	vector<double> NewSignalR;
	vector<double> NewSignalI;
	//Интерполяция сплайном
	//InterSpline(BufSignalR, NewSignalR, deltaT); 
	//InterSpline(BufSignalI, NewSignalI, deltaT);
	//Линейная интерполяция
	//Linear_interpolation(BufSignalR, NewSignalR, deltaT);
	//Linear_interpolation(BufSignalI, NewSignalI, deltaT);
	Cubic_Inter_spline(BufSignalR, NewSignalR, alfa);
	Cubic_Inter_spline(BufSignalI, NewSignalI, alfa);
	NewSignal.resize(NewSignalI.size());
	for (int i = 0; i < NewSignal.size(); i++)
	{
		NewSignal[i] = NewSignalR[i] + comjd * NewSignalI[i];
	}
	Signal.clear(); Signal = NewSignal;
}

void Signals_Processing::Cubic_Inter_spline(vector<double>& Old_Data, vector<double>& New_Data, double step)
{
	vector<double>X; X.resize(Old_Data.size());
	vector<double>Y; Y.resize(Old_Data.size());
	for (int i = 0; i < Old_Data.size(); i++)
	{
		X[i] = (i);
		Y[i] = (Old_Data[i]);
	}
	tk::spline s;
	s.set_points(X, Y);
	New_Data.resize((double)Old_Data.size() / (double)step);
	for (size_t i = 0; i < New_Data.size(); i++)
	{
		double x = step * i;
		New_Data[i] = (s(x));
	}
}

void Signals_Processing::Linear_interpolation(vector<double>& Old_Data, vector<double>& New_Data, double step)
{
	New_Data.clear();
	int new_size = ((double)(Old_Data.size() - 1) / step) + 1;
	New_Data.push_back(Old_Data[0]);
	for (int j = 1; j < new_size; j++)
	{
		double pos = step * j;
		int next;
		next = (int)ceil(pos);
		if (next != Old_Data.size())
			New_Data.push_back(((double)next - pos) * Old_Data[next - 1] + (pos - next + 1) * Old_Data[next]);
		else
		{
			next--;
			New_Data.push_back(((double)next - pos) * Old_Data[next - 1] + (pos - next + 1) * Old_Data[next]);
		}
	}
}

void Signals_Processing::InterSpline(vector<double>& Signal, vector<double>& NewSignal, double step)
{
	vector<double> massx;
	vector<double> massy;
	vector<double> mit;
	int m = Signal.size();
	massx.resize(m);       //Массив узлов интерполяции
	massy.resize(m);       //Массив значений в узлах интерполяции
	mit.resize(m);
	massx[0] = 0;    //Заполняем массив узлов интерполяции
	massy[0] = 0;
#pragma omp parallel for
	for (int i = 0; i < m; i++)
	{
		massx[i] = i * step;    //Заполняем массив узлов интерполяции
		massy[i] = Signal[i];         //Заполняем массив значении в узлах интерполяции
	}
	//вычисление наклонов
	mit[0] = ((4 * massy[1]) - massy[2] - 3 * massy[0]) / (2 * step);
#pragma omp parallel for
	for (int k = 1; k < mit.size() - 1; k++)
	{
		mit[k] = (massy[k + 1] - massy[k - 1]) / (2 * step);
	}
	mit[mit.size() - 1] = (3 * massy[mit.size() - 1] + massy[mit.size() - 3] - 4 * massy[mit.size() - 2]) / (2 * step);
	//double k = 0;
	//double z = (spline(massx, massy, mit, k, step, 0));  //Переменная для хранения значения интерполянты в точке 0
	NewSignal.clear();
	int lol = massx[massx.size() - 1];
	NewSignal.resize(lol);
#pragma omp parallel for
	for (int k = 0; k < lol; k++)
	{
		int i = 0;
		while (k > massx[i])
		{
			i++;
		}
		if (i != 0)i--;
		NewSignal[i] = (spline(massx, massy, mit, i, step, k));  //Переменная для хранения значения интерполянты в точке
	}
}

void Signals_Processing::Simple_Signals_Generator(vector <complex<double>>& Signal1, vector <complex<double>>& Signal2, int signalSize, int delaySize)
{
	Signal1.clear();
	Signal2.clear();
	struct obraz
	{
		int W_number; //текущая рабочая частота
		bool b_bit; //текущий передаваемый бит
	};
	int bitrate = BrV; //стандартный битрейт для JTIDS
	int samplingJTIDS = sampling;// частота дискретизации
	bit_time = samplingJTIDS / bitrate; //кол-во отчётов на 1 бит
	int bit_in_word = 1;
	vector <obraz> SignalsObraz;
	SignalsObraz.resize(signalSize * bit_time);

	///////////////////////////////////////////////////
	//scramble
	vector<bool> bit_nabor;
	bit_nabor.resize(signalSize);
	int count1 = 0, count0 = 0;
	for (int i = 0; i < bit_nabor.size(); i++)
	{
		double kkk = 300. + 1000. * rand() / RAND_MAX;
		if (kkk > 500)
		{
			bit_nabor[i] = 1; count1++;
		}
		else
		{
			bit_nabor[i] = 0; count0++;
		}
	}
	int M = 5;
	vector<bool> key; key.resize(M);
	for (int i = 0; i < key.size(); i++)
	{
		double kkk = 0 + 1000. * rand() / RAND_MAX;
		if (kkk > 500) key[i] = 1;
		else key[i] = 0;
	}
	for (u_int i = M; i < bit_nabor.size(); i++)
	{
		bool key_buf = false;
		for (u_int j = 1; j <= M; j++)
			key_buf = (key_buf + key[i - j]) % 2;
		key.push_back(key_buf);
	}
	count0 = count1 = 0;
	for (int i = 0; i < bit_nabor.size(); i++)
	{
		bit_nabor[i] = (bit_nabor[i] + key[i]) % 2;
		if (bit_nabor[i]) count1++;
		else count0++;
	}
	///////////////////////////////////////////////////
	/// for b_bit
	int interval = bit_time;
	int buf_ii = 0;
	double buf = 0 + 1000. * rand() / RAND_MAX;
	bool bit_buf;
	if (buf > 500) bit_buf = 1;
	else bit_buf = 0;
	int l = 0;
	bit_buf = bit_nabor[l];
	for (int i = 0; i < SignalsObraz.size(); i++)
	{
		buf_ii++;
		SignalsObraz[i].b_bit = bit_buf;
		if (buf_ii == interval)
		{
			buf_ii = 0;
			buf = 0 + 1000. * rand() / RAND_MAX;
			bit_buf;
			if (buf > 500) bit_buf = 1;
			else bit_buf = 0;
			l++; if (l == bit_nabor.size())l--;
			bit_buf = bit_nabor[l];
		}
	}
	//////////
	vector<int> frq_counter;
	frq_counter.resize(operating_frequencies.size());
	interval = bit_time * bit_in_word;//интервал счётчика
	buf_ii = 0;//счётчик
	int buf_number = pseudo_bit(0, 50);
	frq_counter[buf_number]++;
	for (int i = 0; i < SignalsObraz.size(); i++)
	{
		buf_ii++;
		SignalsObraz[i].W_number = buf_number;
		if (buf_ii == interval)
		{
			buf_ii = 0;
			//buf_number = pseudo_bit(0, 50);
			bool T = true;
			while (T)
			{
				buf_number = pseudo_bit(0, 50);
				int buf_number2 = pseudo_bit(0, 50);
				if (frq_counter[buf_number] <= frq_counter[buf_number2])
				{
					frq_counter[buf_number]++;
					T = false;
				}
				else
				{
					buf_number = buf_number2;
					frq_counter[buf_number]++;
					T = false;
				}
			}
		}
	}

	Buffaza = 0;
	double delta4astota = bitrate / 4;
	for (int i = 0; i < SignalsObraz.size(); i++)
	{
		double local_frequencies = ((double)operating_frequencies[SignalsObraz[i].W_number] - 1087.5) * 1000000;
		if (SignalsObraz[i].b_bit)Buffaza += 2 * M_PI * (local_frequencies + delta4astota) / sampling;
		else Buffaza += 2 * M_PI * (local_frequencies - delta4astota) / sampling;
		NormalPhaza(Buffaza);
		Signal2.push_back(cos(Buffaza) + comjd * sin(Buffaza));
	}
	delaySize *= bit_time;
	for (int i = delaySize; i < delaySize + Signal2.size() / 2; i++)
		Signal1.push_back(Signal2[i]);
}

void Signals_Processing::Link16_Signals_Generator(vector <complex<double>>& Signal1, vector <complex<double>>& Signal2, int signalSize, int delaySize, bool scramble)
{
	Signal1.clear();
	Signal2.clear();
	struct obraz
	{
		int W_number; //текущая рабочая частота
		bool b_bit; //текущий передаваемый бит
	};
	int bitrate = BrV; //стандартный битрейт для JTIDS
	int samplingJTIDS = sampling;// частота дискретизации
	bit_time = samplingJTIDS / bitrate; //кол-во отчётов на 1 бит
	int bit_in_word = 32; //32 в JTIDS
	vector <obraz> SignalsObraz;
	SignalsObraz.resize(signalSize * bit_time);


	vector<bool> bit_nabor;
	bit_nabor.resize(signalSize);
	int count1 = 0, count0 = 0;
	for (int i = 0; i < bit_nabor.size(); i++)
	{
		double kkk = 0. + 1000. * rand() / RAND_MAX;
		if (kkk > 150)
		{
			bit_nabor[i] = 1; count1++;
		}
		else
		{
			bit_nabor[i] = 0; count0++;
		}
	}
	if (scramble)
	{
		//scramble
		int M = (int)((double)bit_nabor.size() / 10);
		vector<bool> key; key.resize(M);
		//vector<bool> input; input.resize(M);
		for (int i = 0; i < key.size(); i++)
		{
			double kkk1 = 0. + 1000. * rand() / RAND_MAX;
			//double kkk2 = 0. + 1000. * rand() / RAND_MAX;
			if (kkk1 > 500) key[i] = 1;
			else key[i] = 0;
			//if (kkk2 > 500) input[i] = 1;
			//else input[i] = 0;
		}
		for (int i = M; i < bit_nabor.size(); i++)
		{
			bool key_buf = false;
			for (int j = 0; j < M; j++)
				key_buf = (key_buf + key[j] * bit_nabor[i - j - 1]) % 2;
			bit_nabor[i] = key_buf;
		}
		count0 = count1 = 0;
		for (int i = 0; i < bit_nabor.size(); i++)
		{
			if (bit_nabor[i]) count1++;
			else count0++;
		}
		///////////////////////////////////////////////////
	}
	///////////////////////////////////////////////////

	/// for для b_bit
	int interval = bit_time;//интервал счётчика
	int buf_ii = 0;//счётчик
	double buf = 0 + 1000. * rand() / RAND_MAX;
	bool bit_buf;
	if (buf > 500) bit_buf = 1;
	else bit_buf = 0;
	int l = 0;
	bit_buf = bit_nabor[l];
	for (int i = 0; i < SignalsObraz.size(); i++)
	{
		buf_ii++;
		SignalsObraz[i].b_bit = bit_buf;
		if (buf_ii == interval)
		{
			buf_ii = 0;
			buf = 0 + 1000. * rand() / RAND_MAX;
			bit_buf;
			if (buf > 500) bit_buf = 1;
			else bit_buf = 0;
			l++; if (l == bit_nabor.size())l--;
			bit_buf = bit_nabor[l];
		}
	}
	/// for для W_number
	vector<int> frq_counter;
	frq_counter.resize(operating_frequencies.size());
	interval = bit_time * bit_in_word;//интервал счётчика
	buf_ii = 0;//счётчик
	int buf_number = pseudo_bit(0, 50);
	frq_counter[buf_number]++;
	for (int i = 0; i < SignalsObraz.size(); i++)
	{
		buf_ii++;
		SignalsObraz[i].W_number = buf_number;
		if (buf_ii == interval)
		{
			buf_ii = 0;
			//buf_number = pseudo_bit(0, 50);
			bool T = true;
			while (T)
			{
				buf_number = pseudo_bit(0, 50);
				int buf_number2 = pseudo_bit(0, 50);
				if (frq_counter[buf_number] <= frq_counter[buf_number2])
				{
					frq_counter[buf_number]++;
					T = false;
				}
				else
				{
					buf_number = buf_number2;
					frq_counter[buf_number]++;
					T = false;
				}
			}
		}
	}
	///Заполнение сигнала
	Buffaza = 0;
	double delta4astota = bitrate / 4;
	for (int i = 0; i < SignalsObraz.size(); i++)//Исследуемый сигнал
	{//частотная
		double local_frequencies = ((double)operating_frequencies[SignalsObraz[i].W_number] - 1087.5) * 1000000;
		if (SignalsObraz[i].b_bit)Buffaza += 2 * M_PI * (local_frequencies + delta4astota) / sampling;
		else Buffaza += 2 * M_PI * (local_frequencies - delta4astota) / sampling;
		NormalPhaza(Buffaza);
		Signal2.push_back(cos(Buffaza) + comjd * sin(Buffaza));
	}
	delaySize *= bit_time;
	for (int i = delaySize; i < delaySize + Signal2.size() / 2; i++) //Опорный сигнал
		Signal1.push_back(Signal2[i]);
}

void Signals_Processing::FHSS(vector <complex<double>>& Signal1, vector <complex<double>>& Signal2, int signalSize, int delaySize)
{
	struct obraz
	{
		int W_number; //текущая рабочая частота
		bool b_bit; //текущий передаваемый бит
	};
	Signal1.clear();
	Signal2.clear();

	int time_windows_count = 100;//количество перестроек частоты
	int time_windows = signalSize / time_windows_count;//интервал на одной рабочей частоте в отсчётах
	int bitrate = BrV; //стандартный битрейт для JTIDS

	int samplingJTIDS = sampling;
	double SignalTime = (double)signalSize / samplingJTIDS; //длительность сигнала в с
	double WindowTime = 1. / bitrate;//длительность передачи бита в с
	int bits_count = SignalTime / WindowTime;//количество бит для передачи
	int time_bits = signalSize / bits_count;//интервал на один бит в отсчётах
	vector <obraz> SignalsObraz;
	SignalsObraz.resize(signalSize);

	/// for для W_number
	for (int i = 0; i < time_windows_count; i++)
	{
		int buf_number = pseudo_bit(0, 50);
		for (int j = 0; j < time_windows; j++)
		{
			if (i * time_windows + j > signalSize)break;
			else
			{
				SignalsObraz[i * time_windows + j].W_number = buf_number;
			}
		}
	}
	/// for для b_bit
	for (int i = 0; i < bits_count; i++)
	{
		double buf = 0 + 1000. * rand() / RAND_MAX;
		bool bit_buf;
		if (buf > 500) bit_buf = 1;
		else bit_buf = 0;
		for (int j = 0; j < time_bits; j++)
		{
			if (i * time_bits + j > signalSize)break;
			else
			{
				SignalsObraz[i * time_bits + j].b_bit = bit_buf;
			}
		}
	}
	///Заполнение сигнала
	Buffaza = 0;
	double delta4astota = bitrate / 4;
	for (int i = 0; i < signalSize; i++)//Исследуемый сигнал
	{//частотная
		double local_frequencies = ((double)operating_frequencies[SignalsObraz[i].W_number] - 1087.5) * 1000000;
		if (SignalsObraz[i].b_bit)Buffaza += 2 * M_PI * (local_frequencies + delta4astota) / sampling;
		else Buffaza += 2 * M_PI * (local_frequencies - delta4astota) / sampling;
		NormalPhaza(Buffaza);
		Signal2.push_back(cos(Buffaza) + comjd * sin(Buffaza));
	}
	for (int i = delaySize; i < delaySize + Signal2.size() / 2; i++) //Опорный сигнал
		Signal1.push_back(Signal2[i]);
}

void Signals_Processing::FHSS(int signalSize, int delaySize)
{
	FHSS_Signals.clear();
	FHSS_Signals_initial.clear();
	FHSS_Signals.resize(operating_frequencies.size());
	FHSS_Signals_initial.resize(operating_frequencies.size());

	for (int i = 0; i < FHSS_Signals.size(); i++)
	{
		FHSS_Signals[i].resize(signalSize);
	}
	struct obraz
	{
		int W_number; //текущая рабочая частота
		bool b_bit; //текущий передаваемый бит
	};
	int time_windows_count = 200;//количество перестроек частоты
	int time_windows = signalSize / time_windows_count;//интервал на одной рабочей частоте в отсчётах
	int bitrate = BrV; //стандартный битрейт для JTIDS

	int samplingJTIDS = sampling;
	double SignalTime = (double)signalSize / samplingJTIDS; //длительность сигнала в с
	double WindowTime = 1. / bitrate;//длительность передачи бита в с
	int bits_count = SignalTime / WindowTime;//количество бит для передачи
	int time_bits = signalSize / bits_count;//интервал на один бит в отсчётах
	vector <obraz> SignalsObraz;
	SignalsObraz.resize(signalSize);

	/// for для W_number
	for (int i = 0; i < time_windows_count; i++)
	{
		int buf_number = pseudo_bit(0, 50);
		for (int j = 0; j < time_windows; j++)
		{
			if (i * time_windows + j > signalSize)break;
			else
			{
				SignalsObraz[i * time_windows + j].W_number = buf_number;
			}
		}
	}
	/// for для b_bit
	for (int i = 0; i < bits_count; i++)
	{
		double buf = 0 + 1000. * rand() / RAND_MAX;
		bool bit_buf;
		if (buf > 500) bit_buf = 1;
		else bit_buf = 0;
		for (int j = 0; j < time_bits; j++)
		{
			if (i * time_bits + j > signalSize)break;
			else
			{
				SignalsObraz[i * time_bits + j].b_bit = bit_buf;
			}
		}
	}
	///Заполнение сигнала
	Buffaza = 0;
	double delta4astota = bitrate / 4;
	for (int i = 0; i < signalSize; i++) //Исследуемый сигнал
	{//частотная
		double local_frequencies = ((double)operating_frequencies[SignalsObraz[i].W_number] - 1087.5) * 1000000;
		if (SignalsObraz[i].b_bit)Buffaza += 2 * M_PI * (local_frequencies + delta4astota) / sampling;
		else Buffaza += 2 * M_PI * (local_frequencies - delta4astota) / sampling;
		NormalPhaza(Buffaza);
		FHSS_Signals[SignalsObraz[i].W_number][i] = cos(Buffaza) + comjd * sin(Buffaza);
	}

	for (int j = 0; j < FHSS_Signals_initial.size(); j++)
		for (int i = delaySize; i < delaySize + signalSize / 2; i++) //Опорный сигнал
			FHSS_Signals_initial[j].push_back(FHSS_Signals[j][i]);

}

inline int Signals_Processing::pseudo_bit(int start, int end)
{
	return (start + (end - start) * rand() / RAND_MAX);
}

double Signals_Processing::peak_intensity(vector<double> mas)
{
	double sum = 0;
	double localMax = 0;
	for (int i = 0; i < mas.size(); i++)
	{
		sum += mas[i];
		if (mas[i] > localMax)
		{
			localMax = mas[i];
		}
	}
	sum /= mas.size();
	double sigma = 0;
	for (int i = 0; i < mas.size(); i++)
	{
		sigma += pow(mas[i] - sum, 2);
	}
	return (localMax - sum) / sqrt(sigma);
}