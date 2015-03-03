#include "epidemics.h"

void Epidemics::Evolution::add(long long sCount, long long iCount) {
  statusCount[SUSCEPTIBLE].push_back(sCount);
  statusCount[INFECTED].push_back(iCount);
}

void Epidemics::Evolution::add(long long sCount, long long iCount, long long rCount) {
  statusCount[SUSCEPTIBLE].push_back(sCount);
  statusCount[INFECTED].push_back(iCount);
  statusCount[RECOVERED].push_back(rCount);
}

void Epidemics::initializeInfected(vector<int> & initiallyInfected,
    list<int> & infectedVertices, vector<Epidemics::Status> & status) {  
  // Initialize status and list of infected vertices.
  for (int i = 0; i < initiallyInfected.size(); i++) {
    infectedVertices.push_back(initiallyInfected[i]);
    status[initiallyInfected[i]] = INFECTED;
  }
}

Epidemics::Evolution * Epidemics::runSIR(Graph & graph, double delta, double nu,
    vector<int> & initiallyInfected, int maxIterations) {
  list<int> infectedVertices;
  vector<Status> status(graph.getVertexCount(), SUSCEPTIBLE);
  Evolution * evolution = new Evolution();
  
  initializeInfected(initiallyInfected, infectedVertices, status);

  long long iCount = (long long) initiallyInfected.size();
  long long sCount = graph.getVertexCount() - iCount;
  long long rCount = 0;

  for (int t = 0; infectedVertices.size() > 0 &&  t < maxIterations; t++) {
    // Iterate over infected vertices.
    for (auto it = infectedVertices.begin(); it != infectedVertices.end();) {
      int curr = *it;
      auto neighbours = graph.getNeighbours(curr);
      // Infection stage.
      for (auto it2 = neighbours.begin(); it2 != neighbours.end(); it2++) {
        int neighbour = *it2;
        if (status[neighbour] == SUSCEPTIBLE && Stat::uniform() < delta) {
          status[neighbour] = INFECTED;
          infectedVertices.push_front(neighbour);
          sCount--;
          iCount++;
        }
      }
      // Recovery stage.
      if (Stat::uniform() < nu) {
        status[curr] = RECOVERED;
        auto it2 = it;
        it++;
        infectedVertices.erase(it2);
        iCount--;
        rCount++;
      } else {
        it++;
      }
    }
    evolution->add(sCount, iCount, rCount);
  }
  return evolution;
}

Epidemics::Evolution * Epidemics::runSIR(Graph & graph, double delta, double nu,
    int firstInfected, int maxIterations) {
  vector<int> initiallyInfected;
  initiallyInfected.push_back(firstInfected);
  return runSIR(graph, delta, nu, initiallyInfected, maxIterations);
}

Epidemics::Evolution * Epidemics::runSIS(Graph & graph, double delta, double nu,
    vector<int> & initiallyInfected, int maxIterations, bool withReinfection) {
  list<int> infectedVertices;
  vector<Status> status(graph.getVertexCount(), SUSCEPTIBLE);
  Evolution * evolution = new Evolution();
  vector<int> lastInfected(graph.getVertexCount(), -1);
  vector<int> lastRecovered(graph.getVertexCount(), -1);
  
  initializeInfected(initiallyInfected, infectedVertices, status);

  long long iCount = (long long) initiallyInfected.size();
  long long sCount = graph.getVertexCount() - iCount;

  for (int t = 0; infectedVertices.size() > 0 &&  t < maxIterations; t++) {
    // Iterate over infected vertices.
    for (auto it = infectedVertices.begin(); it != infectedVertices.end();) {
      int curr = *it;
      auto neighbours = graph.getNeighbours(curr);
      // Infection stage.
      for (auto it2 = neighbours.begin(); it2 != neighbours.end(); it2++) {
        int neighbour = *it2;
        if (withReinfection) {
          if ((status[neighbour] == SUSCEPTIBLE || lastInfected[neighbour] != t)
              && Stat::uniform() < delta) {
            lastInfected[neighbour] = t;
            if (status[neighbour] == SUSCEPTIBLE) {
              status[neighbour] = INFECTED;
              infectedVertices.push_front(neighbour);
              sCount--;
              iCount++;
            }
          }
        } else {
          if (status[neighbour] == SUSCEPTIBLE && lastRecovered[neighbour] != t
              && Stat::uniform() < delta) {
            status[neighbour] = INFECTED;
            infectedVertices.push_front(neighbour);
            sCount--;
            iCount++;
          }
        } 
      }
      // Recovery stage.
      if (!(withReinfection && lastInfected[curr] == t) && Stat::uniform() < nu) {
        lastRecovered[curr] = t;
        status[curr] = SUSCEPTIBLE;
        auto it2 = it;
        it++;
        infectedVertices.erase(it2);
        iCount--;
        sCount++;
      } else {
        it++;
      }
    }
    evolution->add(sCount, iCount);
  }
  return evolution;
}

Epidemics::Evolution * Epidemics::runSIS(Graph & graph, double delta, double nu,
    int firstInfected, int maxIterations, bool withReinfection) {
  vector<int> initiallyInfected;
  initiallyInfected.push_back(firstInfected);
  return runSIS(graph, delta, nu, initiallyInfected, maxIterations, withReinfection);
}

Epidemics::Evolution * Epidemics::runSIS(Graph & graph, double delta, double nu,
    vector<int> & initiallyInfected, int maxIterations) {
  return runSIS(graph, delta, nu, initiallyInfected, maxIterations, false);
}

Epidemics::Evolution * Epidemics::runSIS(Graph & graph, double delta, double nu,
    int firstInfected, int maxIterations) {
  return runSIS(graph, delta, nu, firstInfected, maxIterations, false);
}

#include <stdio.h>

int main(void) {
  Graph graph;
  graph.readFromFile("ConfigurationSF_1000_023.edgelist");
  Epidemics::Evolution * ans = Epidemics::runSIS(graph, 0.7, 0.5, 0, 30, true);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < ans->statusCount[i].size(); j++) {
      printf ("%lld, ", ans->statusCount[i][j]);
    }
    printf ("\n");
  }
  delete ans;
}