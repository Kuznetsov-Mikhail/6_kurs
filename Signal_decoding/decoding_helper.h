#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <numeric>
#include <complex>

#define M_PI 3.1415926535
#define comjd complex<double>(0,1)
#define comjf complex<float>(0,1)

typedef struct
{
	int n;
	double fs;
	int br;
	double snr;
} signal_params;
using namespace std;

class decoding_helper
{
public:
	decoding_helper() : _sp(), _modulatedGC1(), _modulatedGC2(), _modulatedGC3(), _modulatedGC4() {}

	decoding_helper(signal_params sp) : _sp(sp), _modulatedGC1(), _modulatedGC2(), _modulatedGC3(), _modulatedGC4() {}

	~decoding_helper() {}
	std::vector<int> _mls1, _mls2;
	void generate(std::vector<complex<double>>& s, CString& initialBitsString)
	{
		// initial state of LFSR1
		std::vector<int> stateLFSR1{ 1, 0, 1, 0, 1 };

		// generate first m-sequence (MLS) - g(x) = x^5 + x^3 + x^2 + x + 1
		std::size_t mlsSize = 31;
		std::vector<int> mls1(mlsSize, 0);

		for (std::size_t i = 0; i < mlsSize; i++)
		{
			int first = (((stateLFSR1[4] ^ stateLFSR1[2]) ^ stateLFSR1[1]) ^ stateLFSR1[0]);
			stateLFSR1.insert(stateLFSR1.begin(), first);

			mls1[i] = stateLFSR1.back();

			stateLFSR1.pop_back();
		}

		stateLFSR1.clear();

		// initial state of LFSR2
		std::vector<int> stateLFSR2{ 0, 1, 0, 1, 0 };

		// generate second m-sequence (MLS) - g(x) = x^5 + x^4 + x^2 + x + 1
		std::vector<int> mls2(mlsSize, 0);

		for (std::size_t i = 0; i < mlsSize; i++)
		{
			int first = (((stateLFSR2[4] ^ stateLFSR2[3]) ^ stateLFSR2[1]) ^ stateLFSR2[0]);
			stateLFSR2.insert(stateLFSR2.begin(), first);

			mls2[i] = stateLFSR2.back();

			stateLFSR2.pop_back();
		}

		stateLFSR2.clear();
	/*	_mls1 = mls1;
		_mls2 = mls2;*/
		/*mls1 = { 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0 };
		mls2 = { 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1 };*/
		// generate third m-sequence (MLS) via cyclic shift of the mls2 to the left by 1 bit
		std::vector<int> mls3(mls2);
		std::rotate(mls3.begin(), mls3.begin() + 1, mls3.end());

		// generate fourth m-sequence (MLS) via cyclic shift of the mls2 to the left by 2 bit
		std::vector<int> mls4(mls2);
		std::rotate(mls4.begin(), mls4.begin() + 2, mls4.end());

		// generate fourth m-sequence (MLS) via cyclic shift of the mls2 to the left by 3 bit
		std::vector<int> mls5(mls2);
		std::rotate(mls5.begin(), mls5.begin() + 3, mls5.end());

		// generate Gold's codes
		std::vector<int> GC1(mlsSize + 1, 0);
		std::vector<int> GC2(mlsSize + 1, 0);
		std::vector<int> GC3(mlsSize + 1, 0);
		std::vector<int> GC4(mlsSize + 1, 0);

		for (int i = 0; i < mlsSize; ++i)
		{
			GC1[i] = mls1[i] ^ mls2[i];
			GC2[i] = mls1[i] ^ mls3[i];
			GC3[i] = mls1[i] ^ mls4[i];
			GC4[i] = mls1[i] ^ mls5[i];
		}

		// 32th bit of Gold codes set to 0
		GC1[mlsSize] = 0;
		GC2[mlsSize] = 0;
		GC3[mlsSize] = 0;
		GC4[mlsSize] = 0;

		mls1.clear();
		mls2.clear();
		mls3.clear();
		mls4.clear();
		mls5.clear();

		// generate initial bits
		std::vector<int> bits(_sp.n, 0);

		initialBitsString.Empty();
		CString bufString{};

		for (int idx = 0; idx < _sp.n; ++idx)
		{
			bits[idx] = rand() % 2;

			bufString.Format(_T("%d   "), bits[idx]);
			initialBitsString += bufString;
		}

		// replace symbol { 00, 01, 10, 11 } from bit sequence to one of the Gold codes
		std::vector<int> sequenceOfGoldCodes(_sp.n / 2 * (mlsSize + 1), 0);

		int k = 0;

		for (int i = 0; i < _sp.n; i += 2)
		{
			// symbol 00 corresponds GC1
			if ((bits[i] == 0) && (bits[i + 1] == 0))
			{
				for (int j = 0; j < GC1.size(); ++j)
				{
					sequenceOfGoldCodes[k * GC1.size() + j] = GC1[j];
				}
			}
			// symbol 01 corresponds GC2
			else if ((bits[i] == 0) && (bits[i + 1] == 1))
			{
				for (int j = 0; j < GC2.size(); ++j)
				{
					sequenceOfGoldCodes[k * GC2.size() + j] = GC2[j];
				}
			}
			// symbol 10 corresponds GC3
			else if ((bits[i] == 1) && (bits[i + 1] == 0))
			{
				for (int j = 0; j < GC3.size(); ++j)
				{
					sequenceOfGoldCodes[k * GC3.size() + j] = GC3[j];
				}
			}
			// symbol 11 corresponds GC4
			else if ((bits[i] == 1) && (bits[i + 1] == 1))
			{
				for (int j = 0; j < GC4.size(); ++j)
				{
					sequenceOfGoldCodes[k * GC4.size() + j] = GC4[j];
				}
			}

			++k;
		}

		bits.clear();

		modulate(sequenceOfGoldCodes, s);
		modulate(GC1, _modulatedGC1);
		modulate(GC2, _modulatedGC2);
		modulate(GC3, _modulatedGC3);
		modulate(GC4, _modulatedGC4);

		sequenceOfGoldCodes.clear();
		GC1.clear();
		GC2.clear();
		GC3.clear();
		GC4.clear();
	}

