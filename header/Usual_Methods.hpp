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
	const double getrand_int32Max = 4294967295; // genrand_int32() の最大値。除算のためdouble
	return int(genrand_int32() / getrand_int32Max * pGetMax);
}

//実数乱数生成
double GetRand_Real(double pGetMax)
{
	double get_save;
	const double getrand_int32Max = 4294967295; // genrand_int32() の最大値。除算のためdouble
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
//パス名を受け取ってフォルダを生成する
void Check_Directory(std::string pass) {
	std::vector<std::string> v;
	std::stringstream ss(pass);
	std::string buffer;
	while (std::getline(ss, buffer,'/')) {
		v.push_back(buffer);
	}

	int v_size = int(v.size());
	std::stringstream fname;
	fname << "./";
	for (int i = 1; i < v_size-1; i++) {
		fname << v[i];
		CheckTheFolder::checkExistenceOfFolder(fname.str());
		fname << "/";
	}
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
	File_Name << "/Cruster";
	CheckTheFolder::checkExistenceOfFolder(File_Name.str());
	File_Name << "/" << method;
	CheckTheFolder::checkExistenceOfFolder(File_Name.str());

	Tmp_File_Name << "./csv/PopResult";
	CheckTheFolder::checkExistenceOfFolder(Tmp_File_Name.str());
	Tmp_File_Name << "/" << method;
	CheckTheFolder::checkExistenceOfFolder(Tmp_File_Name.str());
}