import matplotlib.pyplot as plt
import networkx as nx
from cosc3500_ass1 import Graph


def plot_graph(graph, solution=None):
    """
    Constructs and shows a plot of the given graph and a solution, if supplied. 
    Return: None
    """ 
    G=nx.DiGraph()
    G.add_nodes_from(list(range(graph.n)))
    G.add_edges_from([ x for x in graph.edges], color = 'black', width=1)
    width = ["black"] * graph.n

    # If hamiltonian path on top, add solution in red with green start node. 
    if solution:
        G.add_edges_from(solution, color='red', width=1)
        width[solution[0][0]] = 'green'
   
    edges = G.edges()
    colors = [G[u][v]['color'] for u,v in edges]

    plt.plot()
    nx.draw_networkx(G,pos=nx.spring_layout(G),  edges=edges, edge_color=colors, node_color = width)
    plt.show()

def plot_path(graph_path, solution_path): 
    """
    Plots a graph and solution from files stored on disk. 
    graph_path, solution_path: path to the graph and solution 
    return: None
    """
    g=Graph.create_from_file(graph_path)
    if solution_path: 
        f=open(solution_path, "r")
        nodes = [int(f.readline()) for i in range(g.n)]
        path = [(nodes[i], nodes[i+1]) for i in range(g.n-1)]
        plot_graph(g, solution=path)
    plot_graph(g)
        
def main(): 
    plot_path("5_false.txt", None)

if __name__ == '__main__': 
    main()
