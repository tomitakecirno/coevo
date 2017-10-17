#pragma once
/*=======================================
自分の各種手法まとめ
CoansMode0	卒論の手法
CoansMode1	現手法：現評価方法(評価値に補正を加える)
CoansMode2	現手法：前評価方法(評価値に補正を加えない)
CoansMode3	前手法：現評価方法
CoansMode4	前手法：前評価方法
=======================================*/
#include "config.hpp"
#include "coansmodule.hpp"
#include "CsvModules.h"
#include "CrusteringMethod.h"
#include "Usual_Methods.hpp"
#include <time.h>
#include <string>
#include <fstream>

/*手法のクラス*/
class Coans{
//公開メンバ
public :
	//手法のメインルーチン
	void	Coans_Tasks(int Trial);			//現手法
	//対戦回数を取得
	int		Get_MatchUp_Num();
	//以下実験用
//非公開メンバ
private:
	virtual void Crustering() = 0;
	virtual void Generate_Opp() = 0;
	virtual void Return_Re(int Child_Num, int Opp_Num) = 0;
	virtual void Cal_Fitness() = 0;
	//このクラスと派生クラスからアクセス可能
protected:
	int Machup_Num;						//対戦回数
	int Cr_Num;							//クラスター番号
	int Opp_Num;						//対戦回数
	int MainParent;						//主親
	int Method_Num;						//手法比較の時に使う
	int	Cru_K;							//クラスタリングパラメーター
	std::string Dir;
	std::vector<int> SubParent;			//副親
	std::vector<playerTK> Pop;			//進化させる集団
	std::vector<playerTK> Child;		//子
	std::vector<playerTK> Opponent;		//対戦相手
	//以下実験用
	std::vector<int> Cr_Pop;			//クラスタ番号を格納
	std::vector<int> Re_Pop;			//メインの対戦結果を格納
	std::vector<int> Re_Opp;			//対戦相手の
};
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
	Make_Directory(Dir, Method_Num, Trial, KU, PER, Cru_K);
	Cr_Pop.resize(KO);
	Re_Pop.resize(KO);

	//初期世代の戦略を記録
	Machup_Num = 0;
	//任意の世代数ループ
	int Loop_Time_Start;
	int Loop_Time_End;
	int Loop_Time;
	//Crustering();

	std::cout << "Initiarized..." << std::endl;
	for (int Gene_Loop = 1; Gene_Loop < KU+1; Gene_Loop++) {
		Loop_Time_Start = clock();
		std::cout << Method_Num << ":" << Trial << ":" << Cru_K << ":" << Gene_Loop;
		std::cout << "  |  ";
		std::cout << "1" << ',';
		Crustering(); //クラスタリング。手法によって変わる
		//実験用
		if (Gene_Loop % PER == 0 || Gene_Loop == 1) {
			std::cout << "9" << ',';
			//集団のクラスタ番号をベクターに格納
			//SetVec_Cr_Pop(Pop);
			//格納したベクターをCsvクラスへ
		}
		else {
		}
		int tmpIndex;
		int tmpSub;
		SubParent.resize(0);
		std::cout << "2" << ',';
		//主親選ぶ
		MainParent = GetRand_Int(KO);
		//副親を選ぶ
		for (int i = 0; i < PARENT; i++) {
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
			/*
			std::vector<std::vector<double> > Child_Dis(CHILD + 1);
			for (int i = 0; i < CHILD + 1; i++) {
				Child_Dis[i].resize(CHILD);
			}
			for (int i = 0; i < CHILD+1; i++) {
				for (int j = i+1; j < CHILD; j++) {
					Child_Dis[i][j] = cal_kotai_distance(Child[i],Child[j]);
					Child_Dis[j][i] = Child_Dis[i][j];
				}
			}
			std::cout << std::endl;
			for (int i = 0; i < CHILD + 1; i++) {
				std::cout << i << ":[";
				for (int j = 0; j < CHILD; j++) {
					std::cout << Child_Dis[i][j] << ',';
				}
				std::cout << ']';
				std::cout << std::endl;
			}

			exit(0);
			*/
			//対戦相手の個体を選ぶ
			std::cout << "4" << ',';
			Generate_Opp();

			//w1,w2,w3:子個体の戦略，w1_T,w2_T,w3_T:対戦相手の戦略
			//player1:子個体，player2:対戦相手0
			std::cout << "5" << ',';
			int opponentLength = int(Opponent.size());

			for (int i = 0; i < opponentLength; i++) {
				Opponent[i].Result.resize(CHILD+1);
			}
			for (int i = 0; i < CHILD+1; i++) {
				Child[i].Result.resize(opponentLength);
			}
			//ここで対戦
			for (int r = 0; r < opponentLength; r++) {//相手ループ
				StrategySet_T(Opponent[r]);
				for (int c = 0; c < CHILD + 1; c++) {//集団ループ
					StrategySet_M(Child[c]);
					Machup_Num++;
					Competition();//対戦 player1 = 子個体 palyer2 = 対戦相手？
					Return_Re(c, r);
				}
			}
			std::cout << "6" << ',';
			//適応度計算
			Cal_Fitness();

			std::cout << "7" << ',';
			//Best個体を集団へ
			int Index = Choice_Best_Index(Child, false);
			Pop[MainParent] = Child[Index];

			//実験用
			if (Gene_Loop % PER == 0) {
				std::cout << "10" << ',';
				//100世代毎に戦略を記録
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
		}
		Loop_Time_End = clock();
		Loop_Time = (Loop_Time_End - Loop_Time_Start) / CLOCKS_PER_SEC;
		std::cout << "Time per gene : " << Loop_Time << " [sec]" << std::endl;
		assert( Gene_Loop <= KU );
	}
}
int Coans::Get_MatchUp_Num() {
	return Machup_Num;
}

