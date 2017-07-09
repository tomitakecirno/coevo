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
	void CoansTasks(int Trial);
	//対戦回数を取得
	int GetMatchUpN();
	//以下実験用
	void SetData_Cruster(std::vector<playerTK> &Pop);	//クラスタ番号をCSVクラスへ
	void SetData_PopResult(std::vector<playerTK> &Pop);	//集団の対戦結果を記録
	void SetData_OppResult(std::vector<playerTK> &Opp);	//相手集団の対戦結果をCSVクラスへ
//非公開メンバ
private:
	virtual void Crustering() = 0;
	virtual void GenerateOpponent() = 0;
	virtual void ReturnResult(int ChildNumber, int OpponentNumber) = 0;
	virtual void CalculateFitness() = 0;
//このクラスと派生クラスからアクセス可能
protected:
	int BattleNumber;					//対戦回数
	int CrusterNumber;					//クラスター番号
	int OpponentNumber;					//対戦回数
	int MainParent;						//主親
	std::vector<int> SubParent;			//副親
	std::vector<playerTK> Pop;			//進化させる集団
	std::vector<playerTK> Child;		//子
	std::vector<playerTK> Opponent;		//対戦相手
	//以下実験用
	std::vector<int> PopCruster;
	std::vector<int> PopResult;
	std::vector<int> OppResult;
};
/*手法のメインルーチン*/
void Coans::CoansTasks(int Trial)
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
	PopCruster.resize(KO);
	PopResult.resize(KO);

	BattleNumber = 0;
	//任意の世代数ループ
	for (int e = 0; e < KU; e++) {
		std::cout << "試行回数:" << Trial << "  ";
		std::cout << "世代数:" << e << std::endl;

		Crustering(); //クラスタリング。手法によって変わる
		//実験用
		/*
		if (e / PER == 0) {
			SetData_Cruster(Pop);
			Csv1.Csv_SetPopCruster(PopCruster);
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
			GenerateOpponent();

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
					BattleNumber++;
					Competition();//対戦 player1 = 子個体 palyer2 = 対戦相手？

					ReturnResult(c,r);
				}
			}

			//適応度計算
			CalculateFitness();
			for (int i = 0; i < CHILD; i++) {
				FitnessChild(Child[i], Opponent, false);
			}

			//Best個体を集団へ
			int Index = 0;
			double Max = Child[0].eval;

			int Length = int(Child.size());
			for (int i = 1; i < Length; i++) {
				if (Max < Child[i].eval) {
					Index = i;
					Max = Child[i].eval;
				}
			}
			Pop[MainParent] = Child[Index];

			//実験用
			/*
			if (e / PER == 0) {
				//解以外を初期化
				for (int i = 1; i < KO; i++) {
					Pop[i].Init();
				}
				//Floreanoと対戦
				//MachUpFloreano(Pop);
				//個体の平均勝率をベクターへ
				SetData_PopResult(Pop);
				Csv1.Csv_SetPopResult(PopResult);
			}
			*/
			//集団の解以外初期化
			for (int i = 0; i < KO; i++) {
				Pop[i].Init();
			}
		}
	}
	//Csv1.FileWrite_PopCruster();
	//Csv1.FileWrite_PopResult();
}
void Coans::SetData_Cruster(std::vector<playerTK> &Pop){
	for (int i = 0; i < KO; i++) {
		PopCruster[i] = Pop[i].nitch;
	}
}
void Coans::SetData_PopResult(std::vector<playerTK> &Pop) {
	for (int i = 0; i < KO; i++) {
		//対戦結果のベクターの和を格納する
		PopResult[i] = int(std::accumulate(Pop[i].Result.begin(), Pop[i].Result.end(), 0.0));
	}
	std::cout << "SetData_PopResult内:" << std::endl;
	for (int i = 0; i < KO; i++) {
		std::cout << PopResult[i] << ',';
	}
	std::cout << std::endl;
}
void Coans::SetData_OppResult(std::vector<playerTK> &Opp) {
	for (int i = 0; i < OpponentNumber; i++) {
		//対戦結果のベクターの和を格納する
		OppResult[i] = int(std::accumulate(Opp[i].Result.begin(), Opp[i].Result.end(), 0.0));
	}
}
/*対戦回数を返す*/
int Coans::GetMatchUpN() {
	return BattleNumber;
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
	virtual void GenerateOpponent();
	virtual void ReturnResult(int ChildNumber, int OpponentNumber);
	virtual void CalculateFitness();
};
void CoansMode1::Crustering() {
	//近傍リスト生成＆クラスタ番号割り振り
	MakeList(Pop);
	CrusterNumber = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(CrusterNumber, i, Pop) == 1) {
			CrusterNumber++;
		}
	}
}
void CoansMode1::GenerateOpponent() {
	choice_oppoment(Pop, Opponent, CrusterNumber);
	OpponentNumber = CrusterNumber;
}
void CoansMode1::ReturnResult(int ChildNumber, int OpponentNumber) {
	if (player1.win == 1) {
		Opponent[OpponentNumber].Result[ChildNumber] = 0;
		Child[ChildNumber].Result[OpponentNumber] = 1;
	}
	else if (player2.win == 1) {
		Opponent[OpponentNumber].Result[ChildNumber] = 1;
		Child[ChildNumber].Result[OpponentNumber] = 0;
	}
	else {
		Opponent[OpponentNumber].Result[ChildNumber] = 0;
		Child[ChildNumber].Result[OpponentNumber] = 0;
	}
}
void CoansMode1::CalculateFitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, false);
	}
}
