#pragma once
/*
Csvの入出力に関するモジュール置き場
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
#include "Usual_Methods.hpp"
#include "config.hpp"

#define ENEMY 10

class CsvModules {
public:
	CsvModules() {
		std::cout << "Please Input Method's Number !" << std::endl;
		exit(1);
	}
	CsvModules(std::string str, int method, int Trial, int Gene, int Per, int k = 0) {
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
	void Stra_Output_Pop(std::vector<playerTK> &Pop, int index);
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
	std::string Dir;
	std::vector<std::vector<int>> Cr_P;
	std::vector<std::vector<int>> Re_P;
};
//世代毎のクラスタ番号を格納
void CsvModules::Stra_Output_Pop(std::vector<playerTK> &Pop,int index) {
	int pop_size = int(Pop.size());
	std::vector<std::vector<std::vector<double>>> w1_Main;
	std::vector<std::vector<std::vector<double>>> w2_Main;
	std::vector<std::vector<std::vector<double>>> w3_Main;

	w1_Main = std::vector<std::vector<std::vector<double>>>(pop_size, std::vector<std::vector<double>>(I1, std::vector<double>(J1, 0)));
	w2_Main = std::vector<std::vector<std::vector<double>>>(pop_size, std::vector<std::vector<double>>(I2, std::vector<double>(J2, 0)));
	w3_Main = std::vector<std::vector<std::vector<double>>>(pop_size, std::vector<std::vector<double>>(I2, std::vector<double>(J1, 0)));

	//様式を合わせる
	for (int i = 0; i < pop_size; i++) {
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				w1_Main[i][j][k] = Pop[i].w1_CO[j][k];
			}
		}
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				w2_Main[i][j][k] = Pop[i].w2_CO[j][k];
			}
		}
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				w3_Main[i][j][k] = Pop[i].w3_CO[j][k];
			}
		}
	}
	//bef = trueの時AI，falseの時AIC
	FILE *fp_main;
	std::stringstream File_Name;
	std::stringstream Tmp_File_Name;

	File_Name << "./" << Dir << "/" << Csv_Method << "/" << Csv_Trial << "/" << Csv_Gene / Csv_Per;
	//File_Name << "./" << Dir;

	for (int i = 0; i < pop_size; i++) {
		Tmp_File_Name << File_Name.str() << "/" << i << ".dat";
		//ファイル書き込み
		if ((fp_main = fopen(Tmp_File_Name.str().c_str(), "wb+")) == NULL) {
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		fwrite(w1_Main[i].data(), sizeof(double), I1*J1, fp_main);
		fwrite(w2_Main[i].data(), sizeof(double), I2*J2, fp_main);
		fwrite(w3_Main[i].data(), sizeof(double), I2*J1, fp_main);

		//クリア
		Tmp_File_Name.str("");
		Tmp_File_Name.clear(std::stringstream::goodbit);
		fclose(fp_main);
	}
}
//世代毎の対戦結果を格納
/*
void CsvModules::SetCsv_Re_P(std::vector<int> &Vector_PopResult){
	int Length = int(Vector_PopResult.size());
	Re_P[VecLen_Re_P].resize(Length);
	for (int i = 0; i < VecLen_Re_P; i++) {
		Re_P[VecLen_Re_P][i] = Vector_PopResult[i];
	}
	VecLen_Re_P++;
}
*/
//クラスターをCSVファイルへ出力
/*
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
*/
/*
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
*/
//csvを統合するクラス
class CsvModules_Intend {
public:
	void Create_Data(std::vector<int> &me, int n, int trial, int gene, int per);
protected:
	int method_size;
	int Gene;
	int Method_Trial;
	int Per;
	std::vector<int> method;
	std::vector<std::vector<std::string>> Output; // Out_Ave[世代][手法][ave/max/min]
	std::vector<std::vector<std::string>> Output_Per; // Out_Ave[世代][手法][ave/max/min]

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
	//世代数と個体の情報を記録するベクター
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
			//ファイル名設定
			std::stringstream FileName;
			std::vector<std::vector<std::string>> table;
			FileName << Tmp_FileName.str() << "PopResult_" << trial << "_" << Opp << ".csv";

