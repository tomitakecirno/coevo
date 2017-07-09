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
//�p�����[�^�[��������
void CsvModules::Init(int Trial, int Gene, int Per) {
	CsvTrial = Trial;
	CsvGene = Gene;
	CsvPer = Per;
	VecLeng_PopCruster = 0;
	VecLeng_PopResult = 0;

	PopCruster.resize(CsvGene / CsvPer);
	PopResult.resize(CsvGene / CsvPer);
}
//���㖈�̃N���X�^�ԍ����i�[
void CsvModules::Csv_SetPopCruster(std::vector<int> &Vector_Cruster) {
	int Length = int(Vector_Cruster.size());
	//���T�C�Y
	PopCruster[VecLeng_PopCruster].resize(Length);
	for (int i = 0; i < Length; i++) {
		PopCruster[VecLeng_PopCruster][i] = Vector_Cruster[i];
	}
	VecLeng_PopCruster++;
}
//���㖈�̑ΐ팋�ʂ��i�[
void CsvModules::Csv_SetPopResult(std::vector<int> &Vector_PopResult){
	int Length = int(Vector_PopResult.size());
	PopResult[VecLeng_PopResult].resize(Length);
	for (int i = 0; i < VecLeng_PopResult; i++) {
		PopResult[VecLeng_PopResult][i] = Vector_PopResult[i];
	}
	VecLeng_PopResult++;
}
//�N���X�^�[��CSV�t�@�C���֏o��
void CsvModules::FileWrite_PopCruster() {
	//�t�@�C�����ݒ�
	char fname[30];
	sprintf(fname, "./csv/Cruster/Cruster_%d.csv", CsvTrial);
	//�t�@�C���o�̓X�g���[��
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
	//�t�@�C�����ݒ�
	char fname[40];
	sprintf(fname, "./csv/PopResult/PopResult_%d.csv", CsvTrial);
	//�t�@�C���o�̓X�g���[��
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
