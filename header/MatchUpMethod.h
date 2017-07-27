#pragma once
#include "config.hpp"
#include <iostream>
#include <fstream>
#include <time.h>

class Match {
public:
	void Init_Parameter(int Method_P, int Method_O, int Pop_N, int Opp_N, int Gene_N, int Per_N, int Cru = 0);
	void Match_And_SetDATA(int Pop_Trial, int Opp_Trial);
	void File_Write_CSV(int Pop_trial, int Opp_trial);
private:
	int Gene;	//���㐔
	int Per;	//��؂�
	int Loop_Length;	//���[�v
	int Pop_Length;	//���㐔
	int Opp_Length;
	int Pop_Method;	//���㐔
	int Opp_Method;
	int	Cru_K;
	std::vector<std::vector<std::vector<double> > > Pop_Result;
	std::vector<std::vector<double> > Opp_Result;
	std::vector<std::vector<double> > ToCsv_Data;
protected:
	void Set_CsvData(int trial);
	void PvP(int Pop_Trial, int Opp_Trial, int Gene);
};

void Match::Init_Parameter(int Method_P, int Method_O, int Pop_N, int Opp_N, int Gene_N, int Per_N, int Cru) {
	Gene = Gene_N;	//���㐔
	Per = Per_N;
	Loop_Length = Gene_N / Per_N;
	Cru_K = Cru;

	Pop_Method = Method_P;
	Opp_Method = Method_O;

	Pop_Length = Pop_N;	//���㐔
	Opp_Length = Opp_N;

	//���v���C���[�̏�����
	Pop_Result.resize(Pop_Length); //�����o�[��
	for (int i = 0; i < Pop_Length; i++) {
		Pop_Result[i].resize(Loop_Length); //���㐔
		for (int j = 0; j < Loop_Length; j++) {
			Pop_Result[i][j].resize(Opp_Length); //�ΐ푊�萔
		}
	}
	//����v���C���[�̏�����
	Opp_Result.resize(Opp_Length);
	for (int i = 0; i < Opp_Length; i++) {
		Opp_Result[i].resize(Pop_Length); //�ΐ푊�萔
	}
	//Csv�t�@�C���o�͗p�x�N�^�[�̏�����
	ToCsv_Data.resize(Loop_Length);
	for (int i = 0; i < Loop_Length; i++) {
		ToCsv_Data.resize(Pop_Length); //�ΐ푊�萔
	}
}
//���v���C���[�Ƒ���v���C���[���킹��
void Match::PvP(int Pop_Trial, int Opp_Trial, int Gene) {
	for (int AI_Pop = 0; AI_Pop < Pop_Length; AI_Pop++) {
		//cout << ai1 << " ";
		char filename[50];
		if (Pop_Method == 0) {
			sprintf_s(filename, ("AI/%d/%d/%d/%d.dat"), Pop_Method, Pop_Trial, Gene, AI_Pop);
		}else if (Pop_Method == 1) {
			sprintf_s(filename, ("AI/%d/%d/%d/%d/%d.dat"), Pop_Method, Pop_Trial, Cru_K, Gene, AI_Pop);
		}
		if ((file = fopen(filename, "rb")) == NULL) {
			printf("file open error!!\n");
			exit(0);
		}
		else {
			if (Pop_Method == 1) {
				std::cout << "Cru:" << Cru_K << std::endl;
			}
			std::cout
				<< "open:Population_"
				<< Pop_Method << '_'
				<< Pop_Trial << '_'
				<< Gene << '_'
				<< AI_Pop << std::endl;
		}
		//���v���C���[�̐헪�i�[
		fread(w1, sizeof(double), I1 * J1, file);
		fread(w2, sizeof(double), I2 * J2, file);
		fread(w3, sizeof(double), I2 * J1, file);
		fclose(file);

		//�ΐ푊��
		for (int AI_Opp = 0; AI_Opp < Opp_Length; AI_Opp++) {
			sprintf(filename, ("AIT/%d/%d/%d.dat"), Opp_Method, Opp_Trial, AI_Opp);
			if ((fp = fopen(filename, "rb")) == NULL) {
				printf("file open error!!\n");
				exit(0);
			}
			//����v���C���[�̐헪�i�[
			fread(w1_T, sizeof(double), I1 * J1, fp);
			fread(w2_T, sizeof(double), I2 * J2, fp);
			fread(w3_T, sizeof(double), I2 * J1, fp);
			fclose(fp);

			//�ΐ�
			Competition();

			if (player1.win == 1) {
				Pop_Result[AI_Pop][Gene][AI_Opp] = 1;
			}
			else if (player2.win == 1) {
				Pop_Result[AI_Pop][Gene][AI_Opp] = 0;
			}
			else {
				Pop_Result[AI_Pop][Gene][AI_Opp] = 0;
			}
		}
	}

}
void Match::Match_And_SetDATA(int Pop_Trial, int Opp_Trial) {
	for (int gene = 0; gene < Loop_Length; gene++) {
		std::cout << "generation:" << gene << std::endl;
		clock_t start = clock();
		//�ΐ킵�Č��ʂ��i�[
		PvP(Pop_Trial, Opp_Trial, gene);
		//�ΐ팋�ʂ��畽�Ϗ����������߂�
		Set_CsvData(gene);
		clock_t end = clock();

		int time = (end - start) / CLOCKS_PER_SEC;
		int hour = time / 3600;
		int minute = time / 3600 / 60;
		int second = time % 60;
		std::cout << "Process time:" << std::oct << std::showbase << hour << ':';
		std::cout << std::oct << std::showbase << minute << ':';
		std::cout << std::oct << std::showbase << second << std::endl;
	}
	std::cout << "Set Data" << std::endl;
}
void Match::Set_CsvData(int g) {
	//0�ŏ�����
	std::vector<double> Pop_Eval(Pop_Length, 0);
	//0�ŏ�����
	//���v���C���[�̕]���l�����߂�
	for (int p = 0; p < Pop_Length; p++) {
		for (int o = 0; o < Opp_Length; o++) {
			Pop_Eval[p] += Pop_Result[p][g][o];
			//std::cout << Pop_Eval[p] << ',';
		}
	}
	//std::cout << std::endl;
	ToCsv_Data[g] = Pop_Eval;
}
void Match::File_Write_CSV(int Pop_trial, int Opp_trial) {
	char fname[50];
	sprintf(fname, "./csv/PopResult/PopResult_%d_%d_%d.csv", Pop_Method, Pop_trial, Opp_trial);
	//�t�@�C���o�̓X�g���[��
	std::ofstream ofs(fname);

	ofs << "���㐔" << ',';
	for (int j = 0; j < Pop_Length; j++) {
		ofs << "p" << j << ',';
	}
	ofs << std::endl;

	//���ʂ��o��
	for (int i = 0; i < Loop_Length; i++) {
		ofs << i*Per << ',';
		for (int j = 0; j < Pop_Length; j++) {
			ofs << ToCsv_Data[i][j] << ',';
			//std::cout << ToCsv_Data[i][j] << ',';
		}
		ofs << std::endl;
	}
	std::cout << "File Written" << std::endl;
}