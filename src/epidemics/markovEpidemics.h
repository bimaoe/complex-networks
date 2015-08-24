#include "../graph/graph.h"
#include <vector>

class MarkovEpidemics {
/** This class implements the SI, SIS and SIR epidemic processes.
**/
public:
  enum Status {
    SUSCEPTIBLE = 0,
    INFECTED = 1,
    RECOVERED = 2
  };

  class Evolution {
  public:
    int usedStatus; // Should be 2 for SI and SIS and 3 for SIR.
    vector<double> statusCount[3]; // Should be indexed with the Status enum.
    void add(double sCount, double iCount);
    void add(double sCount, double iCount, double rCount);
    double getInfectedCount(void); /* Get the expected number of infected vertices.
                                      Includes recovered vertices for SIR. */
  };

  /** Initialize the status vector.
    Parameters:
      initialStatusProbability: The status vector.
      vertexCount: The number of vertices in the graph.
      i0: The initial probability of being infected.
  **/
  static void initializeStatus(vector<double> initialStatusProbability[3], int vertexCount,
      double i0);

  /** Calculate the expected value of each status per iteration for SIR model.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      initialStatusProbability: The probability of being S/I/R for each node.
      maxIterations: Maximum number of iterations to run.
  **/
  static MarkovEpidemics::Evolution * runSIR(Graph & graph, double delta, double nu,
    vector<double> initialStatusProbability[3], int maxIterations);

  /** Calculate the expected value of each status per iteration for SIR model.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      i0: The initial probability of being infected.
      maxIterations: Maximum number of iterations to run.
  **/
  static MarkovEpidemics::Evolution * runSIR(Graph & graph, double delta, double nu, double i0,
      int maxIterations);

  /** Calculate the expected value of each status per iteration for SIR model.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      firstInfected: The initially infected vertex's ID.
      maxIterations: Maximum number of iterations to run.
  **/
  static MarkovEpidemics::Evolution * runSIR(Graph & graph, double delta, double nu,
      int firstInfected, int maxIterations);

  /** Calculate the expected value of each status per iteration for SIS model.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      initialStatusProbability: The probability of being S/I for each node.
      maxIterations: Maximum number of iterations to run.
      withReinfection: Whether the SIS will be with or without reinfection.
    Notes:
      Reinfection means that a vertex can recover and be reinfected in the same time step.
  **/
  static MarkovEpidemics::Evolution * runSIS(Graph & graph, double delta, double nu,
    vector<double> initialStatusProbability[3], int maxIterations, bool withReinfection);

  /** Calculate the expected value of each status per iteration for SIS model.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      i0: The initial probability of being infected.
      maxIterations: Maximum number of iterations to run.
      withReinfection: Whether the SIS will be with or without reinfection.
    Notes:
      Reinfection means that a vertex can recover and be reinfected in the same time step.
  **/
  static MarkovEpidemics::Evolution * runSIS(Graph & graph, double delta, double nu, double i0,
      int maxIterations, bool withReinfection);

  /** Calculate the expected value of each status per iteration for SIS model.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      firstInfected: The initially infected vertex's ID.
      maxIterations: Maximum number of iterations to run.
      withReinfection: Whether the SIS will be with or without reinfection.
    Notes:
      Reinfection means that a vertex can recover and be reinfected in the same time step.
  **/
  static MarkovEpidemics::Evolution * runSIS(Graph & graph, double delta, double nu,
      int firstInfected, int maxIterations, bool withReinfection);

  /** Calculate the expected value of each status per iteration for SIS model without reinfection.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      i0: The initial probability of being infected.
      maxIterations: Maximum number of iterations to run.
      withReinfection: Whether the SIS will be with or without reinfection.
    Notes:
      Reinfection means that a vertex can recover and be reinfected in the same time step.
  **/
  static MarkovEpidemics::Evolution * runSIS(Graph & graph, double delta, double nu, double i0,
      int maxIterations);

  /** Calculate the expected value of each status per iteration for SIS model without reinfection.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      first: The initially infected vertex's ID.
      maxIterations: Maximum number of iterations to run.
      withReinfection: Whether the SIS will be with or without reinfection.
    Notes:
      Reinfection means that a vertex can recover and be reinfected in the same time step.
  **/
  static MarkovEpidemics::Evolution * runSIS(Graph & graph, double delta, double nu,
      int firstInfected, int maxIterations);

  /** Calculate the expected value of each status per iteration for SI model.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      initialStatusProbability: The probability of being S/I for each node.
      maxIterations: Maximum number of iterations to run.
  **/
  static MarkovEpidemics::Evolution * runSI(Graph & graph, double delta,
    vector<double> initialStatusProbability[3], int maxIterations);

  /** Calculate the expected value of each status per iteration for SI model.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      i0: The initial probability of being infected.
      maxIterations: Maximum number of iterations to run.
  **/
  static MarkovEpidemics::Evolution * runSI(Graph & graph, double delta, double i0,
      int maxIterations);

  /** Calculate the expected value of each status per iteration for SIS model.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      firstInfected: The initially infected vertex's ID.
      maxIterations: Maximum number of iterations to run.
  **/
  static MarkovEpidemics::Evolution * runSI(Graph & graph, double delta, int firstInfected,
      int maxIterations);
};