/*
	mode1
	ウォード法の階層的クラスタリングで明示的にクラスタリング
	評価はHPの差分
*/
class CoansMode1 : public Coans {
public:
	CoansMode1(std::string str) {
		Method_Num = 1;
		Dir = str;
	}
private:
	//クラスタリングに使うパラメーター
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode1::Crustering() {
	//近傍リスト生成
	//MakeList(Pop, true, false, false);
	//クラスタ番号割り振り
	Cru_Upgma(Pop, Cru_K);
	Cr_Num = Cru_K;
}
void CoansMode1::Generate_Opp() {
	choice_oppoment(Pop, Opponent, Cru_K);
	Opp_Num = Cru_K;
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

/*
	mode2
	交叉	：List1
	対戦相手：各クラスタの代表
	適応度	：自分と相手のHPの差分
*/
class CoansMode2 : public Coans {
public:
	CoansMode2(std::string str) {
		Method_Num = 2;
		Dir = str;
		Cru_K = 0;
	}
private:
	//クラスタリングに使うパラメーター
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode2::Crustering() {
	//近傍リスト生成
	MakeList(Pop, K_List1, K_List2, 0);
	Cr_Num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(Cr_Num, i, Pop) == 1) {
			Cr_Num++;
		}
	}
}
void CoansMode2::Generate_Opp() {
	choice_oppoment(Pop, Opponent, Cr_Num);
	Opp_Num = Cr_Num;
}
void CoansMode2::Return_Re(int Child_Num, int Opp_Num) {
	Opponent[Opp_Num].Result[Child_Num] = player2.hp - player1.hp;
	Child[Child_Num].Result[Opp_Num] = player1.hp - player2.hp;
}
void CoansMode2::Cal_Fitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, false);
	}
}

/*
	mode3
	交叉	：List1
	対戦相手：集団全体
	適応度	：自分と相手のHPの差分
*/
class CoansMode3 : public Coans {
public:
	CoansMode3(std::string str) {
		Method_Num = 3;
		Dir = str;
		Cru_K = 0;
	}
private:
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode3::Crustering() {
	//近傍リスト生成＆クラスタ番号割り振り
	MakeList(Pop, K_List1, K_List2, 0);
	Cr_Num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(Cr_Num, i, Pop) == 1) {
			Cr_Num++;
		}
	}
}
void CoansMode3::Generate_Opp() {
	Opponent = Pop;
}
void CoansMode3::Return_Re(int Child_Num, int Opp_Num) {
	Opponent[Opp_Num].Result[Child_Num] = player2.hp - player1.hp;
	Child[Child_Num].Result[Opp_Num] = player1.hp - player2.hp;
}
void CoansMode3::Cal_Fitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, false);
	}
}

