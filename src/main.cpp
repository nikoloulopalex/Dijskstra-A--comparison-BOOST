#include <iostream>
#include <stdlib.h>
#include <boost/array.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/heap/priority_queue.hpp>
#include <boost/graph/graphviz.hpp>
#include <map>
#include <limits>
#include <vector>
#include <algorithm>
#include <math.h>
#include <ctime>

using namespace std;
using namespace boost;

struct vertex_info
{
    int x;
    int y;
};

typedef adjacency_list<listS, vecS, bidirectionalS, vertex_info, property<edge_weight_t, int>> Graph;
typedef graph_traits<Graph>::vertex_descriptor vertex_descriptor;
typedef graph_traits<Graph>::edge_descriptor edge_descriptor;
typedef std::pair<int, int> Edge;
typedef boost::graph_traits<Graph> Traits;
typedef graph_traits<Graph>::vertex_iterator vertex_iter;

#include "dijkstra.hpp"
#include "a_star.hpp"
#include "grid_graph.hpp"
#include "random_graph.hpp"

// Extracts a node given it's coordinates
vertex_descriptor findVertex(Graph &g, int x, int y)
{
    std::pair<vertex_iter, vertex_iter> vp;
    for (vp = vertices(g); vp.first != vp.second; ++vp.first)
    {
        vertex_descriptor v = *vp.first;
        if (g[v].x == x && g[v].y == y)
            return v;
    }
}

int main()
{
    // Testing Grid graphs with r rows and c columns
    int r = 30, c = 1000;
    std::cout << "Testing Grid Graph with " << r << " rows and " << c << " columns " << std::endl;

    Grid_graph *grid_graph = new Grid_graph(r, c);
    Graph g_graph = grid_graph->create();
    property_map<Graph, edge_weight_t>::type g_weightmap = get(edge_weight, g_graph);

    // Choosing random nodes from first and last column
    vertex_descriptor g_src = findVertex(g_graph, (rand() % r), 0);
    vertex_descriptor g_target = findVertex(g_graph, (rand() % r), c - 1);

    // Dijkstra shortest path
    Dijkstra *g_dijkstra = new Dijkstra(g_graph, g_src, g_target, g_weightmap);
    std::vector<int> g1_dist = g_dijkstra->run();
    g_dijkstra->print_dijkstra_solution(g1_dist);

    // Astar shortest path
    Astar *g_a_star = new Astar(g_graph, g_src, g_target, g_weightmap, true);
    std::vector<int> g2_dist = g_a_star->run();
    g_a_star->print_astar_solution(g2_dist);

    // Testing Random graphs with n nodes and m edges
    int n = 10000, m = 20000;
    std::cout << "Testing Random Graph with " << n << " nodes and " << m << " edges " << std::endl;

    Random_graph *random_graph = new Random_graph(n, m);
    Graph r_graph = random_graph->create();
    property_map<Graph, edge_weight_t>::type r_weightmap = get(edge_weight, r_graph);

    vertex_descriptor r_src = vertex(rand() % n, r_graph);
    vertex_descriptor r_target = vertex(rand() % n, r_graph);

    // Dijkstra shortest path
    Dijkstra *r_dijkstra = new Dijkstra(r_graph, r_src, r_target, r_weightmap);
    std::vector<int> r1_dist = r_dijkstra->run();
    r_dijkstra->print_dijkstra_solution(r1_dist);

    // Astar shortest path
    Astar *r_a_star = new Astar(r_graph, r_src, r_target, r_weightmap, false);
    std::vector<int> r2_dist = r_a_star->run();
    r_a_star->print_astar_solution(r2_dist);

    return 0;
}