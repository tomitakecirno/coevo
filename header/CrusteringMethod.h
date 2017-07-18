#pragma once
#include "Cruster_Config.h"

int Cal_Vec_Length(int Pop_Size);

/*
void Cru_Upgma(std::vector<Player> &Pop) {
	int Vec_Length = Cal_Vec_Length(int(Pop.size()));
	//Å¬‹——£‚ÌŒÂ‘Ì‚ÌƒyƒA‚ğ‹‚ß‚é
	vector<>Vec_Dis
}
*/

int Cal_Vec_Length(int Pop_Size) {
	int Length = 0;
	for (int i = Pop_Size-1; 0 < i; i--) {
		Length += i;
	}
	return Length;
}
