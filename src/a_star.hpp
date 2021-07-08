class Astar
{
public:
    Astar(Graph g, vertex_descriptor source_node, vertex_descriptor target_node, property_map<Graph, edge_weight_t>::type graph_weights, bool is_cartesian) : G(g), s(source_node), t(target_node), weights(graph_weights), has_coordinates(is_cartesian)
    {
        // check if graph has coordinates heuristic function equals to euclidian distance
        // otherwise calculate lower barrier
        (has_coordinates) ? euclidian() : barrier();
    };
    std::vector<int> run();
    std::vector<int> a_star_search();
    void print_astar_solution(std::vector<int> dist);
    int visited_vertices_Astar;
    double time_Astar;

private:
    property_map<Graph, edge_weight_t>::type weights;
    Graph G;
    vertex_descriptor s;
    vertex_descriptor t;
    std::vector<float> ht;
    bool has_coordinates;
    void euclidian();
    void barrier();
    // void Dijkstra_SP();
    std::vector<int> Dijkstra_Barrier(vertex_descriptor land);
};

void Astar::euclidian()
{
    // heuristic function
    std::vector<float> h(num_vertices(G));
    graph_traits<Graph>::vertex_iterator vi;

    for (vi = vertices(G).first; vi != vertices(G).second; ++vi)
    {
        // heuristic function equals to euclidian distance from vertex i to target node
        h[*vi] = (int)sqrt(pow(G[*vi].x - G[t].x, 2) + pow(G[*vi].y - G[t].y, 2));
    }
    // assign heuristic function to class member heuristic function ht
    ht = h;
}
void Astar::barrier()
{
    // heuristic function
    std::vector<float> h(num_vertices(G));

    // select a random node as landmark and run dijkstra for this node
    vertex_descriptor L1 = vertex(rand() % num_vertices(G), G);
    // the vector that holds the distance from all nodes to node L1
    std::vector<int> distL1 = Dijkstra_Barrier(L1);

    int maxElementIndex = std::max_element(distL1.begin(), distL1.end()) - distL1.begin();
    int maxElement = *std::max_element(distL1.begin(), distL1.end());

    // select a second node as landmark that has the maximum distance from L1
    vertex_descriptor L2 = vertex(maxElementIndex, G);
    // the vector that holds the distance from all nodes to node L2
    std::vector<int> distL2 = Dijkstra_Barrier(L2);

    graph_traits<Graph>::vertex_iterator vi;
    for (vi = vertices(G).first; vi != vertices(G).second; ++vi)
    {
        // best low barrier of heuristic function is the max distance from L1 or L2
        if (distL1[t] - distL1[*vi] > distL2[*vi] - distL2[t])
        {
            h[*vi] = distL1[t] - distL1[*vi];
        }
        else
        {
            h[*vi] = distL2[*vi] - distL2[t];
        }
    }

    // assign heuristic function to class member heuristic function ht
    ht = h;
}

std::vector<int> Astar::run()
{
    return a_star_search();
}

std::vector<int> Astar::a_star_search()
{
    // Prints the weighted edgelist.
    IndexMap index = get(vertex_index, G);
    boost::graph_traits<Graph>::edge_iterator e_it, e_end;
    for (std::tie(e_it, e_end) = boost::edges(G); e_it != e_end; ++e_it)
    {
        weights[*e_it] += ht[boost::target(*e_it, G)] - ht[boost::source(*e_it, G)];
    }
    int vert_examined;
    float time;

    //run dijkstra sp algorithm for the new edge weights
    Dijkstra *dijkstra = new Dijkstra(G, s, t, weights);
    std::vector<int> dist = dijkstra->Dijkstra_SP(&vert_examined, &time);

    // assign vertexes examined and time elapsed to local class variables
    visited_vertices_Astar = vert_examined;
    time_Astar = time;
    return dist;
}

void Astar::print_astar_solution(std::vector<int> dist)
{
    std::cout << "----- A* ------" << std::endl;
    std::cout << "distance to node(" << t << ") from source node(" << s << ")is = " << dist[t] + ht[s] << std::endl;
    std::cout << " time elapsed " << time_Astar;
    std::cout << std::endl;
    std::cout << " vertices examined " << visited_vertices_Astar;
    std::cout << std::endl;
}

std::vector<int> Astar::Dijkstra_Barrier(vertex_descriptor land)
{
    // Create a priority queue with min heap to store vertices that are being preprocessed
    priority_queue<vertex_weight_pair, vector<vertex_weight_pair>, greater<vertex_weight_pair>> pq;
    // map vertices to indexes
    IndexMap index = get(vertex_index, G);
    // a vector to hold vertices distance form source node with start value the maximum integer for every node
    std::vector<int> dist(num_vertices(G), INT_MAX);

    // Insert source itself in priority queue and initialize its distance as 0.
    pq.push(make_pair(0, land));
    dist[index[land]] = 0;

    while (!pq.empty())
    {
        // Get vertex with smaller distance for priority queue
        vertex_descriptor v = pq.top().second;
        pq.pop();
        graph_traits<Graph>::out_edge_iterator out_i, out_end;
        edge_descriptor e;
        // Get all in edges for node v (in or out is the same as graph is bidirectional)
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
    return dist;
}