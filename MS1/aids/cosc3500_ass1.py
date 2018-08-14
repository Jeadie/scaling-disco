import random
from random import shuffle
import string
import math

class Graph(object): 
    """
    Graph data structure. 
    """
    def __init__(self, n, edges=False): 
        self.n = n
        self.graph = [[0 for x in range(n)] for y in range(n)]
        self.edges = edges if edges else []
         

    def add_edge(self, a,b): 
        """
        Add edge from node a to node b. 
        """
        self.edges.append((a, b))
        self.graph[a][b] = 1        

    def has_edge(self, a,b): 
        """
        Return 1 if there exists an edge from a to b, 0 otherwise. 
        """
        return self.graph[a][b]

    def remove_hamiltonian_path(self):
        """
        Removes all the hamiltonian paths from the graph. 
        Returns: None
        """ 
        for i in range(self.n): 
            self._recurse_delete([-1 if j !=i else i for j in range(self.n) ], 1) 
            
    def _recurse_delete(self, solution, i): 
        """
        Recursive DFS for hamiltonian paths. Removes them if found. 
        """
        c_n = solution[i-1]
        for j in range(self.n):
            if(self.has_edge(c_n, j) & (j != c_n) & (j not in solution)): 
                solution[i] = j 
                if(solution[-1] == -1): 
                    if(self._recurse_delete(solution, i+1) != 1): 
                        solution[i] = -1
                else: 
                    return 1
            else: 
                 solution[c_n] = 0 
                 return 0
        
        solution[c_n] = 0
        return 0

    def output_format(self): 
        """
        Constructs the output format required as input to the HPC program. 
        returns: Formatted string that can is required as input to the HPC. 
        """
        output = []
        output.extend([str(self.n), str(len(self.edges))])
        output.extend(["{},".format(x) for x in range(self.n)] )
        output.extend(["{0} {1}".format(edge[0], edge[1]) for edge in self.edges])
        return "\n".join(output)
    
    @staticmethod
    def save_graph(n, hamiltonian, path): 
        """
        Saves a graph to the desired path. 
        n: number of nodes in the graph. 
        hamiltonian: will construct a graph with a hamiltonian path if True, else no path. 
        path: location to save the graph formatted to disk.         
        """
        g = Graph.create_hamiltonian_path(n, int((n**(3/2)))) if hamiltonian else Graph.create_non_hamiltonian_path(n, int((n**(3/2))))
        f = open(path, "w")
        f.write(g.output_format())
        f.close()


    @staticmethod 
    def create_from_file(path): 
        """
        Returns a graph structure from a stored file. 
        path: path to the graph file
        returns: graph object
        """
        f = open(path, "r")
        nodes = int(f.readline())
        edges = int(f.readline())
        names = [f.readline() for i in range(nodes)]
        edges = [f.readline() for i in range(edges)]
        edges = [tuple(int(x) for x in edge.split()) for edge in edges]
        return Graph(nodes, edges=edges)


    @staticmethod
    def create_hamiltonian_path(n, noise): 
        """
        Creates a random graph that has a hamiltonian path. 
        n: Number of nodes for the graph
        noise: Number of additional edges to be added to the graph. 
        Returns: graph data structure. 
        """
        path = list(range(n))
        shuffle(path)
        graph = Graph(n)
        for i in range(1,n):
            graph.add_edge(path[i-1], path[i])

        for i in range(noise):
            a,b = (random.randint(0, n-1), random.randint(0, n-1))
            if graph.has_edge(a,b):
                noise+=1
            graph.add_edge(a,b)

        return graph

    @staticmethod
    def create_non_hamiltonian_path(n, edges): 
        """
        Creates a random graph that does not have a hamiltonian path. 
        n: Number of nodes for the graph
        edges: The number of edges to be added to the graph if there is no hamiltonian path
          formed. In general will be less.
        Returns: graph data structure. 
        """
        graph = Graph(n)
        for i in range(edges):
            a,b = (random.randint(0, n-1), random.randint(0, n-1))
            if graph.has_edge(a,b):
                edges+=1
            graph.add_edge(a,b)
        graph.remove_hamiltonian_path()
        return graph


def main():
    sizes = []
    sizes.extend([n for n in range(40)])
    sizes.extend([n for n in range(40, 100, 2)])
    sizes.extend([n for n in range(100, 1000, 10)])
   # sizes.extend([n for n in range(1000, 5000, 500)])
    for n in sizes:
        Graph.save_graph(n, True, "{0}_True.txt".format(n))
        print("{0}_True.txt".format(n))
        Graph.save_graph(n, False, "{0}_False.txt".format(n))
        print("{0}_False.txt".format(n))

if __name__ == '__main__': 
    main()
