#include "../header/CrusteringMethod.h"
#include "../header/Cruster_Config.h"
#include <time.h>

using namespace std;

int main() {

	init_genrand((unsigned)time(NULL)); /*—”‰Šú‰»*/
	vector<Player> Pop(KO);
	for (int i = 0; i < KO; i++) {
		Pop[i].Init();
	}
	//Cru_Upgma(Pop);
	return 0;
}