	void ccf(const std::vector<complex<double>>& s,
		std::vector<double>& ccfWithGC1,
		std::vector<double>& ccfWithGC2,
		std::vector<double>& ccfWithGC3,
		std::vector<double>& ccfWithGC4,
		CString& restoredBitsString)
	{
		ccfWithGC1.clear();
		ccfWithGC2.clear();
		ccfWithGC3.clear();
		ccfWithGC4.clear();

		double valRe1 = 0.0, valIm1 = 0.0;
		double valRe2 = 0.0, valIm2 = 0.0;
		double valRe3 = 0.0, valIm3 = 0.0;
		double valRe4 = 0.0, valIm4 = 0.0;

		for (std::size_t i = 0; i < s.size(); ++i)
		{
			valRe1 = 0.0;
			valIm1 = 0.0;

			valRe2 = 0.0;
			valIm2 = 0.0;

			valRe3 = 0.0;
			valIm3 = 0.0;

			valRe4 = 0.0;
			valIm4 = 0.0;

			for (std::size_t j = 0; j < _modulatedGC1.size(); ++j)
			{
				std::size_t idx = ((j + i) >= s.size()) ? (j + i) - s.size() : (j + i);

				valRe1 += (_modulatedGC1[j].real() * s[idx].real() + _modulatedGC1[j].imag() * s[idx].imag());
				valIm1 += (_modulatedGC1[j].real() * (-s[idx].imag()) + _modulatedGC1[j].imag() * s[idx].real());

				valRe2 += (_modulatedGC2[j].real() * s[idx].real() + _modulatedGC2[j].imag() * s[idx].imag());
				valIm2 += (_modulatedGC2[j].real() * (-s[idx].imag()) + _modulatedGC2[j].imag() * s[idx].real());

				valRe3 += (_modulatedGC3[j].real() * s[idx].real() + _modulatedGC3[j].imag() * s[idx].imag());
				valIm3 += (_modulatedGC3[j].real() * (-s[idx].imag()) + _modulatedGC3[j].imag() * s[idx].real());

				valRe4 += (_modulatedGC4[j].real() * s[idx].real() + _modulatedGC4[j].imag() * s[idx].imag());
				valIm4 += (_modulatedGC4[j].real() * (-s[idx].imag()) + _modulatedGC4[j].imag() * s[idx].real());
			}

			ccfWithGC1.push_back(1.0 / _modulatedGC1.size() * sqrt(valRe1 * valRe1 + valIm1 * valIm1));
			ccfWithGC2.push_back(1.0 / _modulatedGC2.size() * sqrt(valRe2 * valRe2 + valIm2 * valIm2));
			ccfWithGC3.push_back(1.0 / _modulatedGC3.size() * sqrt(valRe3 * valRe3 + valIm3 * valIm3));
			ccfWithGC4.push_back(1.0 / _modulatedGC4.size() * sqrt(valRe4 * valRe4 + valIm4 * valIm4));
		}

		std::vector<double>::iterator max1 = std::max_element(ccfWithGC1.begin(), ccfWithGC1.end());
		std::vector<double>::iterator max2 = std::max_element(ccfWithGC2.begin(), ccfWithGC2.end());
		std::vector<double>::iterator max3 = std::max_element(ccfWithGC3.begin(), ccfWithGC3.end());
		std::vector<double>::iterator max4 = std::max_element(ccfWithGC4.begin(), ccfWithGC4.end());

		std::for_each(ccfWithGC1.begin(), ccfWithGC1.end(), [&max1](double& x) { x /= *max1; });
		std::for_each(ccfWithGC2.begin(), ccfWithGC2.end(), [&max2](double& x) { x /= *max2; });
		std::for_each(ccfWithGC3.begin(), ccfWithGC3.end(), [&max3](double& x) { x /= *max3; });
		std::for_each(ccfWithGC4.begin(), ccfWithGC4.end(), [&max4](double& x) { x /= *max4; });

		restoredBitsString.Empty();

		// duration of one symbol
		double T = 1.0 / (_sp.br / 2.0);
		// points inside one symbol
		int pointsPerSymbol = (int)(T * _sp.fs);

		const int sizeOfMaximumsArray = 4;
		double maximums[sizeOfMaximumsArray] = { 0.0, 0.0, 0.0, 0.0 };

		for (std::size_t i = 0; i < ccfWithGC1.size(); i += _modulatedGC1.size())
		{
			memset(maximums, 0, sizeOfMaximumsArray * sizeof(double));

			for (std::size_t j = 0; j < _modulatedGC1.size(); ++j)
			{
				std::size_t idx = ((i + j - pointsPerSymbol) < 0) ? ((i + j - pointsPerSymbol) + ccfWithGC1.size()) : (i + j - pointsPerSymbol);

				maximums[0] = maximums[0] < ccfWithGC1[idx] ? ccfWithGC1[idx] : maximums[0];
				maximums[1] = maximums[1] < ccfWithGC2[idx] ? ccfWithGC2[idx] : maximums[1];
				maximums[2] = maximums[2] < ccfWithGC3[idx] ? ccfWithGC3[idx] : maximums[2];
				maximums[3] = maximums[3] < ccfWithGC4[idx] ? ccfWithGC4[idx] : maximums[3];
			}

			int symbolIndex = 0;
			double globalMax = maximums[0];

			for (int k = 1; k < sizeOfMaximumsArray; ++k)
			{
				if (globalMax < maximums[k])
				{
					globalMax = maximums[k];
					symbolIndex = k;
				}
			}

			CString bufString{ "" };

			switch (symbolIndex)
			{
			case 0:
				bufString.Format(_T("%d   "), 0);
				restoredBitsString += bufString;
				bufString.Format(_T("%d   "), 0);
				restoredBitsString += bufString;
				break;
			case 1:
				bufString.Format(_T("%d   "), 0);
				restoredBitsString += bufString;
				bufString.Format(_T("%d   "), 1);
				restoredBitsString += bufString;
				break;
			case 2:
				bufString.Format(_T("%d   "), 1);
				restoredBitsString += bufString;
				bufString.Format(_T("%d   "), 0);
				restoredBitsString += bufString;
				break;
			case 3:
				bufString.Format(_T("%d   "), 1);
				restoredBitsString += bufString;
				bufString.Format(_T("%d   "), 1);
				restoredBitsString += bufString;
				break;
			default:
				break;
			}
		}
	}

private:
	void noise(double snr, std::vector<complex<double>>& s)
	{
		std::size_t size = s.size();

		double sum_kv_n = 0.0;
		int M = 0;
		double* arr_n = new double[size];
		for (std::size_t i = 0; i < size; ++i)
		{
			M = rand() % 9 + 12;
			double sum_Qsi = 0.0;
			for (int j = 0; j < M; ++j)
			{
				sum_Qsi += (rand() % 21 - 10) / 10.0;
			}
			arr_n[i] = sum_Qsi / M;
			sum_kv_n += arr_n[i] * arr_n[i];
		};

		double sum_kv_Sig_re = 0.0;
		double sum_kv_Sig_im = 0.0;
		for (std::size_t t = 0; t < size; ++t)
		{
			sum_kv_Sig_re += s[t].real() * s[t].real();
			sum_kv_Sig_im += s[t].imag() * s[t].imag();
		};

		double _alpha_re = 0.0;
		double _alpha_im = 0.0;
		_alpha_re = sqrt(pow(10, -snr / 10) * sum_kv_Sig_re / (sum_kv_n));
		_alpha_im = sqrt(pow(10, -snr / 10) * sum_kv_Sig_im / (sum_kv_n));

		for (std::size_t i = 0; i < size; ++i)
			s[i] = (s[i].real() + _alpha_re * arr_n[i]) + comjd * (s[i].imag() + _alpha_im * arr_n[i]);
		delete[] arr_n;
	}

