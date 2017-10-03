#pragma once
/*=======================================
自分の卒論手法
=======================================*/
#include "config.hpp"
#include "coansmodule.hpp"
#include "CsvModules.h"
#include "CrusteringMethod.h"
#include "Usual_Methods.hpp"
#include <time.h>
#include <string>

/*卒論手法のクラス*/
class Coans_GT2016 {
	//公開メンバ
public:
	//手法のメインルーチン
	void	Coans_GT2016_Tasks(int Trial);
	void	Update_Opponent(int index);
	int		Cal_Gra_Nitch(int index);
	int		Get_MatchUp_Num();
	//戦略を書き込み
	void	File_Write_Pop(int trial, int gene, bool s1);
	//以下実験用
	void	SetVec_Cr_Pop(std::vector<playerTK> &Pop);	//クラスタ番号をCSVクラスへ
	void	SetVec_Re_Pop(std::vector<playerTK> &Pop);	//集団の対戦結果を記録
	void	SetVec_Re_Opp(std::vector<playerTK> &Opp);	//相手集団の対戦結果をCSVクラスへ
														//非公開メンバ
protected:
	Coans_GT2016(std::string str, int k = 0) {
		Machup_Num = 0;					//対戦回数
		Cr_Num = 1;						//クラスター番号
		Opp_Num = 0;					//対戦回数
		Method_Num = k;					//手法比較の時に使う
		Cru_K = 0;						//クラスタリングパラメーター
		Dir = str;
	}
	int Machup_Num;						//対戦回数
	int Cr_Num;							//クラスター番号
	int Opp_Num;						//対戦回数
	int Method_Num;						//手法比較の時に使う
	int	Cru_K;							//クラスタリングパラメーター
	int MainParent;						//主親
	std::string Dir;
	std::vector<int> SubParent;			//副親
	std::vector<playerTK> Pop;			//進化させる集団
	std::vector<playerTK> Child;		//子
	std::vector<playerTK> Opponent;		//対戦相手
	std::vector<double> Gra_Nitch_w1;
	std::vector<double> Gra_Nitch_w2;
	std::vector<double> Gra_Nitch_w3;
	//以下実験用
	std::vector<int> Cr_Pop;			//クラスタ番号を格納
	std::vector<int> Re_Pop;			//メインの対戦結果を格納
	std::vector<int> Re_Opp;			//対戦相手の
};
void Coans_GT2016::Coans_GT2016_Tasks(int Trial)
{
	std::cout << "Initiarize" << std::endl;

	//集団宣言
	Pop.resize(KO);
	Opponent.resize(KO);
	init_genrand((unsigned)time(NULL)); /*乱数初期化*/

										//集団初期化
	for (int i = 0; i < KO; i++) {
		Pop[i].Init();
		Pop[i].Init_w();
	}
	for (int i = 0; i < KO; i++) {
		Opponent[i].Init();
		Opponent[i].Init_w();
	}

	Make_Directory(Dir, Method_Num, Trial, KU, PER, Cru_K);
	CsvModules Csv1(Method_Num);
	Csv1.Init(Method_Num, Trial, KU, PER, Cru_K);
	Cr_Pop.resize(KO);
	Re_Pop.resize(KO);

	//初期世代の戦略を記録
	File_Write_Pop(Trial, 0, true);
	//任意の世代数ループ
	int Loop_Time_Start;
	int Loop_Time_End;
	int Loop_Time;
	exit(0);
	for (int Gene_Loop = 1; Gene_Loop < KU + 1; Gene_Loop++) {
		Loop_Time_Start = clock();
		std::cout << Method_Num << ":" << Trial << ":" << Cru_K << ":" << Gene_Loop;
		std::cout << "  |  ";
		std::cout << "1" << ',';

		//クラスタリング
		MakeList(Pop, K_List1, 0, 0);
		MakeList(Opponent, 0, K_List2, 0);
		Cr_Num = 1;
		for (int i = 0; i < KO; i++) {
			if (SetNitch(Cr_Num, i, Pop) == 1) {
				Cr_Num++;
			}
		}

		//実験用
		if (Gene_Loop % PER == 0 || Gene_Loop == 1) {
			std::cout << "9" << ',';
			//集団のクラスタ番号をベクターに格納
			SetVec_Cr_Pop(Pop);
			//格納したベクターをCsvクラスへ
			Csv1.SetCsv_Cr_P(Cr_Pop);
		}
		else {
			SetVec_Cr_Pop(Pop);
		}

		int tmpIndex;
		int tmpSub;
		SubParent.resize(0); //副親
		std::cout << "2" << ',';
		//主親選ぶ
		MainParent = GetRand_Int(KO);
		//副親を選ぶ
		for (int i = 0; i < K_List1; i++) {
			if (Pop[MainParent].List1.empty()) {
				break;
			}
			tmpIndex = GetRand_Int(int(Pop[MainParent].List1.size()));
			tmpSub = Pop[MainParent].List1[tmpIndex];
			SubParent.push_back(tmpSub); //要素を追加
			Pop[MainParent].List1.erase(Pop[MainParent].List1.begin() + tmpIndex); //追加した要素を削除
		}
		//副親があれば以下の処理を行う
		if (!SubParent.empty()) {
			//子個体生成
			Child.resize(CHILD + 1);

			std::cout << "3" << ',';
			//拡張XLM
			Child[0] = Pop[MainParent];
			ExtensionXLM(MainParent, SubParent, Pop, Child);
			//対戦相手の個体を選ぶ
			std::cout << "4" << ',';

			//w1,w2,w3:子個体の戦略，w1_T,w2_T,w3_T:対戦相手の戦略
			//player1:子個体，player2:対戦相手0
			std::cout << "5" << ',';
			//ここで対戦
			for (int r = 0; r < KO; r++) {//相手ループ
				StrategySet_T(Opponent[r]);
				for (int c = 0; c < CHILD + 1; c++) {//集団ループ
					StrategySet_M(Child[c]);
					Machup_Num++;
					Competition();//対戦 player1 = 子個体 palyer2 = 対戦相手？
					Opponent[r].Result[c] = (player2.hp - player1.hp) / 300;
					Child[c].Result[r] = (player1.hp - player2.hp) / 300;
				}
			}
			std::cout << "6" << ',';
			//適応度計算
			int Length = int(Child.size());
			for (int i = 0; i < Length; i++) {
				FitnessChild(Child[i], Opponent, false);
			}
			std::cout << "7" << ',';
			//Best個体を集団へ
			int Index = Choice_Best_Index(Child, false);
			//Best → Child[Index];
			if (Index != 0) {
				if (Child[0].eval < Child[Index].eval) {
					//削除予定の個体があれば
					int k;
					for (k = 0; Opponent[k].delete_flag == 0 && k < KO; k++) {}
					if (k != KO) {
						Opponent[k] = Child[Index];
						Opponent[k].comp_flag = 1;
						Opponent[k].delete_flag = 0;
					}if (k == KO) {
						Update_Opponent(Index);
					}
				}
			}
			for (int i = 0; i < KO; i++) {
				if (Opponent[i].delete_flag != 1) {
					if (Opponent[i].comp_flag == 0) {
						Opponent[i].gene_count++;
					}
					else if (Opponent[i].comp_flag == 1) {
						Opponent[i].gene_count = 0;
					}
					Opponent[i].comp_flag = 0;
					if (DELETE <= Opponent[i].gene_count) {
						Opponent[i].delete_flag = 1;
						Opponent[i].nitch = 0;
					}
				}
			}
			//実験用
			if (Gene_Loop % PER == 0) {
				std::cout << "10" << ',';
				//100世代毎に戦略を記録
				File_Write_Pop(Trial, Gene_Loop / PER, true);
			}
			std::cout << "8";
			std::cout << "  [";
			for (int c = 0; c < CHILD + 1; c++) {
				std::cout << Child[c].eval << ",";
			}
			std::cout << "]" << std::endl;
			//集団の解以外初期化
			for (int i = 0; i < KO; i++) {
				Pop[i].Init();
			}
			std::cout << "  [";
			int Cr_Pop_Length = int(Cr_Pop.size());
			for (int cp = 0; cp < Cr_Pop_Length; cp++) {
				std::cout << Cr_Pop[cp] << ",";
			}
			std::cout << "]" << std::endl;
		}
		Loop_Time_End = clock();
		Loop_Time = (Loop_Time_End - Loop_Time_Start) / CLOCKS_PER_SEC;
		std::cout << "Time per gene : " << Loop_Time << " [sec]" << std::endl;
		assert(Gene_Loop <= KU);
	}
	Csv1.Fwrite_Cr_P();
	//Csv1.Fwrite_Re_P(C);
}
void Coans_GT2016::SetVec_Cr_Pop(std::vector<playerTK> &Pop) {
	//初期化
	Cr_Pop.assign(KO, 0);
	for (int i = 0; i < KO; i++) {
		assert(Pop[i].nitch < KO);
		assert(0 <= Pop[i].nitch);
		Cr_Pop[Pop[i].nitch]++;
	}
	Cr_Pop.erase(std::remove(Cr_Pop.begin(), Cr_Pop.end(), 0), Cr_Pop.end());
	assert(!Cr_Pop.empty());
	/*
	std::cout << "[";
	int Cr_Pop_Length = int(Cr_Pop.size());
	for (int cp = 0; cp < Cr_Pop_Length; cp++) {
	std::cout << Cr_Pop[cp] << ",";
	}
	std::cout << "]" << std::endl;
	*/
}
void Coans_GT2016::SetVec_Re_Pop(std::vector<playerTK> &Pop) {
	for (int i = 0; i < KO; i++) {
		//対戦結果のベクターの和を格納する
		Re_Pop[i] = int(std::accumulate(Pop[i].Result.begin(), Pop[i].Result.end(), 0.0));
	}
}
void Coans_GT2016::SetVec_Re_Opp(std::vector<playerTK> &Opp) {
	for (int i = 0; i < Opp_Num; i++) {
		//対戦結果のベクターの和を格納する
		Re_Opp[i] = int(std::accumulate(Opp[i].Result.begin(), Opp[i].Result.end(), 0.0));
	}
}
int Coans_GT2016::Get_MatchUp_Num() {
	return Machup_Num;
}
void Coans_GT2016::File_Write_Pop(int trial, int gene, bool s1)
{
	//様式を合わせる
	for (int i = 0; i < KO; i++) {
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
	//bef = trueの時AI，falseの時AIC
	char filename[50];
	for (int i = 0; i < KO; i++) {
		if (Method_Num == 0) {
			sprintf(filename, ("AI/%d/%d/%d/%d.dat"), Method_Num, trial, gene, i);
		}
		else if (0 < Method_Num) {
			sprintf(filename, ("AI/%d/%d/%d/%d/%d.dat"), Method_Num, trial, Cru_K, gene, i);
		}
		if ((fp = fopen(filename, "wb+")) == NULL) {
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		/*
		else {
		sprintf(filename, ("AIT/%d/%d.dat"), trial, i);
		if ((fp = fopen(filename, "wb+")) == NULL) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
		}
		}
		*/
		fwrite(w1_GA[i], sizeof(double), I1*J1, fp);
		fwrite(w2_GA[i], sizeof(double), I2*J2, fp);
		fwrite(w3_GA[i], sizeof(double), I2*J1, fp);
		fclose(fp);
	}
}

void Coans_GT2016::Update_Opponent(int index)
{
	/*いらない相手を残すと有害。勝ち数で残す個体判断しているので、余計な勝ち数がカウントされる。*/
	/*距離が最小のニッチの重心を見つけて、ニッチ番号を取得*/
	int obj_count;
	int Nit_Count = 0;
	double min_dis;
	double tmp_dis;

	//一番近いクラスタの番号を入れる
	Child[index].nitch = Cal_Gra_Nitch(index);

	/*戦闘データ初期化*/
	for (int i = 0; i<KO; i++) {
		if (Opponent[i].nitch == Child[index].nitch) {
			Opponent[i].eval = 0;
			Opponent[i].flag = 0;
			Nit_Count++;
		}
	}
	//対戦用のベクターを作る
	std::vector<playerTK> Oppc(Nit_Count+1);
	std::vector<double> Oppc_Eval( Nit_Count+1 );
	for (int i = 0; i < KO; i++) {
		if (Opponent[i].nitch == Child[index].nitch) {
			Oppc[Nit_Count] = Opponent[i];
			Nit_Count--;
		}
	}
	assert(0 <= Nit_Count);
	Child[index].eval = 0;
	Child[index].flag = 0;
	Oppc[0] = Child[index];

	/*該当するニッチの個体と子個体でリーグ戦*/
	int Oppc_size = int( Oppc.size() );
	for (int i = 0; i < Oppc_size; i++) {
		//自クラスタ内の個体と対戦
		Oppc[i].Result.resize(Oppc_size - 1);
		StrategySet_M(Oppc[i]);
		for (int j = i + 1; j < Oppc_size - 1; j++) {
			StrategySet_T(Oppc[j]);
			Competition();//対戦 player1 = 子個体 palyer2 = 対戦相手？
			Oppc[i].Result[j] = (player1.hp - player2.hp) / 300;
			Oppc[j].Result[i] = (player2.hp - player1.hp) / 300;
			Machup_Num++;
		}
		//子個体と対戦
		Opponent[i].flag = 1; 		/*フラグ立て*/
		Opponent[i].comp_flag = 1; 	/*生存競争したフラグ*/
	}
	for (int i = 0; i < Oppc_size; i++) {
		Oppc_Eval[i] = std::accumulate(Oppc[i].Result.begin(), Oppc[i].Result.end(), 0.0);
	}

	/*評価値の最小個体を求める*/
	auto min = min_element(Oppc_Eval.begin(), Oppc_Eval.end());
	int min_index = int(distance(Oppc_Eval.begin(), min));

	/*削除予定の個体の枠へ子個体を入れる、または相手集団の中で子個体より弱い相手のところに入れる*/
	int j;
	for (int i = 0; i < Oppc_size; i++) {
		if (min_index != i) {
			for (j = 0; j < KO && Opponent[j].nitch != Child[index].nitch; j++) {}
			Opponent[j] = Oppc[i];
			Opponent[j].comp_flag = 1;
			Opponent[j].delete_flag = 0;
		}
	}
	/*対戦データおよびニッチ番号初期化*/
	for (int i = 0; i < KO; i++) {
		Opponent[i].flag = 0;		Opponent[i].eval = 0;		Opponent[i].nitch = 0;		Opponent[i].List2.clear();	}
	MakeList(Opponent, 0, K_List2, 0);
}
int Coans_GT2016::Cal_Gra_Nitch(int index)
{
	std::vector<int> Nitch_Num;
	std::vector< std::vector < std::vector<double> > > Sum_N_W1;
	std::vector< std::vector < std::vector<double> > > Sum_N_W2;
	std::vector< std::vector < std::vector<double> > > Sum_N_W3;

	Nitch_Num = std::vector<int>(Cr_Num, 0);
	Sum_N_W1 = std::vector<std::vector<std::vector<double>>>(Cr_Num, std::vector<std::vector<double>>(I1, std::vector<double>(J1, 0)));
	Sum_N_W2 = std::vector<std::vector<std::vector<double>>>(Cr_Num, std::vector<std::vector<double>>(I2, std::vector<double>(J2, 0)));
	Sum_N_W3 = std::vector<std::vector<std::vector<double>>>(Cr_Num, std::vector<std::vector<double>>(J1, std::vector<double>(I2, 0)));

	for (int Opp = 0; Opp < KO; Opp++) {
		//親の解のそれぞれのベクトルを足す
		//w1_CO
		int Opp_Nitch = Opponent[Opp].nitch;
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				Sum_N_W1[Opp_Nitch][j][k] += Opponent[Opp].w1_CO[j][k];
			}
		}
		//w2_CO
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				Sum_N_W2[Opp_Nitch][j][k] += Opponent[Opp].w2_CO[j][k];
			}
		}
		//w3_CO
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				Sum_N_W3[Opp_Nitch][j][k] += Opponent[Opp].w3_CO[j][k];
			}
		}
		Nitch_Num[Opp_Nitch]++;
	}
	//ベクトルの重心を求める
	//w1_CO
	for (int Nit = 0; Nit < Cr_Num; Nit++) {
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				Sum_N_W1[Nit][j][k] = Sum_N_W1[Nit][j][k] / Nitch_Num[Nit];
			}
		}
		//w2_CO
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				Sum_N_W2[Nit][j][k] = Sum_N_W2[Nit][j][k] / Nitch_Num[Nit];
			}
		}
		//w3_CO
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				Sum_N_W3[Nit][j][k] = Sum_N_W3[Nit][j][k] / Nitch_Num[Nit];
			}
		}
	}
	//重心と子個体の距離計算
	double cal_sum = 0;
	std::vector<double> Dis_Vector(Cr_Num);
	//w1の距離
	for (int Nit = 0; Nit < Cr_Num; Nit++) {
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				Dis_Vector[Nit] += (Sum_N_W1[Nit][j][k] - Child[index].w1_CO[j][k])*(Sum_N_W1[Nit][j][k] - Child[index].w1_CO[j][k]);
			}
		}
		//w2の距離
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				Dis_Vector[Nit] += (Sum_N_W2[Nit][j][k] - Child[index].w2_CO[j][k])*(Sum_N_W2[Nit][j][k] - Child[index].w2_CO[j][k]);
			}
		}
		//w3の距離
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				Dis_Vector[Nit] += (Sum_N_W3[Nit][j][k] - Child[index].w3_CO[j][k])*(Sum_N_W3[Nit][j][k] - Child[index].w3_CO[j][k]);
			}
		}
		Dis_Vector[Nit] = sqrt(Dis_Vector[Nit]);
	}
	auto min = min_element(Dis_Vector.begin(), Dis_Vector.end());
	auto min_Nit = int(distance(Dis_Vector.begin(), min));
	return min_Nit;
}