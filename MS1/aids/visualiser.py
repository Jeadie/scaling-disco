import networkx as nx
from cosc3500_ass1 import Graph
import matplotlib.pyplot as plt


def plot_graph(graph, solution): 
    G=nx.DiGraph()
    G.add_nodes_from(list(range(graph.n)))
    G.add_edges_from([ x for x in graph.edges if x not in solution], color = 'black', width=1)
    G.add_edges_from(solution, color='red', width=1)
    edges = G.edges()

    colors = [G[u][v]['color'] for u,v in edges]
    width = ["red"] * graph.n
    width[solution[0][0]] = 'green'
    plt.plot()
    nx.draw_networkx(G,pos=nx.spring_layout(G),  edges=edges, edge_color=colors, node_color = width)
    plt.show()

def plot_path(graph_path, solution_path): 
    g=Graph.create_from_file(graph_path)
    f=open(solution_path, "r")
    nodes = [int(f.readline()) for i in range(g.n)]
    path= [(nodes[i], nodes[i+1]) for i in range(g.n-1)]
    plot_graph(g, path)

        
def main(): 
    print("jjj")
    plot_path("tests/8_True.txt", "tests/8.txt")
    print("hhh")

if __name__ == '__main__': 
    main()