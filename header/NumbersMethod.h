#include "Cruster_Config.h"

class Numbers {
public:
	int NumbersMatch(Player *one, Player *another)
private:

};
//‘Îí
/*************
‘Îí‚³‚¹‚éŠÖ” 0‚Å1”Ô–Ú‚Ìˆø”‚ÌŒÂ‘Ì‚ªŸ‚¿A1‚Å2”Ô–Ú‚Ìˆø”‚ÌŒÂ‘Ì‚ªŸ‚¿
*************/
/*
int NumbersMatch(Player *one, Player *another)
{
	int i;
	int min_one = 0, min_another = 0;
	double dis_one = cal_coord_distance(one, &Optimal[0]);
	double dis_another = cal_coord_distance(another, &Optimal[0]);
	double save_one;
	double save_another;

	for (i = 1; i < Optimal_N; i++) {
		if (i == 3) {
			save_one = cal_coord_distance(one, &Optimal[i]) + Optimal[i].hosei_n;
			save_another = cal_coord_distance(another, &Optimal[i]) + Optimal[i].hosei_n;
		}
		else if (i != 3) {
			save_one = cal_coord_distance(one, &Optimal[i]) + Optimal[i].hosei_n;
			save_another = cal_coord_distance(another, &Optimal[i]) + Optimal[i].hosei_n;
		}
		if (save_one < dis_one) {
			dis_one = save_one;
			min_one = i;
		}
		if (save_another < dis_another) {
			dis_another = save_another;
			min_another = i;
		}
	}

	double distance_one;
	double distance_another;

	distance_one = cal_coord_distance(one, &Optimal[min_one]);
	distance_another = cal_coord_distance(another, &Optimal[min_another]);

	battle_n++;
	one->eval = distance_one;
	another->eval = distance_another;
	if (one->eval < another->eval) {
		return 0;
	}
	else if (one->eval > another->eval) {
		return 1;
	}
}
*/
/*
double cal_coord_distance(Indiv *one, Xy_str *coord)
{
		double cal_sum = 0;
		int i;
		for (i = 0; i<DEM; i++) {
			cal_sum += (one->n[i] - coord->n[i])*(one->n[i] - coord->n[i]);
		}
		return(sqrt(cal_sum));
}
*/