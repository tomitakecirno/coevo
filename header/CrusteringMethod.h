#pragma once
#include "Cruster_Config.h"

//int Cal_Vec_Length(int Pop_Size);

/*
void Cru_Upgma(std::vector<Player> &Pop) {
	int Vec_Length = Cal_Vec_Length(int(Pop.size()));
	//�ŏ������̌̂̃y�A�����߂�
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
/* �N���X�^�[����           */
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
					// ���@�C�O���[�v���C�ϐ��̐��C�y�сC�f�[�^�̐�
	scanf("%d %d %d %d", &method, &L, &n, &N);

	g = new int [N];
	x = new double * [N];
					// �f�[�^
	for (i1 = 0; i1 < N; i1++) {
		x[i1] = new double [n];
		for (i2 = 0; i2 < n; i2++)
			scanf("%lf", &x[i1][i2]);
	}
					// �N���X�^�[����
	cluster(method, L, N, n, x, g);
					// �o��
	k = 1;
	while (k <= L) {
		k1 = -1;
		printf("�O���[�v %d\n", k);
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
/* �N���X�^�[����                */
/*      method : =1 : �ŒZ�����@ */
/*               =2 : �Œ������@ */
/*               =3 : ���W�A���@ */
/*               =4 : �d�S�@     */
/*               =5 : �Q���ϖ@   */
/*               =6 : �E�H�[�h�@ */
/*      L : �O���[�v�̐�         */
/*      N : �f�[�^�̐�           */
/*      n : �ϗʂ̐�             */
/*      x : �f�[�^               */
/*      g : ��������O���[�v�ԍ� */
/*********************************/
/*
#include <math.h>

void cluster(int method, int L, int N, int n, double **x, int *g)
{
	double **r, min;
	int ci = 0, cj = 0, i1, i2, k, M = N, *n_g;
					// �����ݒ�
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
					// ���s
	while (M > L) {
						// �ŏ������̃N���X�^�[��T��
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
						// �N���X�^�[��Z�����C���̃N���X�^�[�Ƃ̋������v�Z
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
					// �̈�̊J��
	for (i1 = 0; i1 < N; i1++)
		delete [] r[i1];
	delete [] r;
	delete [] n_g;
}
*/
/*********************************************/
/* �Q�̃f�[�^�Ԃ̋���                      */
/*      method : =0 : ���[�N���b�h����       */
/*               =1 : ���[�N���b�h�����̂Q�� */
/*      i,j : �f�[�^�ԍ�                     */
/*      n : �ϗʂ̐�                         */
/*      x : �f�[�^                           */
/*      return : ����                        */
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
/* �Q�̃N���X�^�[�Ԃ̋���                    */
/*      method : =1 : �ŒZ�����@               */
/*               =2 : �Œ������@               */
/*               =3 : ���W�A���@               */
/*               =4 : �d�S�@                   */
/*               =5 : �Q���ϖ@                 */
/*               =6 : �E�H�[�h�@               */
/*      Dio : �N���X�^�[i�ƃN���X�^�[o�Ƃ̋��� */
/*      Djo : �N���X�^�[j�ƃN���X�^�[o�Ƃ̋��� */
/*      Dij : �N���X�^�[i�ƃN���X�^�[j�Ƃ̋��� */
/*      ni : �N���X�^�[i�Ɋ܂܂��f�[�^��     */
/*      nj : �N���X�^�[j�Ɋ܂܂��f�[�^��     */
/*      no : �N���X�^�[o�Ɋ܂܂��f�[�^��     */
/*      x,y : �f�[�^                           */
/*      return : ����                          */
/***********************************************/
/*
double range_c(int method, double Dio, double Djo, double Dij, int ni, int nj, int no)
{
	double r = 0.0;
	int nk;

	switch (method) {
					// �ŒZ�����@
		case 1:
			r = (Dio <= Djo) ? Dio : Djo;
			break;
					// �Œ������@
		case 2:
			r = (Dio >= Djo) ? Dio : Djo;
			break;
					// ���W�A���@
		case 3:
			r = 0.5 * Dio + 0.5 * Djo - 0.25 * Dij;
			break;
					// �d�S�@
		case 4:
			nk = ni + nj;
			r  = ni * Dio / nk + nj * Djo / nk - (double)ni * nj * Dij / ((double)nk * nk);
			break;
					// �Q���ϖ@
		case 5:
			nk = ni + nj;
			r  = ni * Dio / nk + nj * Djo / nk;
			break;
					// �E�H�[�h�@
		case 6:
			nk = ni + nj;
			r  = ((ni + no) * Dio + (nj + no) * Djo - no * Dij) / ((double)nk + no);
			break;
	}

	return r;
}
*/