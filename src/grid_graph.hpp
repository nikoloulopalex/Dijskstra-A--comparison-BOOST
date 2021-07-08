class Grid_graph
{
public:
    Grid_graph(int rows, int columns) : r(rows), c(columns){};
    Graph create();

private:
    int r;
    int c;
};

Graph Grid_graph::create()
{

    int num_nodes = 9;
    Graph g;
    int current_row = 0;
    // actual max weight is max_weight+1
    int max_weight = 99;
    while (current_row < r)
    {
        for (int i = 0; i < c; i++)
        {
            vertex_descriptor v = boost::add_vertex(g);
            g[v].x = current_row, g[v].y = i;
            // connects every node with the previous node from same row
            if (i != 0)
            {
                vertex_descriptor pred = vertex(current_row * c + i - 1, g);
                add_edge(pred, v, (rand() % max_weight) + 1, g);
            }
            // connects first node of every row with firt node of the previous row
            if (i == 0 && current_row > 0)
            {
                vertex_descriptor pred_row = vertex((current_row - 1) * c, g);
                add_edge(pred_row, v, (rand() % max_weight) + 1, g);
            }
            // connects last node of every row with the last node of the previous row
            if (i == c - 1 && current_row > 0)
            {
                vertex_descriptor pred_row = vertex((current_row * c) - 1, g);
                add_edge(pred_row, v, (rand() % max_weight) + 1, g);
            }
        }
        current_row++;
    }
    return g;
}
