#pragma once
/*
Csvの入出力に関するモジュール置き場
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
#include "Usual_Methods.hpp"

#define ENEMY 1

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
	int enemy;
	std::string Dir;
	std::vector<std::vector<std::vector<double>>> Input;
	std::vector<std::vector<std::string>> Out_Ave; // Out_Ave[手法][平均]
	std::vector<std::vector<double>> Out_Max;
	std::vector<std::vector<double>> Out_Min;
	bool GetContents(std::string filename, std::vector<std::vector<std::string>>& table);
	bool OutContents(std::string filename, std::vector<std::vector<std::string>>& table);
	void Correct(std::vector<std::vector<std::string>>& table, std::vector<std::vector<double>> &input);
	void Cal_Ave_Max_Min(std::vector<std::vector<std::vector<double>>> &input, std::vector<std::vector<double>> &Max, std::vector<std::vector<double>> &Min, std::vector<std::vector<double>> &Ave);
	void f_write(std::vector<std::vector<std::vector<double>>> &input);
};

void CsvModules_Intend::Create_Data(int method, int n, int trial, int gene, int per) {
	//世代数と個体の情報を記録するベクター
	Method_Num = method;
	Method_KO = n;
	Method_Trial = trial;
	Gene = gene;
	Per = per;
	enemy = enemy;
	Input = std::vector<std::vector<std::vector<double>>>(ENEMY, std::vector<std::vector<double>>(Gene/Per + 1, std::vector<double>(n, 0)));

	char fname[50];
	std::stringstream Tmp_FileName;
	Tmp_FileName << "./csv/PopResult/" << Method_Num << "/";

	for (int Opp = 0; Opp < ENEMY; Opp++) {
		for (int i = 0; i < Gene / Per + 1; i++) {
			//ファイル名設定
			std::stringstream FileName;
			std::vector<std::vector<std::string>> Tmp_Table;
			FileName << Tmp_FileName.str() << "PopResult_" << trial << "_" << Opp << ".csv";

			//ファイル読み込み
			if ( !GetContents(FileName.str(),Tmp_Table) ) {
				std::cout << "file open error! " << FileName.str() << std::endl;
			}
			/*
			std::cout << "GetContents外:" << std::endl;
			std::cout << "table:" << std::endl;

			for (int i = 0; i < Tmp_Table.size(); i++) {
				std::cout << i << ":[";
				for (int j = 0; j < Tmp_Table[i].size(); j++) {
					std::cout << Tmp_Table[i][j] << ",";
				}
				std::cout << "]" << std::endl;
			}
			*/
			//必要な部分だけ抽出
			Correct(Tmp_Table, Input[Opp]);
			FileName.str("");
			FileName.clear(std::stringstream::goodbit);
		}
	}
	f_write(Input);
}
//ファイル読み込み
bool CsvModules_Intend::GetContents(std::string filename, std::vector<std::vector<std::string>>& table)
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
	//table = std::vector<std::vector<std::string>>(Gene / Per + 2, std::vector<std::string>(KO+1, "0"));
	std::cout << "GetContents内:" << std::endl;
	std::cout << "table:" << std::endl;

	for (int i = 0; i < table.size(); i++) {
		std::cout << i << ":[";
		for (int j = 0; j < table[i].size(); j++) {
			std::cout << table[i][j].c_str() << ",";
		}
		std::cout << "]" << std::endl;
	}
	return true;
}
bool CsvModules_Intend::OutContents(std::string filename, std::vector<std::vector<std::string>>& table)
{
	// ファイルを開く
	std::ofstream ofs(filename);
	if (!ofs.is_open())
	{
		// ファイルが開けなかった場合は終了する
		return false;
	}

	for (const auto &el : table) {
		for (const auto &em : el) {
			ofs << em << ",";
		}
		ofs << std::endl;
	}
	return true;
}
//抽出
void CsvModules_Intend::Correct(std::vector<std::vector<std::string>>& table, std::vector<std::vector<double>> &input) {
	//check

	int Table_Lengh_Y = int(table.size());
	for (int i = 1; i < Table_Lengh_Y; i++) {
		int Table_Lengh_X = int(table[i].size());

		for (int j = 1; j < Table_Lengh_X; j++) {
			input[i - 1][j - 1] = atoi( table[i][j].c_str() );
		}
	}

	std::cout << "Correct内:" << std::endl;
	std::cout << "input:" << std::endl;
	for (int i = 0; i < input.size(); i++) {
		std::cout << i << ":[";
		for (int j = 0; j < input[i].size(); j++) {
			std::cout << input[i][j] << ",";
		}
		std::cout << "]" << std::endl;
	}
}
//最大値, 最小値, 平均取得
void CsvModules_Intend::Cal_Ave_Max_Min(std::vector<std::vector<std::vector<double>>> &input, std::vector<std::vector<double>> &Max, std::vector<std::vector<double>> &Min, std::vector<std::vector<double>> &Ave)
{
	Max = std::vector<std::vector<double>>(ENEMY, std::vector<double>(Gene / Per + 1, 0));
	Min = std::vector<std::vector<double>>(ENEMY, std::vector<double>(Gene / Per + 1, 0));
	Ave = std::vector<std::vector<double>>(ENEMY, std::vector<double>(Gene / Per + 1, 0));

	for (int i = 0; i < ENEMY; i++) {
		for (int j = 0; j < Gene / Per + 1; j++) {
			//最大値のindex
			auto max = *max_element(input[i][j].begin(), input[i][j].end());
			//最小値のindex
			auto min = *min_element(input[i][j].begin(), input[i][j].end());

			Max[i][j] = input[i][j][max];
			Min[i][j] = input[i][j][min];
			Ave[i][j] = accumulate(input[i][j].begin(), input[i][j].end(), 0.0)/KO;
		}
	}
}
//ファイル書き込み
void CsvModules_Intend::f_write(std::vector<std::vector<std::vector<double>>> &input) {
	//inputから最大最小平均を求める
	std::vector<std::vector<double>> Max;
	std::vector<std::vector<double>> Min;
	std::vector<std::vector<double>> Ave;

	Cal_Ave_Max_Min(Input, Max, Min, Ave);

	std::vector<std::vector<double>> tmp_Ave;
	std::vector<double> ave_Ave;
	tmp_Ave = std::vector<std::vector<double>>(Gene/Per+1, std::vector<double>(ENEMY,0));
	ave_Ave = std::vector<double>(Gene / Per + 1, 0);

	for (int i = 0; i < ENEMY; i++) {
		for (int j = 0; j < Gene / Per + 1; j++) {
			tmp_Ave[j][i] = Ave[i][j];
		}
	}
	//対戦相手に対する世代毎の平均
	for (int i = 0; i < Gene / Per + 1; i++) {
		ave_Ave[i] = accumulate(tmp_Ave[i].begin(), tmp_Ave[i].end(), 0.0) / KO;
	}
	//書き込み
	/*
	std::string Out_fname = "./csv/Result.csv";
	std::ofstream ofs(Out_fname);
	OutContents(Out_fname, Out_Ave);
	*/
}
