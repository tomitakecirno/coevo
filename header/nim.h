#pragma once
#include<iostream>
#include<vector>

#define NIM		3	//ニムの山の数
#define POLL1	3	//山1
#define POLL2	3	//山2
#define POLL3	3	//山3

class nim {
public:
	nim()
	{
		nim_n = NIM;
		nim_status.resize(nim_n);
		for (int i = 0; i < nim_n; i++) {
			nim_status[i] = POLL1;
		}
	}

	void get_nim(std::vector<int> n);
protected:
	int nim_n;	//山の数
	std::vector<int> nim_status; //それぞれの山の石の数
};

void nim::get_nim(std::vector<int> n) {
	n.resize(nim_n);
	for (int i = 0; i < nim_n; i++) {
		n[i] = nim_status[i];
	}
}