/*
	mode4
	交叉	：List1
	対戦相手：集団全体
	適応度	：自分と相手のHPの差分＋適応度に追加補正
*/
class CoansMode4 : public Coans {
public:
	CoansMode4(std::string str) {
		Method_Num = 4;
		Dir = str;
		Cru_K = 0;
	}
private:
	virtual void Set_Method();
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode4::Crustering() {
	//近傍リスト生成＆クラスタ番号割り振り
	MakeList(Pop, K_List1, K_List2, 0);
	Cr_Num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(Cr_Num, i, Pop) == 1) {
			Cr_Num++;
		}
	}
}
void CoansMode4::Set_Method() {
	Method_Num = 4;
}
void CoansMode4::Generate_Opp() {
	Opponent = Pop;
}
void CoansMode4::Return_Re(int Child_Num, int Opp_Num) {
	Opponent[Opp_Num].Result[Child_Num] = (player2.hp - player1.hp)/300;
	Child[Child_Num].Result[Opp_Num] = (player1.hp - player2.hp)/300;
}
void CoansMode4::Cal_Fitness() {
	int Length = int(Child.size());
	for (int i = 0; i < Length; i++) {
		FitnessChild(Child[i], Opponent, true);
	}
}


/*
	卒論手法のクラス
*/
class Coans_GT2016 {
//公開メンバ
public:
	Coans_GT2016(std::string str, int n, int g, int g_p, int p, int c, int k = 0) {
		Method_Num = 0;					//手法比較の時に使う
		Pop_n = n;							//自集団の個体数
		Gene = g;							//世代数
		Per = g_p;							//間隔
		Pare = p;							//親個体数
		Child = c;							//子個体数
		Machup_Num = 0;					//対戦回数
		Opp_Num = 0;					
		Cru_K = k;						//クラスタリングパラメーター
		Dir = str;		
	}
	//手法のメインルーチン
	void	Coans_GT2016_Tasks(int Trial);
	int		Get_MatchUp_Num();
	//戦略を書き込み
	//以下実験用
	void	SetVec_Cr_Pop(std::vector<playerTK> &Pop);	//クラスタ番号をCSVクラスへ
	void	SetVec_Re_Pop(std::vector<playerTK> &Pop);	//集団の対戦結果を記録
	void	SetVec_Re_Opp(std::vector<playerTK> &Opp);	//相手集団の対戦結果をCSVクラスへ
//非公開メンバ
protected:
	int Method_Num;						//手法比較の時に使う
	int Pop_n;							//自集団の個体数
	int Gene;							//世代数
	int Per;							//間隔
	int Pare;							//親個体数
	int Child;							//子個体数
	int Machup_Num;						//対戦回数
	int Cr_Num;							//クラスター番号
	int Opp_Num;						
	int	Cru_K;							//クラスタリングパラメーター
	int MainParent;						//主親
	std::string Dir;
	std::vector<playerTK> Pop;			//進化させる集団
	std::vector<playerTK> Opponent;		//対戦相手
	void Update_Opponent(playerTK child);
	int	 Cal_Gra_Nitch(playerTK child);
	//以下実験用
	std::vector<int> Cr_Pop;			//クラスタ番号を格納
	std::vector<int> Re_Pop;			//メインの対戦結果を格納
	std::vector<int> Re_Opp;			//対戦相手の
};
void  Coans_GT2016::Coans_GT2016_Tasks(int Trial)
{
	std::cout << "Initiarize" << std::endl;

	//集団宣言
	Pop.resize(Pop_n);
	Opponent.resize(Pop_n);
	init_genrand((unsigned)time(NULL)); /*乱数初期化*/

										//集団初期化
	for (int i = 0; i < Pop_n; i++) {
		Pop[i].Init();
		Pop[i].Init_w();
	}
	for (int i = 0; i < Pop_n; i++) {
		Opponent[i].Init();
		Opponent[i].Init_w();
	}
	Make_Directory(Dir, Method_Num, Trial, Gene, Per, Cru_K);
	Cr_Pop.resize(Pop_n);
	Re_Pop.resize(Pop_n);

	//初期世代の戦略を記録
	//任意の世代数ループ
	int Loop_Time_Start;
	int Loop_Time_End;
	int Loop_Time;

	MakeList(Opponent, 0, K_List2, 0);
	Cr_Num = 1;
	for (int i = 0; i < Pop_n; i++) {
		if (SetNitch(Cr_Num, i, Opponent) == 1) {
			Cr_Num++;
		}
	}

	for (int Gene_Loop = 1; Gene_Loop < Gene + 1; Gene_Loop++) {
		Loop_Time_Start = clock();
		std::cout << Method_Num << ":" << Trial << ":" << Cru_K << ":" << Gene_Loop;
		std::cout << "  |  ";
		std::cout << "1" << ',';

		//クラスタリング
		MakeList(Pop, K_List1, 0, 0);
		std::cout << "1-1" << ',';

		//実験用
		if (Gene_Loop % Per == 0 || Gene_Loop == 1) {
			std::cout << "12" << ',';
			//集団のクラスタ番号をベクターに格納
			//SetVec_Cr_Pop(Pop);
		}
		else {
			SetVec_Cr_Pop(Pop);
		}

		int tmpIndex;
		int tmpSub;
		std::vector<int> SubParent;	//副親
		std::cout << "2" << ',';
		//主親選ぶ
		MainParent = GetRand_Int(Pop_n);
		//副親を選ぶ
		for (int i = 0; i < Pare; i++) {
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
			std::vector<playerTK> Child(Child + 1);
			Child[0] = Pop[MainParent];
			int child_size = int(Child.size());

			std::cout << "3" << ',';
			//拡張XLM
			ExtensionXLM(MainParent, SubParent, Pop, Child);
			//対戦相手の個体を選ぶ
			std::cout << "4" << ',';

			//w1,w2,w3:子個体の戦略，w1_T,w2_T,w3_T:対戦相手の戦略
			//player1:子個体，player2:対戦相手0
			std::cout << "5" << ',';
			//相手集団との対戦結果を格納するベクターをリサイズ
			for (int i = 0; i < Pop_n; i++) {
				Opponent[i].Result.resize(child_size);
			}
			for (int i = 0; i < child_size; i++) {
				Child[i].Result.resize(Pop_n);
			}
			//ここで対戦
			for (int r = 0; r < Pop_n; r++) {//相手ループ
				StrategySet_T(Opponent[r]);
				for (int c = 0; c < child_size; c++) {//集団ループ
					StrategySet_M(Child[c]);
					Machup_Num++;
					Competition();//対戦 player1 = 子個体 palyer2 = 対戦相手？
					Opponent[r].Result[c] = (player2.hp - player1.hp) / 300;
					Child[c].Result[r] = (player1.hp - player2.hp) / 300;
				}
			}
			std::cout << "6" << ',';
			//適応度計算
			for (int i = 0; i < child_size; i++) {
				FitnessChild(Child[i], Opponent, false);
			}
			std::cout << "7" << ',';
			//Best個体を集団へ
			int Index = Choice_Best_Index(Child, false);
			//Index = 3;
			//Best → Child[Index];

			std::cout << "8" << ',';
			//削除予定の個体があれば
			if (Index != 0) {
				int k;
				for (k = 0; k < Pop_n; k++) {
					if (Opponent[k].delete_flag != 0) {
						break;
					}
				}
				if (k != Pop_n) {
					Opponent[k] = Child[Index];
					Opponent[k].comp_flag = 1;
					Opponent[k].delete_flag = 0;
				}if (k == Pop_n) {
					std::cout << "8-1" << ',';
					Update_Opponent(Child[Index]);
				}
			}
			std::cout << "9" << ',';
			for (int i = 0; i < Pop_n; i++) {
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
			if (Gene_Loop % Per == 0) {
				std::cout << "11" << ',';
				//100世代毎に戦略を記録
			}
			std::cout << "10" << ',';
			std::cout << "  [";
			for (int c = 0; c < child_size; c++) {
				std::cout << Child[c].eval << ",";
			}
			std::cout << "]" << std::endl;
			//集団の解以外初期化
			for (int i = 0; i < Pop_n; i++) {
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
		//assert(Gene_Loop <= KU);
	}
}
int	  Coans_GT2016::Get_MatchUp_Num() {
	return Machup_Num;
}
void  Coans_GT2016::Update_Opponent(playerTK child)
{
	/*いらない相手を残すと有害。勝ち数で残す個体判断しているので、余計な勝ち数がカウントされる。*/
	/*距離が最小のニッチの重心を見つけて、ニッチ番号を取得*/
	int Nit_Count = 0;
	int Nit_Num = Cal_Gra_Nitch(child);
	//一番近いクラスタの番号を入れる
	child.nitch = Nit_Num;

	/*戦闘データ初期化*/
	for (int i = 0; i<KO; i++) {
		if (Opponent[i].nitch == Nit_Num) {
			Opponent[i].eval = 0;
			Opponent[i].flag = 0;
			Nit_Count++;
		}
	}
	//対戦用のベクターを作る
	std::vector<playerTK> Oppc(Nit_Count + 1);
	std::vector<double> Oppc_Eval(Nit_Count + 1);
	for (int i = 0; i < KO; i++) {
		if (Opponent[i].nitch == Nit_Num) {
			Oppc[Nit_Count] = Opponent[i];
			Nit_Count--;
		}
	}
	assert(0 <= Nit_Count);
	child.eval = 0;
	child.flag = 0;
	Oppc[0] = child;

	/*該当するニッチの個体と子個体でリーグ戦*/
	int Oppc_size = int(Oppc.size());
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
			for (j = 0; j < KO && Opponent[j].nitch != Nit_Num; j++) {}
			Opponent[j] = Oppc[i];
			Opponent[j].comp_flag = 1;
			Opponent[j].delete_flag = 0;
		}
	}
	/*対戦データおよびニッチ番号初期化*/
	for (int i = 0; i < KO; i++) {
		Opponent[i].flag = 0;
		Opponent[i].eval = 0;
		Opponent[i].nitch = 0;
		Opponent[i].List2.clear();
	}
	MakeList(Opponent, 0, K_List2, 0);
	Cr_Num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(Cr_Num, i, Opponent) == 1) {
			Cr_Num++;
		}
	}
}
int   Coans_GT2016::Cal_Gra_Nitch(playerTK child)
{
	std::vector<int> Nitch_Num;
	std::vector< std::vector < std::vector<double> > > Sum_N_W1;
	std::vector< std::vector < std::vector<double> > > Sum_N_W2;
	std::vector< std::vector < std::vector<double> > > Sum_N_W3;

	Nitch_Num = std::vector<int>(Cr_Num, 0);
	Sum_N_W1 = std::vector<std::vector<std::vector<double>>>(Cr_Num + 1, std::vector<std::vector<double>>(I1, std::vector<double>(J1, 0.0)));
	Sum_N_W2 = std::vector<std::vector<std::vector<double>>>(Cr_Num + 1, std::vector<std::vector<double>>(I2, std::vector<double>(J2, 0.0)));
	Sum_N_W3 = std::vector<std::vector<std::vector<double>>>(Cr_Num + 1, std::vector<std::vector<double>>(J1, std::vector<double>(I2, 0.0)));

	for (int Opp = 0; Opp < KO; Opp++) {
		//親の解のそれぞれのベクトルを足す
		//w1_CO
		int Opp_Nitch = Opponent[Opp].nitch;
		//std::cout << "Opp:" << Opp << ',';
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
	//std::cout << "8-1-2" << ',';
	//ベクトルの重心を求める
	//w1_CO
	for (int Nit = 1; Nit < Cr_Num; Nit++) {
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
	//std::cout << "8-1-3" << ',';
	//重心と子個体の距離計算
	double cal_sum = 0;
	std::vector<double> Dis_Vector(Cr_Num);
	//w1の距離
	for (int Nit = 1; Nit < Cr_Num; Nit++) {
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				Dis_Vector[Nit] += (Sum_N_W1[Nit][j][k] - child.w1_CO[j][k])*(Sum_N_W1[Nit][j][k] - child.w1_CO[j][k]);
			}
		}
		//w2の距離
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				Dis_Vector[Nit] += (Sum_N_W2[Nit][j][k] - child.w2_CO[j][k])*(Sum_N_W2[Nit][j][k] - child.w2_CO[j][k]);
			}
		}
		//w3の距離
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				Dis_Vector[Nit] += (Sum_N_W3[Nit][j][k] - child.w3_CO[j][k])*(Sum_N_W3[Nit][j][k] - child.w3_CO[j][k]);
			}
		}
		Dis_Vector[Nit] = sqrt(Dis_Vector[Nit]);
	}
	//std::cout << "8-1-4" << ',';
	auto min = min_element(Dis_Vector.begin()+1, Dis_Vector.end());
	auto min_Nit = int(distance(Dis_Vector.begin(), min));
	return min_Nit;
}

