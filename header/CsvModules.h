/*
Csvの入出力に関するモジュール置き場
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>

class CsvModules {
public:
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

	Cr_P.resize(Csv_Gene / Csv_Per);
	Re_P.resize(Csv_Gene / Csv_Per);
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
	else if (Csv_Method == 1) {
		sprintf(fname, "./csv/Cruster/%d/Cruster_%d_%d.csv", Csv_Method, Csv_Trial, Csv_K);
	}
	//ファイル出力ストリーム
	std::ofstream ofs( fname );

	for (int i = 0; i < Csv_Gene / Csv_Per; i++) {
		ofs << Csv_Per*(i + 1) << ',';
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
	else if (Csv_Method == 1) {
		sprintf(fname, "./csv/PopResult/%d/PopResult_%d_%d.csv", Csv_Method, Csv_Trial, Csv_K);
	}
	//ファイル出力ストリーム
	std::ofstream ofs(fname);

	for (int i = 0; i < Csv_Gene / Csv_Per; i++) {
		ofs << Csv_Per*(i + 1) << ',';
		int Length = int(Re_P[i].size());
		for (int j = 0; j < Length; j++) {
			ofs << Re_P[i][j] << ',';
		}
		ofs << std::endl;
	}
}