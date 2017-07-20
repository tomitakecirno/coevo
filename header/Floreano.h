#pragma once
#include "config.hpp"
#include <iostream>

class FloreMethods {
public :
	void Main_Tasks();
	void Fwrite_Floreano(int trial);
private:
	std::vector<playerTK> Pop;
	std::vector<playerTK> Opp;
protected:
	void BLX(std::vector<playerTK> &Tmp_Pop);
};

/*交叉BLX*/
void FloreMethods::BLX(std::vector<playerTK> &Tmp_Pop)
{
	using namespace std;
	double dx, min_ca, max_ca;
	int tmp1, tmp2;
	vector<vector<double> > tmpw1(I1);
	vector<vector<double> > tmpw2(I2);
	vector<vector<double> > tmpw3(J1);

	if (Tmp_Pop.empty()) {
		cout << "Tmp_Popが空です" << endl;
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
				//差の絶対値
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
/*Floreanoの手法*/
void FloreMethods::Main_Tasks(void) {
	using namespace std;
	//初期集団生成

	Pop.resize(KO);
	Opp.resize(FLORET);

	init_genrand((unsigned)time(NULL));
	//集団初期化

	vector<int> tmpN(KO);
	for (int i = 0; i < KO; i++) {
		Pop[i].Init();
		Pop[i].Init_w();
		tmpN[i] = i;
	}
	int tmprand;
	//相手集団取得
	for (int i = 0; i<FLORET; i++) {
		tmprand = GetRand_Int( int(tmpN.size()) );
		//相手集団へ加える
		Opp[i] = Pop[tmpN[tmprand]];
		tmpN.erase(tmpN.begin() + tmprand);
	}
	//以下ループ
	for (int e = 0; e < F_KU; e++) {
		cout << "世代数:" << e << endl;
		/*対戦*/
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
				/*
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
				*/
				Pop[i].Result[j] = (player1.hp - player2.hp);
				Opp[j].Result[i] = (player2.hp - player1.hp);
			}
		}
		//評価値計算
		for (int i = 0; i < KO; i++) {
			FitnessChild(Pop[i], Opp, false);
		}
		
		//確認用
		cout << "評価値";
		for (int i = 0; i < KO; i++) {
			cout << Pop[i].eval << ',';
		}
		cout << endl;

		vector<playerTK> Tmp_Pop(KO);

		//上位1/5を残す
		//int count_Num;
		for (int i = 0; i < KO / 5; i++) {
			int index = Choice_Best_Index(Pop);
			//cout << "index:" << index << endl;
			Tmp_Pop[i] = Pop[index];
			Pop.erase(Pop.begin() + index); //要素を削除
		}
		//対戦相手の更新.最古の個体を置き換える
		//e%FLORETで置き換える個体を取得できる．
		//Opp[e%FLORET] = Tmp_Pop[0];
		//交叉
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
/*メイン集団書き込み*/
void FloreMethods::Fwrite_Floreano(int trial)
{
	int PopLength = int(Pop.size());
	//様式を合わせる
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
	//datファイルに書き込み
	char filename[50];
	int method = 0;
	sprintf(filename, ("AIT/%d/%d"), method, trial);
	CheckTheFolder::checkExistenceOfFolder(filename);
	for (int i = 0; i < PopLength; i++) {
		sprintf(filename, ("AIT/%d/%d/%d.dat"), method, trial, i);
		if ((fp = fopen(filename, "wb+")) == NULL) {
			printf("file open error!!\n");
			exit(0);
		}
		fwrite(w1_GA[i], sizeof(double), I1*J1, fp);
		fwrite(w2_GA[i], sizeof(double), I2*J2, fp);
		fwrite(w3_GA[i], sizeof(double), I2*J1, fp);
		fclose(fp);
	}
}