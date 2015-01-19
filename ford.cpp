#include "ford.hpp"
bool searchForNegativeCycles(int source, lista_sasiedztwa l)
{
int infinity = std::numeric_limits<int>::max();
  int n = l.size();
  std::vector<int> dist;
  dist.resize(n,infinity);
  dist[source] = 0;
  for (int i = 0; i < n; i++)
  {
    std::vector<krawedz>::iterator it;
    for(int u = 0; u < n; u++)
    {
      if (dist[u] != infinity)
      {
        for(it = l[u].begin(); it != l[u].end(); it++)
        {
          if(dist[(*it).first] > dist[u] + (*it).second)
          {
            dist[(*it).first] = dist[u] + (*it).second;
          }
        }
      }
    }
  }

  for (int i = 0; i < n; i++)
  {
    std::vector<krawedz>::iterator it;
    for(int u = 0; u < n; u++)
    {
      if (dist[u] != infinity)
      {
        for(it = l[u].begin(); it != l[u].end(); it++)
        {
          if(dist[(*it).first] != infinity && dist[(*it).first] > dist[u] + (*it).second)
          {
            return true;
          }
        }
      }
    }
  }
    return false;
}

std::vector<int>  bellmanFordAlgorithm(int source, lista_sasiedztwa l)
{
int infinity = std::numeric_limits<int>::max();
  int n = l.size();
  std::vector<int> dist;
  dist.resize(n,infinity);
  dist[source] = 0;
  for(int i = 0; i <n; i++)
  {
    for (int u = 0; u < n; u++)
    {
      std::vector<krawedz>::iterator it;
      if (dist[u] != infinity)
      {
        for(it = l[u].begin(); it != l[u].end(); it++)
        {
          if(dist[(*it).first] > dist[u] + (*it).second)
          {
            dist[(*it).first] = dist[u] + (*it).second;
            if (i == n-1)
            {
              return std::vector<int>();
            }
          }
        }
      }
    }
  }
  return dist;
}
