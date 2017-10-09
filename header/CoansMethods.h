#pragma once
/*=======================================
�����̊e���@�܂Ƃ�
CoansMode0	���_�̎�@
CoansMode1	����@�F���]�����@(�]���l�ɕ␳��������)
CoansMode2	����@�F�O�]�����@(�]���l�ɕ␳�������Ȃ�)
CoansMode3	�O��@�F���]�����@
CoansMode4	�O��@�F�O�]�����@
=======================================*/
#include "config.hpp"
#include "coansmodule.hpp"
#include "CsvModules.h"
#include "CrusteringMethod.h"
#include "Usual_Methods.hpp"
#include <time.h>
#include <string>
#include <fstream>

/*��@�̃N���X*/
class Coans{
//���J�����o
public :
	//��@�̃��C�����[�`��
	void	Coans_Tasks(int Trial);			//����@
	//�ΐ�񐔂��擾
	int		Get_MatchUp_Num();
	//�헪����������
	void	File_Write_Pop(int trial, int gene, bool s1);
	//�ȉ������p
	void	SetVec_Cr_Pop(std::vector<playerTK> &Pop);	//�N���X�^�ԍ���CSV�N���X��
	void	SetVec_Re_Pop(std::vector<playerTK> &Pop);	//�W�c�̑ΐ팋�ʂ��L�^
	void	SetVec_Re_Opp(std::vector<playerTK> &Opp);	//����W�c�̑ΐ팋�ʂ�CSV�N���X��
//����J�����o
private:
	virtual void Crustering() = 0;
	virtual void Generate_Opp() = 0;
	virtual void Return_Re(int Child_Num, int Opp_Num) = 0;
	virtual void Cal_Fitness() = 0;
	//���̃N���X�Ɣh���N���X����A�N�Z�X�\
protected:
	int Machup_Num;						//�ΐ��
	int Cr_Num;							//�N���X�^�[�ԍ�
	int Opp_Num;						//�ΐ��
	int MainParent;						//��e
	int Method_Num;						//��@��r�̎��Ɏg��
	int	Cru_K;							//�N���X�^�����O�p�����[�^�[
	std::string Dir;
	std::vector<int> SubParent;			//���e
	std::vector<playerTK> Pop;			//�i��������W�c
	std::vector<playerTK> Child;		//�q
	std::vector<playerTK> Opponent;		//�ΐ푊��
	//�ȉ������p
	std::vector<int> Cr_Pop;			//�N���X�^�ԍ����i�[
	std::vector<int> Re_Pop;			//���C���̑ΐ팋�ʂ��i�[
	std::vector<int> Re_Opp;			//�ΐ푊���
};
void Coans::Coans_Tasks(int Trial)
{
	//�W�c�錾
	Pop.resize(KO);
	init_genrand((unsigned)time(NULL)); /*����������*/

	//�W�c������
	for (int i = 0; i < KO; i++) {
		Pop[i].Init();
		Pop[i].Init_w();
	}
	Make_Directory(Dir, Method_Num, Trial, KU, PER, Cru_K);
	CsvModules Csv1(Method_Num);
	Csv1.Init(Method_Num, Trial, KU, PER, Cru_K);
	Cr_Pop.resize(KO);
	Re_Pop.resize(KO);

	//��������̐헪���L�^
	File_Write_Pop(Trial, 0, true);
	Machup_Num = 0;
	//�C�ӂ̐��㐔���[�v
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
		Crustering(); //�N���X�^�����O�B��@�ɂ���ĕς��
		//�����p
		if (Gene_Loop % PER == 0 || Gene_Loop == 1) {
			std::cout << "9" << ',';
			//�W�c�̃N���X�^�ԍ����x�N�^�[�Ɋi�[
			SetVec_Cr_Pop(Pop);
			//�i�[�����x�N�^�[��Csv�N���X��
			Csv1.SetCsv_Cr_P(Cr_Pop);
		}
		else {
			SetVec_Cr_Pop(Pop);
		}
		int tmpIndex;
		int tmpSub;
		SubParent.resize(0);
		std::cout << "2" << ',';
		//��e�I��
		MainParent = GetRand_Int(KO);
		//���e��I��
		for (int i = 0; i < PARENT; i++) {
			if (Pop[MainParent].List1.empty()) {
				break;
			}
			tmpIndex = GetRand_Int( int(Pop[MainParent].List1.size()) );
			tmpSub = Pop[MainParent].List1[tmpIndex];
			SubParent.push_back(tmpSub); //�v�f��ǉ�
			Pop[MainParent].List1.erase(Pop[MainParent].List1.begin() + tmpIndex); //�ǉ������v�f���폜
		}
		//���e������Έȉ��̏������s��
		if (!SubParent.empty()) {
			//�q�̐���
			Child.resize(CHILD + 1);

			std::cout << "3" << ',';
			//�g��XLM
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
			//�ΐ푊��̌̂�I��
			std::cout << "4" << ',';
			Generate_Opp();

			//w1,w2,w3:�q�̂̐헪�Cw1_T,w2_T,w3_T:�ΐ푊��̐헪
			//player1:�q�́Cplayer2:�ΐ푊��0
			std::cout << "5" << ',';
			int opponentLength = int(Opponent.size());

			for (int i = 0; i < opponentLength; i++) {
				Opponent[i].Result.resize(CHILD+1);
			}
			for (int i = 0; i < CHILD+1; i++) {
				Child[i].Result.resize(opponentLength);
			}
			//�����őΐ�
			for (int r = 0; r < opponentLength; r++) {//���胋�[�v
				StrategySet_T(Opponent[r]);
				for (int c = 0; c < CHILD + 1; c++) {//�W�c���[�v
					StrategySet_M(Child[c]);
					Machup_Num++;
					Competition();//�ΐ� player1 = �q�� palyer2 = �ΐ푊��H
					Return_Re(c, r);
				}
			}
			std::cout << "6" << ',';
			//�K���x�v�Z
			Cal_Fitness();

			std::cout << "7" << ',';
			//Best�̂��W�c��
			int Index = Choice_Best_Index(Child, false);
			Pop[MainParent] = Child[Index];

			//�����p
			if (Gene_Loop % PER == 0) {
				std::cout << "10" << ',';
				//100���㖈�ɐ헪���L�^
				File_Write_Pop(Trial, Gene_Loop / PER, true);
			}
			std::cout << "8";
			std::cout << "  [";
			for (int c = 0; c < CHILD + 1; c++) {
				std::cout << Child[c].eval << ",";
			}
			std::cout << "]" << std::endl;
			//�W�c�̉��ȊO������
			for (int i = 0; i < KO; i++) {
				Pop[i].Init();
			}
			/*
			std::cout << "  [";
			int Cr_Pop_Length = int(Cr_Pop.size());
			for (int cp = 0; cp < Cr_Pop_Length; cp++) {
				std::cout << Cr_Pop[cp] << ",";
			}
			std::cout << "]" << std::endl;
			*/
		}
		Loop_Time_End = clock();
		Loop_Time = (Loop_Time_End - Loop_Time_Start) / CLOCKS_PER_SEC;
		std::cout << "Time per gene : " << Loop_Time << " [sec]" << std::endl;
		assert( Gene_Loop <= KU );
	}
	Csv1.Fwrite_Cr_P();
	//Csv1.Fwrite_Re_P(C);
}
void Coans::SetVec_Cr_Pop(std::vector<playerTK> &Pop){
	//������
	Cr_Pop.assign(KO, 0);
	for (int i = 0; i < KO; i++) {
		assert( Pop[i].nitch < KO );
		assert( 0 <= Pop[i].nitch );
		Cr_Pop[ Pop[i].nitch ]++;
	}
	Cr_Pop.erase( std::remove(Cr_Pop.begin(), Cr_Pop.end(), 0), Cr_Pop.end() );
	assert( !Cr_Pop.empty() );
	/*
	std::cout << "[";
	int Cr_Pop_Length = int(Cr_Pop.size());
	for (int cp = 0; cp < Cr_Pop_Length; cp++) {
		std::cout << Cr_Pop[cp] << ",";
	}
	std::cout << "]" << std::endl;
	*/
}
void Coans::SetVec_Re_Pop(std::vector<playerTK> &Pop) {
	for (int i = 0; i < KO; i++) {
		//�ΐ팋�ʂ̃x�N�^�[�̘a���i�[����
		Re_Pop[i] = int(std::accumulate(Pop[i].Result.begin(), Pop[i].Result.end(), 0.0));
	}
}
void Coans::SetVec_Re_Opp(std::vector<playerTK> &Opp) {
	for (int i = 0; i < Opp_Num; i++) {
		//�ΐ팋�ʂ̃x�N�^�[�̘a���i�[����
		Re_Opp[i] = int(std::accumulate(Opp[i].Result.begin(), Opp[i].Result.end(), 0.0));
	}
}
int Coans::Get_MatchUp_Num() {
	return Machup_Num;
}
void Coans::File_Write_Pop(int trial, int gene, bool s1)
{
	//�l�������킹��
	for (int i = 0; i < KO; i++) {
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				w1_GA[i][j][k] = Pop[i].w1_CO[j][k];
			}
		}
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				w2_GA[i][j][k] = Pop[i].w2_CO[j][k];
			}
		}
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				w3_GA[i][j][k] = Pop[i].w3_CO[j][k];
			}
		}
	}
	//bef = true�̎�AI�Cfalse�̎�AIC
	char filename[50];
	for (int i = 0; i < KO; i++) {
		if (Cru_K == 0) {
			sprintf(filename, ("AI/%d/%d/%d/%d.dat"), Method_Num, trial, gene, i);
		}
		else if (0 < Cru_K) {
			sprintf(filename, ("AI/%d/%d/%d/%d/%d.dat"), Method_Num, trial, Cru_K, gene, i);
		}
		if ((fp = fopen(filename, "wb+")) == NULL) {
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		/*
		else {
			sprintf(filename, ("AIT/%d/%d.dat"), trial, i);
			if ((fp = fopen(filename, "wb+")) == NULL) {
				fprintf(stderr, "%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
		}
		*/
		fwrite(w1_GA[i], sizeof(double), I1*J1, fp);
		fwrite(w2_GA[i], sizeof(double), I2*J2, fp);
		fwrite(w3_GA[i], sizeof(double), I2*J1, fp);
		fclose(fp);
	}
	std::cout << "0 Generation written..." << std::endl;
}

