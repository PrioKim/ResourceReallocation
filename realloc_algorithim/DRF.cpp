#include <iostream>
#include <string>

using namespace std;

#define NR_USER		2

float total_res[2] = { 9, 18 }; // CPU, MEM
 
float demands_vec[16][2] = { { 1, 4 }, { 3, 1 }, { 0, 0 } }; // {CPU, MEM}

int dominant_res[16] = { 0, };// 0 : CPU, 1 : MEM
float dominant_share[16] = { 0, };
float consumed_res[2] = { 0, };
float allocated_res[16][2] = { 0, };


int main()
{
	cout << "demands vector : ";
	for (int i = 0; i < NR_USER; i++){
		cout << "{" << demands_vec[i][0] << ", " << demands_vec[i][1] << "} ";
	}
	cout << endl;


	while (1) {
		int user_pick = 0;

		float tmp_share = dominant_share[0];
		for (int i = 1; i < NR_USER; i++){ // pick user i with lowest dominant share s_i
			if (tmp_share > dominant_share[i])
				user_pick = i;
		}

		 // D_i <- demand of user i's next task
		
		if (consumed_res[0] + demands_vec[user_pick][0] <= total_res[0]
			&& consumed_res[1] + demands_vec[user_pick][1] <= total_res[1]) { // if C + D_i <= R then

			consumed_res[0] += demands_vec[user_pick][0];
			allocated_res[user_pick][0] += demands_vec[user_pick][0];
			
			consumed_res[1] += demands_vec[user_pick][1];
			allocated_res[user_pick][1] += demands_vec[user_pick][1];

			float share1 = (allocated_res[user_pick][0] / total_res[0]);
			float share2 = (allocated_res[user_pick][1] / total_res[1]);
			dominant_share[user_pick] = share1 > share2 ? share1 : share2;
			
		}
		else
			break;
	}

	cout << "complete : ";
	for (int i = 0; i < NR_USER; i++){
		cout << "{" << allocated_res[i][0] << ", " << allocated_res[i][1] << "} ";
	}
	cout << endl;

	return 0;
}