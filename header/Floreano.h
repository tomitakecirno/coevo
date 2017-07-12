#pragma once
#include "config.hpp"
#include <iostream>

class FloreMethods {
public :
	void Main_Tasks();
	void Fwrite_Floreano();
private:
	std::vector<playerTK> Pop;
	std::vector<playerTK> Opp;
protected:
	void BLX(std::vector<playerTK> &Tmp_Pop);
};

/*����BLX*/
void FloreMethods::BLX(std::vector<playerTK> &Tmp_Pop)
{
	using namespace std;
	double dx, min_ca, max_ca;
	int tmp1, tmp2;
	vector<vector<double> > tmpw1(I1);
	vector<vector<double> > tmpw2(I2);
	vector<vector<double> > tmpw3(J1);

	if (Tmp_Pop.empty()) {
		cout << "Tmp_Pop����ł�" << endl;
		exit(0);
	}
	int Tmp_Pop_Length = int(Tmp_Pop.size());
	for (int p = Tmp_Pop_Length/5; p < KO; p++) {
		while (1) {
			tmp1 = GetRand_Int(KO / 5);
			tmp2 = GetRand_Int(KO / 5);
			if (tmp1 == tmp2) {
				break;
			}
		}
		Tmp_Pop[p].w1_CO.resize(I1);
		for (int i = 0; i < I1; i++) {
			Tmp_Pop[p].w1_CO[i].resize(J1);
			for (int j = 0; j < J1; j++) {
				//���̐�Βl
				dx = fabs(Tmp_Pop[tmp1].w1_CO[i][j] - Tmp_Pop[tmp2].w1_CO[i][j]);
				min_ca = fmin(Tmp_Pop[tmp1].w1_CO[i][j], Tmp_Pop[tmp2].w1_CO[i][j]) - 0.3*dx;
				max_ca = fmax(Tmp_Pop[tmp1].w1_CO[i][j], Tmp_Pop[tmp2].w1_CO[i][j]) + 0.3*dx;
				Tmp_Pop[p].w1_CO[i][j] = min_ca + GetRand_Real(max_ca - min_ca);
			}
		}
		Tmp_Pop[p].w2_CO.resize(I2);
		for (int i = 0; i < I2; i++) {
			Tmp_Pop[p].w2_CO[i].resize(J2);
			for (int j = 0; j < J2; j++) {
				dx = fabs(Tmp_Pop[tmp1].w2_CO[i][j] - Tmp_Pop[tmp2].w2_CO[i][j]);
				min_ca = fmin(Tmp_Pop[tmp1].w2_CO[i][j], Tmp_Pop[tmp2].w2_CO[i][j]) - 0.3*dx;
				max_ca = fmax(Tmp_Pop[tmp1].w2_CO[i][j], Tmp_Pop[tmp2].w2_CO[i][j]) + 0.3*dx;
				Tmp_Pop[p].w2_CO[i][j] = min_ca + GetRand_Real(max_ca - min_ca);
			}
		}
		Tmp_Pop[p].w3_CO.resize(J1);
		for (int i = 0; i < J1; i++) {
			Tmp_Pop[p].w3_CO[i].resize(I2);
			for (int j = 0; j < I2; j++) {
				dx = fabs(Tmp_Pop[tmp1].w3_CO[i][j] - Tmp_Pop[tmp2].w3_CO[i][j]);
				min_ca = fmin(Tmp_Pop[tmp1].w3_CO[i][j], Tmp_Pop[tmp2].w3_CO[i][j]) - 0.3*dx;
				max_ca = fmax(Tmp_Pop[tmp1].w3_CO[i][j], Tmp_Pop[tmp2].w3_CO[i][j]) + 0.3*dx;
				Tmp_Pop[p].w3_CO[i][j] = min_ca + GetRand_Real(max_ca - min_ca);
			}
		}
	}
}
/*Floreano�̎�@*/
void FloreMethods::Main_Tasks(void) {
	using namespace std;
	//�����W�c����

	Pop.resize(KO);
	Opp.resize(FLORET);

	init_genrand((unsigned)time(NULL));
	//�W�c������

	vector<int> tmpN(KO);
	for (int i = 0; i < KO; i++) {
		Pop[i].Init();
		Pop[i].Init_w();
		tmpN[i] = i;
	}
	int tmprand;
	//����W�c�擾
	for (int i = 0; i<FLORET; i++) {
		tmprand = GetRand_Int( int(tmpN.size()) );
		//����W�c�։�����
		Opp[i] = Pop[tmpN[tmprand]];
		tmpN.erase(tmpN.begin() + tmprand);
	}
	//�ȉ����[�v
	for (int e = 0; e < KU; e++) {
		cout << "���㐔:" << e << endl;
		/*�ΐ�*/

		cout << "�W�c��:" << int(Pop.size()) << endl;
		for (int i = 0; i < KO; i++) {
			Pop[i].Result.resize(FLORET);
		}
		for (int i = 0; i < FLORET; i++) {
			Opp[i].Result.resize(KO);
		}
		for (int i = 0; i < KO; i++) {
			for (int j = 0; j < FLORET; j++) {
				StrategySet_M(Pop[i]);
				StrategySet_T(Opp[j]);
				Competition();
				if (player1.win == 1) {
					Pop[i].Result[j] = 1;
					Opp[j].Result[i] = 0;
				}
				else if(player1.win == 0) {
					Pop[i].Result[j] = 0;
					Opp[j].Result[i] = 1;
				}
				else {
					Pop[i].Result[j] = 1;
					Opp[j].Result[i] = 0;
				}
				/*
				Pop[i].Result[j] = (player1.hp - player2.hp) / 300;
				Opp[j].Result[i] = (player2.hp - player1.hp) / 300;
				*/
			}
		}
		//�]���l�v�Z
		for (int i = 0; i < FLORET; i++) {
			FitnessChild(Opp[i], Pop, true);
		}
		for (int i = 0; i < KO; i++) {
			FitnessChild(Pop[i], Opp, true);
		}
		
		//�m�F�p
		cout << "�]���l";
		for (int i = 0; i < KO; i++) {
			cout << Pop[i].eval << ',';
		}
		cout << endl;

		vector<double> tmpEval(KO);
		vector<playerTK> Tmp_Pop(KO);
		//��U�ۑ�
		for (int i = 0; i < KO; i++) {
			tmpEval[i] = Pop[i].eval;
		}
		//���1/5���c��
		int count_Num;
		int Count_Rand;
		vector<double>::iterator index;
		for (int i = 0; i < KO / 5; i++) {
			double max = *max_element(tmpEval.begin(), tmpEval.end());
			//�����]���n�̌̂���������ꍇ�̓����_��
			count_Num = count(tmpEval.begin(), tmpEval.end(), max);
			cout << "count_Num:" << count_Num << endl;
			if (count_Num == 1) {
				//�C���f�b�N�X���擾
				index = find(tmpEval.begin(), tmpEval.end(), max);
			}
			else if(1 < count_Num){
				Count_Rand = GetRand_Int(count_Num);
				int Count_Max = 0;
				int tmpEval_Length = int(tmpEval.size());
				index = tmpEval.begin();
				for (int j = 0; j < Count_Rand+1; j++) {
					if (j != 0) {
						index++;
					}
					index = find(index, tmpEval.end(), max);
				}
			}
			Tmp_Pop[i] = Pop[int(*index)];
			Pop.erase(Pop.begin() + int(*index)); //�v�f���폜
			tmpEval.erase(tmpEval.begin() + int(*index)); //�v�f���폜
			//cout << "index:" << int(*index) << endl;
		}
		//�ΐ푊��̍X�V.�ŌÂ̌̂�u��������
		//e%FLORET�Œu��������̂��擾�ł���D
		Opp[e%FLORET] = Tmp_Pop[0];
		//����
		BLX(Tmp_Pop);
		Pop.resize(KO);
		Pop = Tmp_Pop;
		for (int i = 0; i < KO; i++) {
			Pop[i].Init();
		}
		for (int j = 0; j < FLORET; j++) {
			Opp[j].Init();
		}
	}
}
/*���C���W�c��������*/
void FloreMethods::Fwrite_Floreano()
{
	int PopLength = int(Pop.size());
	//�l�������킹��
	for (int i = 0; i < PopLength; i++) {
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				w1_GA[i][j][k] = Pop[i].w1_CO[j][k];
			}
		}
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				w2_GA[i][j][k] = Pop[i].w2_CO[j][k];
			}
		}
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				w3_GA[i][j][k] = Pop[i].w3_CO[j][k];
			}
		}
	}
	//dat�t�@�C���ɏ�������
	for (int i = 0; i < PopLength; i++) {
		sprintf(filename, ("AI_Opp/Floreano/%d.dat"), i);
		if ((fp = fopen(filename, "wb+")) == NULL) {
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		fwrite(w1_GA[i], sizeof(double), I1*J1, fp);
		fwrite(w2_GA[i], sizeof(double), I2*J2, fp);
		fwrite(w3_GA[i], sizeof(double), I2*J1, fp);
		fclose(fp);
	}
}