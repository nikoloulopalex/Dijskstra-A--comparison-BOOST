class Random_graph
{
public:
    Random_graph(int nodes, int edges) : n(nodes), m(edges){};
    Graph create();

private:
    int n;
    int m;
};

Graph Random_graph::create()
{
    Graph g;
    // actual max weight is max_weight+1
    int max_weight = 99;
    // add n random nodes to the graph
    for (int i = 0; i < n; i++)
    {
        vertex_descriptor v = boost::add_vertex(g);
    }
    // add m random edges to the graph
    for (int j = 0; j < m; j++)
    {
        vertex_descriptor v = vertex(rand() % n, g);
        vertex_descriptor u = vertex(rand() % n, g);
        add_edge(u, v, (rand() % max_weight) + 1, g);
    }

    return g;
}
