#include "../header/CrusteringMethod.h"
#include "../header/Cruster_Config.h"
#include <fstream>
#include <time.h>

using namespace std;

int main() {
	vector<Player> Pop(KO);
	for (int i = 0; i < KO; i++) {
		Pop[i].Init();
	}
	//Cru_Upgma(Pop);
	return 0;
}