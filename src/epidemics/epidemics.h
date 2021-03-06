#include "../graph/graph.h"
#include "../stat/stat.h"
#include <vector>
#include <list>

class Epidemics {
/** This class implements the SI, SIS and SIR epidemic processes.
**/
public:
  Stat stat;

  enum Status {
    SUSCEPTIBLE = 0,
    INFECTED = 1,
    RECOVERED = 2
  };

  class Evolution {
  public:
    int usedStatus; // Should be 2 for SI and SIS and 3 for SIR.
    vector<vector<Status> > status; // Status vector for every time step.
    vector<long long> statusCount[3]; // Should be indexed with the Status enum.
    int size(void);
    void add(long long sCount, long long iCount);
    void add(long long sCount, long long iCount, long long rCount);
    /** Get the number of infected vertices. Includes recovered vertices for
        SIR.
    **/
    long long getInfectedCount(int iteration);
    void operator += (Evolution *e);
  };

  static void initialize(void);

  /** Initialize the infected list and the status vector. **/
  static void initializeInfected(vector<int> & initiallyInfected, list<int> & infectedVertices,
      vector<Status> & status);

  /** Run the SIR model in a graph.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      iniciallyInfected: The initially infected vertices' IDs.
      maxIterations: Maximum number of iterations to run.
  **/
  static Epidemics::Evolution * runSIR(Graph & graph, double delta, double nu,
      vector<int> & initiallyInfected, int maxIterations);

  /** Run the SIR model in a graph.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      firstInfected: The initially infected vertex's ID.
      maxIterations: Maximum number of iterations to run.
  **/
  static Epidemics::Evolution * runSIR(Graph & graph, double delta, double nu, int firstInfected,
      int maxIterations);

  /** Run the SIS model in a graph.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      iniciallyInfected: The initially infected vertices' IDs.
      maxIterations: Maximum number of iterations to run.
      withReinfection: Whether the SIS will be with or without reinfection.
    Notes:
      Reinfection means that a vertex can recover and be reinfected in the same time step.
  **/
  static Epidemics::Evolution * runSIS(Graph & graph, double delta, double nu,
      vector<int> & initiallyInfected, int maxIterations, bool withReinfection);

  /** Run the SIS model in a graph.
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
  static Epidemics::Evolution * runSIS(Graph & graph, double delta, double nu, int firstInfected,
      int maxIterations, bool withReinfection);

  /** Run the SIS model without reinfection in a graph.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      iniciallyInfected: The initially infected vertices' IDs.
      maxIterations: Maximum number of iterations to run.
    Notes:
      Reinfection means that a vertex can recover and be reinfected in the same time step.
  **/
  static Epidemics::Evolution * runSIS(Graph & graph, double delta, double nu,
      vector<int> & initiallyInfected, int maxIterations);

  /** Run the SIS model without reinfection in a graph.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      firstInfected: The initially infected vertex's ID.
      maxIterations: Maximum number of iterations to run.
    Notes:
      Reinfection means that a vertex can recover and be reinfected in the same time step.
  **/
  static Epidemics::Evolution * runSIS(Graph & graph, double delta, double nu, int firstInfected,
      int maxIterations);

  /** Run the SI model in a graph.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      iniciallyInfected: The initially infected vertices' IDs.
      maxIterations: Maximum number of iterations to run.
  **/
  static Epidemics::Evolution * runSI(Graph & graph, double delta,
      vector<int> & initiallyInfected, int maxIterations);

  /** Run the SI model in a graph.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      firstInfected: The initially infected vertex's ID.
      maxIterations: Maximum number of iterations to run.
  **/
  static Epidemics::Evolution * runSI(Graph & graph, double delta, int firstInfected,
      int maxIterations);
};