#pragma once
/*=======================================
自分の各種手法まとめ
CoansMode1	現手法：現評価方法(評価値に補正を加える)
CoansMode2	現手法：前評価方法(評価値に補正を加えない)
CoansMode3	前手法：現評価方法
CoansMode4	前手法：前評価方法
=======================================*/
#include "config.hpp"
#include "coansmodule.hpp"
#include "CsvModules.h"

/*手法のテンプレート*/
class Coans{
//公開メンバ
public :
	//手法のメインルーチン
	void Coans_Tasks(int Trial);
	//対戦回数を取得
	int Get_MatchUp_Num();
	//戦略を書き込み
	void File_Write_Pop(int trial, bool s1);
	//以下実験用
	void SetVec_Cr_Pop(std::vector<playerTK> &Pop);	//クラスタ番号をCSVクラスへ
	void SetVec_Re_Pop(std::vector<playerTK> &Pop);	//集団の対戦結果を記録
	void SetVec_Re_Opp(std::vector<playerTK> &Opp);	//相手集団の対戦結果をCSVクラスへ
//非公開メンバ
private:
	virtual void Set_Method() = 0;
	virtual void Crustering() = 0;
	virtual void Generate_Opp() = 0;
	virtual void Return_Re(int Child_Num, int Opp_Num) = 0;
	virtual void Cal_Fitness() = 0;
//このクラスと派生クラスからアクセス可能
protected:
	int Machup_Num;					//対戦回数
	int Cr_Num;					//クラスター番号
	int Opp_Num;					//対戦回数
	int MainParent;						//主親
	int Method_Num;						//手法比較の時に使う
	std::vector<int> SubParent;			//副親
	std::vector<playerTK> Pop;			//進化させる集団
	std::vector<playerTK> Child;		//子
	std::vector<playerTK> Opponent;		//対戦相手
	//以下実験用
	std::vector<int> Cr_Pop; //クラスタ番号を格納
	std::vector<int> Re_Pop; //メインの対戦結果を格納
	std::vector<int> Re_Opp; //対戦相手の
};
/*手法のメインルーチン*/
void Coans::Coans_Tasks(int Trial)
{
	std::cout << "Initiarize" << std::endl;

	Set_Method();

	//集団宣言
	Pop.resize(KO);
	init_genrand((unsigned)time(NULL)); /*乱数初期化*/

	//集団初期化
	for (int i = 0; i < KO; i++) {
		Pop[i].Init();
		Pop[i].Init_w();
	}

	std::cout << "Csv class generate" << std::endl;
	CsvModules Csv1;
	Csv1.Init(Trial, KU, PER);
	Cr_Pop.resize(KO);
	Re_Pop.resize(KO);

	Machup_Num = 0;
	//任意の世代数ループ
	for (int gene = 0; gene < KU; gene++) {
		std::cout << Trial << ":" << gene;
		std::cout << "  |  ";
		std::cout << "1" << ',';
		Crustering(); //クラスタリング。手法によって変わる
		//実験用
		if (gene % PER == 0) {
			std::cout << "9" << ',';
			//集団のクラスタ番号をベクターに格納
			SetVec_Cr_Pop(Pop);
			//格納したベクターをCsvクラスへ
			Csv1.SetCsv_Cr_P(Cr_Pop);
		}
		int tmpIndex;
		int tmpSub;
		SubParent.resize(0);
		std::cout << "2" << ',';
		//主親選ぶ
		MainParent = GetRand_Int(KO);
		//副親を選ぶ
		for (int i = 0; i < KL1; i++) {
			if (Pop[MainParent].List1.empty()) {
				break;
			}
			tmpIndex = GetRand_Int( int(Pop[MainParent].List1.size()) );
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
			Generate_Opp();

			//w1,w2,w3:子個体の戦略，w1_T,w2_T,w3_T:対戦相手の戦略
			//player1:子個体，player2:対戦相手0
			std::cout << "5" << ',';
			int opponentLength = int(Opponent.size());

			for (int i = 0; i < opponentLength; i++) {
				Opponent[i].Result.resize(CHILD);
			}
			for (int i = 0; i < CHILD; i++) {
				Child[i].Result.resize(opponentLength);
			}
			//ここで対戦
			for (int r = 0; r < opponentLength; r++) {//相手ループ
				StrategySet_T(Opponent[r]);
				for (int c = 0; c < CHILD; c++) {//集団ループ
					StrategySet_M(Child[c]);
					Machup_Num++;
					Competition();//対戦 player1 = 子個体 palyer2 = 対戦相手？

					Return_Re(c,r);
				}
			}
			std::cout << "6" << ',';
			//適応度計算
			Cal_Fitness();
			for (int i = 0; i < CHILD; i++) {
				FitnessChild(Child[i], Opponent, false);
			}

			std::cout << "7" << ',';
			//Best個体を集団へ
			int Index = Choice_Best_Index(Child);
			Pop[MainParent] = Child[Index];

			//実験用
			/*
			if (gene % PER == 0) {
				std::cout << "10" << ',';
				//Floreanoと対戦
				//MachUpFloreano(Pop);
				//個体の平均勝率をベクターへ
				SetVec_Re_Pop(Pop);
				Csv1.SetCsv_Re_P(Re_Pop);
			}
			*/
			std::cout << "8" << ',';
			//集団の解以外初期化
			for (int i = 0; i < KO; i++) {
				Pop[i].Init();
			}
		}
		std::cout << std::endl;
	}
	Csv1.Fwrite_Cr_P();
	Csv1.Fwrite_Re_P();
}
void Coans::SetVec_Cr_Pop(std::vector<playerTK> &Pop){
	//初期化
	Cr_Pop.assign(Cr_Num,0);
	for (int i = 0; i < KO; i++) {
		Cr_Pop[ Pop[i].nitch ]++;
	}
	for (int i = 0; i < Cr_Num; i++) {
		if (Cr_Pop[i] == 0) {
			Cr_Pop.erase(Cr_Pop.begin() + i ); //要素を削除
		}
	}
}
void Coans::SetVec_Re_Pop(std::vector<playerTK> &Pop) {
	for (int i = 0; i < KO; i++) {
		//対戦結果のベクターの和を格納する
		Re_Pop[i] = int(std::accumulate(Pop[i].Result.begin(), Pop[i].Result.end(), 0.0));
	}
}
void Coans::SetVec_Re_Opp(std::vector<playerTK> &Opp) {
	for (int i = 0; i < Opp_Num; i++) {
		//対戦結果のベクターの和を格納する
		Re_Opp[i] = int(std::accumulate(Opp[i].Result.begin(), Opp[i].Result.end(), 0.0));
	}
}
/*対戦回数を返す*/
int Coans::Get_MatchUp_Num() {
	return Machup_Num;
}
void Coans::File_Write_Pop(int trial, bool s1)
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
	for (int i = 0; i < KO; i++) {
		if (s1) {
			sprintf(filename, ("AI/%d/%d/%d.dat"), Method_Num, trial, i);
			if ((fp = fopen(filename, "wb+")) == NULL) {
				fprintf(stderr, "%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
		}
		else {
			sprintf(filename, ("AIT/%d/%d.dat"), trial, i);
			if ((fp = fopen(filename, "wb+")) == NULL) {
				fprintf(stderr, "%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
		}
		fwrite(w1_GA[i], sizeof(double), I1*J1, fp);
		fwrite(w2_GA[i], sizeof(double), I2*J2, fp);
		fwrite(w3_GA[i], sizeof(double), I2*J1, fp);
		fclose(fp);
	}
}
/*
mode1
対戦相手を各クラスタからの個体に限定
評価は勝ち負けの数
評価値に補正を加える
*/
class CoansMode1 : public Coans {
private :
	virtual void Set_Method();
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode1::Crustering() {
	//近傍リスト生成＆クラスタ番号割り振り
	MakeList(Pop);
	Cr_Num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(Cr_Num, i, Pop) == 1) {
			Cr_Num++;
		}
	}
}
void CoansMode1::Set_Method() {
	Method_Num = 0;
}
void CoansMode1::Generate_Opp() {
	choice_oppoment(Pop, Opponent, Cr_Num);
	Opp_Num = Cr_Num;
}
void CoansMode1::Return_Re(int Child_Num, int Opp_Num) {
	if (player1.win == 1) {
		Opponent[Opp_Num].Result[Child_Num] = 0;
		Child[Child_Num].Result[Opp_Num] = 1;
	}
	else if (player2.win == 1) {
		Opponent[Opp_Num].Result[Child_Num] = 1;
		Child[Child_Num].Result[Opp_Num] = 0;
	}
	else {
		Opponent[Opp_Num].Result[Child_Num] = 0;
		Child[Child_Num].Result[Opp_Num] = 0;
	}
}
void CoansMode1::Cal_Fitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, false);
	}
}
