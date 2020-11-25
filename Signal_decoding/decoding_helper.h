#pragma once
#include <vector>
#include <ctime>
#include <complex>
#include <algorithm>

#define M_PI 3.1415926535
#define comjd complex<double>(0,1)
#define comjf complex<float>(0,1)

using namespace std;
using vc_double = std::vector<complex<double>>;

class decoding_helper
{
public:
	decoding_helper() {}
	~decoding_helper() {}
	vector<vc_double> Gold_filters;
private:
	vector<bool> input; //input bits
	vector<bool> Gold_input;
	vector<bool> output; //output bits
	vector<bool> Gold_output;
	vector<bool> random_sequence1;
	vector<bool> random_sequence2;
	vector<vector<bool>> Gold_cods;


	int count = 30;
	int mods_count = 4;
public:
	vc_double signal;
	int input_generation(int bits_count)
	{
		if (bits_count % 2 == 1) return 1;
		signal.clear();
		//input data generation
		srand(time(0));
		input.resize(bits_count);
		for (int i = 0; i < input.size(); i++)
			if (rand() > RAND_MAX / 2) input[i] = true;
			else input[i] = false;
		//output data generation
		output.resize((input.size() / 2) * count);
		for (int i = 0; i < input.size(); i += 2)
		{
			if (input[i] == true && input[i + 1] == true)
				for (int j = 0; j < count; j++)
					output[j + (i / 2) * count] = Gold_cods[0][j];
			if (input[i] == true && input[i + 1] == false)
				for (int j = 0; j < count; j++)
					output[j + (i / 2) * count] = Gold_cods[1][j];
			if (input[i] == false && input[i + 1] == true)
				for (int j = 0; j < count; j++)
					output[j + (i / 2) * count] = Gold_cods[2][j];
			if (input[i] == false && input[i + 1] == false)
				for (int j = 0; j < count; j++)
					output[j + (i / 2) * count] = Gold_cods[3][j];
		}
		return 0;
	}
	int signal_generation(int sampling, int bitrate, double snr)
	{
		if (input.empty()) return 1;
		QAM4_generation(sampling, bitrate, output, signal);
		addNoize(this->signal, snr);
	}
	int Init(int sampling, int bitrate)
	{
		input.clear();
		signal.clear();
		srand(time(0));
		//random_sequence generation
		random_sequence1.resize(count);
		random_sequence2.resize(count);
		for (int i = 0; i < count; i++)
			if (rand() > RAND_MAX / 2) random_sequence1[i] = true;
			else random_sequence1[i] = false;
		for (int i = 0; i < count; i++)
			if (rand() > RAND_MAX / 2) random_sequence2[i] = true;
			else random_sequence2[i] = false;
		//random_sequence1 = { 1,1,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,1,0,0,0,0,1,0,0,1,0,1,1,0 };
		//random_sequence2 = { 1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,0,0,1,1,1 };
		//Gold_cods generation
		Gold_cods.resize(mods_count);
		vector<int> old_shifts;
		for (int i = 0; i < Gold_cods.size(); i++)
			Gold_cods[i].resize(count);
		for (int i = 0; i < Gold_cods.size(); i++)
		{
			int shift = i;
			std::vector<int>::iterator it = std::find(old_shifts.begin(), old_shifts.end(), shift);
			if (it != old_shifts.end())
			{
				i--; continue;
			}
			old_shifts.push_back(shift);
			for (int j = 0; j < count; j++)
				Gold_cods[i][j] = XOR(random_sequence1[j], random_sequence2[(j + shift) % count]);
		}
		//filters generation
		Gold_filters.resize(mods_count);
		for (int i = 0; i < mods_count; i++)
			QAM4_generation(sampling, bitrate, Gold_cods[i], Gold_filters[i]);
		return 0;
	}
	string Golds_convolution(vector<vector<double>>& results,int sampling, int bitrate)
	{
		results.resize(Gold_filters.size());
		for (int i = 0; i < Gold_filters.size(); i++)
		{
			int a = convolution(Gold_filters[i], results[i]);
			if (a == 1) return "";
		}
		stringstream ss;
		for (int i = 0; i < input.size() / 2; i++)
		{
			int index = (i * count / 2) * (sampling / bitrate);
			if (results[0][index] > results[1][index] &&
				results[0][index] > results[2][index] &&
				results[0][index] > results[3][index])
				ss << "11 ";
			if (results[1][index] > results[0][index] &&
				results[1][index] > results[2][index] &&
				results[1][index] > results[3][index])
				ss << "10 ";
			if (results[2][index] > results[1][index] &&
				results[2][index] > results[0][index] &&
				results[2][index] > results[3][index])
				ss << "01 ";
			if (results[3][index] > results[1][index] &&
				results[3][index] > results[2][index] &&
				results[3][index] > results[0][index])
				ss << "00 ";
		}
		string result_str = ss.str();
		return result_str.c_str();
	}

