#pragma once
#include "../header/coansmodule.hpp"
#include "../header/CoansMethods.h"
#include "../header/MatchUpMethod.h"
#include "../header/Floreano.h"
#include "../header/config.hpp"
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <cstring>

using namespace std;

/****************************
���C�����[�`��
DE
0:�ΐ푊��w�K
1:�v���C���[VS�v���C���[
2:ANS�N���X�^�����O��@�w�K
3:�K�w�I�N���X�^�����O��@
***************************/
void MatchUp_CSV(std::vector<int> &Count);
void Show_Time(clock_t Start, clock_t End);
void Init_Test();

/***********************************************
PARAMETER
Main_Mode	:���[�h	(0: 1: 2:��Ď�@�̊w�K 3:)
Main_Method	:��Ď�@
Main_Trial	:���s��
Main_K		:�N���X�^�����O�p�����[�^�[(?)
***********************************************/
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int Main_Mode;
	int Main_Trial;
	int Main_K;
	int	Main_Method;

	/*
		argv[1]:Method
		argv[2]:Trial
		argv[3]:k
	*/
	if (__argc < 3) {
		//�p�����[�^�[�����œ������p
		Main_Mode = 3;
		Main_Method = 2;
		Main_Trial = 0;
		Main_K = 0;
	}else if(__argc == 4) {
		Main_Mode = atoi(__argv[1]);
		Main_Method = atoi(__argv[2]);
		Main_Trial = atoi(__argv[3]);
		Main_K = 0;
	}
	else if (__argc == 5) {
		Main_Mode = atoi(__argv[1]);
		Main_Method = atoi(__argv[2]);
		Main_Trial = atoi(__argv[3]);
		Main_K = atoi(__argv[4]);
	}

	std::cout << "Mode:" << Main_Mode << std::endl;
	std::cout << "��@:" << Main_Method << std::endl;
	std::cout << "���s��:" << Main_Trial << std::endl;
	std::cout << "�N���X�^��:" << Main_K << std::endl;
	std::cout << "���㐔:" << KU << std::endl;
	std::cout << "��؂�:" << PER << std::endl;

	int MatchUp_Count=0;

	clock_t Start_Main = clock();
	//�����p�ΐ푊��w�K
	if (Main_Mode == 0) {
		Make_Directory_AIT(0,TRIAL);
		FloreMethods Flore_1;
		for (int t = 4; t < TRIAL; t++) {
			Flore_1.Main_Tasks();
			Flore_1.Fwrite_Floreano(t);
		}
	}
	//�w�K�ŋL�^�����f�[�^�����ƂɎ��ۂɑΐ�
	else if (Main_Mode == 1) {
		Match Match_0("AI", Main_Method, 0, KO, KOT, KU, PER, Main_K);
		Make_CSV_Directory(Main_Method);

		for (int Opp_t = 0; Opp_t < F_TRIAL; Opp_t++) {
			//����@vsFloreano ���㐔:2000(100����Ԋu) ����@�W�c50 Floreano�W�c30
			Match_0.Match_And_SetDATA(Main_Trial, Opp_t);
			Match_0.File_Write_CSV(Main_Trial, Opp_t);
		}
		std::cout << "Process time:" << time << std::endl;
		Match_0.Decide_Best(Main_Trial);
	}

	//����@�w�K
	else if (Main_Mode == 2) {
		//����@
		if (Main_Method == 0) {
			Coans_GT2016 Coans_GT2016("2016");
			Coans_GT2016.Coans_GT2016_Tasks(Main_Trial);
			MatchUp_Count = Coans_GT2016.Get_MatchUp_Num();
		}
		//�K�w�I�N���X�^�����O�𐷂荞�񂾎�@
		if (Main_Method == 1) {
			CoansMode1 Mode1("AI");
			Mode1.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode1.Get_MatchUp_Num();
		}
		//�K�w�I�N���X�^�����O�{List3
		else if (Main_Method == 2) {
			CoansMode2 Mode2("AI");
			Mode2.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode2.Get_MatchUp_Num();
		}else if (Main_Method == 3) {
			CoansMode3 Mode3("AI");
			Mode3.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode3.Get_MatchUp_Num();
		}
		else if (Main_Method == 4) {
			CoansMode4 Mode4("AI");
			Mode4.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode4.Get_MatchUp_Num();
		}
	}
	//csv����
	else if (Main_Mode == 3) {
		CsvModules_Intend Method;
		Method.Create_Data(Main_Method, KO, Main_Trial, KU, PER);
	}
	//�e�X�g
	else if (Main_Mode == 4) {
		Init_Test();
		gamemode();
	}

	clock_t End_Main = clock();
	Show_Time(Start_Main, End_Main);
	std::cout << "match_up num:" << MatchUp_Count << std::endl;

	//�v���C
	//WaitKey();		// �L�[���͑҂�
	DxLib_End();			// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
}

void Show_Time(clock_t Start, clock_t End) {
	int time = (Start - Start) / CLOCKS_PER_SEC;
	std::cout << "Process time:" << time << "[sec]" << std::endl;
}


void MatchUp_CSV(std::vector<int> &Count) {
	//�t�@�C�����ݒ�
	char fname[30];
	sprintf(fname, "MatchUp_Log.csv");

	//�t�@�C���o�̓X�g���[��
	std::ofstream ofs(fname);
	for (int i = 0; i < TRIAL; i++) {
		ofs << Count[i] << ',';
	}
	ofs << std::endl;
}

void Init_Test() {
	double Test_W1[I1][J1];//1P
	double Test_W2[I2][J2];
	double Test_W3[I2][J1];

	for (int i = 0; i < I1; i++) {
		for (int j = 0; j < J1; j++) {
			Test_W1[i][j] = GetRand_Real(10);
		}
	}
	for (int i = 0; i < I2; i++) {
		for (int j = 0; j < J2; j++) {
			Test_W2[i][j] = GetRand_Real(10);
		}
	}
	for (int i = 0; i < J1; i++) {
		for (int j = 0; j < I2; j++) {
			Test_W3[i][j] = GetRand_Real(10);
		}
	}

	//AI�t�H���_�����
	std::stringstream File_Name;
	FILE *fp_test;

	File_Name << "./AI";
	CheckTheFolder::checkExistenceOfFolder(File_Name.str());

	//�t�@�C���I�[�v��
	//test.dat�֏�������
	if ((fp_test = fopen("./AI/test.dat", "wb+")) == NULL) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	fwrite(Test_W1, sizeof(double), I1*J1, fp_test);
	fwrite(Test_W2, sizeof(double), I2*J2, fp_test);
	fwrite(Test_W3, sizeof(double), I2*J1, fp_test);
	fclose(fp_test);

	//test_T.dat�֏�������
	if ((fp_test = fopen("./AI/test_T.dat", "wb+")) == NULL) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	fwrite(Test_W1, sizeof(double), I1*J1, fp_test);
	fwrite(Test_W2, sizeof(double), I2*J2, fp_test);
	fwrite(Test_W3, sizeof(double), I2*J1, fp_test);
	fclose(fp_test);
}