#pragma once
#include "MT.h"
#include "Check_the_folder.h"
#include <stdio.h>

/*****************
自然数乱数生成
*****************/
int GetRand_Int(int pGetMax)
{
	const double getrand_int32Max = 4294967295; // genrand_int32() の最大値。除算のためdouble
	return int(genrand_int32() / getrand_int32Max * pGetMax);
}

/*****************
実数乱数生成
*****************/
double GetRand_Real(double pGetMax)
{
	double get_save;
	const double getrand_int32Max = 4294967295; // genrand_int32() の最大値。除算のためdouble
	get_save = getrand_int32Max / 2;

	return ((double)genrand_int32() - get_save) / get_save*pGetMax;
}
//フォルダーなかったら作る
void Make_Directory_AI(int method, int trial, int gene, int per) {
	char filename[50];
	CheckTheFolder::checkExistenceOfFolder("./AI");
	sprintf_s(filename, 50, "./AI/%d", method);
	CheckTheFolder::checkExistenceOfFolder(filename);
	sprintf_s(filename, 50, "./AI/%d/%d", method, trial);
	CheckTheFolder::checkExistenceOfFolder(filename);
	for (int g = 0; g < gene / per + 1; g++) {
		sprintf_s(filename, 50, "./AI/%d/%d/%d", method, trial, g);
		CheckTheFolder::checkExistenceOfFolder(filename);
	}
	std::cout << "Create Directory..." << std::endl;
}

void Make_Directory_AI_1(int method, int trial, int k, int gene, int per) {
	char filename[50];
	// ./AI
	CheckTheFolder::checkExistenceOfFolder("./AI");
	// ./AI/Method
	sprintf_s(filename, 50, "./AI/%d", method);
	CheckTheFolder::checkExistenceOfFolder(filename);
	// ./AI/Method/Triai
	sprintf_s(filename, 50, "./AI/%d/%d", method, trial);
	CheckTheFolder::checkExistenceOfFolder(filename);
	// ./AI/Method/Triai
	sprintf_s(filename, 50, "./AI/%d/%d/%d", method, trial, k);
	CheckTheFolder::checkExistenceOfFolder(filename);
	for (int g = 0; g < gene / per + 1; g++) {
		sprintf_s(filename, 50, "./AI/%d/%d/%d/%d", method, trial, k, g);
		CheckTheFolder::checkExistenceOfFolder(filename);
	}
	std::cout << "Create Directory..." << std::endl;
}

void Make_Directory_AIT(int method, int trial) {
	char filename[50];
	CheckTheFolder::checkExistenceOfFolder("./AIT");
	sprintf_s(filename, 50, "./AIT/%d", method);
	CheckTheFolder::checkExistenceOfFolder(filename);
	sprintf_s(filename, 50, "./AIT/%d/%d", method, trial);
	CheckTheFolder::checkExistenceOfFolder(filename);
}

void Make_CSV_Directory(int method) {
	char filename[50];
	sprintf_s(filename, 50, "./csv");
	CheckTheFolder::checkExistenceOfFolder(filename);
	sprintf_s(filename, 50, "./csv/Cruster");
	CheckTheFolder::checkExistenceOfFolder(filename);
	sprintf_s(filename, 50, "./csv/Cruster/%d", method);
	CheckTheFolder::checkExistenceOfFolder(filename);


	sprintf_s(filename, 50, "./csv/PopResult");
	CheckTheFolder::checkExistenceOfFolder(filename);
	sprintf_s(filename, 50, "./csv/PopResult/%d", method);
	CheckTheFolder::checkExistenceOfFolder(filename);
}