/*
	mode1
	�E�H�[�h�@�̊K�w�I�N���X�^�����O�Ŗ����I�ɃN���X�^�����O
	�]����HP�̍���
*/
class CoansMode1 : public Coans {
public:
	CoansMode1(std::string str) {
		Method_Num = 1;
		Dir = str;
	}
private:
	//�N���X�^�����O�Ɏg���p�����[�^�[
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode1::Crustering() {
	//�ߖT���X�g����
	//MakeList(Pop, true, false, false);
	//�N���X�^�ԍ�����U��
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
	����	�FList1
	�ΐ푊��F�e�N���X�^�̑�\
	�K���x	�F�����Ƒ����HP�̍���
*/
class CoansMode2 : public Coans {
public:
	CoansMode2(std::string str) {
		Method_Num = 2;
		Dir = str;
		Cru_K = 0;
	}
private:
	//�N���X�^�����O�Ɏg���p�����[�^�[
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int ChildNumber, int Opp_Num);
	virtual void Cal_Fitness();
};
void CoansMode2::Crustering() {
	//�ߖT���X�g����
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
	����	�FList1
	�ΐ푊��F�W�c�S��
	�K���x	�F�����Ƒ����HP�̍���
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
	//�ߖT���X�g�������N���X�^�ԍ�����U��
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
	����	�FList1
	�ΐ푊��F�W�c�S��
	�K���x	�F�����Ƒ����HP�̍����{�K���x�ɒǉ��␳
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
	//�ߖT���X�g�������N���X�^�ԍ�����U��
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


/*���_��@�̃N���X*/
class Coans_GT2016 {
	//���J�����o
public:
	Coans_GT2016(std::string str, int k = 0) {
		Machup_Num = 0;					//�ΐ��
		Cr_Num = 1;						//�N���X�^�[�ԍ�
		Opp_Num = 0;					//�ΐ��
		Method_Num = 0;					//��@��r�̎��Ɏg��
		Cru_K = 0;						//�N���X�^�����O�p�����[�^�[
		Dir = str;
	}
	//��@�̃��C�����[�`��
	void	Coans_GT2016_Tasks(int Trial);
	void	Update_Opponent(int index);
	int		Cal_Gra_Nitch(int index);
	int		Get_MatchUp_Num();
	//�헪����������
	void	File_Write_Pop(int trial, int gene, bool s1);
	//�ȉ������p
	void	SetVec_Cr_Pop(std::vector<playerTK> &Pop);	//�N���X�^�ԍ���CSV�N���X��
	void	SetVec_Re_Pop(std::vector<playerTK> &Pop);	//�W�c�̑ΐ팋�ʂ��L�^
	void	SetVec_Re_Opp(std::vector<playerTK> &Opp);	//����W�c�̑ΐ팋�ʂ�CSV�N���X��
														//����J�����o
protected:
	int Machup_Num;						//�ΐ��
	int Cr_Num;							//�N���X�^�[�ԍ�
	int Opp_Num;						//�ΐ��
	int Method_Num;						//��@��r�̎��Ɏg��
	int	Cru_K;							//�N���X�^�����O�p�����[�^�[
	int MainParent;						//��e
	std::string Dir;
	std::vector<int> SubParent;			//���e
	std::vector<playerTK> Pop;			//�i��������W�c
	std::vector<playerTK> Child;		//�q
	std::vector<playerTK> Opponent;		//�ΐ푊��
	std::vector<double> Gra_Nitch_w1;
	std::vector<double> Gra_Nitch_w2;
	std::vector<double> Gra_Nitch_w3;
	//�ȉ������p
	std::vector<int> Cr_Pop;			//�N���X�^�ԍ����i�[
	std::vector<int> Re_Pop;			//���C���̑ΐ팋�ʂ��i�[
	std::vector<int> Re_Opp;			//�ΐ푊���
};
void Coans_GT2016::Coans_GT2016_Tasks(int Trial)
{
	std::cout << "Initiarize" << std::endl;

	//�W�c�錾
	Pop.resize(KO);
	Opponent.resize(KO);
	init_genrand((unsigned)time(NULL)); /*����������*/

										//�W�c������
	for (int i = 0; i < KO; i++) {
		Pop[i].Init();
		Pop[i].Init_w();
	}
	for (int i = 0; i < KO; i++) {
		Opponent[i].Init();
		Opponent[i].Init_w();
	}
	Make_Directory_GT2016(Trial, KU, PER);
	CsvModules Csv1(Method_Num);
	Csv1.Init(Method_Num, Trial, KU, PER, Cru_K);
	Cr_Pop.resize(KO);
	Re_Pop.resize(KO);

	//��������̐헪���L�^
	File_Write_Pop(Trial, 0, true);
	//�C�ӂ̐��㐔���[�v
	int Loop_Time_Start;
	int Loop_Time_End;
	int Loop_Time;
	for (int Gene_Loop = 1; Gene_Loop < KU + 1; Gene_Loop++) {
		Loop_Time_Start = clock();
		std::cout << Method_Num << ":" << Trial << ":" << Cru_K << ":" << Gene_Loop;
		std::cout << "  |  ";
		std::cout << "1" << ',';

		//�N���X�^�����O
		MakeList(Pop, K_List1, 0, 0);
		std::cout << "1-1" << ',';
		MakeList(Opponent, 0, K_List2, 0);
		Cr_Num = 1;
		for (int i = 0; i < KO; i++) {
			if (SetNitch(Cr_Num, i, Opponent) == 1) {
				Cr_Num++;
			}
		}

		//�����p
		if (Gene_Loop % PER == 0 || Gene_Loop == 1) {
			std::cout << "12" << ',';
			//�W�c�̃N���X�^�ԍ����x�N�^�[�Ɋi�[
			SetVec_Cr_Pop(Pop);
			//�i�[�����x�N�^�[��Csv�N���X��
			Csv1.SetCsv_Cr_P(Cr_Pop);
		}
		else {
			SetVec_Cr_Pop(Pop);
		}

		int tmpIndex;
		int tmpSub;
		SubParent.resize(0); //���e
		std::cout << "2" << ',';
		//��e�I��
		MainParent = GetRand_Int(KO);
		//���e��I��
		for (int i = 0; i < PARENT; i++) {
			if (Pop[MainParent].List1.empty()) {
				break;
			}
			tmpIndex = GetRand_Int(int(Pop[MainParent].List1.size()));
			tmpSub = Pop[MainParent].List1[tmpIndex];
			SubParent.push_back(tmpSub); //�v�f��ǉ�
			Pop[MainParent].List1.erase(Pop[MainParent].List1.begin() + tmpIndex); //�ǉ������v�f���폜
		}
		//���e������Έȉ��̏������s��
		if (!SubParent.empty()) {
			//�q�̐���
			Child.resize(CHILD + 1);
			Child[0] = Pop[MainParent];

			std::cout << "3" << ',';
			//�g��XLM
			ExtensionXLM(MainParent, SubParent, Pop, Child);
			//�ΐ푊��̌̂�I��
			std::cout << "4" << ',';

			//w1,w2,w3:�q�̂̐헪�Cw1_T,w2_T,w3_T:�ΐ푊��̐헪
			//player1:�q�́Cplayer2:�ΐ푊��0
			std::cout << "5" << ',';
			//����W�c�Ƃ̑ΐ팋�ʂ��i�[����x�N�^�[�����T�C�Y
			for (int i = 0; i < KO; i++) {
				Opponent[i].Result.resize(CHILD + 1);
			}
			for (int i = 0; i < CHILD + 1; i++) {
				Child[i].Result.resize(KO);
			}
			//�����őΐ�
			for (int r = 0; r < KO; r++) {//���胋�[�v
				StrategySet_T(Opponent[r]);
				for (int c = 0; c < CHILD + 1; c++) {//�W�c���[�v
					StrategySet_M(Child[c]);
					Machup_Num++;
					Competition();//�ΐ� player1 = �q�� palyer2 = �ΐ푊��H
					Opponent[r].Result[c] = (player2.hp - player1.hp) / 300;
					Child[c].Result[r] = (player1.hp - player2.hp) / 300;
				}
			}
			std::cout << "6" << ',';
			//�K���x�v�Z
			int Length = int(Child.size());
			for (int i = 0; i < Length; i++) {
				FitnessChild(Child[i], Opponent, false);
			}
			std::cout << "7" << ',';
			//Best�̂��W�c��
			int Index = Choice_Best_Index(Child, false);
			//Index = 3;
			//Best �� Child[Index];

			std::cout << "8" << ',';
			//�폜�\��̌̂������
			if (Index != 0) {
				int k;
				for (k = 0; k < KO; k++) {
					if (Opponent[k].delete_flag != 0) {
						break;
					}
				}
				if (k != KO) {
					Opponent[k] = Child[Index];
					Opponent[k].comp_flag = 1;
					Opponent[k].delete_flag = 0;
				}if (k == KO) {
					std::cout << "8-1" << ',';
					Update_Opponent(Index);
				}
			}
			std::cout << "9" << ',';
			for (int i = 0; i < KO; i++) {
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
			//�����p
			if (Gene_Loop % PER == 0) {
				std::cout << "11" << ',';
				//100���㖈�ɐ헪���L�^
				File_Write_Pop(Trial, Gene_Loop / PER, true);
			}
			std::cout << "10" << ',';
			std::cout << "  [";
			for (int c = 0; c < CHILD + 1; c++) {
				std::cout << Child[c].eval << ",";
			}
			std::cout << "]" << std::endl;
			//�W�c�̉��ȊO������
			for (int i = 0; i < KO; i++) {
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
		assert(Gene_Loop <= KU);
	}

	Csv1.Fwrite_Cr_P();
	//Csv1.Fwrite_Re_P(C);
}
void Coans_GT2016::SetVec_Cr_Pop(std::vector<playerTK> &Pop) {
	//������
	Cr_Pop.assign(KO, 0);
	for (int i = 0; i < KO; i++) {
		assert(Pop[i].nitch < KO);
		assert(0 <= Pop[i].nitch);
		Cr_Pop[Pop[i].nitch]++;
	}
	Cr_Pop.erase(std::remove(Cr_Pop.begin(), Cr_Pop.end(), 0), Cr_Pop.end());
	assert(!Cr_Pop.empty());
	/*
	std::cout << "[";
	int Cr_Pop_Length = int(Cr_Pop.size());
	for (int cp = 0; cp < Cr_Pop_Length; cp++) {
	std::cout << Cr_Pop[cp] << ",";
	}
	std::cout << "]" << std::endl;
	*/
}
void Coans_GT2016::SetVec_Re_Pop(std::vector<playerTK> &Pop) {
	for (int i = 0; i < KO; i++) {
		//�ΐ팋�ʂ̃x�N�^�[�̘a���i�[����
		Re_Pop[i] = int(std::accumulate(Pop[i].Result.begin(), Pop[i].Result.end(), 0.0));
	}
}
void Coans_GT2016::SetVec_Re_Opp(std::vector<playerTK> &Opp) {
	for (int i = 0; i < Opp_Num; i++) {
		//�ΐ팋�ʂ̃x�N�^�[�̘a���i�[����
		Re_Opp[i] = int(std::accumulate(Opp[i].Result.begin(), Opp[i].Result.end(), 0.0));
	}
}
int Coans_GT2016::Get_MatchUp_Num() {
	return Machup_Num;
}
void Coans_GT2016::File_Write_Pop(int trial, int gene, bool s1)
{
	double w1_Main[KO][I1][J1];//�W�c
	double w2_Main[KO][I2][J2];
	double w3_Main[KO][I2][J1];

	//�l�������킹��
	for (int i = 0; i < KO; i++) {
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
	//bef = true�̎�AI�Cfalse�̎�AIC
	FILE *fp_main;
	std::stringstream File_Name;
	std::stringstream Tmp_File_Name;

	File_Name << "./" << Dir << "/" << trial << "/" << gene / PER;
	//File_Name << "./" << Dir;

	for (int i = 0; i < KO; i++) {
		Tmp_File_Name << File_Name.str() << "/" << i << ".dat";
		//�t�@�C����������
		if ((fp_main = fopen(Tmp_File_Name.str().c_str(), "wb+")) == NULL) {
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		fwrite(w1_Main[i], sizeof(double), I1*J1, fp_main);
		fwrite(w2_Main[i], sizeof(double), I2*J2, fp_main);
		fwrite(w3_Main[i], sizeof(double), I2*J1, fp_main);

		//�N���A
		Tmp_File_Name.str("");
		Tmp_File_Name.clear(std::stringstream::goodbit);
		fclose(fp_main);
	}
}

void Coans_GT2016::Update_Opponent(int index)
{
	/*����Ȃ�������c���ƗL�Q�B�������Ŏc���̔��f���Ă���̂ŁA�]�v�ȏ��������J�E���g�����B*/
	/*�������ŏ��̃j�b�`�̏d�S�������āA�j�b�`�ԍ����擾*/
	int Nit_Count = 0;
	int Nit_Num = Cal_Gra_Nitch(index);
	//��ԋ߂��N���X�^�̔ԍ�������
	Child[index].nitch = Nit_Num;

	/*�퓬�f�[�^������*/
	for (int i = 0; i<KO; i++) {
		if (Opponent[i].nitch == Nit_Num) {
			Opponent[i].eval = 0;
			Opponent[i].flag = 0;
			Nit_Count++;
		}
	}
	//�ΐ�p�̃x�N�^�[�����
	std::vector<playerTK> Oppc(Nit_Count + 1);
	std::vector<double> Oppc_Eval(Nit_Count + 1);
	for (int i = 0; i < KO; i++) {
		if (Opponent[i].nitch == Nit_Num) {
			Oppc[Nit_Count] = Opponent[i];
			Nit_Count--;
		}
	}
	assert(0 <= Nit_Count);
	Child[index].eval = 0;
	Child[index].flag = 0;
	Oppc[0] = Child[index];

	/*�Y������j�b�`�̌̂Ǝq�̂Ń��[�O��*/
	int Oppc_size = int(Oppc.size());
	for (int i = 0; i < Oppc_size; i++) {
		//���N���X�^���̌̂Ƒΐ�
		Oppc[i].Result.resize(Oppc_size - 1);
		StrategySet_M(Oppc[i]);
		for (int j = i + 1; j < Oppc_size - 1; j++) {
			StrategySet_T(Oppc[j]);
			Competition();//�ΐ� player1 = �q�� palyer2 = �ΐ푊��H
			Oppc[i].Result[j] = (player1.hp - player2.hp) / 300;
			Oppc[j].Result[i] = (player2.hp - player1.hp) / 300;
			Machup_Num++;
		}
		//�q�̂Ƒΐ�
		Opponent[i].flag = 1; 		/*�t���O����*/
		Opponent[i].comp_flag = 1; 	/*�������������t���O*/
	}
	for (int i = 0; i < Oppc_size; i++) {
		Oppc_Eval[i] = std::accumulate(Oppc[i].Result.begin(), Oppc[i].Result.end(), 0.0);
	}

	/*�]���l�̍ŏ��̂����߂�*/
	auto min = min_element(Oppc_Eval.begin(), Oppc_Eval.end());
	int min_index = int(distance(Oppc_Eval.begin(), min));

	/*�폜�\��̌̘̂g�֎q�̂�����A�܂��͑���W�c�̒��Ŏq�̂��ア����̂Ƃ���ɓ����*/
	int j;
	for (int i = 0; i < Oppc_size; i++) {
		if (min_index != i) {
			for (j = 0; j < KO && Opponent[j].nitch != Nit_Num; j++) {}
			Opponent[j] = Oppc[i];
			Opponent[j].comp_flag = 1;
			Opponent[j].delete_flag = 0;
		}
	}
	/*�ΐ�f�[�^����уj�b�`�ԍ�������*/
	for (int i = 0; i < KO; i++) {
		Opponent[i].flag = 0;
		Opponent[i].eval = 0;
		Opponent[i].nitch = 0;
		Opponent[i].List2.clear();
	}
	MakeList(Opponent, 0, K_List2, 0);
}
int Coans_GT2016::Cal_Gra_Nitch(int index)
{
	std::vector<int> Nitch_Num;
	std::vector< std::vector < std::vector<double> > > Sum_N_W1;
	std::vector< std::vector < std::vector<double> > > Sum_N_W2;
	std::vector< std::vector < std::vector<double> > > Sum_N_W3;

	Nitch_Num = std::vector<int>(Cr_Num, 0);
	Sum_N_W1 = std::vector<std::vector<std::vector<double>>>(Cr_Num, std::vector<std::vector<double>>(I1, std::vector<double>(J1, 0)));
	Sum_N_W2 = std::vector<std::vector<std::vector<double>>>(Cr_Num, std::vector<std::vector<double>>(I2, std::vector<double>(J2, 0)));
	Sum_N_W3 = std::vector<std::vector<std::vector<double>>>(Cr_Num, std::vector<std::vector<double>>(J1, std::vector<double>(I2, 0)));

	std::cout << "8-1-1" << ',';
	std::cout << "Cr_Num:" << Cr_Num << ",";
	std::cout << "Sum_N_W1:" << Sum_N_W1.size() << ",";
	std::cout << "Opponent:" << Opponent.size() << ",";
	for (int Opp = 0; Opp < KO; Opp++) {
		//�e�̉��̂��ꂼ��̃x�N�g���𑫂�
		//w1_CO
		int Opp_Nitch = Opponent[Opp].nitch;
		std::cout << "Opp_Nitch:" << Opp_Nitch << ",";
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
	std::cout << "8-1-2" << ',';
	//�x�N�g���̏d�S�����߂�
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
	std::cout << "8-1-3" << ',';
	//�d�S�Ǝq�̂̋����v�Z
	double cal_sum = 0;
	std::vector<double> Dis_Vector(Cr_Num);
	//w1�̋���
	for (int Nit = 1; Nit < Cr_Num; Nit++) {
		for (int j = 0; j < I1; j++) {
			for (int k = 0; k < J1; k++) {
				Dis_Vector[Nit] += (Sum_N_W1[Nit][j][k] - Child[index].w1_CO[j][k])*(Sum_N_W1[Nit][j][k] - Child[index].w1_CO[j][k]);
			}
		}
		//w2�̋���
		for (int j = 0; j < I2; j++) {
			for (int k = 0; k < J2; k++) {
				Dis_Vector[Nit] += (Sum_N_W2[Nit][j][k] - Child[index].w2_CO[j][k])*(Sum_N_W2[Nit][j][k] - Child[index].w2_CO[j][k]);
			}
		}
		//w3�̋���
		for (int j = 0; j < J1; j++) {
			for (int k = 0; k < I2; k++) {
				Dis_Vector[Nit] += (Sum_N_W3[Nit][j][k] - Child[index].w3_CO[j][k])*(Sum_N_W3[Nit][j][k] - Child[index].w3_CO[j][k]);
			}
		}
		Dis_Vector[Nit] = sqrt(Dis_Vector[Nit]);
	}
	std::cout << "8-1-4" << ',';
	auto min = min_element(Dis_Vector.begin()+1, Dis_Vector.end());
	auto min_Nit = int(distance(Dis_Vector.begin(), min));
	return min_Nit;
}