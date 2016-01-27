/** This program runs a MT rumor and prints the relative density of ignorants
    as a function of theis connectivity at the end of the spreading process
    for a range of values of alpha.
    The results should be plotted and compared to Fig 1 of "Efficiency and 
    reliability of epidemic data dissemination in complex networks". **/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../graph/graph.h"
#include "../rumor.h"

void addStatus(vector<long long> &v, vector<Rumor::Status> &status) {
  for (int i = 0; i < status.size(); i++) {
    v[i] += (status[i] == Rumor::Status::IGNORANT);
  }
}

int main(int argc, char * argv[]) {
  Graph graph;
  vector<int> alphas = {2, 10}; // Inverse of recovery probability.
  double LAMBDA = 1; // Spreading probability.
  int repetitions = 30;
  vector<string> testFiles = {"Resources/BA.edgelist"};
  char dirName[] = "./results";
  char filename[100];

  Rumor::initialize();

  for (auto testFile : testFiles) {
    graph.readFromFile(testFile);
    fprintf (stderr, "%s\n", testFile.c_str());
    for (auto alpha : alphas) {
      fprintf (stderr, "alpha: %d\n", alpha);
      sprintf(filename, "%s/sreliabilityPerNode%d.test2", dirName, alpha);
      FILE *f = fopen(filename, "w");
      if (!f) {
        fprintf (stderr, "Error: %s could not be opened.\n", filename);
        return 0;
      }
      fprintf (f, "%d\n", alpha);
      vector<long long> ignorantCount(graph.getVertexCount(), 0);
      for (int r = 0; r < repetitions; r++) {
        fprintf (stderr, "\trep: %d\n", r);
        for (int v = 0; v < graph.getVertexCount(); v++) {
          vector<Rumor::Status> currStatus;
          Rumor::Evolution * curr = Rumor::runISR(graph, LAMBDA, 1.0/alpha, v, 200, currStatus);
          addStatus(ignorantCount, currStatus);
        }
      }
      for (int i = 0; i < graph.getVertexCount(); i++) {
        fprintf (f, "%s%d", (i?",":""), graph.getDegree(i));
      }
      fprintf (f, "\n");
      for (int i = 0; i < graph.getVertexCount(); i++) {
        fprintf (f, "%s%lld", (i?",":""), ignorantCount[i]);
      }
      fprintf (f, "\n");
      fclose(f);
    }
  }
}