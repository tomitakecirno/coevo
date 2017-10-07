#pragma once
/*
Csvの入出力に関するモジュール置き場
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
#include "Usual_Methods.hpp"

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
//パラメーターを初期化
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
//世代毎のクラスタ番号を格納
void CsvModules::SetCsv_Cr_P(std::vector<int> &Vector_Cruster) {
	int Length = int(Vector_Cruster.size());
	//リサイズ
	Cr_P[VecLen_Cr_P] = Vector_Cruster;
	VecLen_Cr_P++;
}
//世代毎の対戦結果を格納
void CsvModules::SetCsv_Re_P(std::vector<int> &Vector_PopResult){
	int Length = int(Vector_PopResult.size());
	Re_P[VecLen_Re_P].resize(Length);
	for (int i = 0; i < VecLen_Re_P; i++) {
		Re_P[VecLen_Re_P][i] = Vector_PopResult[i];
	}
	VecLen_Re_P++;
}
//クラスターをCSVファイルへ出力
void CsvModules::Fwrite_Cr_P() {
	//ファイル名設定
	char fname[50];
	if (Csv_Method == 0) {
		sprintf(fname, "./csv/Cruster/%d/Cruster_%d.csv", Csv_Method, Csv_Trial);
	}
	else if (0 < Csv_Method) {
		sprintf(fname, "./csv/Cruster/%d/Cruster_%d_%d.csv", Csv_Method, Csv_K, Csv_Trial);
	}
	//ファイル出力ストリーム
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
	//ファイル名設定
	char fname[40];
	if (Csv_Method == 0) {
		sprintf(fname, "./csv/PopResult/%d/PopResult_%d.csv", Csv_Method, Csv_Trial);
	}
	else if (0 < Csv_Method) {
		sprintf(fname, "./csv/PopResult/%d/PopResult_%d_%d.csv", Csv_Method, Csv_Trial, Csv_K);
	}
	//ファイル出力ストリーム
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
	void Create_Data(int method, int n, int trial, int gene, int per);
protected:
	int Method_Num;
	int Gene;
	int Method_KO;
	int Method_Trial;
	int Per;
	std::string Dir;
	std::vector<std::vector<std::vector<double>>> Input;
	std::vector<std::vector<double>> Out_Ave;
	std::vector<std::vector<double>> Out_Max;
	std::vector<std::vector<double>> Out_Min;
	bool GetContents(std::string filename, std::vector<std::vector<double>>& table);
	//void Correct(std::vector<std::vector<double>>& table, std::vector<std::vector<double>> &input);
};

void CsvModules_Intend::Create_Data(int method, int n, int trial, int gene, int per) {
	//世代数と個体の情報を記録するベクター
	Method_Num = method;
	Method_KO = n;
	Method_Trial = trial;
	Gene = gene;
	Per = per;
	Input = std::vector<std::vector<std::vector<double>>>(10, std::vector<std::vector<double>>(Gene/Per + 1, std::vector<double>(n, 0)));

	char fname[50];
	std::stringstream Tmp_FileName;
	Tmp_FileName << "./csv/PopResult/" << Method_Num << "/";
	for (int Opp = 0; Opp < 1; Opp++) {
		for (int i = 0; i < Gene / Per + 1; i++) {
			std::stringstream FileName;
			std::vector<std::vector<double>> Tmp_Table;

			//ファイル名設定
			FileName << Tmp_FileName.str() << "PopResult_" << trial << "_" << Opp << ".csv";
			if ( !GetContents(FileName.str(),Tmp_Table) ) {
				std::cout << "file open error! " << FileName.str() << std::endl;
			}

			//Correct(Tmp_Table, Input[Opp]);
			FileName.str("");
			FileName.clear(std::stringstream::goodbit);

		}
	}
}

bool CsvModules_Intend::GetContents(std::string filename, std::vector<std::vector<double>>& table)
{
	// ファイルを開く
	std::ifstream ifs(filename);
	if (!ifs.is_open())
	{
		// ファイルが開けなかった場合は終了する
		return false;
	}

	char delimiter = ',';
	while (!ifs.eof()) {
		// １行読み込む
		std::string buffer;
		ifs >> buffer;

		std::vector<double> record;
		std::stringstream streambuffer(buffer);
		std::string token;
		while (getline(streambuffer, token, delimiter))
		{
			record.push_back(atoi(token.c_str()));
		}
		table.push_back(record);
	}
	for (int i = 0; i < table.size(); i++) {
		std::cout << i << ":[";
		for (int j = 0; j < table[i].size(); j++) {
			std::cout << table[i][j] << ",";
		}
		std::cout << "]" << std::endl;
	}
	return true;
}
/*
void CsvModules_Intend::Correct(std::vector<std::vector<double>>& table, std::vector<std::vector<double>> &input) {
	//check

	int Table_Lengh_Y = int(table.size());
	int input_Lengh_Y = int(input.size());
	assert(Table_Lengh_Y == Gene/Per + 1);
	assert(input_Lengh_Y == Gene/Per);
	for (int i = 0; i < Table_Lengh_Y; i++) {
		int Table_Lengh_X = int(table[i].size());
		int input_Lengh_X = int(input[i].size());

		assert(Table_Lengh_X == KO + 1);
		assert(input_Lengh_X == KO);
		for (int j = 1; j < Table_Lengh_X; j++) {
			input[i - 2][j - 1] = atoi( table[i][j].c_str() );
		}
	}
}
*/