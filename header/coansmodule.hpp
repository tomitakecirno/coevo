/*-----------------------------------
自分のANS使った手法の関数まとめ
-----------------------------------*/
#include<iostream>
#include "config.hpp"

int coans(int mode, int trial);
double cal_kotai_distance(playerTK one, playerTK another);
void MakeList(std::vector<playerTK> &pop);
void AnsList1(std::vector< std::vector<int> > &IndexSave, std::vector<playerTK> &pop);
void AnsList2(std::vector< std::vector<int> > &IndexSave, std::vector<playerTK> &pop);
int SetNitch(int nitch_number, int kotai, std::vector<playerTK> &pop);
void ExtensionXLM(int MainPare, std::vector<int> &SubPare, std::vector<playerTK> &pop, std::vector<playerTK> &child);
std::vector<playerTK> choice_oppoment(std::vector<playerTK> &pop, int count_nitch);
void show_w(playerTK &one);
void FileWrite(std::vector<playerTK> &pop, int mode, int trial);
void ALL(int mode, int oppoment, int trial);
void ALLWatch(int mode, int trial);

/*****************
共進化ANS 1で今のやつ。2で前のやつ
*****************/
int coans(int mode, int trial)
{
	using namespace std;
	cout << "mode:" << mode << endl;
	cout << "trial:" << trial << endl;
	bool s1, s2, s3;
	//s1:相手集団の切り替え　s2:評価方法の切り替え
	//mode:0 現手法：現評価方法(評価値に補正を加える)
	//mode:1 現手法：前評価方法(評価値に補正を加えない)
	//mode:2 前手法：現評価方法
	//mode:3 前手法：前評価方法
	switch (mode) {
	case 0:
		s1 = true;
		s2 = true;
		s3 = true;
		break;
	case 1:
		s1 = true;
		s2 = false;
		s3 = true;
		break;
	case 2:
		s1 = false;
		s2 = true;
		s3 = true;
		break;
	case 3:
		s1 = false;
		s2 = false;
		s3 = true;
		break;

	case 4:
		s1 = true;
		s2 = true;
		s3 = false;
		break;
	case 5:
		s1 = true;
		s2 = true;
		s3 = false;
		break;
	case 6:
		s1 = false;
		s2 = true;
		s3 = false;
		break;
	case 7:
		s1 = false;
		s2 = false;
		s3 = false;
		break;
	default:
		break;
	}

	//集団宣言
	vector<playerTK> pop(KO);
	init_genrand((unsigned)time(NULL)); /*乱数初期化*/
	int battle = 0;

	//集団初期化
	for (int i = 0; i < KO; i++) {
		pop[i].Init();
		pop[i].Init_w();
	}
	for (int e = 0; e < KU; e++) {
		cout << "世代数:" << e << endl;
		MakeList(pop);
		//クラスタ番号割り振り
		int count_nitch = 1;
		for (int i = 0; i < KO; i++) {
			if (SetNitch(count_nitch, i, pop) == 1) {
				count_nitch++;
			}
		}
		int MainPare;
		int tmpIndex;
		int tmpSub;
		vector<int> SubPare;
		//主親、副親を選ぶ
		MainPare = GetRand_Int(KO);
		for (int i = 0; i < KL1; i++) {
			if (pop[MainPare].List1.empty()) {
				break;
			}
			tmpIndex = GetRand_Int(pop[MainPare].List1.size());
			tmpSub = pop[MainPare].List1[tmpIndex];
			SubPare.push_back(tmpSub); //要素を追加
			pop[MainPare].List1.erase(pop[MainPare].List1.begin() + tmpIndex); //追加した要素を削除
		}
		if (!SubPare.empty()) {
			//子個体生成
			vector<playerTK> child(CHILD+1);
			vector<playerTK> oppoment;
			//拡張XLM
			child[0] = pop[MainPare];
			cout << "tes";
			ExtensionXLM(MainPare, SubPare, pop,child);
			//対戦相手の個体を選ぶ
			if (s1) {
				oppoment = choice_oppoment(pop, count_nitch);
			}else {
				oppoment = pop;
			}
			//w1,w2,w3:子個体の戦略，w1_T,w2_T,w3_T:対戦相手の戦略
			//player1:子個体，player2:対戦相手0
			int oppomentLength = oppoment.size();
			for (int r = 0; r < oppomentLength; r++) {//相手ループ
			//cout << r << ":";
			for (int i = 0; i < I1; i++) {
			for (int j = 0; j < J1; j++) {
			w1_T[i][j] = oppoment[r].w1_CO[i][j];
			}
			}
			for (int i = 0; i < I2; i++) {
			for (int j = 0; j < J2; j++) {
			w2_T[i][j] = oppoment[r].w2_CO[i][j];
			}
			}
			for (int i = 0; i < J1; i++) {
			for (int j = 0; j < I2; j++) {
			w3_T[i][j] = oppoment[r].w3_CO[i][j];
			}
			}
			//number:(子個体＋親個体)数
			//対戦用配列に格納
			for (int c = 0; c < CHILD; c++) {//集団ループ
			//cout << c << " ";
			for (int i = 0; i < I1; i++) {
			for (int j = 0; j < J1; j++) {
			w1[i][j] = child[c].w1_CO[i][j];
			}
			}
			for (int i = 0; i < I2; i++) {
			for (int j = 0; j < J2; j++) {
			w2[i][j] = child[c].w2_CO[i][j];
			}
			}
			for (int i = 0; i < I2; i++) {
			for (int j = 0; j < J1; j++) {
			w3[i][j] = child[c].w3_CO[i][j];
			}
			}
			battle++;
			Competition();//対戦 player1 = 子個体 palyer2 = 対戦相手？
			if (s2) {
			if (player1.win == 1) {
			oppoment[r].Result.push_back(0);
			child[c].Result.push_back(1);
			}
			else if (player2.win == 1) {
			oppoment[r].Result.push_back(1);
			child[c].Result.push_back(0);
			}
			else {
			oppoment[r].Result.push_back(0);
			child[c].Result.push_back(0);
			}
			}
			else {
			oppoment[r].Result.push_back((player2.hp - player1.hp) / 300.0);
			child[c].Result.push_back((player1.hp - player2.hp) / 300.0);
			}
			}
			//cout << endl;
			}
			//
			//適応度計算
			for (int i = 0; i < CHILD; i++){
				FitnessChild(child[i], oppoment,false);
			}
			//表示
			/*
			for (int i = 0; i < child.size(); i++){
			for (int j = 0; j < child[i].Result.size(); j++){
			cout << child[i].Result[j] << " ";
			}
			cout << ":";
			cout << child[i].eval << endl;
			}
			for (int i = 0; i < oppoment.size(); i++){
			for (int j = 0; j < oppoment[i].Result.size(); j++){
			cout << oppoment[i].Result[j] << " ";
			}
			cout << endl;
			}
			*/
			//一番いい個体をメインの集団へ
			//s2:true 勝利数評価
			//s2:false HPの差分の割合評価
			size_t index;
			if (s2) {
				//int
				vector<int>::iterator max;
				vector<int> tmpEval(CHILD + 1);
				for (int i = 0; i<CHILD + 1; i++) {
					tmpEval[i] = int(child[i].eval);
				}
				max = max_element(tmpEval.begin(), tmpEval.end());
				index = distance(tmpEval.begin(), max);
			}
			else {
				//double
				vector<double>::iterator max;
				vector<double> tmpEval(CHILD + 1);
				for (int i = 0; i<CHILD + 1; i++) {
						tmpEval[i] = child[i].eval;
					}
				max = max_element(tmpEval.begin(), tmpEval.end());
				index = distance(tmpEval.begin(), max);
			}
			cout << "index:" << index << endl;
			cout << "eval:" << child[index].eval << endl;
			cout << endl;
			//集団の解以外初期化
			for (int i = 0; i < KO; i++){
				pop[i].Init();
			}
		}
	}
	//個体の戦略書き込み
	//FileWrite(pop,mode,trial);
	return battle;
}

