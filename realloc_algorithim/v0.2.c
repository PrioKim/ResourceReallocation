#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define CPU			8		// 8 core
#define MEM			16		// 16 GB
#define VM_COUNT	4
#define TOTAL_RESOURCE	16
#define RESOURCE_LIMIT	(int)(TOTAL_RESOURCE * 0.66)
#define VM_LIMIT	(((CPU) <= (MEM)) ? (CPU) : (MEM))
#define AGING_CONST	0.99f

float reasonable_usage = 0.7f; //s
int consumed_resource_all_VM = 0; // c

//int consumed_resource_each_vm[10] = { 0 };//V
int consumed_resource_each_vm[30] = { 4, 2, 1, 1, 0 };//V
//int demand_of_vm[10] = { 2,8,8,6,0 }; // D
int demand_of_vm[30] = { 4, 11, 8, 3, 0 }; // D
float weighted_vector[30] = { 0.5f, 0.8f, 0.7f, 0.4f, 0 };//W
float want_assign_resource[30] = { 0, }; //A
float usage_each_vm[30] = { 0.9f, 0.9f, 0.99f, 0.99f, 0 };//U

int remain_vm_cnt = VM_COUNT;
int aging_for_vm_cnt = VM_COUNT;
int i = 0, j = 0;
float vector_sum = 0.0, assign_sum = 0;

float aging_demand_cont = 0.9f;
float aging_aging = 0.9f;
int idx_demand_1[30] = { 0, };

int num_compute = 0;

int main()
{
	for (i = 0; i < VM_COUNT; i++)
	{
		vector_sum += weighted_vector[i];
	}


	for (i = 0; i < VM_COUNT; i++)
	{
		want_assign_resource[i] = (weighted_vector[i] / vector_sum) * TOTAL_RESOURCE; // �ʹ� ���Ϳ� ���� �Ҵ��... ���� ���ڿ��� �����ſ���

		//demand_of_vm[i] = ceil((usage_each_vm[i] * consumed_resource_each_vm[i]) / reasonable_usage);
		if (demand_of_vm[i] >= RESOURCE_LIMIT)
			demand_of_vm[i] = RESOURCE_LIMIT;
		consumed_resource_each_vm[i] = 0;
	}

	if (VM_COUNT == VM_LIMIT)
	{
		printf("VM���� �ִ� ���� ������ �ּ� ���ҽ��� �й�\n");
		return 0;
	}


	for (int v = 0; v < VM_COUNT; v++) // vm �ϳ��� ó�� �Ǵ���, ���δ� ó���� �ȵǴ���, VM �ϳ��� �ڿ��Ҵ��� �Ǹ� break�ϰ� a_i�� �ٽ� ���
	{
		num_compute++;
		j++;
		//if (want_assign_resource[i] != 0 && 
		//		(want_assign_resource[i] >= demand_of_vm[i] || remain_vm_cnt == 1))
		//	// a_i�� 0�� ���� �Ҵ��� �Ϸ�� ����̴�. a_i�� 0�� �ƴϰ�, (a_i >= d_i�� �� �Ǵ� ���� VM�� �ϳ��ϰ��) �Ҵ��Ѵ�.
		fprintf(stdout, "-----------------------------------\n");
		fprintf(stdout, "for loop[%d]\n", j);
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
		fprintf(stdout, "\n%17s : %d", "remain vm cnt", remain_vm_cnt);
		fprintf(stdout, "\n%17s : %d", "VM Index", v);
		fprintf(stdout, "\n-----------------------------------");
		fprintf(stdout, "\n\n");



		if (want_assign_resource[v] != 0) // ���� �Ҵ��� �ȵ� VM�̸� if ����
		{
			if (want_assign_resource[v] >= demand_of_vm[v]) // �ý��ۿ��� �Ҵ��ϰ��� �ϴ� �ڿ��� ����ӽ��� �䱸�ϴ� �ڿ����� ���ų� ���� ��, �������̾ �����ϰ� �����ȵǰ�
			{
				consumed_resource_each_vm[v] = demand_of_vm[v];
			}
			else if(remain_vm_cnt ==1)// �������� ����ε� ���� �ڿ��� ���ϴ� ��ŭ�� �� ���� ��
			{
				consumed_resource_each_vm[v] = (TOTAL_RESOURCE - consumed_resource_all_VM);
			}
			else // �ڿ� �Ҵ� �Ұ���
			{
				//// aging(di) 
				aging_for_vm_cnt--; // 
				if (aging_for_vm_cnt == 0){
					for (i = 0; i < VM_COUNT; i++)
						demand_of_vm[i] = ceil(demand_of_vm[i] * aging_demand_cont);

					aging_demand_cont = aging_demand_cont * aging_demand_cont; //pow(aging_demand_cont, 2); // ���� ��� 2,3�� 0.7�� ���ص� 1.4, 2.1�̹Ƿ� õ���Լ� ���ϸ� �ٽ� 2,3�� �ǹ�����. �׷��� ����¡�� ���ʿ�
					//aging_demand_cont = aging_demand_cont * aging_aging;
					aging_for_vm_cnt = VM_COUNT;
				}
				if (v == VM_COUNT - 1)
					v = -1;

				////
				continue;
			}

			consumed_resource_all_VM += consumed_resource_each_vm[v]; // �Ҵ��� �� �ڿ� ����
			want_assign_resource[v] = 0; // �ڿ��� �Ҵ�� VM�� a_i = 0
			remain_vm_cnt--; // �ڿ� �Ҵ�
			v = -1;
			aging_for_vm_cnt = VM_COUNT;

			if (!remain_vm_cnt)
				break;

			assign_sum = 0;
			for (i = 0; i < VM_COUNT; i++)
				assign_sum += want_assign_resource[i];

			for (i = 0; i < VM_COUNT; i++){
				want_assign_resource[i] = (want_assign_resource[i] / assign_sum) * (TOTAL_RESOURCE - consumed_resource_all_VM);
					
				fprintf(stdout, "a_i = %f\n", want_assign_resource[i]);
			}
			fprintf(stdout, "consumed_all_vm = %d\n TOTAL_RESOURCE = %d\n", consumed_resource_all_VM, TOTAL_RESOURCE);
		}

		

		if (v == VM_COUNT - 1 && remain_vm_cnt != 0)
			v = -1;
	}

	

	printf("assigned resources : ");
	for (i = 0; i < VM_COUNT; i++)
	{
		printf("%d ", consumed_resource_each_vm[i]);
	}

	putchar('\n');
	putchar('\n');
	putchar('\n');

	printf("Compute Num : %d\n", num_compute);


	return 0;
}