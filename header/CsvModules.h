/*
Csv�̓��o�͂Ɋւ��郂�W���[���u����
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>

class CsvModules {
public:
	void Init(int Trial, int Gene, int Per);
	void SetCsv_Cr_P(std::vector<int> &Vector_Cruster);
	void SetCsv_Re_P(std::vector<int> &Vector_PopResult);
	void Fwrite_Cr_P();
	void Fwrite_Re_P();
	//void FileWrite_OppResult();
private:
	int Csv_Trial;
	int Csv_Gene;
	int Csv_Per;
	int VecLen_Cr_P;
	int VecLen_Re_P;
	std::vector<std::vector<int> > Cr_P;
	std::vector<std::vector<int> > Re_P;
};
//�p�����[�^�[��������
void CsvModules::Init(int Trial, int Gene, int Per) {
	Csv_Trial = Trial;
	Csv_Gene = Gene;
	Csv_Per = Per;
	VecLen_Cr_P = 0;
	VecLen_Re_P = 0;

	Cr_P.resize(Csv_Gene / Csv_Per);
	Re_P.resize(Csv_Gene / Csv_Per);
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
	char fname[30];
	sprintf(fname, "./csv/Cruster/Cruster_%d.csv", Csv_Trial);
	//�t�@�C���o�̓X�g���[��
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
	//�t�@�C�����ݒ�
	char fname[40];
	sprintf(fname, "./csv/PopResult/PopResult_%d.csv", Csv_Trial);
	//�t�@�C���o�̓X�g���[��
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
//void FileWrite_PopResult();
//void FileWrite_OppResult();