/*************
距離計測
*************/
double cal_kotai_distance(playerTK one, playerTK another)
{
	double cal_sum = 0;
	//w1の距離
	for (int j = 0; j < I1; j++) {
		for (int k = 0; k < J1; k++) {
			cal_sum += (one.w1_CO[j][k] - another.w1_CO[j][k])*(one.w1_CO[j][k] - another.w1_CO[j][k]);
		}
	}
	//w2の距離
	for (int j = 0; j < I2; j++) {
		for (int k = 0; k < J2; k++) {
			cal_sum += (one.w2_CO[j][k] - another.w2_CO[j][k])*(one.w2_CO[j][k] - another.w2_CO[j][k]);
		}
	}
	//w3の距離
	for (int j = 0; j < J1; j++) {
		for (int k = 0; k < I2; k++) {
			cal_sum += (one.w3_CO[j][k] - another.w3_CO[j][k])*(one.w3_CO[j][k] - another.w3_CO[j][k]);
		}
	}
	return sqrt(cal_sum);
}

/********************
近傍リストを作る
********************/
void MakeList(std::vector<playerTK> &pop)
{
	using namespace std;
	vector<vector<double> > DisSaveList1(KO);
	vector<vector<double> > DisSaveList2(KO);
	vector<vector<int> >    IndexSaveList1(KO); //K番目以内の個体を記録しておく
	vector<vector<int> >    IndexSaveList2(KO); //K番目以内の個体を記録しておく
	vector<double>::iterator min;
	size_t index;

	/*自身以外の個体との距離を総当たりで計測.自身も含む.*/
	/*ついでに近い順に個体の番号も取得*/
	for (int i = 0; i<KO; i++) {
		for (int j = 0; j<KO; j++) {
			if (i != j)
				DisSaveList1[i].push_back(cal_kotai_distance(pop[i], pop[j]));
			else
				DisSaveList1[i].push_back(100000);
		}
	}
	//一旦移しておく
	DisSaveList2 = DisSaveList1;
	for (int i = 0; i < KO; i++) {
		//IndexSaveList1を作る
		for (int j = 0; j < KL1; j++) {
			min = min_element(DisSaveList1[i].begin(), DisSaveList1[i].end());
			index = distance(DisSaveList1[i].begin(), min);
			IndexSaveList1[i].push_back(index);
			DisSaveList1[i][index] = 100000;
		}
		//IndexSaveList2を作る
		for (int j = 0; j < KL2; j++) {
			min = min_element(DisSaveList2[i].begin(), DisSaveList2[i].end());
			index = distance(DisSaveList2[i].begin(), min);
			IndexSaveList2[i].push_back(index);
			DisSaveList2[i][index] = 100000;
		}
	}
	//AnsList1
	AnsList1(IndexSaveList1, pop);
	//AnsList2
	AnsList2(IndexSaveList2, pop);
}

