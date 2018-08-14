import random
from random import shuffle
import string
import math

class Graph(object): 

    def __init__(self, n, edges=False): 
        self.n = n
        self.graph = [[0 for x in range(n)] for y in range(n)]
        self.edges = edges if edges else []
         
    def add_edge(self, a,b): 
        self.edges.append((a, b))
        self.graph[a][b] = 1        

    def has_edge(self, a,b): 
        return self.graph[a][b]

    @staticmethod 
    def create_from_file(path): 
        f = open(path, "r")
        nodes = int(f.readline())
        edges = int(f.readline())
        names = [f.readline() for i in range(nodes)]
        edges = [f.readline() for i in range(edges)]
        edges = [tuple(int(x) for x in edge.split()) for edge in edges]
        return Graph(nodes, edges=edges)

    @staticmethod
    def create_hamiltonian_path(n, noise): 
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




    def remove_hamiltonian_path(self): 
        for i in range(self.n): 
            self.recurse_delete([-1 if j !=i else i for j in range(self.n) ], 1) 
            
    def recurse_delete(self, solution, i): 
        c_n = solution[i-1]
        for j in range(self.n):
            if(self.has_edge(c_n, j) & (j != c_n) & (j not in solution)): 
                solution[i] = j 
                if(solution[-1] == -1): 
                    if(self.recurse_delete(solution, i+1) != 1): 
                        solution[i] = -1
                else: 
                    return 1
            else: 
                 solution[c_n] = 0 
                 return 0
        
        solution[c_n] = 0
        return 0

    @staticmethod
    def create_non_hamiltonian_path(n, edges): 
        """
        g = Graph(n-3)
        for i in range(edges-3): 
            a,b = (random.randint(0, g.n-1), random.randint(0, g.n-1))
            if g.has_edge(a,b):
                edges+=1               
            g.add_edge(a,b)

        
        for i in range(g.n, n):
            g.add_edge(random.randint(0, g.n-1), i)
        g.n = n
        return g
        """
        graph = Graph(n)
        for i in range(edges):
            a,b = (random.randint(0, n-1), random.randint(0, n-1))
            if graph.has_edge(a,b):
                edges+=1
            graph.add_edge(a,b)
        graph.remove_hamiltonian_path()
        return graph




    def output_format(self): 
        output = []
        output.extend([str(self.n), str(len(self.edges))])
        output.extend([str(x) for x in range(self.n)] )
        output.extend(["{0} {1}".format(edge[0], edge[1]) for edge in self.edges])
        return "\n".join(output)
    
def save_graph(n, hamiltonian, path): 
    g = Graph.create_hamiltonian_path(n, int((n**(3/2)))) if hamiltonian else Graph.create_non_hamiltonian_path(n, int((n**(3/2))))
    f = open(path, "w")
    f.write(g.output_format())
    f.close()


def main():
    for n in range(24, 101, 2): #[5, 10, 15, 50, 200, 1000, 5000, 25000, 50000]: 
        #save_graph(n, True, "{0}_True.txt".format(n))
       # print("{0}_True.txt".format(n))
        save_graph(n, False, "{0}_False.txt".format(n))
        print("{0}_False.txt".format(n))

if __name__ == '__main__': 
    main()
