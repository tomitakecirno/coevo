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
	//以下実験用
	void SetVec_Cr_Pop(std::vector<playerTK> &Pop);	//クラスタ番号をCSVクラスへ
	void SetVec_Re_Pop(std::vector<playerTK> &Pop);	//集団の対戦結果を記録
	void SetVec_Re_Opp(std::vector<playerTK> &Opp);	//相手集団の対戦結果をCSVクラスへ
//非公開メンバ
private:
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
	std::vector<int> SubParent;			//副親
	std::vector<playerTK> Pop;			//進化させる集団
	std::vector<playerTK> Child;		//子
	std::vector<playerTK> Opponent;		//対戦相手
	//以下実験用
	std::vector<int> Cr_Pop;
	std::vector<int> Re_Pop;
	std::vector<int> Re_Opp;
};
/*手法のメインルーチン*/
void Coans::Coans_Tasks(int Trial)
{
	//集団宣言
	Pop.resize(KO);
	init_genrand((unsigned)time(NULL)); /*乱数初期化*/

	//集団初期化
	for (int i = 0; i < KO; i++) {
		Pop[i].Init();
		Pop[i].Init_w();
	}

	CsvModules Csv1;
	Csv1.Init(Trial, KU, PER);
	Cr_Pop.resize(KO);
	Re_Pop.resize(KO);

	Machup_Num = 0;
	//任意の世代数ループ
	for (int e = 0; e < KU; e++) {
		std::cout << "試行回数:" << Trial << "  ";
		std::cout << "世代数:" << e << std::endl;

		Crustering(); //クラスタリング。手法によって変わる
		//実験用
		/*
		if (e / PER == 0) {
			SetVec_Cr_P(Pop);
			Csv1.Set_Cr_P(PopCruster);
		}
		*/
		int tmpIndex;
		int tmpSub;
		SubParent.resize(0);
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

			//拡張XLM
			Child[0] = Pop[MainParent];
			ExtensionXLM(MainParent, SubParent, Pop, Child);
			//対戦相手の個体を選ぶ
			Generate_Opp();

			//w1,w2,w3:子個体の戦略，w1_T,w2_T,w3_T:対戦相手の戦略
			//player1:子個体，player2:対戦相手0
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

			//適応度計算
			Cal_Fitness();
			for (int i = 0; i < CHILD; i++) {
				FitnessChild(Child[i], Opponent, false);
			}

			//Best個体を集団へ
			int Index = Choice_Best_Index(Child);
			Pop[MainParent] = Child[Index];

			//実験用
			if (e / PER == 0) {
				//解以外を初期化
				for (int i = 1; i < KO; i++) {
					Pop[i].Init();
				}
				//Floreanoと対戦
				//MachUpFloreano(Pop);
				//個体の平均勝率をベクターへ
				SetVec_Re_Pop(Pop);
				Csv1.Set_Re_P(Re_Pop);
			}
			//集団の解以外初期化
			for (int i = 0; i < KO; i++) {
				Pop[i].Init();
			}
		}
	}
	Csv1.Fwrite_Cr_P();
	Csv1.Fwrite_Re_P();
}
void Coans::SetVec_Cr_Pop(std::vector<playerTK> &Pop){
	for (int i = 0; i < KO; i++) {
		Cr_Pop[i] = Pop[i].nitch;
	}
}
void Coans::SetVec_Re_Pop(std::vector<playerTK> &Pop) {
	for (int i = 0; i < KO; i++) {
		//対戦結果のベクターの和を格納する
		Re_Pop[i] = int(std::accumulate(Pop[i].Result.begin(), Pop[i].Result.end(), 0.0));
	}
	std::cout << "SetData_PopResult内:" << std::endl;
	for (int i = 0; i < KO; i++) {
		std::cout << Re_Pop[i] << ',';
	}
	std::cout << std::endl;
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

/*
mode1
対戦相手を各クラスタからの個体に限定
評価は勝ち負けの数
評価値に補正を加える
*/
class CoansMode1 : public Coans {
private :
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