/******************
List1
******************/
void AnsList1(std::vector< std::vector<int> > &IndexSave, std::vector<playerTK> &pop)
{
	//近傍リスト1
	for (int i = 0; i<KO; i++) {
		if (IndexSave[i].empty()) {} //空だったら何もしない
		else {
			int IndexSaveLength = IndexSave[i].size();
			for (int j = 0; j<IndexSaveLength; j++) {
				pop[i].List1.push_back(IndexSave[i][j]);
			}
		}
	}
}

/******************
List2
******************/
void AnsList2(std::vector< std::vector<int> > &IndexSave, std::vector<playerTK> &pop)
{
	//近傍リスト2
	int Tmp;
	for (int i = 0; i<KO; i++) {
		if (IndexSave[i].empty()) {} //空だったら何もしない
		else {
			int IndexSaveLength = IndexSave[i].size();
			for (int j = 0; j<IndexSaveLength; j++) {
				Tmp = IndexSave[i][j]; //一旦インデックスを保存
				if (count(IndexSave[Tmp].begin(), IndexSave[Tmp].end(), i) != 0)
					pop[i].List2.push_back(Tmp); //相手にも存在すれば近傍リストへ加える
			}
		}
	}
}

/*****************************
再帰的にニッチを割り当てていく
*****************************/
int SetNitch(int nitch_number, int kotai, std::vector<playerTK> &pop)
{
	int tmp;
	if (pop[kotai].nitch == 0) {
		pop[kotai].nitch = nitch_number;
		if (!pop[kotai].List2.empty()) {
			int List2Length = pop[kotai].List2.size();
			for (int i = 0; i < List2Length; i++) {
				tmp = pop[kotai].List2[i];
				SetNitch(nitch_number, tmp, pop);
			}
		}
		return 1;
	}
	return 0;
}

