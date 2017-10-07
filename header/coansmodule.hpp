#pragma once
/*-----------------------------------
ANS���g������Ď�@�̊֐��܂Ƃ�
-----------------------------------*/
#include "config.hpp"

int coans(int mode, int trial);
double cal_kotai_distance(playerTK one, playerTK another);
void MakeList(std::vector<playerTK> &pop, int Para_KL1, int Para_KL2, int Para_KL3);
void AnsList1(std::vector< std::vector<int> > &IndexSave, std::vector<playerTK> &pop);
void AnsList2(std::vector< std::vector<int> > &IndexSave, std::vector<playerTK> &pop);
void AnsList3(std::vector< std::vector<int> > &IndexSave, std::vector<playerTK> &pop);
int SetNitch(int nitch_number, int kotai, std::vector<playerTK> &pop);
void ExtensionXLM(int MainPare, std::vector<int> &SubPare, std::vector<playerTK> &pop, std::vector<playerTK> &child);
void choice_oppoment(std::vector<playerTK> &pop, std::vector<playerTK> &oppoment, int count_nitch);
void show_w(playerTK &one);
void FileWrite(std::vector<playerTK> &pop, int mode, int trial);
void ALL(int mode, int oppoment, int trial);
void ALLWatch(int mode, int trial);


//�����v��
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


//�ߖT���X�g�����
void MakeList(std::vector<playerTK> &pop, int Para_KL1, int Para_KL2, int Para_KL3)
{
	using namespace std;
	vector<vector<double> > DisSaveList1(KO);
	vector<vector<double> > DisSaveList2(KO);
	vector<vector<double> > DisSaveList3(KO);
	vector<vector<int> >    IndexSaveList1(KO); //K�Ԗڈȓ��̌̂��L�^���Ă���
	vector<vector<int> >    IndexSaveList2(KO); //K�Ԗڈȓ��̌̂��L�^���Ă���
	vector<vector<int> >    IndexSaveList3(KO); //K�Ԗڈȓ��̌̂��L�^���Ă���
	vector<double>::iterator min;
	size_t index;

	/*���g�ȊO�̌̂Ƃ̋����𑍓�����Ōv��.���g���܂�.*/
	/*���łɋ߂����Ɍ̂̔ԍ����擾*/
	for (int i = 0; i<KO; i++) {
		DisSaveList1[i].resize(KO);
		for (int j = 0; j<KO; j++) {
			if (i != j)
				DisSaveList1[i][j] = cal_kotai_distance(pop[i], pop[j]);
			else
				DisSaveList1[i][j] = 100000;
		}
	}
	//std::cout << "1-1-1" << ',';
	//��U�ڂ��Ă���
	DisSaveList2 = DisSaveList1;
	DisSaveList3 = DisSaveList1;
	for (int i = 0; i < KO; i++) {
		if (Para_KL1) {
			//IndexSaveList1�����
			IndexSaveList1[i].resize(Para_KL1);
			for (int j = 0; j < Para_KL1; j++) {
				min = min_element(DisSaveList1[i].begin(), DisSaveList1[i].end());
				index = distance(DisSaveList1[i].begin(), min);
				IndexSaveList1[i][j] = int(index);
				DisSaveList1[i][index] = 100000;
			}
		}
		//IndexSaveList2�����
		if (Para_KL2) {
			IndexSaveList2[i].resize(Para_KL2);
			for (int j = 0; j < Para_KL2; j++) {
				min = min_element(DisSaveList2[i].begin(), DisSaveList2[i].end());
				index = distance(DisSaveList2[i].begin(), min);
				IndexSaveList2[i][j] = int(index);
				DisSaveList2[i][index] = 100000;
			}
		}if (Para_KL3) {
			IndexSaveList3[i].resize(Para_KL3);
			for (int j = 0; j < Para_KL3; j++) {
				min = min_element(DisSaveList3[i].begin(), DisSaveList3[i].end());
				index = distance(DisSaveList3[i].begin(), min);
				IndexSaveList3[i][j] = int(index);
				DisSaveList3[i][index] = 100000;
			}
		}
	}
	//std::cout << "1-1-2" << ',';
	//AnsList1
	if(Para_KL1){
		AnsList1(IndexSaveList1, pop);
	}
	if (Para_KL2) {
		AnsList2(IndexSaveList2, pop);
	}
	if (Para_KL3) {
		AnsList3(IndexSaveList3, pop);
	}
	//std::cout << "1-1-3" << ',';
}


//�ߖT���X�g�����.List1�I�����[
void Make_List1_Only(std::vector<playerTK> &pop)
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
				DisSaveList1[i].push_back(10000);
		}
	}
	//��U�ڂ��Ă���
	DisSaveList2 = DisSaveList1;
	for (int i = 0; i < KO; i++) {
		//IndexSaveList1�����
		for (int j = 0; j < KL1; j++) {
			min = min_element(DisSaveList1[i].begin(), DisSaveList1[i].end());
			index = distance(DisSaveList1[i].begin(), min);
			IndexSaveList1[i].push_back(int(index));
			DisSaveList1[i][index] = 10000;
		}
	}
	//AnsList1
	AnsList1(IndexSaveList1, pop);
}

//List1
void AnsList1(std::vector< std::vector<int> > &IndexSave, std::vector<playerTK> &pop)
{
	//�ߖT���X�g1
	for (int i = 0; i<KO; i++) {
		if (IndexSave[i].empty()) {} //�󂾂����牽�����Ȃ�
		else {
			int IndexSaveLength = int(IndexSave[i].size());
			for (int j = 0; j<IndexSaveLength; j++) {
				pop[i].List1.push_back(IndexSave[i][j]);
			}
		}
	}
}

