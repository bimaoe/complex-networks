/** This program runs a MT rumor and prints the average reliability starting
    from all nodes for a range of alphas for a BA network.
    The results should be compared to Table 1 of "Efficiency and reliability of
    epidemic data dissemination in complex networks". **/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../graph/graph.h"
#include "../rumor.h"

int main(int argc, char * argv[]) {
  Graph graph;
  vector<int> alphas = {1, 2, 3, 4, 5, 6, 7, 8, 9}; // Inverse of recovery probability.
  double LAMBDA = 1; // Spreading probability.
  int repetitions = 30;
  vector<string> testFiles = {"Resources/BA.edgelist"};

  Rumor::initialize();

  for (auto testFile : testFiles) {
    graph.readFromFile(testFile);
    printf ("%s: ", testFile.c_str());
    for (auto alpha : alphas) {
      fprintf(stderr, "alpha = %d\n", alpha);
      double reliability = 0;
      for (int r = 0; r < repetitions; r++) {
        fprintf (stderr, "\trep: %d\n", r);
        for (int v = 0; v < graph.getVertexCount(); v++) {
          Rumor::Evolution * curr = Rumor::runISR(graph, LAMBDA, 1.0/alpha, v, 200);
          reliability += curr->getReliability();
          delete curr;
        }
      }
      reliability /= (graph.getVertexCount() * graph.getVertexCount());
      reliability /= repetitions;
      printf ("%lf ", reliability);
    }
    printf ("\n");
  }
}