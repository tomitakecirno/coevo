#pragma once
/*
Csv�̓��o�͂Ɋւ��郂�W���[���u����
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
#include "config_nim.hpp"
#include "player_nim.h"
#include "../header/Usual_Methods.hpp"

#define ENEMY 10

class CsvModules {
public:
	//�t�@�C���ǂݍ���
	template<class Vec_out>
	bool GetContents(std::string filename, Vec_out &input);
	//�t�@�C����������
	template<class Vec_in>
	bool csv_fwrite(std::string fname, Vec_in &vector, int per=1);
};
template<class Vec_out>
bool CsvModules::GetContents(std::string filename, Vec_out &input)
{
	std::vector<std::vector<std::string>> table;
	// �t�@�C�����J��
	std::ifstream ifs(filename);
	if (!ifs.is_open())
	{
		// �t�@�C�����J���Ȃ������ꍇ�͏I������
		return false;
	}

	char delimiter = ',';
	while (!ifs.eof()) {
		// �P�s�ǂݍ���
		std::string buffer;
		ifs >> buffer;

		std::vector<std::string> record;
		std::stringstream streambuffer(buffer);
		std::string token;
		while (getline(streambuffer, token, delimiter))
		{
			record.push_back(token);
		}
		table.push_back(record);
	}
	table.erase(table.end() - 1);
	
	int table_len_Y = int(table.size());
	input.resize(table_len_Y - 1);
	for (int i = 1; i < table_len_Y; i++) {
		int table_len_X = int(table[i].size());
		input[i-1].resize(table_len_X - 1);
		for (int j = 1; j < table_len_X; j++) {
			input[i - 1][j - 1] = atoi(table[i][j].c_str());
		}
	}
	return true;
}

template<class Vec_in>
bool CsvModules::csv_fwrite(std::string fname, Vec_in &vector, int per) {
	int vector_size = int(vector.size());

	int max_size = 0;
	for (int i = 0; i < vector_size; i++) {
		int vector_size2 = int(vector[i].size());
		if (max_size < vector_size2) {
			max_size = vector_size2;
		}
	}
	std::ofstream ofs(fname);
	if (!ofs.is_open())
	{
		// �t�@�C�����J���Ȃ������ꍇ�͏I������
		return false;
	}
	//1�i��
	ofs << " " << ',';
	for (int i = 0; i < max_size; i++) {
		ofs << i + 1 << ',';
	}
	ofs << std::endl;

	//2�i�ڈȍ~
	for (int i = 0; i < vector_size; i++) {
		ofs << per*i << ',';
		for (const auto &em : vector[i]) {
			ofs << em << ',';
		}
		ofs << std::endl;
	}
	std::cout << "file:" << fname << "...done" << std::endl;
	return true;
}

/*
	�����p��csv
*/
/*
class Csv_exp : public CsvModules {
public:
	Csv_exp(std::string str, int method, int Trial, int Gene, int Per, int k = 0) {
		Dir = str;
		Csv_Method = method;
		Csv_Trial = Trial;
		Csv_Gene = Gene;
		Csv_Per = Per;
		Csv_K = k;
		Cr_P.resize(Gene / Per + 1);
		Re_P.resize(Gene / Per + 1);

		Make_CSV_Directory(method);
		std::cout << "Csv Initialized..." << std::endl;
	}
	//�N���X�^�[in
	void SetVec_Cr_Pop(std::vector<playerNim> &Pop, int index);
	//�ΐ팋��
	void SetVec_Re_Pop(std::vector<playerNim> &Pop, int index);
	void fwrite_Cr_P();
protected:
	int Csv_Method;
	int Csv_Trial;
	int Csv_Gene;
	int Csv_Per;
	int Csv_K;
	int Csv_Parent;
	int Csv_Child;
	std::string Dir;
	std::vector<std::vector<int>> Cr_P;
	std::vector<std::vector<int>> Re_P;
};
void Csv_exp::SetVec_Cr_Pop(std::vector<playerNim> &Pop, int index) {
	Cr_P[index].resize(KO);
	for (int i = 0; i < KO; i++) {
		Cr_P[index][i] = Pop[i].get_nitch();
	}
}
void Csv_exp::SetVec_Re_Pop(std::vector<playerNim> &Pop, int index)
{
	Re_P[index].resize(KO);
	for (int i = 0; i < KO; i++) {
		Re_P[index][i] = Pop[i].get_eval();
	}
}
void Csv_exp::fwrite_Cr_P() {
	int Cr_P_size = int(Cr_P.size());
	int Cr_P_size2 = int(Cr_P[0].size());
	std::vector<std::vector<int>> tmp_Cr_P(Cr_P_size);

	//�N���X�^���Ń��T�C�Y
	for (int i = 0; i < Cr_P_size; i++) {
		auto max = *max_element(Cr_P[i].begin(), Cr_P[i].end());
		tmp_Cr_P[i].assign(max + 1, 0);
	}
	//�N���X�^�̌̐����J�E���g��C�x�N�^�[��0�̗v�f���폜
	for (int i = 0; i < Cr_P_size; i++) {
		for (int j = 0; j < Cr_P_size2; j++) {
			tmp_Cr_P[i][Cr_P[i][j]]++;
		}
		tmp_Cr_P[i].erase(std::remove(tmp_Cr_P[i].begin(), tmp_Cr_P[i].end(), 0), tmp_Cr_P[i].end());
	}

	//�t�@�C�����ݒ�
	char fname[50];
	char fname2[50];
	if (Csv_K == 0) {
		//�S�Ă̌̂̃N���X�^�ԍ����L�^
		sprintf(fname, "./csv/%d/Cruster_%d_%d_%d.csv", Csv_Method, Csv_Method, Csv_Trial, Csv_Gene);
		//�N���X�^���̌̐����L�^
		sprintf(fname2, "./csv/%d/Cruster2_%d_%d_%d.csv", Csv_Method, Csv_Method, Csv_Trial, Csv_Gene);
	}
	else if (0 < Csv_K) {
		sprintf(fname, "./csv/%d/Cruster_%d_%d_%d_%d.csv", Csv_Method, Csv_Method, Csv_Trial, Csv_Gene, Csv_K);
		sprintf(fname2, "./csv/%d/Cruster2_%d_%d_%d_%d.csv", Csv_Method, Csv_Method, Csv_Trial, Csv_Gene, Csv_K);
	}
	//�t�@�C���o��
	csv_fwrite(std::string(fname), Cr_P);
	csv_fwrite(std::string(fname2), tmp_Cr_P);
}

//csv�𓝍�����N���X
class CsvModules_Intend : public CsvModules{
public:
	void Create_Data(std::vector<int> &me, int n, int trial, int gene, int per);
protected:
	int method_size;
	int Gene;
	int Method_Trial;
	int Per;
	std::vector<int> method;
	std::vector<std::vector<std::string>> Output; // Out_Ave[����][��@][ave/max/min]
	std::vector<std::vector<std::string>> Output_Per; // Out_Ave[����][��@][ave/max/min]

	void Init_Output();
	void Init_Output_Per(int me);
	void Set_Output(int me, std::vector<std::vector<std::vector<double>>> &input);
	bool OutContents(std::string filename);
	bool OutContents_Per(std::string filename);
	void Cal_Ave_Max_Min(std::vector<std::vector<std::vector<double>>> &input, std::vector<std::vector<double>> &Max, std::vector<std::vector<double>> &Min, std::vector<std::vector<double>> &Ave);
	void cal_average(std::vector<std::vector<double>> &input, std::vector<double> &output);
};
void CsvModules_Intend::Init_Output() {

	Output = std::vector<std::vector<std::string>>((Gene / Per + 2) * 3, std::vector<std::string>(method_size+1, " "));
	
	for (int i = 0; i < (Gene / Per + 2) * 3; i++) {
		if (i % (Gene / Per + 2) == 0) {
			switch(i / (Gene / Per + 2)){
			case 0:
				Output[i][0] = "Average";
				break;
			case 1:
				Output[i][0] = "Max";
				break;
			case 2:
				Output[i][0] = "Min";
				break;
			default:
				std:: cerr << "Error";
				break;
			}
		}
		else {
			int tmp = i % (Gene / Per + 2) - 1;
			Output[i][0] = std::to_string(tmp*Per);
		}
	}
}
void CsvModules_Intend::Init_Output_Per(int me) {
	Output_Per = std::vector<std::vector<std::string>>((Gene / Per + 2) * 3, std::vector<std::string>(ENEMY+1, " "));
	std::stringstream method_name;
	method_name << "Method:" << method[me];
	for (int i = 0; i < (Gene / Per + 2) * 3; i++) {
		if (i % (Gene / Per + 2) == 0) {
			Output_Per[i][0] = method_name.str();
		}
		else {
			int tmp = i % (Gene / Per + 2) - 1;
			Output_Per[i][0] = std::to_string(tmp*Per);
		}
	}
}
void CsvModules_Intend::Create_Data(std::vector<int> &me, int n, int trial, int gene, int per) {
	//���㐔�ƌ̂̏����L�^����x�N�^�[
	method_size = int(me.size());
	method = me;
	Method_Trial = trial;
	Gene = gene;
	Per = per;

	Init_Output();
	for (int i = 0; i < method_size; i++) {
		std::cout << "method_size:" << method_size << std::endl;
		std::cout << "method:" << method[i] << " start..." << std::endl;
		std::stringstream tmp_fname;
		std::vector<std::vector<std::vector<double>>> Input;

		tmp_fname << "./csv/" << method[i] << "/";
		Input = std::vector<std::vector<std::vector<double>>>(ENEMY, std::vector<std::vector<double>>(Gene / Per + 1, std::vector<double>(n, 0)));
		for (int Opp = 0; Opp < ENEMY; Opp++) {
			//�t�@�C�����ݒ�
			std::stringstream fname;
			std::vector<std::vector<std::string>> table;
			fname << tmp_fname.str() << "PopResult_" << trial << "_" << Opp << ".csv";
			//�t�@�C���ǂݍ���
			if (!GetContents(fname.str(), Input[Opp])) {
				std::cout << "file input error! " << std::endl;
			}
			fname.str("");
			fname.clear(std::stringstream::goodbit);
			std::cout << "   Enemy:" << Opp << " done..." << std::endl;
		}

		Init_Output_Per(i);
		Set_Output(i,Input);

		std::stringstream filename_Per;
		filename_Per << "./csv/Result_Per_Enemy_" << method[i] << ".csv";
		OutContents_Per(filename_Per.str());

		std::cout << "method:" << method[i] << " done..." << std::endl;
	}
}
//�t�@�C���ǂݍ���(2�����x�N�^�[)
//�t�@�C���o��(2�����x�N�^�[)
bool CsvModules_Intend::OutContents(std::string filename)
{
	// �t�@�C�����J
	std::ofstream ofs(filename);
	if (!ofs.is_open())
	{
		// �t�@�C�����J���Ȃ������ꍇ�͏I������
		return false;
	}

	for (const auto &el : Output) {
		for (const auto &em : el) {
			ofs << em << ",";
		}
		ofs << std::endl;
	}
	std::cout << "filename:" << filename << "...done" << std::endl;
	return true;
}

bool CsvModules_Intend::OutContents_Per(std::string filename)
{
	// �t�@�C�����J��
	std::ofstream ofs(filename);
	if (!ofs.is_open())
	{
		// �t�@�C�����J���Ȃ������ꍇ�͏I������
		return false;
	}

	for (const auto &el : Output_Per) {
		for (const auto &em : el) {
			ofs << em << ",";
		}
		ofs << std::endl;
	}
	std::cout << "filename:" << filename << "...done" << std::endl;
	return true;
}

//�ő�l, �ŏ��l, ���ώ擾
void CsvModules_Intend::Cal_Ave_Max_Min(std::vector<std::vector<std::vector<double>>> &input, std::vector<std::vector<double>> &Max, std::vector<std::vector<double>> &Min, std::vector<std::vector<double>> &Ave)
{
	Max = std::vector<std::vector<double>>(ENEMY, std::vector<double>(Gene / Per + 1, 0));
	Min = std::vector<std::vector<double>>(ENEMY, std::vector<double>(Gene / Per + 1, 0));
	Ave = std::vector<std::vector<double>>(ENEMY, std::vector<double>(Gene / Per + 1, 0));

	for (int i = 0; i < ENEMY; i++) {
		for (int j = 0; j < Gene / Per + 1; j++) {
			//�ő�l
			auto max = *max_element(input[i][j].begin(), input[i][j].end());
			//�ŏ��l
			auto min = *min_element(input[i][j].begin(), input[i][j].end());

			Max[i][j] = input[i][j][int(max)];
			Min[i][j] = input[i][j][int(min)];
			Ave[i][j] = accumulate(input[i][j].begin(), input[i][j].end(), 0.0)/KO;
		}
	}
}

//�������ݗp�̃x�N�^�[�Ƀf�[�^���Z�b�g
void CsvModules_Intend::Set_Output(int me, std::vector<std::vector<std::vector<double>>> &input) {
	//input����ő�ŏ����ς����߂�
	std::vector<std::vector<double>> Max;
	std::vector<std::vector<double>> Min;
	std::vector<std::vector<double>> Ave;

	//���薈�Ƀf�[�^���܂Ƃ߂�
	Cal_Ave_Max_Min(input, Max, Min, Ave);
	std::cout << "   Cal_Ave_Max_Min ...done" << std::endl;
	//�܂Ƃ߂��f�[�^�̕��ς��Ƃ�
	std::vector<double> ave_Ave;
	std::vector<double> ave_Max;
	std::vector<double> ave_Min;
	cal_average(Ave, ave_Ave);
	cal_average(Max, ave_Max);
	cal_average(Min, ave_Min);
	std::cout << "   cal_average ...done" << std::endl;

	//�S�Ă̕��ς��o��
	std::stringstream method_name;
	method_name << "method" << method[me];
	for (int i = 0; i < Gene / Per + 2; i++) {
		if (i == 0) {
			Output[i][me + 1] = method_name.str();
			Output[i + (Gene / Per + 2)][me + 1] = method_name.str();
			Output[i + (Gene / Per + 2)*2][me + 1] = method_name.str();
		}
		else {
			Output[i][me + 1] = std::to_string(ave_Ave[i - 1]);
			Output[i + (Gene / Per + 2)][me + 1] = std::to_string(ave_Max[i - 1]);
			Output[i + (Gene / Per + 2) * 2][me + 1] = std::to_string(ave_Min[i - 1]);
		}
	}

	//�G���̐��l���o��
	for (int Opp = 0; Opp < ENEMY; Opp++) {
		std::stringstream opp_name;
		opp_name << "Opp:" << Opp;
		for (int i = 0; i < Gene / Per + 2; i++) {
			if (i == 0) {
				try {
					Output_Per[i].at(Opp + 1) = opp_name.str();
				}
				catch(std::out_of_range e){
					std::cout << e.what() << std::endl;
				}
				Output_Per[i][Opp + 1] = opp_name.str();
				Output_Per[i + (Gene / Per + 2)][Opp + 1] = opp_name.str();
				Output_Per[i + (Gene / Per + 2) * 2][Opp + 1] = opp_name.str();
			}
			else {
				Output_Per[i][Opp + 1] = std::to_string(Ave[Opp][i - 1]);
				Output_Per[i + (Gene / Per + 2)][Opp + 1] = std::to_string(Max[Opp][i - 1]);
				Output_Per[i + (Gene / Per + 2) * 2][Opp + 1] = std::to_string(Min[Opp][i - 1]);
			}
		}
	}
	std::cout << "   Set_Output ...done" << std::endl;
}

//�e��f�[�^�𐮂��ĕ��ς����߂�
void CsvModules_Intend::cal_average(std::vector<std::vector<double>> &input, std::vector<double> &output) {
	std::vector<std::vector<double>> tmp_input;

	output = std::vector<double>(Gene / Per + 1, 0);
	tmp_input = std::vector<std::vector<double>>(Gene / Per + 1, std::vector<double>(ENEMY, 0));

	for (int i = 0; i < ENEMY; i++) {
		for (int j = 0; j < Gene / Per + 1; j++) {
			tmp_input[j][i] = input[i][j];
		}
	}
	//�ΐ푊��ɑ΂��鐢�㖈�̕���
	for (int i = 0; i < Gene / Per + 1; i++) {
		output[i] = accumulate(tmp_input[i].begin(), tmp_input[i].end(), 0.0) / ENEMY;
	}
}*/