//List2
void AnsList2(std::vector< std::vector<int> > &IndexSave, std::vector<playerTK> &pop)
{
	//�ߖT���X�g2
	int Tmp;
	for (int i = 0; i<KO; i++) {
		if (IndexSave[i].empty()) {} //�󂾂����牽�����Ȃ�
		else {
			int IndexSaveLength = int(IndexSave[i].size());
			for (int j = 0; j<IndexSaveLength; j++) {
				Tmp = IndexSave[i][j]; //��U�C���f�b�N�X��ۑ�
				if (count(IndexSave[Tmp].begin(), IndexSave[Tmp].end(), i) != 0)
					pop[i].List2.push_back(Tmp); //����ɂ����݂���΋ߖT���X�g�։�����
			}
		}
	}
}

//List3
void AnsList3(std::vector< std::vector<int> > &IndexSave, std::vector<playerTK> &pop)
{
	//�ߖT���X�g1
	for (int i = 0; i < KO; i++) {
		int IndexSaveLength = int(IndexSave[i].size());
		for (int j = 0; j < IndexSaveLength; j++) {
			//i=j�̂Ƃ��͉������Ȃ�
			if (i == j) {}
			else {
				assert(!IndexSave[j].empty()); //�󂾂�����G���[
				if ( count(IndexSave[j].begin(), IndexSave[j].end(), i) != 0) {
					pop[i].List1.push_back(j);
				}
			}
		}
	}
}

//�ċA�I�Ƀj�b�`�����蓖�ĂĂ���
int SetNitch(int nitch_number, int kotai, std::vector<playerTK> &pop)
{
	int tmp;
	if (pop[kotai].nitch == 0) {
		pop[kotai].nitch = nitch_number;
		if (!pop[kotai].List2.empty()) {
			int List2Length = int(pop[kotai].List2.size());
			for (int i = 0; i < List2Length; i++) {
				tmp = pop[kotai].List2[i];
				SetNitch(nitch_number, tmp, pop);
			}
		}
		return 1;
	}
	return 0;
}

//�g�UELM.w1,w2,w3���ꂼ�ꕪ���Đ�������.
void ExtensionXLM(int MainPare, std::vector<int> &SubPare, std::vector<playerTK> &pop, std::vector<playerTK> &child)
{
	double sum_n_w1[I1][J1] = { 0 };
	double sum_n_w2[I2][J2] = { 0 };
	double sum_n_w3[J1][I2] = { 0 };
	int length = 0; //���e�̐����擾
					//w1,w2,w3���ꂼ�ꕪ���Ď擾

					//�e�̉��̂��ꂼ��̃x�N�g���𑫂�
	int SubPareLength = int(SubPare.size());
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
			SubPare_Gra_w1[j][k] = sum_n_w1[j][k] / SubPareLength;
		}
	}
	//w2_CO
	for (int j = 0; j < I2; j++) {
		for (int k = 0; k < J2; k++) {
			SubPare_Gra_w2[j][k] = sum_n_w2[j][k] / SubPareLength;
		}
	}
	//w3_CO
	for (int j = 0; j < J1; j++) {
		for (int k = 0; k < I2; k++) {
			SubPare_Gra_w3[j][k] = sum_n_w3[j][k] / SubPareLength;
		}
	}

	//�d�S����e�ɑ΂���x�N�g�������߂�
	double vector_w1[PARENT][I1][J1] = { 0 };
	double vector_w2[PARENT][I2][J2] = { 0 };
	double vector_w3[PARENT][J1][I2] = { 0 };
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
	for (int c = 1; c < CHILD+1; c++) {
		//������
		child[c].Init();
		child[c].Init_0();
		for (int i = 0; i < PARENT; i++) {
			coe_w1 = rand_normal(0, 1 / sqrt(PARENT));
			coe_w2 = rand_normal(0, 1 / sqrt(PARENT));
			coe_w3 = rand_normal(0, 1 / sqrt(PARENT));
			//w1_CO
			for (int j = 0; j < I1; j++) {
				for (int k = 0; k < J1; k++) {
					child[c].w1_CO[j][k] += coe_w1 * vector_w1[i][j][k];
				}
			}
			//w2_CO
			for (int j = 0; j < I2; j++) {
				for (int k = 0; k < J2; k++) {
					child[c].w2_CO[j][k] += coe_w2 * vector_w2[i][j][k];
				}
			}
			//w3_CO
			for (int j = 0; j < J1; j++) {
				for (int k = 0; k < I2; k++) {
					child[c].w3_CO[j][k] += coe_w3 * vector_w3[i][j][k];
				}
			}
		}
	}

}

//�ΐ푊���I��
void choice_oppoment(std::vector<playerTK> &pop, std::vector<playerTK> &oppoment, int count_nitch)
{
	using namespace std;
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
	tmpindex.resize(count_nitch);
	for (int i = 1; i < count_nitch; i++) {
		int tmpClusterLength = int(tmpCluster[i].size());
		if (tmpClusterLength != 0) {
			tmprand = GetRand_Int(tmpClusterLength);
			tmpindex[i] = tmpCluster[i][tmprand];
		}
	}
	int tmpindexLength = int(tmpindex.size());
	oppoment.resize(tmpindexLength);
	for (int i = 0; i < tmpindexLength; i++) {
		oppoment[i] = pop[ tmpindex[i] ];
	}
}

//��\��
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