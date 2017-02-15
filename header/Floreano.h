#include "config.hpp"
#include<iostream>

void FileFloreanoWrite(std::vector<playerTK> &pop);
void FloreanoEval(void);
void BLX(std::vector<playerTK> &tmppop);

/*����BLX*/
void BLX(std::vector<playerTK> &tmppop)
{
	using namespace std;
	double dx, min_ca, max_ca;
	int tmp1, tmp2;

	vector<vector<double> > tmpw1(I1);
	vector<vector<double> > tmpw2(I2);
	vector<vector<double> > tmpw3(J1);

	for (int p = 6; p < KO; p++) {
		while (1) {
			tmp1 = GetRand_Int(KO / 5);
			tmp2 = GetRand_Int(KO / 5);
			if (tmp1 != tmp2) {
				break;
			}
		}
		tmppop[p].w1_CO.resize(I1);
		for (int i = 0; i < I1; i++) {
			tmppop[p].w1_CO[i].resize(J1);
			for (int j = 0; j < J1; j++) {
				//���̐�Βl
				dx = fabs(tmppop[tmp1].w1_CO[i][j] - tmppop[tmp2].w1_CO[i][j]);
				min_ca = fmin(tmppop[tmp1].w1_CO[i][j], tmppop[tmp2].w1_CO[i][j]) - 0.3*dx;
				max_ca = fmax(tmppop[tmp1].w1_CO[i][j], tmppop[tmp2].w1_CO[i][j]) + 0.3*dx;
				tmpw1[i][j] = min_ca + GetRand_Real(max_ca - min_ca);
			}
		}
		tmppop[p].w2_CO.resize(I2);
		for (int i = 0; i < I2; i++) {
			tmppop[p].w2_CO[i].resize(J2);
			for (int j = 0; j < J2; j++) {
				dx = fabs(tmppop[tmp1].w2_CO[i][j] - tmppop[tmp2].w2_CO[i][j]);
				min_ca = fmin(tmppop[tmp1].w2_CO[i][j], tmppop[tmp2].w2_CO[i][j]) - 0.3*dx;
				max_ca = fmax(tmppop[tmp1].w2_CO[i][j], tmppop[tmp2].w2_CO[i][j]) + 0.3*dx;
				tmpw2[i][j] = min_ca + GetRand_Real(max_ca - min_ca);
			}
		}
		tmppop[p].w3_CO.resize(J1);
		for (int i = 0; i < J1; i++) {
			tmppop[p].w3_CO[i].resize(I2);
			for (int j = 0; j < I2; j++) {
				dx = fabs(tmppop[tmp1].w3_CO[i][j] - tmppop[tmp2].w3_CO[i][j]);
				min_ca = fmin(tmppop[tmp1].w3_CO[i][j], tmppop[tmp2].w3_CO[i][j]) - 0.3*dx;
				max_ca = fmax(tmppop[tmp1].w3_CO[i][j], tmppop[tmp2].w3_CO[i][j]) + 0.3*dx;
				tmpw3[i][j] = min_ca + GetRand_Real(max_ca - min_ca);
			}
		}
	}
}

/*Floreano�̎�@*/
void FloreanoEval(void) {
	using namespace std;
	//�����W�c����
	vector<playerTK> pop(KO);
	vector<playerTK> oppoment(FLORET);

	init_genrand((unsigned)time(NULL));
	//�W�c������

	vector<int> tmpN(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].Init();
		pop[i].Init_w();
		tmpN[i] = i;
	}
	int tmprand;
	//����W�c�擾
	for (int i = 0; i<FLORET; i++) {
		tmprand = GetRand_Int(tmpN.size());
		//����W�c�։�����
		oppoment[i] = pop[tmpN[tmprand]];
		tmpN.erase(tmpN.begin() + tmprand);
	}
	//�ȉ����[�v
	for (int e = 0; e < KU; e++) {
		cout << e << endl;
		/*�ΐ�*/
		for (int i = 0; i < KO; i++) {
			pop[i].Result.resize(FLORET);
		}
		for (int i = 0; i < FLORET; i++) {
			oppoment[i].Result.resize(KO);
		}
		for (int i = 0; i < KO; i++) {
			for (int j = 0; j < FLORET; j++) {
				StrategySet_M(pop[i]);
				StrategySet_T(oppoment[j]);
				Competition();
				pop[i].Result[j] = (player1.hp - player2.hp) / 300;
				oppoment[j].Result[i] = (player2.hp - player1.hp) / 300;
			}
		}
		//�]���l�v�Z
		for (int i = 0; i < FLORET; i++) {
			FitnessChild(oppoment[i], pop, true);
		}
		for (int i = 0; i < KO; i++) {
			FitnessChild(pop[i], oppoment, true);
		}
		vector<double> tmpEval;
		vector<playerTK> tmppop(KO);
		//��U�ۑ�
		for (int i = 0; i < KO; i++) {
			tmpEval.push_back(pop[i].eval);
		}
		//���1/5���c��
		for (int i = 0; i < KO / 5; i++) {
			vector<double>::iterator max = max_element(tmpEval.begin(), tmpEval.end());
			size_t index = distance(tmpEval.begin(), max);
			tmppop[i] = pop[index];
			tmpEval[index] = 0;
		}
		//�ΐ푊��̍X�V
		//e%FLORET�Œu��������̂��擾�ł���D
		oppoment[e%FLORET] = tmppop[0];
		//����
		BLX(tmppop);
		pop = tmppop;
		for (int i = 0; i < KO; i++) {
			pop[i].Init();
		}
		for (int j = 0; j < FLORET; j++) {
			oppoment[j].Init();
		}
	}
	FileFloreanoWrite(pop);
	exit(0);
}

/*���C���W�c��������*/
void FileFloreanoWrite(std::vector<playerTK> &pop)
{
	int PopLength = pop.size();
	//�l�������킹��
	for (int i = 0; i < PopLength; i++) {
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				w1_GA[i][j][k] = pop[i].w1_CO[j][k];
			}
		}
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				w2_GA[i][j][k] = pop[i].w2_CO[j][k];
			}
		}
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				w3_GA[i][j][k] = pop[i].w3_CO[j][k];
			}
		}
	}
	//bef = true�̎�AI�Cfalse�̎�AIC
	for (int i = 0; i < PopLength; i++) {
		sprintf(filename, ("AIT/4/%d.dat"), i);
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