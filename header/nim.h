#pragma once
#include<iostream>
#include<vector>

#define NIM		3	//�j���̎R�̐�
#define POLL1	3	//�R1
#define POLL2	3	//�R2
#define POLL3	3	//�R3

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
	int nim_n;	//�R�̐�
	std::vector<int> nim_status; //���ꂼ��̎R�̐΂̐�
};

void nim::get_nim(std::vector<int> n) {
	n.resize(nim_n);
	for (int i = 0; i < nim_n; i++) {
		n[i] = nim_status[i];
	}
}