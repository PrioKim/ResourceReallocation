#include <iostream>

using namespace std;

#define TOTAL_RES	10
#define NR_USER		4

int main()
{
	float demands[16] = { 2.0, 2.6, 4.0, 5.0, 0 };
	float allocation[16] = { 0, };

	float remain_res = TOTAL_RES;
	int remain_user = NR_USER;
	float diff = 0;
	
	cout << "Demands : ";
	for (int i = 0; i < NR_USER; i++)
		cout << demands[i] << " ";
	cout << endl;

	while (remain_res) {
		float alloc_res = remain_res / remain_user;
		for (int i = 0; i < NR_USER; i++) {
			if (demands[i]){
				diff = demands[i] - alloc_res;
				if (diff <= 0){
					allocation[i] += demands[i];
					remain_res -= demands[i];
					remain_user--;
					demands[i] = 0;					
				}
				else {
					demands[i] -= alloc_res;
					remain_res -= alloc_res;
					allocation[i] += alloc_res;
				}
			}
		}

		cout << "Alloc : ";
		for (int i = 0; i < NR_USER; i++)
			cout << allocation[i] << " ";
		cout << "remain : " << remain_res;
		cout << endl;
		

	}

	cout << "complete : ";
	for (int i = 0; i < NR_USER; i++)
		cout << allocation[i] << " ";
	cout << endl;

	return 0;
}