	string get_input_data()
	{
		if (input.empty())return "";

		stringstream ss;
		for (int i = 0; i < input.size(); i += 2)
			ss << input[i] << input[i + 1] << " ";
		string result = ss.str();
		return result.c_str();
	}
	string get_output_data()
	{
		if (output.empty())return "";
		stringstream ss;
		for (auto bit : output)
		{
			ss << bit;
		}
		string result = ss.str();
		return result.c_str();
	}
private:
	int convolution(const vc_double& filter, vector<double>& result)
	{
		if (signal.size() < filter.size()) return 1;
		result.resize(signal.size() - filter.size());
		vector<complex<double>> vc_result;
		vc_result.resize(signal.size() - filter.size());
		for (int i = 0; i < vc_result.size(); i++)
		{
			vc_result[i] = 0;
			for (int j = 0; j < filter.size(); j++)
			{
				vc_result[i] += (signal[i + j] * filter[j]);
			}
		}
		for (int i = 0; i < result.size(); i++)
		{
			result[i] = abs(vc_result[i]);
		}
		return 0;
	}
	//Функция нормировки фазы до +-2M_PI
	void NormalPhaza(double& faza)
	{
		while (1)
			if (faza > 0)
				if (faza > (2 * M_PI))  faza -= 2 * M_PI;
				else break;
			else
				if (faza < (-2 * M_PI)) faza += 2 * M_PI;
				else break;
	}
	void QAM4_generation(int sampling, int bitrate, const vector<bool>& data, vc_double& result)
	{
		int bit_time = sampling / bitrate; //кол-во отчётов на 1 бит
		int local_bits = data.size() / 2;
		int N = bit_time * local_bits; //result size
		vector<int>obraz; obraz.resize(N);
		///////////////////////////////////////////////////
		for (int i = 0; i < data.size(); i += 2)
		{
			if (data[i] == true && data[i + 1] == true)
				for (int j = 0; j < bit_time; j++)
					obraz[j + (i / 2) * bit_time] = 1;
			if (data[i] == true && data[i + 1] == false)
				for (int j = 0; j < bit_time; j++)
					obraz[j + (i / 2) * bit_time] = 7;
			if (data[i] == false && data[i + 1] == true)
				for (int j = 0; j < bit_time; j++)
					obraz[j + (i / 2) * bit_time] = 3;
			if (data[i] == false && data[i + 1] == false)
				for (int j = 0; j < bit_time; j++)
					obraz[j + (i / 2) * bit_time] = 5;
		}
		//result.resize(N);
		result.clear();
		double Buffaza = 0;
		for (int i = 0; i < local_bits; i++)
		{
			Buffaza = obraz[i * bit_time] * (M_PI / 4.);
			for (int j = 0; j < bit_time; j++)
			{
				Buffaza += 2 * M_PI / sampling;
				NormalPhaza(Buffaza);
				result.push_back(cos(Buffaza) + comjd * sin(Buffaza));
			}
		}
		//////////
		//for (int i = 0; i < obraz.size(); i ++)
		//{
		//	if (obraz[i] == bit_buf);
		//	else
		//	{
		//		bit_buf = obraz[i];
		//		Buffaza = bit_buf * (M_PI / 4.);
		//	}
		//	NormalPhaza(Buffaza);
		//	result[i] = cos(Buffaza) + comjd * sin(Buffaza);
		//}
	}
	void addNoize(vector < complex<double> >& mass, double NoizeV)
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
			mass[i] += alfa * shum_c[i] + alfa * comjd * shum_c[i];
		}
	}
	bool XOR(bool a, bool b)
	{
		if (a == true && b == true) return false;
		if (a == false && b == false) return false;
		if (a == false && b == true) return true;
		if (a == true && b == false) return true;
	}
};

