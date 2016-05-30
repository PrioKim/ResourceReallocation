#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define VM_COUNT	4
#define TOTAL_RESOURCE	16
#define RESOURCE_LIMIT	TOTAL_RESOURCE * 0.66
#define AGING_CONST	0.99f

float reasonable_usage = 0.7f; //s
int consumed_resource_all_VM = 0; // c

//int consumed_resource_each_vm[10] = { 0 };//V
int consumed_resource_each_vm[30] = { 4, 2, 1, 1, 0 };//V
//int demand_of_vm[10] = { 2,8,8,6,0 }; // D
int demand_of_vm[30] = { 4, 7, 8, 3, 0 }; // D
float weighted_vector[30] = { 0.5f, 0.8f, 0.3f, 0.4f, 0 };//W
float want_assign_resource[30] = { 0, }; //A
float usage_each_vm[30] = { 0.9f, 0.9f, 0.99f, 0.99f, 0 };//U

int remain_vm_cnt = VM_COUNT;
int aging_for_vm_cnt = VM_COUNT;
int i = 0, j = 0;
float vector_sum = 0.0, assign_sum = 0;

float aging_demand_cont = 0.9f;
float aging_aging = 0.9f;

int num_compute = 0; 
int n = VM_COUNT-1; // vm index

void print_info(){
	fprintf(stdout, "-----------------------------------\n");
	fprintf(stdout, "recursion depth : %d\n", n);
	fprintf(stdout, "%20s", "a_i(assign_avail) : ");
	for (i = 0; i < VM_COUNT; i++)
	{
		fprintf(stdout, "%f ", want_assign_resource[i]);

	}

	fprintf(stdout, "\n");
	fprintf(stdout, "%20s", "d_i(demand) : ");
	for (i = 0; i < VM_COUNT; i++)
		fprintf(stdout, "%d ", demand_of_vm[i]);

	fprintf(stdout, "\nassigned_resource : ");
	for (i = 0; i < VM_COUNT; i++)
		fprintf(stdout, "%d ", consumed_resource_each_vm[i]);

	fprintf(stdout, "\n%17s : %f", "aging constant", aging_demand_cont);
	fprintf(stdout, "\nconsumed_all_vm = %d\n TOTAL_RESOURCE = %d\n", consumed_resource_all_VM, TOTAL_RESOURCE);
	fprintf(stdout, "remain_vm_cnt = %d\n", remain_vm_cnt);
	fprintf(stdout, "compute_num = %d\n", num_compute);

	fprintf(stdout, "\n-----------------------------------");
	fprintf(stdout, "\n\n");
}

void aging(){
	//// aging(di) 
	if (n == 0){
		for (i = 0; i < VM_COUNT; i++)
			demand_of_vm[i] = ceil(demand_of_vm[i] * aging_demand_cont);

		aging_demand_cont = aging_demand_cont * aging_demand_cont; //pow(aging_demand_cont, 2); // 예를 들어 2,3에 0.7을 곱해도 1.4, 2.1이므로 천장함수 취하면 다시 2,3이 되버린다. 그래서 에이징이 더필요
	}
	////
}

int Allocation(){
	num_compute++;
	if (want_assign_resource[n] != 0) // 아직 할당인 안된 VM이면 if 진입
	{
		if (want_assign_resource[n] >= demand_of_vm[n]) // 시스템에서 할당하고자 하는 자원이 가상머신이 요구하는 자원보다 많거나 같을 때, 마지막이어도 과도하게 배정안되게
		{
			consumed_resource_each_vm[n] = demand_of_vm[n];
		}
		else if (remain_vm_cnt == 1)// 마지막인 경우인데 남은 자원이 원하는 만큼줄 수 없을 때
		{
			consumed_resource_each_vm[n] = (TOTAL_RESOURCE - consumed_resource_all_VM);
		}
		else // 자원 할당 불가능
		{
			return 0;
		}

		consumed_resource_all_VM += consumed_resource_each_vm[n]; // 할당한 총 자원 증가
		want_assign_resource[n] = 0; // 자원이 할당된 VM의 a_i = 0
		remain_vm_cnt--; // 자원 할당
		n = VM_COUNT - 1;

		assign_sum = 0;
		for (i = 0; i < VM_COUNT; i++)
			assign_sum += want_assign_resource[i];

		for (i = 0; i < VM_COUNT; i++){
			want_assign_resource[i] = (want_assign_resource[i] / assign_sum) * (TOTAL_RESOURCE - consumed_resource_all_VM);

			fprintf(stdout, "a_i = %f\n", want_assign_resource[i]);
		}
		return 1;
	}
	return 2;
}

int RUF()
{
	int ruf_r;
	print_info();
	int r = Allocation();
	aging();
	if (remain_vm_cnt == 0)
		return -1;	
	else if (r == 0 || r == 2)	{
		n--;
		if (n < 0)
			n = VM_COUNT-1;
		ruf_r = RUF();
	}
	else  if (r == 1)	{
		ruf_r = RUF();
	}
	if (ruf_r == -1)
		return -1;

	return 0;
}

int main()
{
	for (i = 0; i < VM_COUNT; i++)
	{
		vector_sum += weighted_vector[i];
	}


	for (i = 0; i < VM_COUNT; i++)
	{
		want_assign_resource[i] = (weighted_vector[i] / vector_sum) * TOTAL_RESOURCE; // 너무 벡터에 따라 할당됨... ㄴㄴ 총자원이 적은거였어

		//demand_of_vm[i] = ceil((usage_each_vm[i] * consumed_resource_each_vm[i]) / reasonable_usage);
		if (demand_of_vm[i] >= RESOURCE_LIMIT)
			demand_of_vm[i] = RESOURCE_LIMIT;
		consumed_resource_each_vm[i] = 0;
	}

	RUF();

	for (i = 0; i < VM_COUNT; i++)
	{
		printf("%d ", consumed_resource_each_vm[i]);
	}

	putchar('\n');
	putchar('\n');

	return 0;
}