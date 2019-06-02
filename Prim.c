#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NO_PARENT   -1
#define MST_ERROR -2
#include <stddef.h>


typedef struct {
	unsigned short src;
	unsigned short dst;
} edge;

typedef enum {
	black,
	white
} colour;

typedef struct {
	unsigned int length;
	colour colour;

} matrix;





typedef struct {
	matrix *adjacency_matrix;

	short *parent;

	unsigned int *distance;

	edge *mst;

	unsigned int vertices;
	unsigned int edges;

	size_t mst_size;
} graph;
 int closestvertex(const graph *graph) {
	short v = 0;

	unsigned int min = UINT_MAX;

	for (unsigned int t = 0; t < graph->vertices; ++t) {

		if ((graph->adjacency_matrix[offset(t, t)].colour != black) && (graph->distance[t] <= min)) {
			min = graph->distance[t];
			v = (short)t;
		}

	}

	return v;
}

 void findparents(const unsigned int v, graph *graph) {

	for (unsigned int u = 0; u < graph->vertices; ++u) {

		if ((graph->adjacency_matrix[offset(u, u)].colour != black)
			&& (graph->adjacency_matrix[offset(v, u)].length != UINT_MAX)
			&& (graph->adjacency_matrix[offset(v, u)].length < graph->distance[u])) {

			graph->distance[u] = graph->adjacency_matrix[offset(v, u)].length;
			graph->parent[u] = (short)v;

		}

	}

}

int findmst(graph *graph) {

	if (graph->vertices && graph->edges) {
		for (unsigned int i = 0; i < graph->vertices; ++i) {

			unsigned int v = (unsigned int)closestvertex(graph);

			graph->adjacency_matrix[offset(v, v)].colour = black;

			findparents(v, graph);

			if (graph->parent[v] != NO_PARENT) {
				graph->mst[graph->mst_size].src = (unsigned short)graph->parent[v];
				graph->mst[graph->mst_size].dst = (unsigned short)v;
				graph->mst_size++;
			}

		}
	}

	if (graph->mst_size != graph->vertices - 1 || !graph->vertices)
		return MST_ERROR;

	return 0;
}



graph *initgraph(unsigned int edges, unsigned int vertices) {

	graph *newgraph = (graph *)malloc(sizeof(graph));
	if (newgraph == NULL)
		return NULL;

	newgraph->adjacency_matrix = (matrix *)malloc(vertices * (1 + vertices) / 2 * sizeof(matrix));
	if (newgraph->adjacency_matrix == NULL) {
		free(newgraph);
		return NULL;
	}

	newgraph->parent = (short *)malloc(vertices * sizeof(short));
	if (newgraph->parent == NULL) {
		free(newgraph->adjacency_matrix);
		free(newgraph);
		return NULL;
	}

	newgraph->distance = (unsigned int *)malloc(vertices * sizeof(unsigned int));
	if (newgraph->distance == NULL) {
		free(newgraph->adjacency_matrix);
		free(newgraph->parent);
		free(newgraph);
		return NULL;
	}

	newgraph->mst = (edge *)malloc((vertices > 0 ? vertices - 1 : 0) * sizeof(edge));
	if (newgraph->mst == NULL) {
		free(newgraph->distance);
		free(newgraph->parent);
		free(newgraph->adjacency_matrix);
		free(newgraph);
		return NULL;
	}

	for (unsigned int i = 0; i < vertices * (1 + vertices) / 2; i++) {
		newgraph->adjacency_matrix[i].length = UINT_MAX;
		newgraph->adjacency_matrix[i].colour = white;
	}

	for (unsigned int j = 0; j < vertices; ++j) {
		newgraph->parent[j] = NO_PARENT;
		newgraph->distance[j] = UINT_MAX;
	}

	newgraph->edges = edges;
	newgraph->vertices = vertices;
	newgraph->mst_size = 0;

	return newgraph;
}

void destroygraph(graph *graph) {
	if (graph == NULL)
		return;

	free(graph->adjacency_matrix);
	free(graph->distance);
	free(graph->mst);
	free(graph->parent);

	free(graph);
}

unsigned int offset(unsigned int v, unsigned int u) {
	if (v != u) {
		return (1 + (u > v ? u : v)) * (u > v ? u : v) / 2 + (u > v ? v : u);
	}
	else return (1 + v) * v / 2 + v;
}






int main() {

	int edges = 0;
	int vertices = 0;

	if (scanf("%d", &vertices) == EOF) {
		printf("bad number of lines");
		return 0;
	}

	if (vertices < 0 || vertices > 5000) {
		printf("bad number of vertices");
		return 0;
	}

	if (scanf("%d", &edges) == EOF) {
		printf("bad number of lines");
		return 0;
	}

	if (edges < 0 || edges > vertices * (vertices + 1) / 2) {
		printf("bad number of edges");
		return 0;
	}

	graph *graph = initgraph((unsigned int)edges, (unsigned int)vertices);
	

	for (int i = 0; i < edges; ++i) {

		int src = 0;
		int dst = 0;
		int length = 0;

		if (scanf("%d %d %d", &src, &dst, &length) == EOF) {
			printf("bad number of lines");
			destroygraph(graph);
			return 0;
		}

		if (src < 1 || src > vertices || dst < 1 || dst > vertices) {
			printf("bad vertex");
			destroygraph(graph);
			return 0;
		}

		if (length < 0 || length > INT_MAX) {
			printf("bad length");
			destroygraph(graph);
			return 0;
		}

		src--;
		dst--;

		graph->adjacency_matrix[offset((unsigned int)src, (unsigned int)dst)].length = (unsigned int)length;
	}

	if (findmst(graph) != MST_ERROR) {

		for (size_t k = 0; k < graph->mst_size; k++) {
			printf("%d %d\n", graph->mst[k].src + 1, graph->mst[k].dst + 1);
		}

	}
	else printf("no spanning tree");

	destroygraph(graph);

	return 0;
}
