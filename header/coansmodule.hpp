/*-----------------------------------
������ANS�g������@�̊֐��܂Ƃ�
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
���i��ANS 1�ō��̂�B2�őO�̂��
*****************/
int coans(int mode, int trial)
{
	using namespace std;
	cout << "mode:" << mode << endl;
	cout << "trial:" << trial << endl;
	bool s1, s2, s3;
	//s1:����W�c�̐؂�ւ��@s2:�]�����@�̐؂�ւ�
	//mode:0 ����@�F���]�����@(�]���l�ɕ␳��������)
	//mode:1 ����@�F�O�]�����@(�]���l�ɕ␳�������Ȃ�)
	//mode:2 �O��@�F���]�����@
	//mode:3 �O��@�F�O�]�����@
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

	//�W�c�錾
	vector<playerTK> pop(KO);
	init_genrand((unsigned)time(NULL)); /*����������*/
	int battle = 0;

	//�W�c������
	for (int i = 0; i < KO; i++) {
		pop[i].Init();
		pop[i].Init_w();
	}
	for (int e = 0; e < KU; e++) {
		cout << "���㐔:" << e << endl;
		MakeList(pop);
		//�N���X�^�ԍ�����U��
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
		//��e�A���e��I��
		MainPare = GetRand_Int(KO);
		for (int i = 0; i < KL1; i++) {
			if (pop[MainPare].List1.empty()) {
				break;
			}
			tmpIndex = GetRand_Int(pop[MainPare].List1.size());
			tmpSub = pop[MainPare].List1[tmpIndex];
			SubPare.push_back(tmpSub); //�v�f��ǉ�
			pop[MainPare].List1.erase(pop[MainPare].List1.begin() + tmpIndex); //�ǉ������v�f���폜
		}
		if (!SubPare.empty()) {
			//�q�̐���
			vector<playerTK> child(CHILD+1);
			vector<playerTK> oppoment;
			//�g��XLM
			child[0] = pop[MainPare];
			cout << "tes";
			ExtensionXLM(MainPare, SubPare, pop,child);
			//�ΐ푊��̌̂�I��
			if (s1) {
				oppoment = choice_oppoment(pop, count_nitch);
			}else {
				oppoment = pop;
			}
			//w1,w2,w3:�q�̂̐헪�Cw1_T,w2_T,w3_T:�ΐ푊��̐헪
			//player1:�q�́Cplayer2:�ΐ푊��0
			int oppomentLength = oppoment.size();
			for (int r = 0; r < oppomentLength; r++) {//���胋�[�v
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
			//number:(�q�́{�e��)��
			//�ΐ�p�z��Ɋi�[
			for (int c = 0; c < CHILD; c++) {//�W�c���[�v
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
			Competition();//�ΐ� player1 = �q�� palyer2 = �ΐ푊��H
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
			//�K���x�v�Z
			for (int i = 0; i < CHILD; i++){
				FitnessChild(child[i], oppoment,false);
			}
			//�\��
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
			//��Ԃ����̂����C���̏W�c��
			//s2:true �������]��
			//s2:false HP�̍����̊����]��
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
			//�W�c�̉��ȊO������
			for (int i = 0; i < KO; i++){
				pop[i].Init();
			}
		}
	}
	//�̂̐헪��������
	//FileWrite(pop,mode,trial);
	return battle;
}

/*************
�����v��
*************/
double cal_kotai_distance(playerTK one, playerTK another)
{
	double cal_sum = 0;
	//w1�̋���
	for (int j = 0; j < I1; j++) {
		for (int k = 0; k < J1; k++) {
			cal_sum += (one.w1_CO[j][k] - another.w1_CO[j][k])*(one.w1_CO[j][k] - another.w1_CO[j][k]);
		}
	}
	//w2�̋���
	for (int j = 0; j < I2; j++) {
		for (int k = 0; k < J2; k++) {
			cal_sum += (one.w2_CO[j][k] - another.w2_CO[j][k])*(one.w2_CO[j][k] - another.w2_CO[j][k]);
		}
	}
	//w3�̋���
	for (int j = 0; j < J1; j++) {
		for (int k = 0; k < I2; k++) {
			cal_sum += (one.w3_CO[j][k] - another.w3_CO[j][k])*(one.w3_CO[j][k] - another.w3_CO[j][k]);
		}
	}
	return sqrt(cal_sum);
}

/********************
�ߖT���X�g�����
********************/
void MakeList(std::vector<playerTK> &pop)
{
	using namespace std;
	vector<vector<double> > DisSaveList1(KO);
	vector<vector<double> > DisSaveList2(KO);
	vector<vector<int> >    IndexSaveList1(KO); //K�Ԗڈȓ��̌̂��L�^���Ă���
	vector<vector<int> >    IndexSaveList2(KO); //K�Ԗڈȓ��̌̂��L�^���Ă���
	vector<double>::iterator min;
	size_t index;

	/*���g�ȊO�̌̂Ƃ̋����𑍓�����Ōv��.���g���܂�.*/
	/*���łɋ߂����Ɍ̂̔ԍ����擾*/
	for (int i = 0; i<KO; i++) {
		for (int j = 0; j<KO; j++) {
			if (i != j)
				DisSaveList1[i].push_back(cal_kotai_distance(pop[i], pop[j]));
			else
				DisSaveList1[i].push_back(100000);
		}
	}
	//��U�ڂ��Ă���
	DisSaveList2 = DisSaveList1;
	for (int i = 0; i < KO; i++) {
		//IndexSaveList1�����
		for (int j = 0; j < KL1; j++) {
			min = min_element(DisSaveList1[i].begin(), DisSaveList1[i].end());
			index = distance(DisSaveList1[i].begin(), min);
			IndexSaveList1[i].push_back(index);
			DisSaveList1[i][index] = 100000;
		}
		//IndexSaveList2�����
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
	//�ߖT���X�g1
	for (int i = 0; i<KO; i++) {
		if (IndexSave[i].empty()) {} //�󂾂����牽�����Ȃ�
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
	//�ߖT���X�g2
	int Tmp;
	for (int i = 0; i<KO; i++) {
		if (IndexSave[i].empty()) {} //�󂾂����牽�����Ȃ�
		else {
			int IndexSaveLength = IndexSave[i].size();
			for (int j = 0; j<IndexSaveLength; j++) {
				Tmp = IndexSave[i][j]; //��U�C���f�b�N�X��ۑ�
				if (count(IndexSave[Tmp].begin(), IndexSave[Tmp].end(), i) != 0)
					pop[i].List2.push_back(Tmp); //����ɂ����݂���΋ߖT���X�g�։�����
			}
		}
	}
}

/*****************************
�ċA�I�Ƀj�b�`�����蓖�ĂĂ���
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
�g�UELM.w1,w2,w3���ꂼ�ꕪ���Đ�������.
***************************************/
void ExtensionXLM(int MainPare, std::vector<int> &SubPare, std::vector<playerTK> &pop, std::vector<playerTK> &child)
{
	using namespace std;

	double sum_n_w1[I1][J1] = { 0 };
	double sum_n_w2[I2][J2] = { 0 };
	double sum_n_w3[J1][I2] = { 0 };
	int length = 0; //���e�̐����擾
					//w1,w2,w3���ꂼ�ꕪ���Ď擾

					//�e�̉��̂��ꂼ��̃x�N�g���𑫂�
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

	//�x�N�g���̏d�S�����߂�
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

	//�d�S����e�ɑ΂���x�N�g�������߂�
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
	//�q�̂̐헪����
	double coe_w1;
	double coe_w2;
	double coe_w3;
	vector<vector<double> > sum_coe_w1(I1, vector<double>(J1));
	vector<vector<double> > sum_coe_w2(I2, vector<double>(J2));
	vector<vector<double> > sum_coe_w3(J1, vector<double>(I2));
	cout << "tess";
	for (int c = 1; c < CHILD + 1; c++) {
		//������
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
�ΐ푊���I��
**********************************/
std::vector<playerTK> choice_oppoment(std::vector<playerTK> &pop, int count_nitch)
{
	using namespace std;
	vector<playerTK> opp; //����W�c������
	vector<vector<int> > tmpCluster(count_nitch); //�C���f�b�N�X�������
	vector<int> tmpindex; //����W�c�֒ǉ�����̂̃C���f�b�N�X�������
	int tmprand;
	//�e�N���X�^�Ɍ̂̃C���f�b�N�X��U�蕪����
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
��\��
*****************/
void show_w(playerTK &one)
{
	using namespace std;
	cout << "��1" << endl;
	for (int j = 0; j < I1; j++) {
		cout << j << ":";
		for (int k = 0; k < J1; k++) {
			cout << one.w1_CO[j][k] << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "��2" << endl;
	for (int j = 0; j < I2; j++) {
		cout << j << ":";
		for (int k = 0; k < J2; k++) {
			cout << one.w2_CO[j][k] << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "��3" << endl;
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
���C���W�c��������
*****************/
void FileWrite(std::vector<playerTK> &pop, int mode, int trial)
{
	//�l�������킹��
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
	//bef = true�̎�AI�Cfalse�̎�AIC
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

/*�ΐ킳���ď������Ƃ������߂�.*/
void ALL(int mode, int oppoment, int trial) {
	using namespace std;
	bool s1, s2;
	switch (mode) {
	case 0:
		cout << "���݂̎�@/���݂̕]��vsFloreano" << endl;
		s1 = true;
		s2 = true;
		break;
	case 1:
		cout << "���݂̎�@/�O�̕]��vsFloreano" << endl;
		s1 = true;
		s2 = false;
		break;
	case 2:
		cout << "�O�̎�@/���݂̕]��vsFloreano" << endl;
		s1 = false;
		s2 = true;
		break;
	case 3:
		cout << "�O�̎�@/�O�̕]��vsFloreano" << endl;
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
		//�ΐ푊��
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
	//�K���x�̈�ԍ����̂��擾.test.dat�֋L�^.���łɏ����L�^.
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
	//���Ϗ����ƍō�����
	int winlength = win_count.size();
	cout << "win:";
	for (int i = 0; i < winlength; i++) {
		cout << win_count[i] << " ";
	}
	cout << endl;
	double ave_win = accumulate(win_count.begin(), win_count.end(), 0) / KO;
	cout << "�ō�����:" << win_count[index] << endl;
	cout << "���Ϗ���:" << ave_win << endl;
}

/*�����̏W�c���ł��ꂼ��ΐ킳���āC��ԗǂ������̂̋���������D*/
void ALLWatch(int mode, int trial)
{
	using namespace std;
	bool s1, s2;
	switch (mode) {
	case 0:
		cout << "���݂̎�@/���݂̕]��vsFloreano" << endl;
		s1 = true;
		s2 = true;
		break;
	case 1:
		cout << "���݂̎�@/�O�̕]��vsFloreano" << endl;
		s1 = true;
		s2 = false;
		break;
	case 2:
		cout << "�O�̎�@/���݂̕]��vsFloreano" << endl;
		s1 = false;
		s2 = true;
		break;
	case 3:
		cout << "�O�̎�@/�O�̕]��vsFloreano" << endl;
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
		//�ΐ푊��
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
	//�K���x�̈�ԍ����̂��擾.test.dat�֋L�^.���łɏ����L�^.
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
	//��Ԃ����̂̏�������
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

	//���Ϗ����ƍō�����
	int winlength = win_count.size();
	cout << "win:";
	for (int i = 0; i < winlength; i++) {
		cout << win_count[i] << " ";
	}
	cout << endl;
	double ave_win = accumulate(win_count.begin(), win_count.end(), 0) / KO;
	cout << "�ō�����:" << win_count[index] << endl;
	cout << "���Ϗ���:" << ave_win << endl;
}