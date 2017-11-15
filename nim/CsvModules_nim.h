#pragma once
/*
Csvの入出力に関するモジュール置き場
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
#include "config_nim.hpp"
#include "player_nim.h"
#include "../header/Usual_Methods.hpp"

class CsvModules {
public:
	//ファイル読み込み
	template<class Vec_out>
	bool GetContents(std::string filename, Vec_out &input);
	//ファイル書き込み
	template<class Vec_in>
	bool csv_fwrite(std::string fname, Vec_in &vector, int per = 1);
	template<class Vec_in2>
	bool csv_fwrite2(std::string fname, const Vec_in2 &vector);
};
template<class Vec_out>
bool CsvModules::GetContents(std::string filename, Vec_out &input)
{
	std::vector<std::vector<std::string>> table;
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
	table.erase(table.end() - 1);

	int table_len_Y = int(table.size());
	input.resize(table_len_Y - 1);
	for (int i = 1; i < table_len_Y; i++) {
		int table_len_X = int(table[i].size());
		input[i - 1].resize(table_len_X - 1);
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
		// ファイルが開けなかった場合は終了する
		std::cout << "file open error :" << fname << std::endl;
		return false;
	}
	//1段目
	ofs << " " << ',';
	for (int i = 0; i < max_size; i++) {
		ofs << i + 1 << ',';
	}
	ofs << std::endl;

	//2段目以降
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
template<class Vec_in2>
bool CsvModules::csv_fwrite2(std::string fname, const Vec_in2 &vector) {
	std::ofstream ofs(fname);
	if (!ofs.is_open())
	{
		// ファイルが開けなかった場合は終了する
		return false;
	}
	for (auto &p : vector) {
		for (auto &q : p) {
			ofs << q << ",";
		}
		ofs << std::endl;
	}
	return true;
}
/*
	実験用のcsv
*/
//csvを統合するクラス
class csvmodules_exp : public CsvModules {
public:
	void Create_Data(std::vector<int> &me, int n, int trial, int gene, int per);
	void integration(std::vector<int> &me, int gene, int per);
protected:
	//method, generation, trial, opp
	int method_size;
	int Gene;
	int Method_Trial;
	int Per;
	std::vector<int> method;
	std::vector<std::vector<std::string>> Output; // Out_Ave[世代][手法][ave/max/min]
	std::vector<std::vector<std::string>> Output_Per; // Out_Ave[世代][手法][ave/max/min]
	std::vector<std::vector<std::string>> gene_opp_csv; // Out_Ave[世代][手法][ave/max/min]
	void Init_Output();
	void Init_Output_Per(int me);
	void Init_gene_opp_csv();
	void Set_Output(int me, std::vector<std::vector<std::vector<double>>> &input);
	bool OutContents(std::string filename);
	bool OutContents_Per(std::string filename);
	void Cal_Ave_Max_Min(std::vector<std::vector<std::vector<double>>> &input, std::vector<std::vector<double>> &Max, std::vector<std::vector<double>> &Min, std::vector<std::vector<double>> &Ave);
	void cal_average(std::vector<std::vector<double>> &input, std::vector<double> &output);
};
void csvmodules_exp::Init_Output() 
{
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
void csvmodules_exp::Init_Output_Per(int me) {
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
void csvmodules_exp::Init_gene_opp_csv()
{
	gene_opp_csv = std::vector<std::vector<std::string>>((Gene / Per + 2) * 3 + 1, std::vector<std::string>(ENEMY + 1, " "));
	for (int i = 0; i < (Gene / Per + 2) * 3; i++) {
		if (i % (Gene / Per + 2) == 0) {
			switch (i / (Gene / Per + 2)) {
			case 0:
				gene_opp_csv[i][0] = "Average";
				for (int j = 0; j < ENEMY; j++) {
					std::stringstream ss;
					ss << "opp" << j;
					gene_opp_csv[i][j + 1] = ss.str();
				}
				break;
			case 1:
				gene_opp_csv[i][0] = "Max";
				for (int j = 0; j < ENEMY; j++) {
					std::stringstream ss;
					ss << "opp" << j;
					gene_opp_csv[i][j + 1] = ss.str();
				}
				break;
			case 2:
				gene_opp_csv[i][0] = "Min";
				for (int j = 0; j < ENEMY; j++) {
					std::stringstream ss;
					ss << "opp" << j;
					gene_opp_csv[i][j + 1] = ss.str();
				}
				break;
			default:
				std::cerr << "Error";
				break;
			}
		}
		else {
			int tmp = i % (Gene / Per + 2) - 1;
			gene_opp_csv[i][0] = std::to_string(tmp*Per);
		}
	}
}
void csvmodules_exp::integration(std::vector<int> &me, int gene, int per) {
	method_size = int(me.size());
	method = me;
	Gene = gene;
	Per = per;

	std::vector<std::vector<double>> input;
	std::vector<std::vector<double>> max_vec;
	std::vector<std::vector<double>> min_vec;
	std::vector<std::vector<double>> ave_vec;

	for (int i = 0; i < method_size; i++) {
		std::cout << "start : " << method[i] << std::endl;
		Init_gene_opp_csv();
		std::stringstream ss;
		ss << "./csv/" << method[i];

		for (int j = 0; j < ENEMY; j++) {
			std::cout << "enemy : " << j << std::endl;
			max_vec = std::vector<std::vector<double>>(Gene / Per + 1, std::vector<double>(TRIAL, 0));
			min_vec = std::vector<std::vector<double>>(Gene / Per + 1, std::vector<double>(TRIAL, 0));
			ave_vec = std::vector<std::vector<double>>(Gene / Per + 1, std::vector<double>(TRIAL, 0));
			for (int k = 0; k < TRIAL; k++) {
				char fname[50];
				input = std::vector<std::vector<double>>(Gene / Per + 1, std::vector<double>(KO, 0));
				sprintf_s(fname, "%s/PopResult_%d_%d_%d_%d.csv", ss.str().c_str(), method[i], k, j, Gene);
				if (!GetContents(fname, input)) {
					std::cout << "ファイルが開けません :" << fname << std::endl;
				}
				std::cout << fname << "...open" << std::endl;
				for (int l = 0; l < Gene / Per + 1; l++) {
					max_vec[l][k] = *max_element(input[l].begin(), input[l].end());
					min_vec[l][k] = *min_element(input[l].begin(), input[l].end());
					ave_vec[l][k] = std::accumulate(input[l].begin(), input[l].end(), 0.0) / KO;
				}
			}
			//show_vec_2(ave_vec);
			for (int k = 0; k < Gene / Per + 1; k++) {
				//平均
				gene_opp_csv[k + 1][j + 1] = std::to_string(std::accumulate(ave_vec[k].begin(), ave_vec[k].end(), 0.0) / TRIAL);
				//最大
				gene_opp_csv[k + (Gene / Per + 3)][j + 1] = std::to_string(std::accumulate(max_vec[k].begin(), max_vec[k].end(), 0.0) / TRIAL);
				//最小
				gene_opp_csv[k + (Gene / Per + 3) * 2 - 1][j + 1] = std::to_string(std::accumulate(min_vec[k].begin(), min_vec[k].end(), 0.0) / TRIAL);
			}
			//show_vec_2(gene_opp_csv);
			std::cout << "enemy : " << j << "...done" << std::endl;
		}
		char fname[50];
		sprintf_s(fname, "%s/totalResult_%d_%d.csv", ss.str().c_str(), method[i], Gene);
		csv_fwrite2(fname, gene_opp_csv);
		std::cout << fname << "...written" << std::endl;
	}
}
void csvmodules_exp::Create_Data(std::vector<int> &me, int n, int trial, int gene, int per) {
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

		std::stringstream tmp_fname;
		std::vector<std::vector<std::vector<double>>> Input;

		tmp_fname << CSV_DIR << "/" << method[i] << "/";

		Input = std::vector<std::vector<std::vector<double>>>(ENEMY, std::vector<std::vector<double>>(Gene / Per + 1, std::vector<double>(n, 0)));
		for (int Opp = 0; Opp < ENEMY; Opp++) {
			//ファイル名設定
			std::stringstream fname;
			std::vector<std::vector<std::string>> table;
			fname << tmp_fname.str() << "PopResult_" << trial << "_" << Opp << ".csv";
			//ファイル読み込み
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
		filename_Per << CSV_DIR << "/Result_Per_Enemy_" << method[i] << ".csv";
		OutContents_Per(filename_Per.str());

		std::cout << "method:" << method[i] << " done..." << std::endl;
	}
}
bool csvmodules_exp::OutContents(std::string filename)
{
	// ファイルを開
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
bool csvmodules_exp::OutContents_Per(std::string filename)
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
//最大値, 最小値, 平均取得
void csvmodules_exp::Cal_Ave_Max_Min(std::vector<std::vector<std::vector<double>>> &input, std::vector<std::vector<double>> &Max, std::vector<std::vector<double>> &Min, std::vector<std::vector<double>> &Ave)
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
void csvmodules_exp::Set_Output(int me, std::vector<std::vector<std::vector<double>>> &input) {
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
//各種データを整えて平均を求める
void csvmodules_exp::cal_average(std::vector<std::vector<double>> &input, std::vector<double> &output) {
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