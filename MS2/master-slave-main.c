#include "graph.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <omp.h> 

#define MASTER 0

// MPI message types
#define PLEASE_DIE 0
#define GRAPH_ELEMENTS 1
#define GRAPH_SIZE 2
#define PROCESS_NODE 3 
#define GRAPH_LOADED 4 

int rank; 

#define debug_print(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "rank: %d| %s:%d:%s(): " fmt, rank, __FILE__, \
		                                __LINE__, __func__, __VA_ARGS__); } while (0)


int main(int argc, char** argv) {
	MPI_init(&argc, &argv); 
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	if(rank == MASTER) {
		master(); 
	} else {
		slave(); 
	}
	MPI_Finalize(); 
}

void kill_slaves(int slaves) {
	for (int i = 1; i < slaves; i++) {
		MPI_Send(0, 0, MPI_INT, i, PLEASE_DIE, MPI_COMM_WORLD); 
	}
}

void send_graphs_to_slaves(int slaves, Graph* g) {
    int g_n = g->n; 
	for (int i = 1; i < slaves; i++) {
		MPI_Send(g_n, 1, MPI_INT, i, GRAPH_SIZE, MPI_COMM_WORLD); 
		MPI_Send(g->matrix, g_n*g_n, MPI_INT, GRAPH_ELEMENTS, MPI_COMM_WORLD); 
	}
	
}

int get_graph_from_master(Graph* g) {
	MPI_Status status; 
	MPI_Recv(g->n, 1, MPI_INT, MASTER, MPI_ANY_SOURCE, MPI_COMM_WORLD, &status); 
	if (status.MPI_TAG == PLEASE_DIE) {
		return PLEASE_DIE; 
	}
	MPI_Recv(g->matrix, g->n * g->n, MPI_INT, MPI_ANY_SOURCE, MPI_COMM_WORLD, &status); 
	if (status.MPI_TAG == PLEASE_DIE) {
		return PLEASE_DIE; 
	}
	return GRAPH_LOADED; 
}

void master(int argc, char** argv) {
    int slaves, node ; 
	MPI_Comm_size(MPI_COMM_WORLD, &slaves); 
	Input* i = parse_input_from_file(argv[1]); 
    Graph* g = create_graph(get_node_count(i), get_edge_count(i), get_edge_source(i), get_edge_dest(i)); 
    if (g == NULL) {
        kill_slaves(slaves); 
		fprintf(stdout, "pre-process\n");
		return; 
	}
	send_graph_to_slaves(slaves, g); 
	node = 0; 

	// Give each worker initial work. Unless there are less nodes than workers. 
	int tasks = (slaves -1  > g->n) ? g->n : slaves -1; 
	for (int node = 0; node < tasks; j++) {
		MPI_Send(j, 1 MPI_INT, j+1, PROCESS_NODE, MPI_COMM_WORLD); 
	}
	tasks++; 
	// If any, kill not used slaves (Will never be needed). 
	for (tasks; tasks < slaves; tasks++) {
		 MPI_Send(0, 0, MPI_INT, i, PLEASE_DIE, MPI_COMM_WORLD); 
	}
	
	MPI_Status status; 
	int solution[g->n]; 
	// Hand out work to next returning slave
	while(node < g->n) {
		MPI_Recv(&solution, g->n, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
		if(status.MPI_TAG == FOUND_PATH) {
			output_solution(solution); 
			break; 
		}
		MPI_SEND(node, 1, MPI_INT, status.MPI_SOURCE, PROCESS_NODE, MPI_COMM_WORLD); 
		node++; 
	}

	// Kill all slaves
	kill_slaves(slaves); 
}

void slave() {
    // Get graph from master
    Graph g; 
    if( get_graph_from_master(&g) == PLEASE_DIE ) {
    	return; 
    }
	int start_node; 
	MPI_Status status; 
	while(true) {
		MPI_Recv(&start_node, 1, MPI_INT, MASTER, MPI_ANY_TAG, MPR_COMM_WORLD, &status); 
		if( status.MPI_TAG == PLEASE_DIE ) {
			return; 	
		} else if (iterative_search(start_node, &g) == 1) {
			MPI_Send(&solution, g->n, MPI_INT, MASTER, FOUND_PATH, MPI_COMM_WORLD); 
			return; 
		}

    }
    
}

