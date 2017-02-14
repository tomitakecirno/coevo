#pragma once
#include "MT.h"

/*****************
���R����������
*****************/
int GetRand_Int(int pGetMax)
{
	const double getrand_int32Max = 4294967295; // genrand_int32() �̍ő�l�B���Z�̂���double
	return int(genrand_int32() / getrand_int32Max * pGetMax);
}

/*****************
������������
*****************/
double GetRand_Real(double pGetMax)
{
	double get_save;
	const double getrand_int32Max = 4294967295; // genrand_int32() �̍ő�l�B���Z�̂���double
	get_save = getrand_int32Max / 2;

	return ((double)genrand_int32() - get_save) / get_save*pGetMax;
}