/*
	実験用の関数
*/
//戦略を書き込む
class Exp {
public:
	void Stra_Output_Pop(std::string dir, std::vector<playerTK> &Pop, int method, int trial, int gene);
	void SetVec_Cr_Pop(std::vector<playerTK> &Pop, int Num);
	void SetVec_Re_Pop(std::vector<playerTK> &Pop, int Num);
	void SetVec_Re_Opp(std::vector<playerTK> &Opp, int Num);
protected:
	//各世代のクラスタ情報を格納
	std::vector<std::vector<int>> Cr_Pop;
	std::vector<std::vector<double>> Re_Pop;
	std::vector<std::vector<double>> Re_Opp;
};
//戦略を書き込む
void Exp::Stra_Output_Pop(std::string dir, std::vector<playerTK> &Pop, int method, int trial, int gene)
{
	int pop_size = int(Pop.size());
	std::vector<std::vector<std::vector<double>>> w1_Main;
	std::vector<std::vector<std::vector<double>>> w2_Main;
	std::vector<std::vector<std::vector<double>>> w3_Main;

	w1_Main = std::vector<std::vector<std::vector<double>>>(pop_size, std::vector<std::vector<double>>(I1, std::vector<double>(J1, 0)));
	w2_Main = std::vector<std::vector<std::vector<double>>>(pop_size, std::vector<std::vector<double>>(I2, std::vector<double>(J2, 0)));
	w3_Main = std::vector<std::vector<std::vector<double>>>(pop_size, std::vector<std::vector<double>>(I2, std::vector<double>(J1, 0)));

	//様式を合わせる
	for (int i = 0; i < pop_size; i++) {
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				w1_Main[i][j][k] = Pop[i].w1_CO[j][k];
			}
		}
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				w2_Main[i][j][k] = Pop[i].w2_CO[j][k];
			}
		}
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				w3_Main[i][j][k] = Pop[i].w3_CO[j][k];
			}
		}
	}
	//bef = trueの時AI，falseの時AIC
	FILE *fp_main;
	std::stringstream File_Name;
	std::stringstream Tmp_File_Name;

	File_Name << "./" << dir << "/" << method << "/" << trial << "/" << gene / PER;
	//File_Name << "./" << Dir;

	for (int i = 0; i < pop_size; i++) {
		Tmp_File_Name << File_Name.str() << "/" << i << ".dat";
		//ファイル書き込み
		if ((fp_main = fopen(Tmp_File_Name.str().c_str(), "wb+")) == NULL) {
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		fwrite(w1_Main[i].data(), sizeof(double), I1*J1, fp_main);
		fwrite(w2_Main[i].data(), sizeof(double), I2*J2, fp_main);
		fwrite(w3_Main[i].data(), sizeof(double), I2*J1, fp_main);

		//クリア
		Tmp_File_Name.str("");
		Tmp_File_Name.clear(std::stringstream::goodbit);
		fclose(fp_main);
	}
}
//クラスタを書き込む
void Exp::SetVec_Cr_Pop(std::vector<playerTK> &Pop, int Num) {
	int pop_size = int(Pop.size());

	Cr_Pop.resize(Num+1);
	Cr_Pop[Num].resize(pop_size);
	for (int i = 0; i < pop_size; i++) {
		Cr_Pop[Num][i] = Pop[i].nitch;
	}
	/*
	for (int i = 0; i < pop_size; i++) {
		assert(Pop[i].nitch < Pop_n);
		assert(0 <= Pop[i].nitch);
		Cr_Pop[Pop[i].nitch]++;
	}
	Cr_Pop.erase(std::remove(Cr_Pop.begin(), Cr_Pop.end(), 0), Cr_Pop.end());
	assert(!Cr_Pop.empty());
	*/
}
void Exp::SetVec_Re_Pop(std::vector<playerTK> &Pop, int Num) {
	int pop_size = int(Pop.size());

	Re_Pop.resize(Num + 1);
	Re_Pop[Num].resize(pop_size);

	for (int i = 0; i < pop_size; i++) {
		//対戦結果のベクターの和を格納する
		Re_Pop[Num][i] = double(std::accumulate(Pop[i].Result.begin(), Pop[i].Result.end(), 0.0));
	}
}
void Exp::SetVec_Re_Opp(std::vector<playerTK> &Opp, int Num) {
	int opp_size = int(Opp.size());

	Re_Opp.resize(Num + 1);
	Re_Opp[Num].resize(opp_size);

	for (int i = 0; i < opp_size; i++) {
		//対戦結果のベクターの和を格納する
		Re_Opp[Num][i] = int(std::accumulate(Opp[i].Result.begin(), Opp[i].Result.end(), 0.0));
	}
}