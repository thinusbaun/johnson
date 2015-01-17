#ifndef DIJKSTRA
#define DIJKSTRA

#include "util.hpp"

void dijkstraCalculatePaths(int source, lista_sasiedztwa l, std::vector<int> &d, std::vector<int> &p);

std::vector<int> pathTo(int vertex, const std::vector<int> &previous);
int calculateWeight(const std::vector<krawedz> &krawedzie, int cel);
#endif