/***************************************
拡散ELM.w1,w2,w3それぞれ分けて生成する.
***************************************/
void ExtensionXLM(int MainPare, std::vector<int> &SubPare, std::vector<playerTK> &pop, std::vector<playerTK> &child)
{
	using namespace std;

	double sum_n_w1[I1][J1] = { 0 };
	double sum_n_w2[I2][J2] = { 0 };
	double sum_n_w3[J1][I2] = { 0 };
	int length = 0; //副親の数を取得
					//w1,w2,w3それぞれ分けて取得

					//親の解のそれぞれのベクトルを足す
	int SubPareLength = SubPare.size();
	for (int i = 0; i<SubPareLength; i++) {
		//w1_CO
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				sum_n_w1[j][k] += pop[SubPare[i]].w1_CO[j][k];
			}
		}
		//w2_CO
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				sum_n_w2[j][k] += pop[SubPare[i]].w2_CO[j][k];
			}
		}
		//w3_CO
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				sum_n_w3[j][k] += pop[SubPare[i]].w3_CO[j][k];
			}
		}
		length++;
	}

	//ベクトルの重心を求める
	double SubPare_Gra_w1[I1][J1] = { 0 };
	double SubPare_Gra_w2[I2][J2] = { 0 };
	double SubPare_Gra_w3[J1][I2] = { 0 };
	//w1_CO
	for (int j = 0; j < I1; j++) {
		for (int k = 0; k < J1; k++) {
			SubPare_Gra_w1[j][k] = sum_n_w1[j][k] / SubPare.size();
		}
	}
	//w2_CO
	for (int j = 0; j < I2; j++) {
		for (int k = 0; k < J2; k++) {
			SubPare_Gra_w2[j][k] = sum_n_w2[j][k] / SubPare.size();
		}
	}
	//w3_CO
	for (int j = 0; j < J1; j++) {
		for (int k = 0; k < I2; k++) {
			SubPare_Gra_w3[j][k] = sum_n_w3[j][k] / SubPare.size();
		}
	}

	//重心から親に対するベクトルを求める
	double vector_w1[KL1][I1][J1];
	double vector_w2[KL1][I2][J2];
	double vector_w3[KL1][J1][I2];
	for (int i = 0; i<SubPareLength; i++) {
		//w1_CO
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				vector_w1[i][j][k] = pop[SubPare[i]].w1_CO[j][k] - SubPare_Gra_w1[j][k];
			}
		}
		//w2_CO
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				vector_w2[i][j][k] = pop[SubPare[i]].w2_CO[j][k] - SubPare_Gra_w2[j][k];
			}
		}
		//w3_CO
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				vector_w3[i][j][k] = pop[SubPare[i]].w3_CO[j][k] - SubPare_Gra_w3[j][k];
			}
		}
	}
	//子個体の戦略生成
	double coe_w1;
	double coe_w2;
	double coe_w3;
	vector<vector<double> > sum_coe_w1(I1, vector<double>(J1));
	vector<vector<double> > sum_coe_w2(I2, vector<double>(J2));
	vector<vector<double> > sum_coe_w3(J1, vector<double>(I2));
	cout << "tess";
	for (int c = 1; c < CHILD + 1; c++) {
		//初期化
		child[c].Init();

		//w1_CO
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				sum_coe_w1[j][k] = 0;
			}
		}
		//w2_CO
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				sum_coe_w2[j][k] = 0;
			}
		}
		//w3_CO
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				sum_coe_w3[j][k] = 0;
			}
		}
		for (int i = 0; i < SubPareLength; i++) {
			coe_w1 = rand_normal(0, 1 / sqrt(KL1));
			coe_w2 = rand_normal(0, 1 / sqrt(KL1));
			coe_w3 = rand_normal(0, 1 / sqrt(KL1));
			//w1_CO
			child[c].w1_CO.resize(I1);
			for (int j = 0; j < I1; j++) {
				child[c].w1_CO[i].resize(J1);
				for (int k = 0; k < J1; k++) {
					child[c].w1_CO[j][k] += coe_w1 * vector_w1[i][j][k];
				}
			}
			//w2_CO
			child[c].w2_CO.resize(I2);
			for (int j = 0; j < I2; j++) {
				child[c].w2_CO[i].resize(J2);
				for (int k = 0; k < J2; k++) {
					child[c].w2_CO[j][k] += coe_w2 * vector_w2[i][j][k];
				}
			}
			//w3_CO
			child[c].w3_CO.resize(J1);
			for (int j = 0; j < J1; j++) {
				child[c].w3_CO[i].resize(I2);
				for (int k = 0; k < I2; k++) {
					child[c].w3_CO[j][k] += coe_w3 * vector_w3[i][j][k];
				}
			}
		}
	}
}

