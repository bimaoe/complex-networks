#include "epidemics.h"

int Epidemics::Evolution::size(void) {
  return statusCount[SUSCEPTIBLE].size();
}

void Epidemics::Evolution::add(long long sCount, long long iCount) {
  usedStatus = 2;
  statusCount[SUSCEPTIBLE].push_back(sCount);
  statusCount[INFECTED].push_back(iCount);
}

void Epidemics::Evolution::add(long long sCount, long long iCount, long long rCount) {
  usedStatus = 3;
  statusCount[SUSCEPTIBLE].push_back(sCount);
  statusCount[INFECTED].push_back(iCount);
  statusCount[RECOVERED].push_back(rCount);
}

long long Epidemics::Evolution::getInfectedCount(int iteration = -1) {
  long long infectedCount = 0;
  if (iteration == -1) {
    infectedCount = statusCount[INFECTED].back();
    if (usedStatus == 3) {
      infectedCount += statusCount[RECOVERED].back();
    }
  } else {
    infectedCount = statusCount[INFECTED][iteration];
    if (usedStatus == 3) {
      infectedCount += statusCount[RECOVERED][iteration];
    }
  }
  return infectedCount;
}

void Epidemics::Evolution::operator += (Evolution *e) {
  usedStatus = e->usedStatus;
  for (int i = 0; i < usedStatus; i++) {
    long long last = statusCount[i].size() > 0 ? statusCount[i].back() : 0;
    int minSize = min(statusCount[i].size(), e->statusCount[i].size());
    int j;
    for (j = 0; j < minSize; j++) {
      statusCount[i][j] += e->statusCount[i][j];
    }
    for (; j < e->statusCount[i].size(); j++) {
      statusCount[i].push_back(last + e->statusCount[i][j]);
    }
    for (; j < statusCount[i].size(); j++) {
      statusCount[i][j] += e->statusCount[i].back();
    }
  }
}

void Epidemics::initialize() {
  Stat::initialize();
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
  Evolution * evolution = new Evolution();
  vector<vector<Status> > & status = evolution->status;
  status.push_back(vector<Status>(graph.getVertexCount(), SUSCEPTIBLE));

  initializeInfected(initiallyInfected, infectedVertices, status[0]);

  long long iCount = (long long) initiallyInfected.size();
  long long sCount = graph.getVertexCount() - iCount;
  long long rCount = 0;

  evolution->add(sCount, iCount, rCount);

  for (int t = 0; infectedVertices.size() > 0 &&  t < maxIterations; t++) {
    // Copy the current status vector into the next one to keep the infected
    // and recovered status.
    status.push_back(vector<Status>());
    status[t+1] = status[t];
    // Iterate over infected vertices.
    for (auto it = infectedVertices.begin(); it != infectedVertices.end();) {
      int curr = *it;
      auto neighbours = graph.getNeighbours(curr);
      // Infection stage.
      for (auto it2 = neighbours.begin(); it2 != neighbours.end(); it2++) {
        int neighbour = *it2;
        if (status[t+1][neighbour] == SUSCEPTIBLE && Stat::uniform() < delta) {
          status[t+1][neighbour] = INFECTED;
          infectedVertices.push_front(neighbour);
          sCount--;
          iCount++;
        }
      }
      // Recovery stage.
      if (Stat::uniform() < nu) {
        status[t+1][curr] = RECOVERED;
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

Epidemics::Evolution * Epidemics::runSIR(Graph & graph, double delta, double nu, int firstInfected,
    int maxIterations) {
  vector<int> initiallyInfected;
  initiallyInfected.push_back(firstInfected);
  return runSIR(graph, delta, nu, initiallyInfected, maxIterations);
}

// TODO: Update status of Evolution as in SIR.
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

  evolution->add(sCount, iCount);

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

Epidemics::Evolution * Epidemics::runSIS(Graph & graph, double delta, double nu, int firstInfected,
    int maxIterations, bool withReinfection) {
  vector<int> initiallyInfected;
  initiallyInfected.push_back(firstInfected);
  return runSIS(graph, delta, nu, initiallyInfected, maxIterations, withReinfection);
}

Epidemics::Evolution * Epidemics::runSIS(Graph & graph, double delta, double nu,
    vector<int> & initiallyInfected, int maxIterations) {
  return runSIS(graph, delta, nu, initiallyInfected, maxIterations, false);
}

Epidemics::Evolution * Epidemics::runSIS(Graph & graph, double delta, double nu, int firstInfected,
    int maxIterations) {
  return runSIS(graph, delta, nu, firstInfected, maxIterations, false);
}

// TODO: Update status of Evolution as in SIR.
Epidemics::Evolution * Epidemics::runSI(Graph & graph, double delta,
    vector<int> & initiallyInfected, int maxIterations) {
  list<int> infectedVertices;
  vector<Status> status(graph.getVertexCount(), SUSCEPTIBLE);
  Evolution * evolution = new Evolution();
  
  initializeInfected(initiallyInfected, infectedVertices, status);

  long long iCount = (long long) initiallyInfected.size();
  long long sCount = graph.getVertexCount() - iCount;

  evolution->add(sCount, iCount);

  for (int t = 0; infectedVertices.size() > 0 &&  t < maxIterations; t++) {
    // Iterate over infected vertices.
    for (auto it = infectedVertices.begin(); it != infectedVertices.end(); it++) {
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
    }
    evolution->add(sCount, iCount);
  }
  return evolution;
}

Epidemics::Evolution * Epidemics::runSI(Graph & graph, double delta, int firstInfected,
    int maxIterations) {
  vector<int> initiallyInfected;
  initiallyInfected.push_back(firstInfected);
  return runSI(graph, delta, initiallyInfected, maxIterations);
}

/*

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  Graph graph;
  graph.readFromFile("ConfigurationSF_1000_023.edgelist");
  Epidemics::initialize();
  Epidemics::Evolution ans;
  for (int it = 0; it < 30; it++) {
    Epidemics::Evolution * curr = Epidemics::runSIS(graph, 0.7, 0.3, 0, 30);
    ans += curr;
    delete curr;
  }
  for (int i = 0; i < ans.usedStatus; i++) {
    printf ("%c = [", i?(i-1?'r':'i'):'s');
    for (int j = 0; j < ans.statusCount[i].size(); j++) {
      printf ("%s%lld", j?", ":"", ans.statusCount[i][j]);
    }
    printf ("]\n");
  }
} */