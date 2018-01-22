#pragma once
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <cstring>
//#include "nim.h"
#include "coans_nim.h"
#include "coansmodule_nim.hpp"
#include "matchmethods_nim.h"


using namespace std;

/****************************
���C�����[�`��
DE
0:�ΐ푊��w�K
1:�v���C���[VS�v���C���[
2:ANS�N���X�^�����O��@�w�K
3:�K�w�I�N���X�^�����O��@
***************************/

void Show_Time(clock_t Start, clock_t End);
/***********************************************
PARAMETER
Main_Mode	:���[�h	(0: 1: 2:��Ď�@�̊w�K 3:)
Main_Method	:��Ď�@
Main_Trial	:���s��
Main_K		:�N���X�^�����O�p�����[�^�[(?)
***********************************************/
int main(int argc, char *argv[])
{
	int mode;
	int	method;
	int ku;
	int trial;
	int per;
	int cru_k = 0;

	std::vector<int> method_vec;
	/*
		argv[0]:.exe
		argv[1]:mode
		argv[2]:method
		argv[3]:generation
		argv[4]:trial
		argv[5]:nitch parameter
	*/
	mode = 2;
	//mode = atoi(__argv[1]);
	if (mode == 1 || mode == 2 || mode == 4 || mode == 5) {
		switch (__argc) {
		case 1:
			//Debug mode
			method = 4;
			ku = KU;
			trial = 0;
			break;
		case 3:
			method = atoi(__argv[2]);
			break;
		case 5:
			method = atoi(__argv[2]);
			ku = atoi(__argv[3]);
			trial = atoi(__argv[4]);
			break;
/*
		default:
			cout << "�p�����[�^�[������܂���" << endl;
			cout << "coans.exe [mode][method][generation][trial]" << endl;
			exit(1);
			break;
			*/
		}
		per = PER;
		if (mode == 1) {
			std::cout << "���[�h:�ΐ�" << std::endl;
		}
		else if (mode == 2) {
			std::cout << "���[�h:�w�K" << std::endl;
		}
	}
	if (mode == 3) {
		//method.resize(1);
		//method[0] = 2;
		method_vec.resize(__argc - 2);
		for (int i = 0; i < __argc - 2; i++) {
			method_vec[i] = atoi(__argv[i + 2]);
			//method[i] = 2;
		}
		std::cout << "���[�h:csv����" << std::endl;
	}
	__int64 MatchUp_Count = 0;

	const clock_t Start_Main = clock();
	//�w�K�ŋL�^�����f�[�^�����ƂɎ��ۂɑΐ�

	//����@�w�K
	// DIR, KO, KU, PER, K, PARENT, CHILD
	if (mode == 2) {
		//����@
		//�K�w�I�N���X�^�����O�𐷂荞�񂾎�@
		if (method == 2)
		{
			coans_mode2 Mode2("nim", ku, per, trial);
			Mode2.main_task();
			MatchUp_Count = Mode2.Get_MatchUp_Num();
		}
		if (method == 3)
		{
			coans_mode3 Mode3("nim", ku, per, trial);
			Mode3.main_task();
			MatchUp_Count = Mode3.Get_MatchUp_Num();
		}
		if (method == 4)
		{
			mode4 mode4("nim", trial);
			mode4.main_task2();
		}
	}
	//csv����
	else if (mode == 3) {
		csvmodules_exp Method;
		Method.integration(method_vec, KU, PER);
	}
	else if (mode == 1) {
		Match match(method, 0, trial, KU, PER);
		match.evaluation();
	}
	else if (mode == 7) {
		//test mode
		fuzzy_x_means();
	}
	const clock_t End_Main = clock();
	Show_Time(Start_Main, End_Main);
	std::cout << "match_up num:" << MatchUp_Count << std::endl;

	return 0;				// �\�t�g�̏I��
}

void Show_Time(clock_t Start, clock_t End) {
	int time = (End - Start) / CLOCKS_PER_SEC;
	std::cout << "Process time:" << time << "[sec]" << std::endl;
}