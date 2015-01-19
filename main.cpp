#include "util.hpp"
#include "ford.hpp"
#include "dijkstra.hpp"
#include <numeric>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

int infinity = std::numeric_limits<int>::max();
void loadFromFile(lista_sasiedztwa &lista)
{
    std::ifstream wejscie;
    wejscie.open("we1.txt");
    std::stringstream ss;
    std::string tmp;
    int maxWierzcholek = 0;
    int tempW;
    while(getline(wejscie, tmp))
    {
        ss.clear();
        ss.str(tmp);

        ss >> tempW;
        maxWierzcholek = std::max(maxWierzcholek, tempW);

        ss >> tempW;
        maxWierzcholek = std::max(maxWierzcholek, tempW);
    }
    for (int i = 0; i < maxWierzcholek; i++)
    {
        std::vector<krawedz> listaW;
        lista.push_back(listaW);
    }
    std::cout << "maxWierzcholek: " << maxWierzcholek << std::endl;

    wejscie.clear();
    wejscie.seekg(0, std::ios::beg);
    while(getline(wejscie,tmp))
    {
        ss.clear();
        ss.str(tmp);
        int poczatek;
        ss >> poczatek;
        poczatek--;
        krawedz k;
        ss >> k.first;
        k.first--;
        ss >> k.second;
        lista.at(poczatek).push_back(k);
    }
    wejscie.close();
    return;
}


std::vector<int> calculatePotential(lista_sasiedztwa &l)
{
  std::vector<krawedz> listaW;
  for(size_t i = 0; i < l.size(); i++)
  {
    krawedz k(i,0);
    listaW.push_back(k);
  }

  l.push_back(listaW);
  std::vector<int> odleglosci;
  odleglosci = bellmanFordAlgorithm(l.size()-1, l);
  return odleglosci;
}

void calculateNewWeights(lista_sasiedztwa &l, std::vector<int> d)
{
  l.erase(l.end()); //FIXME: Czy aby napewno to tutaj powinno być?
  for(auto x = l.begin(); x != l.end(); x++)
  {
    for(auto it = (*x).begin(); it != (*x).end(); it++)
    {
      krawedz k = (*it);
      int u = std::distance(l.begin(), x);
      int waga = k.second + d[u] - d[k.first];
      (*it).second = waga;
    }
  }
}

std::vector<std::vector<int>> createMatrix(lista_sasiedztwa &l)
{
  std::vector<std::vector<int>> matrix;
  matrix.resize(l.size());
  for(auto &x: matrix)
  {
    x.resize(l.size(), 0);
  }
  for (size_t i = 0; i < l.size(); i++)
  {
    std::vector<int> d;
    std::vector<int> p;
    dijkstraCalculatePaths(i, l, d, p);
    for(size_t j = 0; j < l.size(); j++)
    {
      //std::vector<int> wagi = pathTo(j,p);
      std::vector<int> tmp = pathTo(j, p);
      std::vector<int> wagi;
      for(unsigned long i = 0; i< tmp.size()-1; i++)
      {
        wagi.push_back(calculateWeight(l[tmp[i] - 1], tmp[i + 1] - 1));
      }

      int waga = std::accumulate(wagi.begin(), wagi.end(), 0);
      if (waga == 0 && j != i)
      {
        waga = -1; //Oznaczamy brak możliwości dojścia do wierzchołka danego.
      }
      matrix[i][j] = waga;
    }
  }
  return matrix;
}


std::vector<std::vector<int>> createMatrix(lista_sasiedztwa &l, std::vector<int> &potentials)
{
  std::vector<std::vector<int>> matrix;
  matrix.resize(l.size());
  int waga = 0;
  for(auto &x: matrix)
  {
    x.resize(l.size(), 0);
  }
  for (size_t i = 0; i < l.size(); i++)
  {
    std::vector<int> d;
    std::vector<int> p;
    dijkstraCalculatePaths(i, l, d, p);
    for(size_t j = 0; j < l.size(); j++)
    {
      //std::vector<int> wagi = pathTo(j,p);
      if( p[j] != -1)
      {
      std::vector<int> tmp = pathTo(j, p);
      std::vector<int> wagi;
      for(unsigned long i = 0; i< tmp.size()-1; i++)
      {
        wagi.push_back(calculateWeight(l[tmp[i] - 1], tmp[i + 1] - 1));
      }

      waga = std::accumulate(wagi.begin(), wagi.end(), 0);
      waga = waga + potentials[j] - potentials[i];
      if (waga == 0 && j != i)
      {
        waga = -1; //Oznaczamy brak możliwości dojścia do wierzchołka danego.
      }
      } else
      {
        if( i == j)
        {
          waga = 0;
        } else
        {
          waga = infinity;
        }
      }
      matrix[i][j] = waga;
    }
  }
  return matrix;
}

bool canUseDijkstra(lista_sasiedztwa &l)
{
  for(auto x: l)
  {
    for(auto y : x)
    {
      if (y.second < 0)
      {
        return false;
      }
    }
  }
  return true;
}

int main(void)
{
  lista_sasiedztwa l;
  std::vector<std::vector<int>> wagi;
  loadFromFile(l);
  bool mozna = canUseDijkstra(l);
  if (mozna)
  {
    wagi = createMatrix(l);
    for(auto a:wagi)
    {
      for (auto w : a)
      {
        std::cout << w << " ";
      }
      std::cout << std::endl;
    }
  } else
  {
    for(size_t i = 0; i < l.size(); i++)
    {
      if (searchForNegativeCycles(i, l))
      {
        std::cout <<i+1;
        std::cout << "Znaleziono ujemy cykl! Ja tak nie robię!" << std::endl;
        return 1;
      }
    }
    std::vector<int> odl = calculatePotential(l);
    calculateNewWeights(l,odl);
    //std::cout <<"haha";
    wagi = createMatrix(l, odl);
    std::setw(3);
    std::setfill(' ');
    for(auto a:wagi)
    {
      for (auto w : a)
      {
        if (w == infinity)
        {
          std::cout  <<std::setw(3) << std::setfill(' ') << "X";
        } else
        {
          std::cout  <<std::setw(3) << std::setfill(' ')<< w ;
        }
      }
      std::cout << std::endl;
    }
  }
  return 0;
}
