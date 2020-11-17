#pragma once
#include <vector>
#include <ctime>
#include <complex>
#include <algorithm>
using namespace std;
using vc_double = vector<complex<double>>;
class decoding_helper
{
public:
	decoding_helper() {}
	~decoding_helper() {}
private:
	vector<bool> input; //input bits
	vector<bool> output; //output bits
	vector<bool> random_sequence;
	vector<vector<bool>> Gold_cods;
	vc_double signal;
	int count = 31;
	int mods_count = 4;
public:
	int Init(int bits_count)
	{
		if (bits_count % 2 == 1) return 1;
		srand(time(0));
		//input data generation
		input.resize(bits_count);
		for (int i = 0; i < input.size(); i++)
			if (rand() > RAND_MAX / 2) input[i] = true;
			else input[i] = false;
		//random_sequence generation
		random_sequence.resize(count);
		for (int i = 0; i < count; i++)
			if (rand() > RAND_MAX / 2) random_sequence[i] = true;
			else random_sequence[i] = false;
		//Gold_cods generation
		Gold_cods.resize(mods_count);
		vector<int> old_shifts;
		for (int i = 0; i < Gold_cods.size(); i++)
			Gold_cods[i].resize(count);
		for (int i = 0; i < Gold_cods.size(); i++)
		{
			int shift = 0 + rand() % count;
			std::vector<int>::iterator it = std::find(old_shifts.begin(), old_shifts.end(), shift);
			if (it != old_shifts.end())
			{
				i--; continue;
			}
			old_shifts.push_back(shift);
			for (int j = 0; j < count; j++)
				Gold_cods[i][j] = random_sequence[j] || random_sequence[(j + shift) % count];
		}
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
};

