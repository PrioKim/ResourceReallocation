#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

#define NR_USER		4

float total_res[2] = { 9, 18 }; // CPU, MEM
float demands[16][2] = { { 2.0, 8.0 },
						{ 2.6, 2.0 },
						{ 4.0, 16.0 },
						{ 5.0, 8.0 },
						};
void MAX_MIN()
{
	float cpu_allocation[16] = { 0, };
	float mem_allocation[16] = { 0, };

	float cpu_remain_res = total_res[0];
	float mem_remain_res = total_res[1];
	int cpu_remain_user = NR_USER;
	int mem_remain_user = NR_USER;

	float cpu_demands[16];
	float mem_demands[16];

	float diff = 0;

	for (int i = 0; i < NR_USER; i++) {
		cpu_demands[i] = demands[i][0];
		mem_demands[i] = demands[i][1];
	}


	cout << "Max-Min " << endl;
	cout << "CPU Demands : ";
	for (int i = 0; i < NR_USER; i++)
		cout << cpu_demands[i] << " ";
	cout << endl;

	cout << "MEM Demands : ";
	for (int i = 0; i < NR_USER; i++)
		cout << mem_demands[i] << " ";
	cout << endl;

	while (cpu_remain_res > 0.00001) {
		float alloc_res = cpu_remain_res / cpu_remain_user;
		for (int i = 0; i < NR_USER; i++) {
			if (cpu_demands[i]){
				diff = cpu_demands[i] - alloc_res;
				if (diff <= 0){
					cpu_allocation[i] += cpu_demands[i];
					cpu_remain_res -= cpu_demands[i];
					cpu_remain_user--;
					cpu_demands[i] = 0;
				}
				else {
					cpu_demands[i] -= alloc_res;
					cpu_remain_res -= alloc_res;
					cpu_allocation[i] += alloc_res;
				}
			}
		}
	}

	while (mem_remain_res > 0.00001) {
		float alloc_res = mem_remain_res / mem_remain_user;
		for (int i = 0; i < NR_USER; i++) {
			if (mem_demands[i]){
				diff = mem_demands[i] - alloc_res;
				if (diff <= 0){
					mem_allocation[i] += mem_demands[i];
					mem_remain_res -= mem_demands[i];
					mem_remain_user--;
					mem_demands[i] = 0;
				}
				else {
					mem_demands[i] -= alloc_res;
					mem_remain_res -= alloc_res;
					mem_allocation[i] += alloc_res;
				}
			}
		}
	}


	cout << "CPU : ";
	for (int i = 0; i < NR_USER; i++)
		cout << cpu_allocation[i] << " ";
	cout << endl;
	cout << "MEM : ";
	for (int i = 0; i < NR_USER; i++)
		cout << mem_allocation[i] << " ";
	cout << endl;
}
void DRF() // demands를 태스크의 벡터로 줘야한다는 것 기억
{

	float demands_vec[16][2] = { 0, }; // {CPU, MEM}

	int dominant_res[16] = { 0, };// 0 : CPU, 1 : MEM
	float dominant_share[16] = { 0, };
	float consumed_res[2] = { 0, };
	float allocated_res[16][2] = { 0, };
	
	for (int i = 0; i < NR_USER; i++) {
		float base;
		
		if (demands[i][0] < demands[i][1]) // { 1, x.x }
			base = demands[i][0];
		else
			base = demands[i][1];

		demands_vec[i][0] = demands[i][0] / base;
		demands_vec[i][1] = demands[i][1] / base;
	}
	
	cout << "DRF " << endl;
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

	cout << "DRF" << endl;

	cout << "CPU : ";
	for (int i = 0; i < NR_USER; i++)
		cout << allocated_res[i][0] << " ";
	cout << endl;
	cout << "MEM : ";
	for (int i = 0; i < NR_USER; i++)
		cout << allocated_res[i][1] << " ";
	cout << endl;
}
void RUF()
{
	const float target_usage = 0.75;
	float aging_const = 0.9;

	float cpu_allocation[16];
	float mem_allocation[16];

	float cpu_consumed_res_by_all = 0;
	float mem_consumed_res_by_all = 0;

	float cpu_before_consumed_res[16] = { 4, 2, 1, 1, 0 };
	float mem_before_consumed_res[16] = { 8, 6, 0.5, 4, 0 };

	float cpu_demands[16];
	float mem_demands[16];

	float cpu_weighted_vec[16] = { 0.5f, 0.8f, 0.3f, 0.4f, 0 };
	float mem_weighted_vec[16] = { 0.2f, 0.9f, 0.5f, 0.2f, 0 };

	float cpu_want_assign_res[16];
	float mem_want_assign_res[16];

	float cpu_usage[16] = { 0.5f, 0.7f, 0.9f, 0.2f, 0 };
	float mem_usage[16] = { 0.6f, 0.3f, 0.7f, 0.9f, 0 };

	float cpu_vector_sum = 0.0;
	float mem_vector_sum = 0.0;

	float cpu_remain_user = NR_USER;
	float mem_remain_user = NR_USER;

	float cnt_for_aging;

	for (int i = 0; i < NR_USER; i++)
	{
		cpu_vector_sum += cpu_weighted_vec[i];
		mem_vector_sum += mem_weighted_vec[i];
	}

	for (int i = 0; i < NR_USER; i++)
	{
		cpu_want_assign_res[i] = (cpu_weighted_vec[i] / cpu_vector_sum) * total_res[0];
		cpu_demands[i] = ceil((cpu_usage[i] * cpu_before_consumed_res[i]) / target_usage);

		mem_want_assign_res[i] = (mem_weighted_vec[i] / mem_vector_sum) * total_res[1];
		mem_demands[i] = ceil((mem_usage[i] * mem_before_consumed_res[i]) / target_usage);
	}

	cnt_for_aging = NR_USER;
	aging_const = 0.9;
	while (cpu_remain_user) {
		float assign_sum = 0.0;
		for (int i = 0; i < NR_USER; i++)
			assign_sum += cpu_want_assign_res[i];

		for (int i = 0; i < NR_USER; i++)
			cpu_want_assign_res[i] =
			(cpu_want_assign_res[i] / assign_sum) * (total_res[0] - cpu_consumed_res_by_all);

		for (int i = 0; i < NR_USER; i++)
		{
			if (cpu_want_assign_res[i] != 0 && (cpu_want_assign_res[i] >= cpu_demands[i] || cpu_remain_user == 1))
			{
				if (cpu_want_assign_res[i] >= cpu_demands[i]) // 마지막이어도 과도하게 배정안되게
				{
					cpu_allocation[i] = cpu_demands[i];
				}
				else
				{
					cpu_allocation[i] = (total_res[0] - cpu_consumed_res_by_all);
				}

				cpu_consumed_res_by_all += cpu_allocation[i];
				cpu_want_assign_res[i] = 0;
				cpu_remain_user--;
				break;
			}
		}

		cnt_for_aging--;
		if (!cnt_for_aging) {
			for (int i = 0; i < NR_USER; i++)
				cpu_demands[i] = ceil(cpu_demands[i] * aging_const);

			aging_const *= aging_const;
			cnt_for_aging = NR_USER;
		}

	}

	cnt_for_aging = NR_USER;
	aging_const = 0.9;
	while (mem_remain_user) {
		float assign_sum = 0.0;
		for (int i = 0; i < NR_USER; i++)
			assign_sum += mem_want_assign_res[i];

		for (int i = 0; i < NR_USER; i++)
			mem_want_assign_res[i] =
			(mem_want_assign_res[i] / assign_sum) * (total_res[1] - mem_consumed_res_by_all);

		for (int i = 0; i < NR_USER; i++)
		{
			if (mem_want_assign_res[i] != 0 && (mem_want_assign_res[i] >= mem_demands[i] || mem_remain_user == 1))
			{
				if (mem_want_assign_res[i] >= mem_demands[i]) // 마지막이어도 과도하게 배정안되게
				{
					mem_allocation[i] = mem_demands[i];
				}
				else
				{
					mem_allocation[i] = (total_res[1] - mem_consumed_res_by_all);
				}

				mem_consumed_res_by_all += mem_allocation[i];
				mem_want_assign_res[i] = 0;
				mem_remain_user--;
				break;
			}
		}

		cnt_for_aging--;
		if (!cnt_for_aging) {
			for (int i = 0; i < NR_USER; i++)
				mem_demands[i] = ceil(mem_demands[i] * aging_const);

			aging_const *= aging_const;
			cnt_for_aging = NR_USER;
		}
	}

	cout << "VRUF ??" << endl;

	cout << "CPU : ";
	for (int i = 0; i < NR_USER; i++)
		cout << cpu_allocation[i] << " ";
	cout << endl;
	cout << "MEM : ";
	for (int i = 0; i < NR_USER; i++)
		cout << mem_allocation[i] << " ";
	cout << endl;
}

int main()
{
	DRF();
	MAX_MIN();
	RUF();
}