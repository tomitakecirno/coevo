#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include<iostream>
#include <vector>
#include <algorithm>
#include"./header/MT.h"

#define I1 15//入力層
#define J1 30//中間層
#define I2 30//        
#define J2 13//出力層
#define KO 30//個体数
#define KOT 50//対戦相手数
#define KU 1//世代数
#define K 2//クラスタ数
#define PARENT 5   //親の数
#define CHILD 10 //子供の数
#define LLL 1
#define KL1 4 //List1で使う変数
#define KL2 2 //List2で使う変数
#define RIVAL 1 //対戦相手の対戦回数
#define COUNT 0//
#define COUNT_T 0//
#define TRIAL 5//
#define FLORET 5

using namespace std;

struct playerTK{
	double eval;
	int nitch;
	vector<double> Result; //対戦結果
	vector<int> List1;
	vector<int> List2;
	vector<vector<double> > w1_CO;
	vector<vector<double> > w2_CO;
	vector<vector<double> > w3_CO;
public:
	void Init();
	void Init_w();
};

double GetRand_Real(double pGetMax);
int GetRand_Int(int pGetMax);
void competition(playerTK &pop, playerTK &oppoment);
void FloreanoEval(void);
void FitnessChild(playerTK &child);
void BLX(vector<playerTK> &tmppop);

/**************
メイン
**************/
int main(void){
  FloreanoEval();
  return 0;
}

/**************
解以外を初期化
**************/
void playerTK::Init()
{
	eval = 0;
	nitch = 0;
	Result.clear();
	List1.clear();
	List2.clear();
}
/**************
解を初期化
**************/
void playerTK::Init_w()
{
	vector<vector<double> > tmpw1(I1);
	vector<vector<double> > tmpw2(I2);
	vector<vector<double> > tmpw3(J1);
	for (int i = 0; i < I1; i++){
		for (int j = 0; j < J1; j++){
			tmpw1[i].push_back(GetRand_Real(10));
		}
	}
	for (int i = 0; i < I2; i++){
		for (int j = 0; j < J2; j++){
			tmpw2[i].push_back(GetRand_Real(10));
		}
	}
	for (int i = 0; i < J1; i++){
		for (int j = 0; j < I2; j++){
			tmpw3[i].push_back(GetRand_Real(10));
		}
	}
	w1_CO = tmpw1;
	w2_CO = tmpw2;
	w3_CO = tmpw3;
}

void FloreanoEval(void) {
	//初期集団生成
	vector<playerTK> pop(KO);
	vector<playerTK> oppoment(FLORET);

	init_genrand((unsigned)time(NULL));
	//集団初期化
 
	vector<int> tmpN(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].Init();
		pop[i].Init_w();
		tmpN[i] = i;
	}
	int tmprand;
	//相手集団取得
	for(int i=0;i<FLORET;i++){
		tmprand = GetRand_Int(tmpN.size());
		//相手集団へ加える
		oppoment[i] = pop[ tmpN[tmprand] ];
		tmpN.erase(tmpN.begin() + tmprand);
	}
	//以下ループ
	for (int e = 0; e < 10; e++) {
	  cout << e << endl;
	  /*対戦*/
	  for(int i=0; i<KO; i++){
	    for(int j=0; j<FLORET; j++){
    	  competition(pop[i],oppoment[j]);
	    }
	  }
	  //評価値計算
	  for(int i=0; i<FLORET; i++){
	    FitnessChild(oppoment[i]);
	  }
	  for(int i=0; i<KO; i++){
	    FitnessChild(pop[i]);
	  }
	  
	  vector<double> tmpEval;
	  vector<playerTK> tmppop(KO);
	  //一旦保存
    for(int i=0; i<KO; i++){
      tmpEval.push_back(pop[i].eval);
	  }
	  //上位1/5を残す
	  for(int i=0; i<KO/5; i++){
      vector<double>::iterator max = max_element(tmpEval.begin(), tmpEval.end());
      size_t index = distance(tmpEval.begin(), max);
      tmppop[i] = pop[index];
      tmpEval[index] = 0;
    }
    //対戦相手の更新
    //e%FLORETで置き換える個体を取得できる．
    oppoment[e%FLORET] = tmppop[0];
    //交叉
    BLX(tmppop);
	}
}

