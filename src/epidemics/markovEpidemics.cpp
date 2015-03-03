#include "markovEpidemics.h"

void MarkovEpidemics::Evolution::add(double sCount, double iCount) {
  statusCount[SUSCEPTIBLE].push_back(sCount);
  statusCount[INFECTED].push_back(iCount);
}

void MarkovEpidemics::Evolution::add(double sCount, double iCount, double rCount) {
  statusCount[SUSCEPTIBLE].push_back(sCount);
  statusCount[INFECTED].push_back(iCount);
  statusCount[RECOVERED].push_back(rCount);
}

void MarkovEpidemics::initializeStatus(vector<double> initialStatusProbability[3], int vertexCount,
    double i0) {
  for (int i = 0; i < vertexCount; i++) {
    initialStatusProbability[SUSCEPTIBLE].push_back(1.0 - i0);
    initialStatusProbability[INFECTED].push_back(i0);
    initialStatusProbability[RECOVERED].push_back(0.0);
  }
}

MarkovEpidemics::Evolution * MarkovEpidemics::runSIR(Graph & graph, double delta, double nu,
    vector<double> initialStatusProbability[3], int maxIterations) {
  Evolution * evolution = new Evolution();
  vector<double> statusProbability[2][3];

  double sCount = 0.0, iCount = 0.0, rCount = 0.0;
  for (int i = 0; i < initialStatusProbability[0].size(); i++) {
    statusProbability[0][SUSCEPTIBLE].push_back(initialStatusProbability[SUSCEPTIBLE][i]);
    statusProbability[0][INFECTED].push_back(initialStatusProbability[INFECTED][i]);
    statusProbability[0][RECOVERED].push_back(initialStatusProbability[RECOVERED][i]);

    sCount += statusProbability[0][SUSCEPTIBLE][i];
    iCount += statusProbability[0][INFECTED][i];
    rCount += statusProbability[0][RECOVERED][i];

    statusProbability[1][SUSCEPTIBLE].push_back(0);
    statusProbability[1][INFECTED].push_back(0);
    statusProbability[1][RECOVERED].push_back(0);
  }
  evolution->add(sCount, iCount, rCount);

  int lastT = 0;
  for (int t = 0; t < maxIterations; t++) {
    int currT = 1 - lastT;
    sCount = iCount = rCount = 0;
    for (int i = 0; i < graph.getVertexCount(); i++) {
      auto neighbours = graph.getNeighbours(i);

      // P(non infection) = Probability of not being infected by any neighbour.
      // P(non infection) = prod_{neighbour} i_{neighbour}(t-1) * (1 - delta)
      double nonInfectionProbability = 1.0;
      for (auto it = neighbours.begin(); it != neighbours.end(); it++) {
        int neighbour = *it;
        nonInfectionProbability *= (1.0 - delta * statusProbability[lastT][INFECTED][neighbour]);
      }

      // s(t) = s(t-1) * P(non infection)
      statusProbability[currT][SUSCEPTIBLE][i] =
          statusProbability[lastT][SUSCEPTIBLE][i] * nonInfectionProbability;
      // i(t) = s(t-1) * P(infection) + i(t-1) * P(non recovery)
      statusProbability[currT][INFECTED][i] =
          statusProbability[lastT][SUSCEPTIBLE][i] * (1.0 - nonInfectionProbability)
          + statusProbability[lastT][INFECTED][i] * (1.0 - nu);
      // r(t) = r(t-1) + i(t-1) * P(recovery)
      statusProbability[currT][RECOVERED][i] =
          statusProbability[lastT][RECOVERED][i]
          + statusProbability[lastT][INFECTED][i] * nu;

      sCount += statusProbability[currT][SUSCEPTIBLE][i];
      iCount += statusProbability[currT][INFECTED][i];
      rCount += statusProbability[currT][RECOVERED][i];
    }
    evolution->add(sCount, iCount, rCount);
    lastT = currT;
  }
  return evolution;
}

MarkovEpidemics::Evolution * MarkovEpidemics::runSIR(Graph & graph, double delta, double nu,
    double i0, int maxIterations) {
  vector<double> initialStatusProbability[3];

  initializeStatus(initialStatusProbability, graph.getVertexCount(), i0);

  return runSIR(graph, delta, nu, initialStatusProbability, maxIterations);
}

MarkovEpidemics::Evolution * MarkovEpidemics::runSIR(Graph & graph, double delta, double nu,
    int firstInfected, int maxIterations) {
  vector<double> initialStatusProbability[3];

  initializeStatus(initialStatusProbability, graph.getVertexCount(), 0.0);

  initialStatusProbability[INFECTED][firstInfected] = 1.0;
  initialStatusProbability[SUSCEPTIBLE][firstInfected] = 0.0;
  return runSIR(graph, delta, nu, initialStatusProbability, maxIterations);
}

