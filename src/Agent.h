#pragma once

#include <vector>

#include "Params.h"

class Agent {
 public:
  Agent(unsigned int ID_)
      : ID(ID_) {}

  bool working() { return active_contract; }

  const unsigned int ID;

 protected:
  bool active_contract = false;

 private:
};

class Worker : public Agent {
 public:
  Worker(unsigned int ID, double ability)
      : Agent(ID)
      , m_ability(ability) {}

 private:
  double m_ability;
};

class Adjudicator : public Agent {
 public:
  Adjudicator(unsigned int ID)
      : Agent(ID) {}

 private:
};

class AgentManager {
 public:
  AgentManager(const AgentParams& params, std::mt19937& rng);

 private:
  unsigned int m_count = 0;

  AgentParams m_params;

  std::mt19937& m_rng;

  std::normal_distribution<double> m_abilityDist;

  std::vector<Worker>      m_workers;
  std::vector<Adjudicator> m_adjudicators;
};
