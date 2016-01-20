#include "rumor.h"

void Rumor::Evolution::add(long long iCount, long long sCount, long long rCount) {
  usedStatus = 3;
  statusCount[IGNORANT].push_back(iCount);
  statusCount[SPREADER].push_back(sCount);
  statusCount[STIFLER].push_back(rCount);
}

long long Rumor::Evolution::getReliability(void) {
  long long spreaderCount = statusCount[SPREADER].back();
  if (usedStatus == 3) {
    spreaderCount += statusCount[STIFLER].back();
  }
  return spreaderCount;
}

void Rumor::Evolution::operator += (Evolution *e) {
  usedStatus = e->usedStatus;
  for (int i = 0; i < usedStatus; i++) {
    long long last = (statusCount[i].size() > 0 ? statusCount[i].back() : 0);
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

void Rumor::initialize() {
  Stat::initialize();
}

void Rumor::initializeSpreader(vector<int> & initiallySpreader,
    list<int> & spreaderVertices, vector<Rumor::Status> & status) {  
  // Initialize status and list of spreader vertices.
  for (int i = 0; i < initiallySpreader.size(); i++) {
    spreaderVertices.push_back(initiallySpreader[i]);
    status[initiallySpreader[i]] = SPREADER;
  }
}

Rumor::Evolution * Rumor::runISR(Graph & graph, double lambda, double alpha,
    vector<int> & initiallySpreader, int maxIterations) {
  vector<Status> status;
  return runISR(graph, lambda, alpha, initiallySpreader, maxIterations, status);
}

Rumor::Evolution * Rumor::runISR(Graph & graph, double lambda, double alpha,
    vector<int> & initiallySpreader, int maxIterations, vector<Status> &status) {
  list<int> spreaderVertices;
  status = vector<Status>(graph.getVertexCount(), IGNORANT);
  Evolution * evolution = new Evolution();
  
  initializeSpreader(initiallySpreader, spreaderVertices, status);

  long long sCount = (long long) initiallySpreader.size();
  long long iCount = graph.getVertexCount() - sCount;
  long long rCount = 0;

  evolution->add(iCount, sCount, rCount);

  for (int t = 0; spreaderVertices.size() > 0 &&  t < maxIterations; t++) {
    //fprintf (stderr, "(t, sz) = (%d %d)\n", t, spreaderVertices.size());
    // Vector to store the nodes that turned into spreaders.
    vector<int> newSpreaderVertices;
    // Vector to store the nodes that turned into stiflers.
    vector<decltype(spreaderVertices.begin())> newStiflers;

    // Iterate over spreader vertices.
    for (auto it = spreaderVertices.begin(); it != spreaderVertices.end(); it++) {
      int curr = *it;
      auto neighbours = graph.getNeighbours(curr);
      vector<int> shuffledNeighbours;
      for (auto it2 = neighbours.begin(); it2 != neighbours.end(); it2++) {
        shuffledNeighbours.push_back(*it2);
      }
      // Shuffle the order of the neighbours.
      for (int i = shuffledNeighbours.size()-1; i >= 0; i--) {
        int j = Stat::randint(i+1);
        swap(shuffledNeighbours[i], shuffledNeighbours[j]);
      }
      // Spreading and recovery stage.
      for (auto it2 = shuffledNeighbours.begin(); it2 != shuffledNeighbours.end(); it2++) {
        int neighbour = *it2;
        if (status[neighbour] == IGNORANT) {
          if (Stat::uniform() < lambda) {
            // Spreading.
            newSpreaderVertices.push_back(neighbour);
            iCount--;
            sCount++;
          }
        } else {
          if (Stat::uniform() < alpha) {
            // Recovery.
            newStiflers.push_back(it);
            sCount--;
            rCount++;
            break;
          }
        }
      }
    }
    // Insert the new spreaders into the spreader list.
    for (auto it = newSpreaderVertices.begin(); it != newSpreaderVertices.end(); it++) {
      status[*it] = SPREADER;
      spreaderVertices.push_back(*it);
    }
    // Remove the new stiflers form the spreader list.
    for (auto it = newStiflers.begin(); it != newStiflers.end(); it++) {
      status[*(*it)] = STIFLER;
      spreaderVertices.erase(*it);
    }
    evolution->add(iCount, sCount, rCount);
  }
  return evolution;
}

Rumor::Evolution * Rumor::runISR(Graph & graph, double lambda, double alpha, int firstSpreader,
    int maxIterations) {
  vector<int> initiallySpreader;
  initiallySpreader.push_back(firstSpreader);
  return runISR(graph, lambda, alpha, initiallySpreader, maxIterations);
}



// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>

// int main(void) {
//   Graph graph;
//   srand(time(NULL));
//   graph.readFromFile("../../../Data/macaco.edgelist");
//   Rumor::initialize();
//   Rumor::Evolution ans;
//   for (int it = 0; it < 30; it++) {
//     fprintf (stderr, "It: %d\n", it);
//     Rumor::Evolution * curr = Rumor::runISR(graph, 0.7, 0.3, 0, 30);
//     ans += curr;
//     delete curr;
//   }
//   for (int i = 0; i < ans.usedStatus; i++) {
//     printf ("%c = [", i?(i-1?'r':'s'):'i');
//     for (int j = 0; j < ans.statusCount[i].size(); j++) {
//       printf ("%s%lld", j?", ":"", ans.statusCount[i][j]);
//     }
//     printf ("]\n");
//   }
// } 