	void modulate(const std::vector<int>& input,
		std::vector<complex<double>>& output)
	{
		// duration of one symbol
		double T = 1.0 / (_sp.br / 2.0);
		// points inside one symbol
		int pointsPerSymbol = (int)(T * _sp.fs);

		// buffer array
		std::vector<complex<double>> bufOutput(input.size() * pointsPerSymbol, { 0.0, 0.0 });

		// phase of signal
		double phase = 0.0;

		for (int i = 0; i < input.size(); i += 2)
		{
			for (int j = 0; j < pointsPerSymbol; ++j)
			{
				phase = (pointsPerSymbol * i + j) / _sp.fs;

				phase = (phase > 2.0 * M_PI) ? phase - 2 * M_PI : phase;
				phase = (phase < 2.0 * M_PI) ? phase + 2 * M_PI : phase;

				// symbol 00 corresponds 5 * M_PI / 4 value
				if ((input[i] == 0) && (input[i + 1] == 0))
				{
					bufOutput[pointsPerSymbol * i + j] = (cos(5.0 * M_PI / 4.0) *
						cos(phase) - sin(5.0 * M_PI / 4.0) * sin(phase)) + \
						comjd * ((cos(5.0 * M_PI / 4.0) *
							sin(phase) + sin(5.0 * M_PI / 4.0) * cos(phase)));
				}
				// symbol 01 corresponds 3 * M_PI / 4 value
				else if ((input[i] == 0) && (input[i + 1] == 1))
				{
					bufOutput[pointsPerSymbol * i + j] = (cos(3.0 * M_PI / 4.0) *
						cos(phase) - sin(3.0 * M_PI / 4.0) * sin(phase)) + \
						comjd * ((cos(3.0 * M_PI / 4.0) *
							sin(phase) + sin(3.0 * M_PI / 4.0) * cos(phase)));
				}
				// symbol 10 corresponds 7 * M_PI / 4 value
				else if ((input[i] == 1) && (input[i + 1] == 0))
				{
					bufOutput[pointsPerSymbol * i + j] = (cos(7.0 * M_PI / 4.0) *
						cos(phase) - sin(7.0 * M_PI / 4.0) * sin(phase)) + \
						comjd * ((cos(7.0 * M_PI / 4.0) *
							sin(phase) + sin(7.0 * M_PI / 4.0) * cos(phase)));
				}
				// symbol 11 corresponds M_PI / 4 value
				else if ((input[i] == 1) && (input[i + 1] == 1))
				{
					bufOutput[pointsPerSymbol * i + j] = (cos(1.0 * M_PI / 4.0) *
						cos(phase) - sin(1.0 * M_PI / 4.0) * sin(phase)) + \
						comjd * ((cos(1.0 * M_PI / 4.0) *
							sin(phase) + sin(1.0 * M_PI / 4.0) * cos(phase)));
				}
			}
		}

		// impose noise on signal
		noise(_sp.snr, bufOutput);

		output.resize(bufOutput.size(), { 0.0, 0.0 });

		// copy from buffer to destination
		for (int i = 0; i < output.size(); ++i)
		{
			output[i] = bufOutput[i];
		}
		bufOutput.clear();
	}
public:
	signal_params _sp;
private:
	std::vector<complex<double>> _modulatedGC1;
	std::vector<complex<double>> _modulatedGC2;
	std::vector<complex<double>> _modulatedGC3;
	std::vector<complex<double>> _modulatedGC4;
};

