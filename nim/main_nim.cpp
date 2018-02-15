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
template<class T>
void show(T value) {
	std::cout << value << ":" << value*2 << std::endl;
}
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
	//mode = 2;
	//method = 6;
	mode = atoi(__argv[1]);
	if (mode == 1 || mode == 2 || mode == 5) {
		switch (__argc) {
		case 1:
			//Debug mode
			std::cout << "Debug mode" << std::endl;
			trial = 0;
			break;
		case 4:
			method = atoi(__argv[2]);
			trial = atoi(__argv[3]);
			break;
/*
		default:
			cout << "�p�����[�^�[������܂���" << endl;
			cout << "coans.exe [mode][method][generation][trial]" << endl;
			exit(1);
			break;
			*/
		}
		if (mode == 1) {
			std::cout << "���[�h:�ΐ�" << std::endl;
		}
		else if (mode == 2) {
			std::cout << "���[�h:�w�K" << std::endl;
		}
	}
	if (mode == 3 || mode == 4) {
		//method.resize(1);
		//method[0] = 2;
		method_vec.resize(__argc - 2);
		for (int i = 0; i < __argc - 2; i++) {
			method_vec[i] = atoi(__argv[i + 2]);
			//method[i] = 2;
		}
		//method_vec.push_back(3);
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
		if (method == 1)
		{
			std::vector<coans_mode1> mode1(TRIAL, { trial });
			mode1[trial].main_task2();
		}
		if (method == 2)
		{
			std::vector<coans_mode2> mode2(TRIAL, { trial });
			mode2[trial].main_task();
		}
		if (method == 3)
		{
			std::vector<coans_mode3> mode3(TRIAL, { trial });
			mode3[trial].main_task();
		}
		if (method == 4)
		{
			std::vector<mode4> mode4(TRIAL, { trial });
			mode4[trial].main_task2();
		}
		if (method == 5)
		{
			std::vector<mode5> mode5(TRIAL, { trial });
			mode5[trial].main_task();
		}
		if (method == 6)
		{
			std::vector<mode6> mode6(TRIAL, { trial });
			mode6[trial].main_task3();
		}
	}
	//csv����
	else if (mode == 3) {
		std::cout << "method :";
		show_vec_1(method_vec);
		//exp_opp_stra_disper(method_vec);
		exp_pop_stra_disper(method_vec);
		//exp_BestRate(method_vec);
		//exp_opp_BestRate(method_vec);
		//exp_pop_pseudoF(method_vec);
		//exp_pop_disper(method_vec);
	}
	else if (mode == 4) {
		if (method_vec.empty()) {
			exit(EXIT_FAILURE);
		}
		save_opp_stra(method_vec);
	}
	const clock_t End_Main = clock();
	Show_Time(Start_Main, End_Main);
	std::cout << "match_up num:" << MatchUp_Count << std::endl;

	return 0;				// �\�t�g�̏I��
}

void Show_Time(clock_t Start, clock_t End) {
	double time = (End - Start) / double(CLOCKS_PER_SEC);
	std::cout << "Process time:" << time << "[sec]" << std::endl;
}