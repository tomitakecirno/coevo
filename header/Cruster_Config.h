#pragma once
#include <iostream>
#include <vector>
#include "../header/getrandcoans.hpp"

#define DEM 3	//次元数
#define KO	30	//初期集団数
#define K	3	//クラスタ関係のパラメーター

struct Player {
	std::vector<double> Str;
	int Cr_Num;
public:
	void Init();
	void Init_WS();
};

//解を初期化
void Player::Init() {
	Str.resize(DEM);
	for (int i = 0; i < DEM; i++) {
		Str[i] = GetRand_Real(100);
	}
	Cr_Num = 0;
}
//解以外を初期化
void Player::Init_WS() {
	Cr_Num = 0;
}