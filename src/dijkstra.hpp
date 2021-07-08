#include <queue>
typedef pair<int, vertex_descriptor> vertex_weight_pair;
typedef property_map<Graph, vertex_index_t>::type IndexMap;

class Dijkstra
{
public:
    Dijkstra(Graph g, vertex_descriptor source_node, vertex_descriptor target_node, property_map<Graph, edge_weight_t>::type graph_weights) : G(g), s(source_node), t(target_node), weights(graph_weights){};
    std::vector<int> run();
    std::vector<int> Dijkstra_SP(int *vert_examined, float *time);
    void print_dijkstra_solution(std::vector<int> dist);

private:
    property_map<Graph, edge_weight_t>::type weights;
    Graph G;
    vertex_descriptor s;
    vertex_descriptor t;
    int visited_vertices_dijkstra;
    double time_dijkstra;
};

std::vector<int> Dijkstra::run()
{
    int vert_examined;
    float time;
    return Dijkstra_SP(&vert_examined, &time);
}

std::vector<int> Dijkstra::Dijkstra_SP(int *vert_examined, float *time)
{
    // Create a priority queue with min heap to store vertices that are being preprocessed
    priority_queue<vertex_weight_pair, vector<vertex_weight_pair>, greater<vertex_weight_pair>> pq;
    // map vertices to indexes
    IndexMap index = get(vertex_index, G);
    // a vector to hold vertices distance form source node with start value the maximum integer for every node
    std::vector<int> dist(num_vertices(G), INT_MAX);
    clock_t begin = clock();
    visited_vertices_dijkstra = 0;
    // Insert source itself in priority queue and initialize its distance as 0.
    pq.push(make_pair(0, s));
    dist[index[s]] = 0;

    while (!pq.empty())
    {
        // Get vertex with smaller distance for priority queue
        vertex_descriptor v = pq.top().second;
        pq.pop();
        // If we finalize the distance from source to target node the algorithm is over
        if (v == t)
            break;
        visited_vertices_dijkstra++;
        graph_traits<Graph>::out_edge_iterator out_i, out_end;
        edge_descriptor e;
        // Get all out edges for node v
        for (tie(out_i, out_end) = out_edges(v, G); out_i != out_end; ++out_i)
        {
            e = *out_i;
            vertex_descriptor src = source(e, G), targ = target(e, G);
            // if the is a shorter path to targ through v add is to the distance vector
            if (dist[targ] > dist[v] + weights[e])
            {
                dist[targ] = dist[v] + weights[e];
                pq.push(make_pair(dist[targ], targ));
            }
        }
    }
    clock_t end = clock();
    time_dijkstra = double(end - begin) / CLOCKS_PER_SEC, *time = time_dijkstra, *vert_examined = visited_vertices_dijkstra;
    // print_dijkstra_solution(dist);
    return dist;
}

void Dijkstra::print_dijkstra_solution(std::vector<int> dist)
{
    std::cout << "----- Dijkstra ------" << std::endl;
    std::cout << "distance to node(" << t << ") from source node(" << s << ")is = " << dist[t] << std::endl;
    std::cout << " time elapsed " << time_dijkstra;
    std::cout << std::endl;
    std::cout << " vertices examined " << visited_vertices_dijkstra;
    std::cout << std::endl;
}
