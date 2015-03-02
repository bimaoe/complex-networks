#include "epidemics.h"

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
  runSIR(graph, delta, nu, initiallyInfected, maxIterations);
}

#include <stdio.h>

int main(void) {
  Graph graph;
  graph.readFromFile();
  Epidemics::Evolution * ans = Epidemics::runSIR(graph, 0.8, 0.3, 0, 100);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < statusCount[i].size(); j++) {
      printf ("%lld, ", statusCount[i][j]);
    }
    printf ("\n");
  }
}