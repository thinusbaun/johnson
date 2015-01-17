#include "dijkstra.hpp"
#include <set>

void dijkstraCalculatePaths(int source, lista_sasiedztwa l, std::vector<int> &d, std::vector<int> &p)
{
int infinity = std::numeric_limits<int>::max();
    int n = l.size();
    d.clear();
    d.resize(n, infinity);
    d[source] = 0;
    p.clear();
    p.resize(n, -1);
    std::set<krawedz> vqueue;
    vqueue.insert(std::make_pair(d[source], source));

    while(!vqueue.empty())
    {
        int dist = vqueue.begin()->first;
        int u = vqueue.begin()->second;
        vqueue.erase(vqueue.begin());

        const std::vector<krawedz> &krawedzie = l[u];
        for(auto iter = krawedzie.begin();iter != krawedzie.end(); iter++)
        {
            int v = (*iter).first;
            int weight = (*iter).second;
            int dtu = weight + dist;
            if (dtu < d[v])
            {
                vqueue.erase(std::make_pair(d[v], v));
                d[v] = dtu;
                p[v] = u;
                vqueue.insert(std::make_pair(d[v], v));
            }
        }
    }
}

std::vector<int> pathTo(
        int vertex, const std::vector<int> &previous)
{
    std::vector<int> path;
    for ( ; vertex != -1; vertex = previous[vertex])
        path.insert(path.begin(), vertex+1);
    return path;
}

int calculateWeight(const std::vector<krawedz> &krawedzie, int cel)
{
    for (auto x:krawedzie)
    {
        if (x.first == cel)
        {
            return x.second;
        }
    }
    return -1;
}

