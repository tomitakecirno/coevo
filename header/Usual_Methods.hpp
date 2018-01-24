#pragma once
#include <iostream>
#include "MT.h"
#include "Check_the_folder.h"
#include <stdio.h>
#include <string>
#include <sstream>

//自然数乱数生成
int GetRand_Int(int pGetMax)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> dist(0, pGetMax - 1);

	const int rand = dist(mt);
	return rand;
}

//実数乱数生成
double GetRand_Real(double pGetMax)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(-pGetMax, pGetMax); //乱数の範囲

	const double rand = dist(mt);
	return rand;
}
double GetRand_Real_0(double pGetMax)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0, pGetMax); //乱数の範囲

	const double rand = dist(mt);
	return rand;
}

void Make_Directory(std::string str, int method, int trial, int gene, int per, int k = 0) {
	std::stringstream File_Name;
	std::stringstream Tmp_File_Name;
	if (k == 0) {
		File_Name << "./" << str;
		CheckTheFolder::checkExistenceAndMakeFolder(File_Name.str());
		File_Name << "/" << method;
		CheckTheFolder::checkExistenceAndMakeFolder(File_Name.str());
		File_Name << "/" << trial;
		CheckTheFolder::checkExistenceAndMakeFolder(File_Name.str());
		for (int g = 0; g < gene / per + 1; g++) {
			Tmp_File_Name << File_Name.str() << "/" << g;
			CheckTheFolder::checkExistenceAndMakeFolder(Tmp_File_Name.str());
			std::cout << Tmp_File_Name.str() << std::endl;
			Tmp_File_Name.str("");
			Tmp_File_Name.clear(std::stringstream::goodbit);
		}
	}
	std::cout << "Create Directory..." << std::endl;
}
void Make_Directory2(std::string str, int method, int trial, int index) 
{
	std::stringstream fname;
	fname << "./" << str;
	CheckTheFolder::checkExistenceAndMakeFolder(fname.str());
	fname << "/" << method;
	CheckTheFolder::checkExistenceAndMakeFolder(fname.str());
	fname << "/" << trial;
	CheckTheFolder::checkExistenceAndMakeFolder(fname.str());
	fname << "/" << index;
	CheckTheFolder::checkExistenceAndMakeFolder(fname.str());
}
void Make_Directory_AIT(int method, int trial) {
	std::stringstream File_Name;

	File_Name << "./AIT";
	CheckTheFolder::checkExistenceAndMakeFolder(File_Name.str());
	File_Name << "/" << method;
	CheckTheFolder::checkExistenceAndMakeFolder(File_Name.str());
	File_Name << "/" << trial;
	CheckTheFolder::checkExistenceAndMakeFolder(File_Name.str());
}
void Make_CSV_Directory(int method) {
	std::stringstream File_Name;
	std::stringstream Tmp_File_Name;

	File_Name << "./csv";
	CheckTheFolder::checkExistenceAndMakeFolder(File_Name.str());
	File_Name << "/" << method;
	CheckTheFolder::checkExistenceAndMakeFolder(File_Name.str());
}
int count_folder(const std::string str) 
{
	int end_flag = 1;
	int folder_num = 0;
	char fname[50];
	while (end_flag) {
		sprintf_s(fname, "%s/%d", str.c_str(), folder_num);
		if (CheckTheFolder::checkExistenceOfFolder(fname)) {
			folder_num++;
		}
		else {
			end_flag = 0;
		}
	}
	return folder_num;
}
//ベクターの情報を表示するだけ
template<class Vec>
void show_vec_1(Vec &vec_1) 
{
	std::cout << "[";
	for (auto &p : vec_1) {
		std::cout << p << ",";
	}
	std::cout << "]" << std::endl;
}
template<class Vec2>
void show_vec_2(Vec2 &vec_2)
{
	for (auto &p : vec_2) {
		std::cout << "[";
		for (auto &q : p) {
			std::cout << q << ",";
		}
		std::cout << "]" << std::endl;
	}
}
template<class Vec3>
int cal_maxIndex(const Vec3 &vec_1) 
{
	const auto max = max_element(vec_1.begin(), vec_1.end());
	const int index = int(std::distance(vec_1.begin(), max));
	return index;
}
template<class Vec4>
int cal_minIndex(const Vec4 &vec_1) {
	const auto min = min_element(vec_1.begin(), vec_1.end());
	const int index = int(std::distance(vec_1.begin(), min));
	return index;
}