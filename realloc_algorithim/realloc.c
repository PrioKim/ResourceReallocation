#include <stdio.h>
#include <math.h>

#define CPU			8		// 8 core
#define MEM			16		// 16 GB
#define VM_COUNT	4
#define VM_LIMIT	(((CPU) <= (MEM)) ? (CPU) : (MEM))
#define AGING_CONST	0.9f


int main()
{
	int total_resource = 16; //t
	float reasonable_usage = 0.7f; //s
	int consumed_resource_all_VM = 0; // c

	//int consumed_resource_each_vm[10] = { 0 };//V
	int consumed_resource_each_vm[10] = { 2, 2, 6, 6, 0 };//V
	//int demand_of_vm[10] = { 2,8,8,6,0 }; // D
	int demand_of_vm[10] = { 0 }; // D
	float weighted_vector[10] = { 0.8f, 0.9f, 0.99f, 0.99f, 0 };//W
	float want_assign_resource[10] = { 0, }; //A
	float usage_each_vm[10] = { 0.9f, 0.9f, 0.99f, 0.99f, 0 };//U

	int remain_vm_cnt = VM_COUNT;
	int aging_for_vm_cnt = VM_COUNT;
	int i = 0, j = 0;
	float vector_sum = 0.0, assign_sum = 0;

	float aging_demand_cont = 0.9f;
	float aging_aging = 0.9f;



	for (i = 0; i < VM_COUNT; i++)
	{
		vector_sum += weighted_vector[i];
	}

	for (i = 0; i < VM_COUNT; i++)
	{
		want_assign_resource[i] = (weighted_vector[i] / vector_sum) * total_resource; // 너무 벡터에 따라 할당됨... ㄴㄴ 총자원이 적은거였어

		demand_of_vm[i] = ceil((usage_each_vm[i] * consumed_resource_each_vm[i]) / reasonable_usage);
	}


	if (VM_COUNT == VM_LIMIT)
	{
		printf("VM수가 최대 수와 같으면 최소 리소스로 분배\n");
		return 0;
	}

	while (remain_vm_cnt > 0)
	{
		j++;
		fprintf(stdout, "-----------------------------------\n");
		fprintf(stdout, "while loop[%d]\n", j);
		fprintf(stdout, "%20s", "a_i(assign_avail) : ");

		assign_sum = 0;
		for (i = 0; i < VM_COUNT; i++)
			assign_sum += want_assign_resource[i];

		for (i = 0; i < VM_COUNT; i++)
		{
			want_assign_resource[i] =
				(want_assign_resource[i] / assign_sum) * (total_resource - consumed_resource_all_VM);
			fprintf(stdout, "%f ", want_assign_resource[i]);
		}
		
		fprintf(stdout, "\n");
		fprintf(stdout, "%20s", "d_i(demand) : ");
		for (i = 0; i < VM_COUNT; i++)
			fprintf(stdout, "%d ", demand_of_vm[i]);

		for (i = 0; i < VM_COUNT; i++)
		{
			if (want_assign_resource[i] != 0 && (want_assign_resource[i] >= demand_of_vm[i] || remain_vm_cnt == 1))
			{
				if (want_assign_resource[i] >= demand_of_vm[i]) // 마지막이어도 과도하게 배정안되게
				{
					consumed_resource_each_vm[i] = demand_of_vm[i];
				}
				else
				{
					consumed_resource_each_vm[i] = (total_resource - consumed_resource_all_VM);
				}

				consumed_resource_all_VM += consumed_resource_each_vm[i];
				want_assign_resource[i] = 0;
				remain_vm_cnt--;
				break;
			}
		}
		fprintf(stdout, "\n%20s : %f", "aging constant" ,aging_demand_cont);
		fprintf(stdout, "\n-----------------------------------");
		fprintf(stdout, "\n\n");

		//// aging(di) 
		aging_for_vm_cnt--; // 한번씩은 원래의 요구사항으로 할당가능한지 확인해야함(VM 마다 다 같은 잣대가 필요함)
		if (aging_for_vm_cnt == 0){
			for (i = 0; i < VM_COUNT; i++)
				demand_of_vm[i] = ceil(demand_of_vm[i] * aging_demand_cont);

			aging_demand_cont = aging_demand_cont * aging_demand_cont; //pow(aging_demand_cont, 2); // 예를 들어 2,3에 0.7을 곱해도 1.4, 2.1이므로 천장함수 취하면 다시 2,3이 되버린다. 그래서 에이징이 더필요
			//aging_demand_cont = aging_demand_cont * aging_aging;
			aging_for_vm_cnt = VM_COUNT;
		}
		////
	}

	printf("assigned resources : ");
	for (i = 0; i < VM_COUNT; i++)
	{
		printf("%d ", consumed_resource_each_vm[i]);
	}

	putchar('\n');



	return 0;
}