MarkovEpidemics::Evolution * MarkovEpidemics::runSIS(Graph & graph, double delta, double nu,
    vector<double> initialStatusProbability[2], int maxIterations, bool withReinfection) {
  Evolution * evolution = new Evolution();
  vector<double> statusProbability[2][3];

  double sCount = 0.0, iCount = 0.0, rCount = 0.0;
  for (int i = 0; i < initialStatusProbability[0].size(); i++) {
    statusProbability[0][SUSCEPTIBLE].push_back(initialStatusProbability[SUSCEPTIBLE][i]);
    statusProbability[0][INFECTED].push_back(initialStatusProbability[INFECTED][i]);

    sCount += statusProbability[0][SUSCEPTIBLE][i];
    iCount += statusProbability[0][INFECTED][i];

    statusProbability[1][SUSCEPTIBLE].push_back(0);
    statusProbability[1][INFECTED].push_back(0);
  }
  evolution->add(sCount, iCount);

  int lastT = 0;
  for (int t = 0; t < maxIterations; t++) {
    int currT = 1 - lastT;
    sCount = iCount = rCount = 0;
    for (int i = 0; i < graph.getVertexCount(); i++) {
      auto neighbours = graph.getNeighbours(i);

      // P(non infection) = Probability of not being infected by any neighbour.
      // P(non infection) = prod_{neighbour} i_{neighbour}(t-1) * (1 - delta)
      double nonInfectionProbability = 1.0;
      for (auto it = neighbours.begin(); it != neighbours.end(); it++) {
        int neighbour = *it;
        nonInfectionProbability *= (1.0 - delta * statusProbability[lastT][INFECTED][neighbour]);
      }

      if (withReinfection) {
        // s(t) = s(t-1) * P(non infection) + i(t-1) * P(recovery) * P(non infection)
        statusProbability[currT][SUSCEPTIBLE][i] =
            statusProbability[lastT][SUSCEPTIBLE][i] * nonInfectionProbability
            + statusProbability[lastT][INFECTED][i] * nu * nonInfectionProbability;
        // i(t) = s(t-1) * P(infection) + i(t-1) * (P(non recovery) + P(recovery) * P(infection))
        statusProbability[currT][INFECTED][i] =
            statusProbability[lastT][SUSCEPTIBLE][i] * (1.0 - nonInfectionProbability)
            + statusProbability[lastT][INFECTED][i] * ((1.0 - nu)
                + (nu * (1.0 - nonInfectionProbability)));
      } else {
        // s(t) = s(t-1) * P(non infection) + i(t-1) * P(recovery)
        statusProbability[currT][SUSCEPTIBLE][i] =
            statusProbability[lastT][SUSCEPTIBLE][i] * nonInfectionProbability
            + statusProbability[lastT][INFECTED][i] * nu;
        // i(t) = s(t-1) * P(infection) + i(t-1) * P(non recovery)
        statusProbability[currT][INFECTED][i] =
            statusProbability[lastT][SUSCEPTIBLE][i] * (1.0 - nonInfectionProbability)
            + statusProbability[lastT][INFECTED][i] * (1.0 - nu);
      }

        sCount += statusProbability[currT][SUSCEPTIBLE][i];
        iCount += statusProbability[currT][INFECTED][i];
    }
    evolution->add(sCount, iCount);
    lastT = currT;
  }
  return evolution;
}

MarkovEpidemics::Evolution * MarkovEpidemics::runSIS(Graph & graph, double delta, double nu,
    double i0, int maxIterations, bool withReinfection) {
  vector<double> initialStatusProbability[3];

  initializeStatus(initialStatusProbability, graph.getVertexCount(), i0);

  return runSIS(graph, delta, nu, initialStatusProbability, maxIterations, withReinfection);
}

MarkovEpidemics::Evolution * MarkovEpidemics::runSIS(Graph & graph, double delta, double nu,
    int firstInfected, int maxIterations, bool withReinfection) {
  vector<double> initialStatusProbability[3];

  initializeStatus(initialStatusProbability, graph.getVertexCount(), 0.0);

  initialStatusProbability[INFECTED][firstInfected] = 1.0;
  initialStatusProbability[SUSCEPTIBLE][firstInfected] = 0.0;
  return runSIS(graph, delta, nu, initialStatusProbability, maxIterations, withReinfection);
}

MarkovEpidemics::Evolution * MarkovEpidemics::runSIS(Graph & graph, double delta, double nu,
    double i0, int maxIterations) {
  return runSIS(graph, delta, nu, i0, maxIterations, false);
}

MarkovEpidemics::Evolution * MarkovEpidemics::runSIS(Graph & graph, double delta, double nu,
    int firstInfected, int maxIterations) {
  return runSIS(graph, delta, nu, firstInfected, maxIterations, false);
}

#include <stdio.h>

int main(void) {
  Graph graph;
  graph.readFromFile("ConfigurationSF_1000_023.edgelist");
  MarkovEpidemics::Evolution * ans = MarkovEpidemics::runSIS(graph, 0.7, 0.5, 0, 30, true);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < ans->statusCount[i].size(); j++) {
      printf ("%lf, ", ans->statusCount[i][j]);
    }
    printf ("\n");
  }
  delete ans;
}