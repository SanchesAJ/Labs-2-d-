#define _CRT_SECURE_NO_WARNINGS 1
#define INF UINT_MAX
#define MAX_NODES 5000

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ������ ������� ���������� �� ���� ����� 
int ranged_input(int low_limit, int upper_limit, char *error_msg)
{
	int temp = 0;

	scanf("%d", &temp);

	if ((temp < low_limit) || (upper_limit < temp))
	{
		puts(error_msg);
		exit(0);
	}

	return temp;
}

/*���������� ���������� �� ���� ������
int dst_cmp(unsigned dist[], unsigned overflow[], unsigned node1, unsigned node2, unsigned node1_addition)
{
	if (overflow[node1] < overflow[node2])
		return -1;
	else if (overflow[node1] > overflow[node2])
		return 1;

	if (dist[node1] + node1_addition < dist[node2])
		return -1;
	else if (dist[node1] + node1_addition > dist[node2])
		return 1;

	return 0; // �����
}
*/
int main(void) {

	int N = ranged_input(0, MAX_NODES, "bad number of vertices\n");
	int S = ranged_input(1, N, "bad vertex\n"); S--;	
	int F = ranged_input(1, N, "bad vertex\n");	F--;	
	int M = ranged_input(0, N * (N + 1) / 2, "bad number of edges\n");

	unsigned int **g = (int**)malloc(N * sizeof(unsigned int*));
	for (int i = 0; i < N; i++)
	{
		g[i] = (unsigned int*)malloc(N * sizeof(unsigned int));

		for (int j = 0; j < N; ++j)
			g[i][j] = INF;
	}


	bool used[MAX_NODES];			// ��������� �������
	unsigned dist[MAX_NODES];		// �� ��������� �������
	unsigned overflow[MAX_NODES];		// ������� ��� ���������� ����������� ������� � INT_MAX
	int parent[MAX_NODES];			// ������ ������

	for (int i = 0; i < N; i++)
	{
		used[i] = false;
		dist[i] = (i == S) ? (0) : (INF);
		overflow[i] = 0;
		parent[i] = -1;
	}

	// ���������� ������� ���������
	for (int i = 0; i < M; ++i)
	{
		int edge_from = ranged_input(1, N, "bad vertex\n") - 1;	
		int edge_to = ranged_input(1, N, "bad vertex\n") - 1;
		unsigned edge_len = ranged_input(0, INT_MAX, "bad length\n");

		g[edge_from][edge_to] = edge_len;
		g[edge_to][edge_from] = edge_len;
	}


	for (int i = 0; i < N; ++i)
	{
		// ������� ������������ �������, ���������� ��������� �� ������
		int v = -1;
		for (int j = 0; j < N; ++j)
			if (!used[j] && (v == -1 || dist[j] < dist[v]))
				v = j;

		if (dist[v] == INF)
			break;

		used[v] = true;

		//����������� ��� �����, ��������� �� ���� �������
		for (int j = 0; j < N; ++j)
		{
			if (g[v][j] == INF)
				continue;	// ��� �����

			int to = j;
			int len = g[v][to];

			//if (dist[v] + len < dist[to])
			if (dst_cmp(dist, overflow, v, to, len) == -1)
			{
				dist[to] = dist[v] + len;
				overflow[to] = overflow[v];

				while (dist[to] > INT_MAX)
				{
					++overflow[to];
					dist[to] -= INT_MAX;
				}

				parent[to] = v;
			}
		}
	}

	// ���������� �� S �� ������ ������� �����
	for (int i = 0; i < N; ++i)
	{
		if (dist[i] == INF)
			printf("oo ");
		else if (overflow[i])
			printf("INT_MAX+ ");
		else
			printf("%d ", dist[i]);
	}
	printf("\n");

	//������� ���-�� overflow �����
	unsigned overflow_count = 0;
	if (overflow[F])
	{
		for (int i = 0; i < N; ++i)
		{
			if (g[F][i] != INF && dist[i] + g[F][i] > INT_MAX)
				++overflow_count;

			if (overflow_count >= 2)
				break;	// ���� ����������
		}
	}

	// ����� ����������� ���� ��� ��������� �� ��� ���������
	if (S == F)
	{
		printf("%d\n", S + 1);
	}
	else if (parent[F] == -1)
	{
		printf("no path\n");
	}
	else if (overflow[F] && overflow_count >= 2)
	{
		printf("overflow\n");
	}
	else
	{
		for (int path_node = F; path_node != S; path_node = parent[path_node])
			printf("%d ", path_node + 1);// ������� ��������� �� 0 �.� ������ ������� �� -1
		printf("%d\n", S + 1);
	}

	for (int i = 0; i < N; ++i)	// ~������� ���������
		free(g[i]);
	free(g);

	return 0;
}