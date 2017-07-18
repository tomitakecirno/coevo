#pragma once
#include <iostream>
#include <vector>
#include "../header/getrandcoans.hpp"

#define DEM 3	//������
#define KO	30	//�����W�c��
#define K	3	//�N���X�^�֌W�̃p�����[�^�[

struct Player {
	std::vector<double> Str;
	int Cr_Num;
public:
	void Init();
	void Init_WS();
};

//����������
void Player::Init() {
	Str.resize(DEM);
	for (int i = 0; i < DEM; i++) {
		Str[i] = GetRand_Real(100);
	}
	Cr_Num = 0;
}
//���ȊO��������
void Player::Init_WS() {
	Cr_Num = 0;
}