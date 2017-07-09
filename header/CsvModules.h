/*
Csvの入出力に関するモジュール置き場
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>

class CsvModules {
public:
	void Init(int Trial, int Gene, int Per);
	void Csv_SetPopCruster(std::vector<int> &Vector_Cruster);
	void Csv_SetPopResult(std::vector<int> &Vector_PopResult);
	void FileWrite_PopCruster();
	void FileWrite_PopResult();
	//void FileWrite_OppResult();
private:
	int CsvTrial;
	int CsvGene;
	int CsvPer;
	int PopNumber;
	int VecLeng_PopCruster;
	int VecLeng_PopResult;
	std::vector<std::vector<int> > PopCruster;
	std::vector<std::vector<int> > PopResult;
};
//パラメーターを初期化
void CsvModules::Init(int Trial, int Gene, int Per) {
	CsvTrial = Trial;
	CsvGene = Gene;
	CsvPer = Per;
	VecLeng_PopCruster = 0;
	VecLeng_PopResult = 0;

	PopCruster.resize(CsvGene / CsvPer);
	PopResult.resize(CsvGene / CsvPer);
}
//世代毎のクラスタ番号を格納
void CsvModules::Csv_SetPopCruster(std::vector<int> &Vector_Cruster) {
	int Length = int(Vector_Cruster.size());
	//リサイズ
	PopCruster[VecLeng_PopCruster].resize(Length);
	for (int i = 0; i < Length; i++) {
		PopCruster[VecLeng_PopCruster][i] = Vector_Cruster[i];
	}
	VecLeng_PopCruster++;
}
//世代毎の対戦結果を格納
void CsvModules::Csv_SetPopResult(std::vector<int> &Vector_PopResult){
	int Length = int(Vector_PopResult.size());
	PopResult[VecLeng_PopResult].resize(Length);
	for (int i = 0; i < VecLeng_PopResult; i++) {
		PopResult[VecLeng_PopResult][i] = Vector_PopResult[i];
	}
	VecLeng_PopResult++;
}
//クラスターをCSVファイルへ出力
void CsvModules::FileWrite_PopCruster() {
	//ファイル名設定
	char fname[30];
	sprintf(fname, "./csv/Cruster/Cruster_%d.csv", CsvTrial);
	//ファイル出力ストリーム
	std::ofstream ofs( fname );

	for (int i = 0; i < CsvGene / CsvPer; i++) {
		ofs << CsvPer*(i + 1) << ',';
		int Length = int(PopCruster[i].size());
		for (int j = 0; j < Length; j++) {
			ofs << PopCruster[i][j] << ',';
		}
		ofs << std::endl;
	}
}
void CsvModules::FileWrite_PopResult(){
	//ファイル名設定
	char fname[40];
	sprintf(fname, "./csv/PopResult/PopResult_%d.csv", CsvTrial);
	//ファイル出力ストリーム
	std::ofstream ofs(fname);

	for (int i = 0; i < CsvGene / CsvPer; i++) {
		ofs << CsvPer*(i + 1) << ',';
		int Length = int(PopResult[i].size());
		for (int j = 0; j < Length; j++) {
			ofs << PopResult[i][j] << ',';
		}
		ofs << std::endl;
	}
}
//void FileWrite_PopResult();
//void FileWrite_OppResult();
