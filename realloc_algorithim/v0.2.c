#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define CPU			8		// 8 core
#define MEM			16		// 16 GB
#define VM_COUNT	4
#define TOTAL_RESOURCE	8
#define RESOURCE_LIMIT	TOTAL_RESOURCE * 0.66
#define VM_LIMIT	(((CPU) <= (MEM)) ? (CPU) : (MEM))
#define AGING_CONST	0.99f

float reasonable_usage = 0.7f; //s
int consumed_resource_all_VM = 0; // c

//int consumed_resource_each_vm[10] = { 0 };//V
int consumed_resource_each_vm[30] = { 4, 2, 1, 1, 0 };//V
//int demand_of_vm[10] = { 2,8,8,6,0 }; // D
int demand_of_vm[30] = { 7, 7, 7, 7, 0 }; // D
float weighted_vector[30] = { 0.9f, 0.9f, 0.9f, 0.9f, 0 };//W
float want_assign_resource[30] = { 0, }; //A
float usage_each_vm[30] = { 0.9f, 0.9f, 0.99f, 0.99f, 0 };//U

int remain_vm_cnt = VM_COUNT;
int aging_for_vm_cnt = VM_COUNT;
int i = 0, j = 0;
float vector_sum = 0.0, assign_sum = 0;

float aging_demand_cont = 0.9f;
float aging_aging = 0.9f;
int idx_demand_1[30] = { 0, };


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

	if (VM_COUNT == VM_LIMIT)
	{
		printf("VM수가 최대 수와 같으면 최소 리소스로 분배\n");
		return 0;
	}

	while (remain_vm_cnt > 0)
	{
		j++;


		///////////////////////////////////////////////////////////////////////// print
		fprintf(stdout, "-----------------------------------\n");
		fprintf(stdout, "while loop[%d]\n", j);
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
		fprintf(stdout, "\n-----------------------------------");
		fprintf(stdout, "\n\n");
		//system("pause");
		/////////////////////////////////////////////////////////////////////////


		for (i = 0; i < VM_COUNT; i++) // vm 하나가 처리 되던가, 전부다 처리가 안되던가, VM 하나에 자원할당이 되면 break하고 a_i를 다시 계산
		{
			//if (want_assign_resource[i] != 0 && 
			//		(want_assign_resource[i] >= demand_of_vm[i] || remain_vm_cnt == 1))
			//	// a_i가 0인 경우는 할당이 완료된 경우이다. a_i가 0이 아니고, (a_i >= d_i일 때 또는 남은 VM이 하나일경우) 할당한다.
			if (want_assign_resource[i] != 0)
			{
				if (want_assign_resource[i] >= demand_of_vm[i]) // 마지막이어도 과도하게 배정안되게
				{
					consumed_resource_each_vm[i] = demand_of_vm[i];
				}
				else if(remain_vm_cnt ==1)// 마지막인 경우인데 남은 자원이 원하는 만큼줄 수 없을 때
				{
					consumed_resource_each_vm[i] = (TOTAL_RESOURCE - consumed_resource_all_VM);
				}
				else
					continue;

				consumed_resource_all_VM += consumed_resource_each_vm[i]; // 할당한 총 자원 증가
				want_assign_resource[i] = 0; // 자원이 할당된 VM의 a_i = 0
				remain_vm_cnt--; // 자원 할당


				assign_sum = 0;
				for (i = 0; i < VM_COUNT; i++)
					assign_sum += want_assign_resource[i];

				for (i = 0; i < VM_COUNT; i++){
					want_assign_resource[i] = (want_assign_resource[i] / assign_sum) * (TOTAL_RESOURCE - consumed_resource_all_VM);
					
					fprintf(stdout, "a_i = %f\n", want_assign_resource[i]);
				}
				fprintf(stdout, "consumed_all_vm = %d\n TOTAL_RESOURCE = %d\n", consumed_resource_all_VM, TOTAL_RESOURCE);
			}
		}

		//// aging(di) 
		aging_for_vm_cnt--; // 
		if (aging_for_vm_cnt == 0){
			for (i = 0; i < VM_COUNT; i++)
				demand_of_vm[i] = ceil(demand_of_vm[i] * aging_demand_cont);

			aging_demand_cont = aging_demand_cont * aging_demand_cont; //pow(aging_demand_cont, 2); // 예를 들어 2,3에 0.7을 곱해도 1.4, 2.1이므로 천장함수 취하면 다시 2,3이 되버린다. 그래서 에이징이 더필요
			//aging_demand_cont = aging_demand_cont * aging_aging;
			aging_for_vm_cnt = VM_COUNT;
		} 
		////
		if (aging_demand_cont < 0.01f)
			break;
	}

	printf("assigned resources : ");
	for (i = 0; i < VM_COUNT; i++)
	{
		printf("%d ", consumed_resource_each_vm[i]);
	}

	putchar('\n');



	return 0;
}