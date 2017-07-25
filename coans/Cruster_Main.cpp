#include "../header/CrusteringMethod.h"
#include "../header/Cruster_Config.h"
#include <time.h>

using namespace std;

int main() {
	for (int d = 100; d < 11000; d *= 10) {
		for (int p = 30; p < 60; p += 10) {
			for (int k = 10; 2 < k; k--) {
				init_genrand((unsigned)time(NULL)); /*—”‰Šú‰»*/
				vector<Player> Pop(p);
				vector<int> Cruster_Num(p, 0);

				for (int i = 0; i < p; i++) {
					Pop[i].Init(d);
				}
				auto start = clock();
				Cru_Upgma(Pop, k);
				auto end = clock();

				auto time = end - start;
				cout << "[" << p << "," << d << "," << k << "]" << endl;
				cout << "process time:" << time << endl;

				for (int n = 0; n < p; n++) {
					Cruster_Num[Pop[n].Cr_Num]++;
				}
				cout << "member:[";
				for (int n = 0; n < p; n++) {
					if (Cruster_Num[n] > 0) {
						cout << Cruster_Num[n] << ",";
					}
				}
				cout << "]" << endl;
			}
		}
		cout << endl;
	}
	return 0;
}