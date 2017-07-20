#pragma once
#include "Cruster_Config.h"

//int Cal_Vec_Length(int Pop_Size);

/*
void Cru_Upgma(std::vector<Player> &Pop) {
	int Vec_Length = Cal_Vec_Length(int(Pop.size()));
	//最小距離の個体のペアを求める
	vector<>Vec_Dis
}
*/
/*
int Cal_Vec_Length(int Pop_Size) {
	int Length = 0;
	for (int i = Pop_Size-1; 0 < i; i--) {
		Length += i;
	}
	return Length;
}
*/
/****************************/
/* クラスター分析           */
/*      coded by Y.Suganuma */
/****************************/
/*
#include <stdio.h>

void cluster(int, int, int, int, double **, int *);
double range(int, int, int, int, double **);
double range_c(int, double, double, double, int, int, int);

main()
{
	double **x;
	int i1, i2, k, k1, L, n, N, *g;
	int method;
					// 方法，グループ数，変数の数，及び，データの数
	scanf("%d %d %d %d", &method, &L, &n, &N);

	g = new int [N];
	x = new double * [N];
					// データ
	for (i1 = 0; i1 < N; i1++) {
		x[i1] = new double [n];
		for (i2 = 0; i2 < n; i2++)
			scanf("%lf", &x[i1][i2]);
	}
					// クラスター分析
	cluster(method, L, N, n, x, g);
					// 出力
	k = 1;
	while (k <= L) {
		k1 = -1;
		printf("グループ %d\n", k);
		for (i1 = 0; i1 < N && k1 < 0; i1++) {
			if (g[i1] >= 0)
				k1 = g[i1];
		}
		for (i1 = 0; i1 < N; i1++) {
			if (g[i1] == k1) {
				printf("   %d", i1);
				for (i2 = 0; i2 < n; i2++)
					printf(" %f", x[i1][i2]);
				printf("\n");
				g[i1] = -1;
			}
		}
		k++;
	}

	for (i1 = 0; i1 < N; i1++)
		delete [] x[i1];
	delete [] x;
	delete [] g;

	return 0;
}
*/
/*********************************/
/* クラスター分析                */
/*      method : =1 : 最短距離法 */
/*               =2 : 最長距離法 */
/*               =3 : メジアン法 */
/*               =4 : 重心法     */
/*               =5 : 群平均法   */
/*               =6 : ウォード法 */
/*      L : グループの数         */
/*      N : データの数           */
/*      n : 変量の数             */
/*      x : データ               */
/*      g : 所属するグループ番号 */
/*********************************/
/*
#include <math.h>

void cluster(int method, int L, int N, int n, double **x, int *g)
{
	double **r, min;
	int ci = 0, cj = 0, i1, i2, k, M = N, *n_g;
					// 初期設定
	k   = (method < 4) ? 0 : 1;
	n_g = new int [N];
	r   = new double * [N];
	for (i1 = 0; i1 < N; i1++) {
		g[i1]   = i1;
		n_g[i1] = 1;
		r[i1]   = new double [N];
		for (i2 = i1+1; i2 < N; i2++)
			r[i1][i2] = range(k, i1, i2, n, x);
	}
	for (i1 = 0; i1 < N; i1++) {
		for (i2 = i1+1; i2 < N; i2++)
			r[i2][i1] = r[i1][i2];
	}
					// 実行
	while (M > L) {
						// 最小距離のクラスターを探す
		min = -1.0;
		for (i1 = 0; i1 < N; i1++) {
			if (g[i1] == i1) {
				for (i2 = i1+1; i2 < N; i2++) {
					if (g[i2] == i2) {
						if (min < 0.0 || r[i1][i2] < min) {
							min = r[i1][i2];
							ci  = i1;
							cj  = i2;
						}
					}
				}
			}
		}
						// クラスターを融合し，他のクラスターとの距離を計算
		for (i1 = 0; i1 < N; i1++) {
			if (g[i1] == i1) {
				for (i2 = i1+1; i2 < N; i2++) {
					if (g[i2] == i2) {
						if (i1 != cj && i2 != cj) {
							if (i1 == ci) {
								r[i1][i2] = range_c(method, r[ci][i2], r[cj][i2], r[ci][cj],
                                                    n_g[ci], n_g[cj], n_g[i2]);
								r[i2][i1] = r[i1][i2];
							}
							else if (i2 == ci) {
								r[i1][i2] = range_c(method, r[i1][ci], r[i1][cj], r[ci][cj],
                                                    n_g[ci], n_g[cj], n_g[i2]);
								r[i2][i1] = r[i1][i2];
							}
						}
					}
				}
			}
		}

		n_g[ci] += n_g[cj];
		for (i1 = 0; i1 < N; i1++) {
			if (g[i1] == cj)
				g[i1] = ci;
		}

		M--;
	}
					// 領域の開放
	for (i1 = 0; i1 < N; i1++)
		delete [] r[i1];
	delete [] r;
	delete [] n_g;
}
*/
/*********************************************/
/* ２つのデータ間の距離                      */
/*      method : =0 : ユークリッド距離       */
/*               =1 : ユークリッド距離の２乗 */
/*      i,j : データ番号                     */
/*      n : 変量の数                         */
/*      x : データ                           */
/*      return : 距離                        */
/*********************************************/
/*
double range(int method, int i, int j, int n, double **x)
{
	double x1, r = 0.0;
	int i2;

	for (i2 = 0; i2 < n; i2++) {
		x1  = x[i][i2] - x[j][i2];
		r  += x1 * x1;
	}
	if (method == 0)
		r = sqrt(r);

	return r;
}
*/
/***********************************************/
/* ２つのクラスター間の距離                    */
/*      method : =1 : 最短距離法               */
/*               =2 : 最長距離法               */
/*               =3 : メジアン法               */
/*               =4 : 重心法                   */
/*               =5 : 群平均法                 */
/*               =6 : ウォード法               */
/*      Dio : クラスターiとクラスターoとの距離 */
/*      Djo : クラスターjとクラスターoとの距離 */
/*      Dij : クラスターiとクラスターjとの距離 */
/*      ni : クラスターiに含まれるデータ数     */
/*      nj : クラスターjに含まれるデータ数     */
/*      no : クラスターoに含まれるデータ数     */
/*      x,y : データ                           */
/*      return : 距離                          */
/***********************************************/
/*
double range_c(int method, double Dio, double Djo, double Dij, int ni, int nj, int no)
{
	double r = 0.0;
	int nk;

	switch (method) {
					// 最短距離法
		case 1:
			r = (Dio <= Djo) ? Dio : Djo;
			break;
					// 最長距離法
		case 2:
			r = (Dio >= Djo) ? Dio : Djo;
			break;
					// メジアン法
		case 3:
			r = 0.5 * Dio + 0.5 * Djo - 0.25 * Dij;
			break;
					// 重心法
		case 4:
			nk = ni + nj;
			r  = ni * Dio / nk + nj * Djo / nk - (double)ni * nj * Dij / ((double)nk * nk);
			break;
					// 群平均法
		case 5:
			nk = ni + nj;
			r  = ni * Dio / nk + nj * Djo / nk;
			break;
					// ウォード法
		case 6:
			nk = ni + nj;
			r  = ((ni + no) * Dio + (nj + no) * Djo - no * Dij) / ((double)nk + no);
			break;
	}

	return r;
}
*/