			//ファイル読み込み
			if (!GetContents(FileName.str(), table)) {
				std::cout << "file open error! " << FileName.str() << std::endl;
			}
			//必要な部分だけ抽出
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

//ファイル読み込み(2次元ベクター)
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

	return true;
}

//ファイル出力(2次元ベクター)
bool CsvModules_Intend::OutContents(std::string filename)
{
	// ファイルを開く
	std::ofstream ofs(filename);
	if (!ofs.is_open())
	{
		// ファイルが開けなかった場合は終了する
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
	// ファイルを開く
	std::ofstream ofs(filename);
	if (!ofs.is_open())
	{
		// ファイルが開けなかった場合は終了する
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
}

//最大値, 最小値, 平均取得
void CsvModules_Intend::Cal_Ave_Max_Min(std::vector<std::vector<std::vector<double>>> &input, std::vector<std::vector<double>> &Max, std::vector<std::vector<double>> &Min, std::vector<std::vector<double>> &Ave)
{
	Max = std::vector<std::vector<double>>(ENEMY, std::vector<double>(Gene / Per + 1, 0));
	Min = std::vector<std::vector<double>>(ENEMY, std::vector<double>(Gene / Per + 1, 0));
	Ave = std::vector<std::vector<double>>(ENEMY, std::vector<double>(Gene / Per + 1, 0));

	for (int i = 0; i < ENEMY; i++) {
		for (int j = 0; j < Gene / Per + 1; j++) {
			//最大値
			auto max = *max_element(input[i][j].begin(), input[i][j].end());
			//最小値
			auto min = *min_element(input[i][j].begin(), input[i][j].end());

			Max[i][j] = input[i][j][int(max)];
			Min[i][j] = input[i][j][int(min)];
			Ave[i][j] = accumulate(input[i][j].begin(), input[i][j].end(), 0.0)/KO;
		}
	}
}

//書き込み用のベクターにデータをセット
void CsvModules_Intend::Set_Output(int me, std::vector<std::vector<std::vector<double>>> &input) {
	//inputから最大最小平均を求める
	std::vector<std::vector<double>> Max;
	std::vector<std::vector<double>> Min;
	std::vector<std::vector<double>> Ave;

	//相手毎にデータをまとめる
	Cal_Ave_Max_Min(input, Max, Min, Ave);
	std::cout << "   Cal_Ave_Max_Min ...done" << std::endl;
	//まとめたデータの平均をとる
	std::vector<double> ave_Ave;
	std::vector<double> ave_Max;
	std::vector<double> ave_Min;
	cal_average(Ave, ave_Ave);
	cal_average(Max, ave_Max);
	cal_average(Min, ave_Min);
	std::cout << "   cal_average ...done" << std::endl;

	//全ての平均を出力
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

	//敵毎の数値を出力
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

//各種データを整えて平均を求める
void CsvModules_Intend::cal_average(std::vector<std::vector<double>> &input, std::vector<double> &output) {
	std::vector<std::vector<double>> tmp_input;

	output = std::vector<double>(Gene / Per + 1, 0);
	tmp_input = std::vector<std::vector<double>>(Gene / Per + 1, std::vector<double>(ENEMY, 0));

	for (int i = 0; i < ENEMY; i++) {
		for (int j = 0; j < Gene / Per + 1; j++) {
			tmp_input[j][i] = input[i][j];
		}
	}
	//対戦相手に対する世代毎の平均
	for (int i = 0; i < Gene / Per + 1; i++) {
		output[i] = accumulate(tmp_input[i].begin(), tmp_input[i].end(), 0.0) / ENEMY;
	}
}