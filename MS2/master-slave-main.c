#include "graph.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <mpi.h>
#include <omp.h> 
#include "input_data.h"
#include <time.h>


#define MASTER 0

// MPI message types
#define PLEASE_DIE 7
#define GRAPH_ELEMENTS 1
#define GRAPH_SIZE 2
#define PROCESS_NODE 3 
#define GRAPH_LOADED 4 
#define FOUND_PATH 5
#define NO_PATH 6


int rank = -1;
FILE* p = (FILE*) 0; //fopen("/dev/null", "w"); 

#define DEBUG 0
#define debug_print(x) do { if (DEBUG) dbg_printf x; } while (0)
void dbg_printf(const char *fmt, ...)
{
		va_list args;
		va_start(args, fmt);
		fprintf(p, "rank: %d", rank); 
		vfprintf(p, fmt, args);
		fprintf(p, "|\n");  
		va_end(args);
}


void master(int argc, char** argv); 
void slave(); 


int main(int argc, char** argv) {
		MPI_Init(&argc, &argv);
		rank = 10; 

		MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
		char file[80];
		sprintf(file, "r%d.txt", rank); 
		p = fopen(file, "w");
		if(rank == MASTER) {
				master(argc, argv); 
		} else {
				slave(); 
		}
		MPI_Finalize(); 
}

void kill_slaves(int slaves) {
		/**
		 * MPI protocol to kill all slave worker nodes.
		 */
		MPI_Request r; 
		int i; 
		for (i = 1; i < slaves; i++) {
				MPI_Isend(0, 0, MPI_INT, i, PLEASE_DIE, MPI_COMM_WORLD, &r); 
		}
}

void send_graphs_to_slaves(int slaves, Graph* g) {
		/*
		 * MPI protocol to send graph structure to slave. 
		 */
		int g_n = g->n; 
		int i; 
		// 0 is master. slaves are then 1 indexed.  
		#pragma omp parallel private(i)
		{
				for (i = 1; i <= slaves; i++) {
						MPI_Send(&g_n, 1, MPI_INT, i, GRAPH_SIZE, MPI_COMM_WORLD); 
						MPI_Send(g->matrix, g_n*g_n, MPI_INT, i, GRAPH_ELEMENTS, MPI_COMM_WORLD); 
				}
		}

}

int get_graph_from_master(Graph* g) {
		/**
		/ * MPI protocol to receive graph struct from master node. 
		 */ 
		MPI_Status status; 
		MPI_Recv(&g->n, 1, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
		if (status.MPI_TAG == PLEASE_DIE) {
				return PLEASE_DIE; 
		}
		g->matrix = calloc(sizeof(int), g->n * g->n); 
		MPI_Recv(g->matrix, g->n * g->n, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
		if (status.MPI_TAG == PLEASE_DIE) {
				return PLEASE_DIE; 
		}
		return GRAPH_LOADED; 
}


void master(int argc, char** argv) {
		/*
		 * Master node to handle the slave node. 
		 */
		time_t start = clock(); 
		int size = 2; 
		int node = 0 ; 
		MPI_Comm_size(MPI_COMM_WORLD, &size); 
		int slaves = size -1; 
		Input* i = parse_input_from_file(argv[1]); 
		Graph* g = create_graph(get_node_count(i), get_edge_count(i), get_edge_source(i), get_edge_dest(i));
		printf("master preprocessing. %f.\n", (double) ((clock() - start)/ CLOCKS_PER_SEC)); 
		if (g == NULL) {
				kill_slaves(slaves); 
				return; 
		}
		time_t worker_start = clock(); 
		send_graphs_to_slaves(slaves, g); 
		node = 0; 
		// Give each worker initial work. Unless there are less nodes than workers. 
		int tasks = (slaves > g->n) ? g->n : slaves; 
		#pragma omp parallel private(node)
		{
				for (node = 0; node < tasks; node++) {
						MPI_Send(&node, 1, MPI_INT, node + 1, PROCESS_NODE, MPI_COMM_WORLD); 
				}
		}
		tasks++; 
		// If any, kill not used slaves (Will never be needed). 
		for (tasks = tasks; tasks < slaves; tasks++) {
				MPI_Send(0, 0, MPI_INT, tasks, PLEASE_DIE, MPI_COMM_WORLD); 
		}
		printf("worker preprocessing. %f.\n", (double) ((clock() - worker_start)/ CLOCKS_PER_SEC)); 
		MPI_Status status; 
		int solution[g->n]; 
		// Hand out work to next returning slave
		while(node < g->n) {
				MPI_Recv(&solution, g->n, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
				if(status.MPI_TAG == FOUND_PATH) {
						printf("Master has found path.\n"); 
						print_solution(solution, g->n); 
						break; 
				} else {
						printf("not a solution. sad \n"); 
				}

				MPI_Send(&node, 1, MPI_INT, status.MPI_SOURCE, PROCESS_NODE, MPI_COMM_WORLD); 
				printf("Master node sent: %d\n", node);
				node++; 
		}

		printf("Complete time: %f.\n", (double) ((clock() - start)/ CLOCKS_PER_SEC)); 
		// Kill all slaves
		kill_slaves(slaves); 
}

void print_graph(Graph* g) {
		/*
		 * Prints the graph with space separated columns and newline separated rows to stdout. 
		 */ 
		int i, j; 
		for(i = 0; i < g->n; i++) {
				for(j = 0; j< g->n; j++) {
						printf("%d ", g->matrix[(g->n * j) + i]);
				}
				printf("\n"); 
		}
}


void print_solution(int* x, int n) {
		/*
		 * Print a newline separated list to stderr of length n.  
		 */ 
		int i;  
		for (i =0; i < n; i++) {
				fprintf(stderr, "%d\n", x[i]); 
		}
}

void check_master() {
		MPI_Status status; 
		MPI_Irecv(NULL, 0, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
		if (status.MPI_TAG == PLEASE_DIE ) {
				printf("Master is done before rank %d.\n", rank);
				MPI_Finalize(); 
				exit(0); 
		}
}

void slave() {
	// Get graph from master
	Graph* g = (Graph*) malloc(sizeof(Graph));

	if( get_graph_from_master(g) == PLEASE_DIE ) {
			return; 
	}

	int start_node; 
	MPI_Status status;
	while(1) {
		Current_solution* sol = create_solution(g->n); 
		MPI_Recv(&start_node, 1, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
		printf("got node: %d\n", start_node); 
		clock_t start = clock();
		if( status.MPI_TAG == PLEASE_DIE ) {
				return; 	
		} else {
			if (iterative_search(start_node, g, sol, start_node) == FOUND_PATH) {
				printf("rank: %d found solution.\n", rank); 
				check_master(); 
				MPI_Send(sol->solution_order, g->n, MPI_INT, MASTER, FOUND_PATH, MPI_COMM_WORLD); 
				printf("Loop %d: %f.\n", rank, (double) ((clock() - start)/ CLOCKS_PER_SEC)); 
				return; 
			} else {
				check_master(); 
				MPI_Send(NULL, 0, MPI_INT, MASTER, NO_PATH, MPI_COMM_WORLD); 	
				printf("Loop %d: %f.\n", rank, (double) ((clock() - start)/ CLOCKS_PER_SEC)); 
			}
		}
		alloc_solution(sol); 
	}
}

