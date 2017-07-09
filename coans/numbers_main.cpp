#include <iostream>
#include "../header/numbers_main.h"

void RexStar(playerTK &pare, playerTK &child);

int main(){
	return 0;
}


/************
JGG＋REX
勝ちポイント基準
************/
void RexStar(std::vector<playerTK> &pare, std::vector<playerTK> &child)
{
	double base_gra[DEM] = { 0 }; /*基準となる重心*/
	double sum_n[DEM] = { 0 }; /*x,yそれぞれの和を一時的に保存しておく配列*/
	/*ベースとなる重心を求める*/
	int pare_size = pare.size();
	for (int i = 0; i<pare_size; i++){
		for (int j = 0; j<DEM; j++){
			sum_n[j] += pare[i].n[j]; /*親のx,yをそれぞれ足す*/
		}
	}
	for (int i = 0; i<DEM; i++){
		base_gra[i] = sum_n[i] / pare_size; /*親のそれぞれx,yの和を割る*/
	}
	/*重心から親に対するベクトルを求める*/
	double vector[pare_size][DEM];
	for (int i = 0; i<pare_size; i++){
		for (int j = 0; j<DEM; j++){
			vector[i][j] = pare[i].n[j] - base_gra[j];
		}
	}
	/*重心をもとに親を複製する*/
	playerTK sub_pare[pare_size];

	for (int i = 0; i<pare_size; i++){
		sub_pare[i].Init();
		for (int j = 0; j<DEM; j++){
			sub_pare[i].n[j] = pare[i].n[j] - 2 * vector[i].n[j];
		}
	}
	/*親をまとめる*/
	playerTK sum_pare[pare_size];
	for (i = 0; i<Np; i++){
		/*前半分にpare構造体を、後半分に複製体を入れる*/		sum_pare[i] = pare[i];		sum_pare[i].Init();

		sum_pare[pare_size + i] = sub_pare[i];		sum_pare[pare_size + i].Init();
	}
	/*ナンバーズを適用して評価のいい順にソート*/
	Pare_Numbers(sum_pare);
	sort_win(sum_pare, Np * 2); 
	/*上位半分の両個体の重心、下位半分の両個体の重心を求める*/
	double high_gra[DEM] = { 0 };
	for (j = 0; j<DEM; j++){
		sum_n[j] = 0;
	}
	for (i = 0; i<Np; i++){
		for (j = 0; j<DEM; j++){
			sum_n[j] += sum_pare[i].n[j];
		}
	}

	for (i = 0; i<DEM; i++){
		high_gra[i] = sum_n[i] / Np;
	}
	/*子個体生成*/
	double diag[DEM];
	double sum_coe[DEM] = { 0 }, coe = 0; /*親のベクトル*乱数の総和*/
	double child_save[DEM];
	int count = 0;
	true_flag = 1;
	while (true_flag){
		/*式11の第２項の値生成*/
		for (i = 0; i<DEM; i++){
			diag[i] = genrand_real1()*T;
			sum_coe[i] = 0;
		}
		/*式(11)の第３項の動作*/
		for (i = 0; i<Np; i++){
			coe = GetRand_Real(sqrt(3 / (DEM + 1)));
			for (j = 0; j<DEM; j++){
				sum_coe[j] += coe * (sum_pare[i].n[j] - base_gra[j]);
			}
		}
		for (i = 0; i<DEM; i++){
			child_save[i] = base_gra[i] + diag[i] * (high_gra[i] - base_gra[i]) + sum_coe[i];
		}
		if (fabs(child_save[0]) <= 100.00 && fabs(child_save[1]) <= 100.00){
			for (i = 0; i<DEM; i++){
				child[count].n[i] = child_save[i]; /*式(11)の動作*/
			}
			count++;
		}
		if (count == Nc){
			true_flag = 0;
		}
	}
}