/*********************************
対戦相手を選ぶ
**********************************/
std::vector<playerTK> choice_oppoment(std::vector<playerTK> &pop, int count_nitch)
{
	using namespace std;
	vector<playerTK> opp; //相手集団を入れる
	vector<vector<int> > tmpCluster(count_nitch); //インデックス入れるやつ
	vector<int> tmpindex; //相手集団へ追加する個体のインデックス入れるやつ
	int tmprand;
	//各クラスタに個体のインデックスを振り分ける
	for (int i = 0; i < count_nitch; i++) {
		for (int j = 0; j < KO; j++) {
			if (pop[j].nitch == i) {
				tmpCluster[i].push_back(j);
			}
		}
	}
	for (int i = 1; i < count_nitch; i++) {
		int tmpClusterLength = tmpCluster[i].size();
		if (tmpClusterLength != 0) {
			tmprand = GetRand_Int(tmpClusterLength);
			tmpindex.push_back(tmpCluster[i][tmprand]);
		}
	}
	int tmpindexLength = tmpindex.size();
	for (int i = 0; i < tmpindexLength; i++) {
		opp.push_back(pop[tmpindex[i]]);
	}
	return opp;
}

/*****************
解表示
*****************/
void show_w(playerTK &one)
{
	using namespace std;
	cout << "第1" << endl;
	for (int j = 0; j < I1; j++) {
		cout << j << ":";
		for (int k = 0; k < J1; k++) {
			cout << one.w1_CO[j][k] << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "第2" << endl;
	for (int j = 0; j < I2; j++) {
		cout << j << ":";
		for (int k = 0; k < J2; k++) {
			cout << one.w2_CO[j][k] << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "第3" << endl;
	for (int j = 0; j < J1; j++) {
		cout << j << ":";
		for (int k = 0; k < I2; k++) {
			cout << one.w3_CO[j][k] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

/*****************
メイン集団書き込み
*****************/
void FileWrite(std::vector<playerTK> &pop, int mode, int trial)
{
	//様式を合わせる
	for (int i = 0; i < KO; i++) {
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
	//bef = trueの時AI，falseの時AIC
	for (int i = 0; i < KO; i++) {
		if (mode < 4) {
			sprintf(filename, ("AI/%d/%d/%d.dat"), mode, trial, i);
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

/*対戦させて勝利数とかを求める.*/
void ALL(int mode, int oppoment, int trial) {
	using namespace std;
	bool s1, s2;
	switch (mode) {
	case 0:
		cout << "現在の手法/現在の評価vsFloreano" << endl;
		s1 = true;
		s2 = true;
		break;
	case 1:
		cout << "現在の手法/前の評価vsFloreano" << endl;
		s1 = true;
		s2 = false;
		break;
	case 2:
		cout << "前の手法/現在の評価vsFloreano" << endl;
		s1 = false;
		s2 = true;
		break;
	case 3:
		cout << "前の手法/前の評価vsFloreano" << endl;
		s1 = false;
		s2 = false;
		break;
	default:
		break;
	}
	int ai1, ai2;
	/*
	int ai_w[AICOUNT] = { 0 }, ai_l[AICOUNT] = { 0 }, ai_d[AICOUNT] = {0};
	int dre[AICOUNT] = { 0 };
	*/
	vector<playerTK> pop(KO);
	vector<playerTK> popsub(KO);
	vector<int> win_count(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].Init();
		popsub[i].Init();
	}

	for (ai1 = 0; ai1 < KO; ai1++) {
		//cout << ai1 << " ";
		sprintf(filename, ("AI/%d/%d/%d.dat"), mode, trial, ai1);
		if ((file = fopen(filename, "rb")) == NULL) {
			printf("file open error!!\n");
			exit(0);
		}
		fread(w1, sizeof(double), I1 * J1, file);
		fread(w2, sizeof(double), I2 * J2, file);
		fread(w3, sizeof(double), I2 * J1, file);

		fclose(file);
		win_count[ai1] = 0;
		//対戦相手
		for (ai2 = 0; ai2 < KO; ai2++) {
			sprintf(filename, ("AIT/%d/%d.dat"), oppoment, ai2);
			if ((fp = fopen(filename, "rb")) == NULL) {
				printf("file open error!!\n");
				exit(0);
			}
			fread(w1_T, sizeof(double), I1 * J1, fp);
			fread(w2_T, sizeof(double), I2 * J2, fp);
			fread(w3_T, sizeof(double), I2 * J1, fp);

			fclose(fp);

			Competition();
			if (player1.win == 1) {
				win_count[ai1]++;
			}
			if (s2) {
				if (player1.win == 1) {
					popsub[ai2].Result.push_back(0);
					pop[ai1].Result.push_back(1);
				}
				else if (player2.win == 1) {
					popsub[ai2].Result.push_back(1);
					pop[ai1].Result.push_back(0);
				}
				else {
					popsub[ai2].Result.push_back(0);
					pop[ai1].Result.push_back(0);
				}
			}
			else {
				popsub[ai2].Result.push_back((player2.hp - player1.hp) / 300.0);
				pop[ai1].Result.push_back((player1.hp - player2.hp) / 300.0);
			}
		}
		/*
		char str12[128] = { 0 };
		sprintf_s(str12, "%d", ai1);
		OutputDebugString(str12);
		OutputDebugString("\n");
		*/
	}
	//適応度の一番高い個体を取得.test.datへ記録.ついでに勝率記録.
	size_t index;
	if (s2) {
		vector<int> tmpEval(KO);
		vector<int>::iterator max;
		for (int i = 0; i < KO; i++) {
			FitnessChild(pop[i], popsub, s2);
			tmpEval[i] = int(pop[i].eval);
		}
		max = max_element(tmpEval.begin(), tmpEval.end());
		index = distance(tmpEval.begin(), max);
	}
	else {
		vector<double> tmpEval(KO);
		vector<double>::iterator max;
		for (int i = 0; i < KO; i++) {
			FitnessChild(pop[i], popsub, s2);
			tmpEval[i] = pop[i].eval;
		}
		max = max_element(tmpEval.begin(), tmpEval.end());
		index = distance(tmpEval.begin(), max);
	}
	//平均勝率と最高勝率
	int winlength = win_count.size();
	cout << "win:";
	for (int i = 0; i < winlength; i++) {
		cout << win_count[i] << " ";
	}
	cout << endl;
	double ave_win = accumulate(win_count.begin(), win_count.end(), 0) / KO;
	cout << "最高勝率:" << win_count[index] << endl;
	cout << "平均勝率:" << ave_win << endl;
}

/*自分の集団内でそれぞれ対戦させて，一番良かった個体の挙動を見る．*/
void ALLWatch(int mode, int trial)
{
	using namespace std;
	bool s1, s2;
	switch (mode) {
	case 0:
		cout << "現在の手法/現在の評価vsFloreano" << endl;
		s1 = true;
		s2 = true;
		break;
	case 1:
		cout << "現在の手法/前の評価vsFloreano" << endl;
		s1 = true;
		s2 = false;
		break;
	case 2:
		cout << "前の手法/現在の評価vsFloreano" << endl;
		s1 = false;
		s2 = true;
		break;
	case 3:
		cout << "前の手法/前の評価vsFloreano" << endl;
		s1 = false;
		s2 = false;
		break;
	default:
		break;
	}
	int ai1, ai2;
	/*
	int ai_w[AICOUNT] = { 0 }, ai_l[AICOUNT] = { 0 }, ai_d[AICOUNT] = {0};
	int dre[AICOUNT] = { 0 };
	*/
	vector<playerTK> pop(KO);
	vector<playerTK> popsub(KO);
	vector<int> win_count(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].Init();
		popsub[i].Init();
	}

	for (ai1 = 0; ai1 < KO; ai1++) {
		//cout << ai1 << " ";
		sprintf(filename, ("AI/%d/%d/%d.dat"), mode, trial, ai1);
		if ((file = fopen(filename, "rb")) == NULL) {
			printf("file open error!!\n");
			exit(0);
		}
		fread(w1, sizeof(double), I1 * J1, file);
		fread(w2, sizeof(double), I2 * J2, file);
		fread(w3, sizeof(double), I2 * J1, file);

		fclose(file);
		win_count[ai1] = 0;
		//対戦相手
		for (ai2 = 0; ai2 < KO; ai2++) {
			sprintf(filename, ("AI/%d/%d/%d.dat"), mode, trial, ai2);
			if ((fp = fopen(filename, "rb")) == NULL) {
				printf("file open error!!\n");
				exit(0);
			}
			fread(w1_T, sizeof(double), I1 * J1, fp);
			fread(w2_T, sizeof(double), I2 * J2, fp);
			fread(w3_T, sizeof(double), I2 * J1, fp);

			fclose(fp);

			Competition();
			if (player1.win == 1) {
				win_count[ai1]++;
			}
			if (s2) {
				if (player1.win == 1) {
					popsub[ai2].Result.push_back(0);
					pop[ai1].Result.push_back(1);
				}
				else if (player2.win == 1) {
					popsub[ai2].Result.push_back(1);
					pop[ai1].Result.push_back(0);
				}
				else {
					popsub[ai2].Result.push_back(0);
					pop[ai1].Result.push_back(0);
				}
			}
			else {
				popsub[ai2].Result.push_back((player2.hp - player1.hp) / 300.0);
				pop[ai1].Result.push_back((player1.hp - player2.hp) / 300.0);
			}
		}
		/*
		char str12[128] = { 0 };
		sprintf_s(str12, "%d", ai1);
		OutputDebugString(str12);
		OutputDebugString("\n");
		*/
	}
	//適応度の一番高い個体を取得.test.datへ記録.ついでに勝率記録.
	size_t index;
	if (s2) {
		vector<int> tmpEval(KO);
		vector<int>::iterator max;
		for (int i = 0; i < KO; i++) {
			FitnessChild(pop[i], popsub, s2);
			tmpEval[i] = int(pop[i].eval);
		}
		max = max_element(tmpEval.begin(), tmpEval.end());
		index = distance(tmpEval.begin(), max);
	}
	else {
		vector<double> tmpEval(KO);
		vector<double>::iterator max;
		for (int i = 0; i < KO; i++) {
			FitnessChild(pop[i], popsub, s2);
			tmpEval[i] = pop[i].eval;
		}
		max = max_element(tmpEval.begin(), tmpEval.end());
		index = distance(tmpEval.begin(), max);
	}
	//一番いい個体の書き込み
	sprintf(filename, ("AI/%d/%d/%d.dat"), mode, trial, index);
	if ((file = fopen(filename, "rb")) == NULL) {
		printf("file open error!!\n");
		exit(0);
	}
	fread(w1, sizeof(double), I1 * J1, file);
	fread(w2, sizeof(double), I2 * J2, file);
	fread(w3, sizeof(double), I2 * J1, file);

	sprintf(filename, ("AI/%d/%d/test.dat"), mode, trial);
	if ((fp = fopen(filename, "wb+")) == NULL) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	fwrite(w1, sizeof(double), I1*J1, fp);
	fwrite(w2, sizeof(double), I2*J2, fp);
	fwrite(w3, sizeof(double), I2*J1, fp);

	fclose(file);
	fclose(fp);

	//平均勝率と最高勝率
	int winlength = win_count.size();
	cout << "win:";
	for (int i = 0; i < winlength; i++) {
		cout << win_count[i] << " ";
	}
	cout << endl;
	double ave_win = accumulate(win_count.begin(), win_count.end(), 0) / KO;
	cout << "最高勝率:" << win_count[index] << endl;
	cout << "平均勝率:" << ave_win << endl;
}