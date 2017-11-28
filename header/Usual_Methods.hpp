#pragma once
#include <iostream>
#include "MT.h"
#include "Check_the_folder.h"
#include <stdio.h>
#include <string>
#include <sstream>

//���R����������
int GetRand_Int(int pGetMax)
{
	const double getrand_int32Max = 4294967296; // genrand_int32() �̍ő�l�B���Z�̂���double
	return int(genrand_int32() / getrand_int32Max * pGetMax);
}

//������������
double GetRand_Real(double pGetMax)
{
	double get_save;
	const double getrand_int32Max = 4294967295; // genrand_int32() �̍ő�l�B���Z�̂���double
	get_save = getrand_int32Max / 2;

	return ((double)genrand_int32() - get_save) / get_save*pGetMax;
}

void Make_Directory(std::string str, int method, int trial, int gene, int per, int k = 0) {
	std::stringstream File_Name;
	std::stringstream Tmp_File_Name;
	if (k == 0) {
		File_Name << "./" << str;
		CheckTheFolder::checkExistenceOfFolder(File_Name.str());
		File_Name << "/" << method;
		CheckTheFolder::checkExistenceOfFolder(File_Name.str());
		File_Name << "/" << trial;
		CheckTheFolder::checkExistenceOfFolder(File_Name.str());
		for (int g = 0; g < gene / per + 1; g++) {
			Tmp_File_Name << File_Name.str() << "/" << g;
			CheckTheFolder::checkExistenceOfFolder(Tmp_File_Name.str());
			std::cout << Tmp_File_Name.str() << std::endl;
			Tmp_File_Name.str("");
			Tmp_File_Name.clear(std::stringstream::goodbit);
		}
	}
	else {
		File_Name << "./" << str;
		CheckTheFolder::checkExistenceOfFolder(File_Name.str());
		File_Name << "/" << method;
		CheckTheFolder::checkExistenceOfFolder(File_Name.str());
		File_Name << "/" << trial;
		CheckTheFolder::checkExistenceOfFolder(File_Name.str());
		File_Name << "/" << k;
		CheckTheFolder::checkExistenceOfFolder(File_Name.str());
		for (int g = 0; g < gene / per + 1; g++) {
			Tmp_File_Name << File_Name.str() << "/" << g;
			CheckTheFolder::checkExistenceOfFolder(Tmp_File_Name.str());
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
	CheckTheFolder::checkExistenceOfFolder(fname.str());
	fname << "/" << method;
	CheckTheFolder::checkExistenceOfFolder(fname.str());
	fname << "/" << trial;
	CheckTheFolder::checkExistenceOfFolder(fname.str());
	fname << "/" << index;
	CheckTheFolder::checkExistenceOfFolder(fname.str());
}
void Make_Directory_AIT(int method, int trial) {
	std::stringstream File_Name;

	File_Name << "./AIT";
	CheckTheFolder::checkExistenceOfFolder(File_Name.str());
	File_Name << "/" << method;
	CheckTheFolder::checkExistenceOfFolder(File_Name.str());
	File_Name << "/" << trial;
	CheckTheFolder::checkExistenceOfFolder(File_Name.str());
}
void Make_Directory_GT2016(int trial, int gene, int per) {
	std::stringstream File_Name;
	std::stringstream Tmp_File_Name;

	File_Name << "./2016";
	CheckTheFolder::checkExistenceOfFolder(File_Name.str());
	File_Name << "/" << trial;
	CheckTheFolder::checkExistenceOfFolder(File_Name.str());

	for (int g = 0; g < gene / per + 1; g++) {
		Tmp_File_Name << File_Name.str() << "/" << g;
		CheckTheFolder::checkExistenceOfFolder(Tmp_File_Name.str());
		Tmp_File_Name.str("");
		Tmp_File_Name.clear(std::stringstream::goodbit);
	}
}
void Make_CSV_Directory(int method) {
	std::stringstream File_Name;
	std::stringstream Tmp_File_Name;

	File_Name << "./csv";
	CheckTheFolder::checkExistenceOfFolder(File_Name.str());
	File_Name << "/" << method;
	CheckTheFolder::checkExistenceOfFolder(File_Name.str());
}

//�x�N�^�[�̏���\�����邾��
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