/***************
交叉
***************/
void BLX(vector<playerTK> &tmppop)
{
  double dx,min_ca,max_ca;
  int tmp1,tmp2;

  vector<vector<double> > tmpw1(I1);
	vector<vector<double> > tmpw2(I2);
	vector<vector<double> > tmpw3(J1);
	
  for(int p = 6; p < KO; p++){
    while(1){
      tmp1 = GetRand_Int(KO/5);
      tmp2 = GetRand_Int(KO/5);
      if(tmp1 != tmp2){
        break;
      }
    }
		for(int i = 0; i < I1; i++) {
      tmpw1[i].reserve(J1);
		  for (int j = 0; j < J1; j++) {
		    //差の絶対値
			  dx = fabs(tmppop[tmp1].w1_CO[i][j] - tmppop[tmp2].w1_CO[i][j]);
			  min_ca = fmin(tmppop[tmp1].w1_CO[i][j],tmppop[tmp2].w1_CO[i][j]) - 0.3*dx;
				max_ca = fmax(tmppop[tmp1].w1_CO[i][j],tmppop[tmp2].w1_CO[i][j]) + 0.3*dx;
        tmpw1[i].push_back(min_ca + GetRand_Real(max_ca - min_ca));
			}
		}
		for (int i = 0; i < I2; i++) {
      tmpw2[i].reserve(J2);
		  for (int j = 0; j < J2; j++) {
			  dx = fabs(tmppop[tmp1].w2_CO[i][j] - tmppop[tmp2].w2_CO[i][j]);
			  min_ca = fmin(tmppop[tmp1].w2_CO[i][j],tmppop[tmp2].w2_CO[i][j]) - 0.3*dx;
				max_ca = fmax(tmppop[tmp1].w2_CO[i][j],tmppop[tmp2].w2_CO[i][j]) + 0.3*dx;
        tmpw2[i].push_back(min_ca + GetRand_Real(max_ca - min_ca));
			}
		}
		for (int i = 0; i < J1; i++) {
      tmpw3[i].reserve(I2);
      for (int j = 0; j < I2; j++) {
			  dx = fabs(tmppop[tmp1].w3_CO[i][j] - tmppop[tmp2].w3_CO[i][j]);
			  min_ca = fmin(tmppop[tmp1].w3_CO[i][j],tmppop[tmp2].w3_CO[i][j]) - 0.3*dx;
				max_ca = fmax(tmppop[tmp1].w3_CO[i][j],tmppop[tmp2].w3_CO[i][j]) + 0.3*dx;
        tmpw3[i].push_back(min_ca + GetRand_Real(max_ca - min_ca));
			}
		}
		tmppop[p].w1_CO = tmpw1;
		tmppop[p].w2_CO = tmpw2;
		tmppop[p].w3_CO = tmpw3;
	}
}

/****************
疑似的な対戦関数
****************/
void FitnessChild(playerTK &child)
{
  int sum_result = 0;
  for(int i=0; i<child.Result.size(); i++){
    sum_result += child.Result[i];
  }
  child.eval = sum_result;
}

/****************
疑似的な対戦関数
****************/
void competition(playerTK &pop, playerTK &oppoment)
{
  int tmprand;
  tmprand = GetRand_Int(2);
  if(tmprand == 1){
    pop.Result.push_back(1);
    oppoment.Result.push_back(0);
  }else {
    pop.Result.push_back(0);
    oppoment.Result.push_back(1);
  }  
}

/*****************
自然数乱数生成
*****************/
int GetRand_Int(int pGetMax)
{
	const double getrand_int32Max = 4294967295; // genrand_int32() の最大値。除算のためdouble
	return int(genrand_int32() / getrand_int32Max * pGetMax);
}

/*****************
実数乱数生成
*****************/
double GetRand_Real(double pGetMax)
{
	double get_save;
	const double getrand_int32Max = 4294967295; // genrand_int32() の最大値。除算のためdouble
	get_save = getrand_int32Max / 2;

	return ((double)genrand_int32() - get_save) / get_save*pGetMax;
}


