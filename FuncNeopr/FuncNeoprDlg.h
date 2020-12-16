
// FuncNeoprDlg.h: файл заголовка
//

#pragma once
#include <math.h>
#include "ChartViewer.h""
#include <complex>
#include <math.h>
#include <vector>
#include <fstream>
#include "afxdialogex.h"
#include <algorithm>

using namespace std;
#define M_PI 3.1415926535

// Диалоговое окно CFuncNeoprDlg
class CFuncNeoprDlg : public CDialogEx
{
// Создание
public:
	CFuncNeoprDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FUNCNEOPR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void GetData(vector<bool>& data, int bits_count)
	{
		data.resize(bits_count * 2);
		for (int i = 0; i < data.size(); i++)
		{
			double kkk = 0. + 1000. * rand() / RAND_MAX;
			if (kkk > 500) data[i] = true;
			else data[i] = false;
		}
	}
	void GetSignals_MSK(vector<complex<double>>& Signal1, vector<complex<double>>& Signal2, double sampling, double f_0, double bitrate, int bits_size, double delay)
	{
		Signal1.clear();
		Signal2.clear();
		vector<bool> data;
		GetData(data, bits_size);
		int bit_time = sampling / bitrate;
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
				Buffaza += (2 * M_PI * (f_0 -delta4astota) / sampling);
			}
			NormalPhaza(Buffaza);
			Signal2[i] = cos(Buffaza) + complex<double>(0, 1) * sin(Buffaza);
		}
		for (int i = 0; i < N1; i++) Signal1[i] = Signal2[i + delay_size];
	}
	void Uncertainty(vector<double>& mass, vector<complex<double>> &Signal1, vector<complex<double>> &Signal2)
	{
		int t_size = Signal1.size();
		int f_size = step2(Signal1.size());
		mass.resize(t_size*f_size,0);
#pragma omp parallel for
		for (int i = 0; i < t_size; i++)
		{
			vector <complex<double>> correlation; //вектор произведения С1 и С2	
			correlation.resize(f_size);
			for (int j = 0; j < Signal2.size() - Signal1.size(); j++)
			{
				correlation[j] = (Signal1[j] * conj(Signal2[j + i]));
			}
			fur(correlation, -1);
			for (int j = 0; j < f_size; j++)
				mass[i * f_size + j] = abs(correlation[j]);
		}

	}
	void t_f_detected(vector<double>& neopr,int t_size, int f_size,double sampling, int& t_finded, double& f_finded)
	{
		int fint_finded;
		std::vector<double>::iterator result;
		result = max_element(neopr.begin(), neopr.end());
		int id_max = distance(neopr.begin(), result); // номер максимального элемента в 1D векторе
		fint_finded = id_max % f_size;
		t_finded = (id_max - fint_finded) / f_size;
		f_finded = ((double)fint_finded / (double)f_size) * sampling;
		if (f_finded > sampling / 2) f_finded = sampling - f_finded;
	}

	int step2(int sizein);

	void fur(vector <complex <double>>& data, int is);

	void addNoize_Complex(vector<complex<double>>& mass, double Noize);


	void NormalPhaza(double& phaza);

	void Dopler_shift(vector<complex<double>>& mass, double sampling, double PhiDopler);

	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();

	CChartViewer Obj_Ris;
	DoubleArray vectorToArray(vector<double>& v);

	void MyViewerDraw3D(vector<double>& data,
		CChartViewer& viewer_num, string PathPic, bool podpisi);
	void MyViewerDraw(string Data, vector<vector<double>>& data, double Xmin, 
		double Xmax, CChartViewer& viewer_num, string PathPic, bool podpis);

	double bitrate;
	double chast_diskr;
	double f_0;
	int delta_t;
	int delta_w;
	double noise;
	int delta_t_finded;
	double delta_w_finded;
	int bitsSize;

	vector <complex<double>> Signal_1, Signal_2;
	double peak_intensity(vector<double> &mas)
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
	template <typename T>
	void Correlation_omp(vector<double>& mass, const vector<complex<T>>& Signal1, const vector<complex<T>>& Signal2)
	{
		mass.resize(Signal2.size() - Signal1.size());
#pragma omp parallel for
		for (int i = 0; i < mass.size(); i++)
		{
			double RrrReal = 0, RrrImage = 0;
			for (int j = 0; j < Signal1.size(); j++)
			{

				RrrReal += (Signal1[j].real() * Signal2[j + i].real()) + (Signal1[j].imag() * Signal2[j + i].imag());
				RrrImage += (Signal1[j].imag() * Signal2[j + i].real()) - (Signal1[j].real() * Signal2[j + i].imag());
			}
			mass[i] = sqrt(pow(RrrReal, 2) + pow(RrrImage, 2));
			mass[i] /= Signal1.size();
		}
	}
	void Uncertainty_omp(vector<double>& mass, vector<complex<double>> Signal1, vector<complex<double>> Signal2, int ksum)
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
	afx_msg void OnBnClickedButton4();
};
