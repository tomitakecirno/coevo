#pragma once
/*
Csv�̓��o�͂Ɋւ��郂�W���[���u����
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
#include "Usual_Methods.hpp"

#define ENEMY 10

class CsvModules {
public:
	CsvModules() {
		std::cout << "Please Input Method's Number !" << std::endl;
		exit(1);
	}
	CsvModules(int method) {
		Csv_Method = method;
		Make_CSV_Directory(method);
	}
	void Init(int method, int Trial, int Gene, int Per, int k = 0);
	void SetCsv_Cr_P(std::vector<int> &Vector_Cruster);
	void SetCsv_Re_P(std::vector<int> &Vector_PopResult);
	void Fwrite_Cr_P();
	void Fwrite_Re_P();
	//void FileWrite_OppResult();
protected:
	int Csv_Method;
	int Csv_Trial;
	int Csv_Gene;
	int Csv_Per;
	int Csv_K;
	int VecLen_Cr_P;
	int VecLen_Re_P;
	std::vector<std::vector<int> > Cr_P;
	std::vector<std::vector<int> > Re_P;
};
//�p�����[�^�[��������
void CsvModules::Init(int method, int Trial, int Gene, int Per, int k) {
	Csv_Method = method;
	Csv_Trial = Trial;
	Csv_Gene = Gene;
	Csv_Per = Per;
	Csv_K = k;
	VecLen_Cr_P = 0;
	VecLen_Re_P = 0;

	Cr_P.resize( (Csv_Gene / Csv_Per) + 1);
	Re_P.resize( (Csv_Gene / Csv_Per) + 1);
	std::cout << "Csv Initialized..." << std::endl;
}
//���㖈�̃N���X�^�ԍ����i�[
void CsvModules::SetCsv_Cr_P(std::vector<int> &Vector_Cruster) {
	int Length = int(Vector_Cruster.size());
	//���T�C�Y
	Cr_P[VecLen_Cr_P] = Vector_Cruster;
	VecLen_Cr_P++;
}
//���㖈�̑ΐ팋�ʂ��i�[
void CsvModules::SetCsv_Re_P(std::vector<int> &Vector_PopResult){
	int Length = int(Vector_PopResult.size());
	Re_P[VecLen_Re_P].resize(Length);
	for (int i = 0; i < VecLen_Re_P; i++) {
		Re_P[VecLen_Re_P][i] = Vector_PopResult[i];
	}
	VecLen_Re_P++;
}
//�N���X�^�[��CSV�t�@�C���֏o��
void CsvModules::Fwrite_Cr_P() {
	//�t�@�C�����ݒ�
	char fname[50];
	if (Csv_Method == 0) {
		sprintf(fname, "./csv/Cruster/%d/Cruster_%d.csv", Csv_Method, Csv_Trial);
	}
	else if (0 < Csv_Method) {
		sprintf(fname, "./csv/Cruster/%d/Cruster_%d_%d.csv", Csv_Method, Csv_K, Csv_Trial);
	}
	//�t�@�C���o�̓X�g���[��
	std::ofstream ofs( fname );

	for (int i = 0; i < VecLen_Cr_P; i++) {
		ofs << Csv_Per*i << ',';
		int Length = int(Cr_P[i].size());
		for (int j = 0; j < Length; j++) {
			ofs << Cr_P[i][j] << ',';
		}
		ofs << std::endl;
	}
}
void CsvModules::Fwrite_Re_P(){
	//�t�@�C�����ݒ�
	char fname[40];
	if (Csv_Method == 0) {
		sprintf(fname, "./csv/PopResult/%d/PopResult_%d.csv", Csv_Method, Csv_Trial);
	}
	else if (0 < Csv_Method) {
		sprintf(fname, "./csv/PopResult/%d/PopResult_%d_%d.csv", Csv_Method, Csv_Trial, Csv_K);
	}
	//�t�@�C���o�̓X�g���[��
	std::ofstream ofs(fname);

	for (int i = 0; i < Csv_Gene / Csv_Per + 1; i++) {
		ofs << Csv_Per*(i + 1) << ',';
		int Length = int(Re_P[i].size());
		for (int j = 0; j < Length; j++) {
			ofs << Re_P[i][j] << ',';
		}
		ofs << std::endl;
	}
}

class CsvModules_Intend {
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
	bool GetContents(std::string filename, std::vector<std::vector<std::string>>& table);
	bool OutContents(std::string filename);
	bool OutContents_Per(std::string filename);
	void Correct(std::vector<std::vector<std::string>>& table, std::vector<std::vector<double>> &input);
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
		std::stringstream Tmp_FileName;
		std::vector<std::vector<std::vector<double>>> Input;

		Tmp_FileName << "./csv/PopResult/" << method[i] << "/";
		Input = std::vector<std::vector<std::vector<double>>>(ENEMY, std::vector<std::vector<double>>(Gene / Per + 1, std::vector<double>(n, 0)));

		for (int Opp = 0; Opp < ENEMY; Opp++) {
			//�t�@�C�����ݒ�
			std::stringstream FileName;
			std::vector<std::vector<std::string>> table;
			FileName << Tmp_FileName.str() << "PopResult_" << trial << "_" << Opp << ".csv";

			//�t�@�C���ǂݍ���
			if (!GetContents(FileName.str(), table)) {
				std::cout << "file open error! " << FileName.str() << std::endl;
			}
			//�K�v�ȕ����������o
			Correct(table, Input[Opp]);
			FileName.str("");
			FileName.clear(std::stringstream::goodbit);
			std::cout << "   Enemy:" << Opp << " done..." << std::endl;
		}

		Init_Output_Per(i);
		Set_Output(i,Input);

		std::stringstream filename_Per;
		filename_Per << "./csv/Result_Per_Enemy_" << method[i] << ".csv";
		OutContents_Per(filename_Per.str());

		std::cout << "method:" << method[i] << " done..." << std::endl;
	}
	/*
	std::stringstream filename;
	filename << "./csv/Result_" << KU << ".csv";
	OutContents(filename.str());
	std::cout << "OutContents ...done" << std::endl;
	*/
}

//�t�@�C���ǂݍ���(2�����x�N�^�[)
bool CsvModules_Intend::GetContents(std::string filename, std::vector<std::vector<std::string>>& table)
{
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
	table.erase(table.end()-1);

	return true;
}

//�t�@�C���o��(2�����x�N�^�[)
bool CsvModules_Intend::OutContents(std::string filename)
{
	// �t�@�C�����J��
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
//���o
void CsvModules_Intend::Correct(std::vector<std::vector<std::string>>& table, std::vector<std::vector<double>> &input) {
	//check

	int Table_Lengh_Y = int(table.size());
	for (int i = 1; i < Table_Lengh_Y; i++) {
		int Table_Lengh_X = int(table[i].size());

		for (int j = 1; j < Table_Lengh_X; j++) {
			input[i - 1][j - 1] = atoi( table[i][j].c_str() );
		}
	}
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
				/*
				try {
					Output_Per[i].at(Opp + 1) = opp_name.str();
				}
				catch(std::out_of_range e){
					std::cout << e.what() << std::endl;
				}
				*/
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
}