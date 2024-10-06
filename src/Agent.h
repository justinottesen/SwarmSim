#pragma once

#include <vector>

#include "Contract.h"
#include "Params.h"
#include "Config.h"

class Agent {
 public:
  Agent(unsigned int ID_)
      : ID(ID_) {}
  virtual ~Agent() {}

  bool working(unsigned int t) const { return cooldown > t; }

  virtual bool shouldAccept(const Contract&) { return true; }

  const unsigned int ID;

 protected:
  unsigned int cooldown = 0;

 private:
};

class Worker : public Agent {
 public:
  Worker(unsigned int ID, double ability)
      : Agent(ID)
      , m_ability(ability) {}

  bool shouldAccept(const Contract& c) override { return c.difficulty < m_ability; }
  void accept(Contract& c) {
    LOG(INFO) << "Worker " << ID << " accepted contract " << c.ID;
    c.available = false;
    cooldown    = c.deadline;
  }

 private:
  double m_ability;
};

class Adjudicator : public Agent {
 public:
  Adjudicator(unsigned int ID)
      : Agent(ID) {}

  void accept(Contract& c) {
    LOG(INFO) << "Adjudicator " << ID << " accepted contract " << c.ID;
    --c.open_adjudicators;
    cooldown = c.deadline;
  }

 private:
};

class AgentManager {
 public:
  AgentManager(const ConfigView<AgentConfig>&& config, std::mt19937& rng);

  std::vector<Worker>&      getWorkers() { return m_workers; }
  std::vector<Adjudicator>& getAdjudicators() { return m_adjudicators; }

 private:
  unsigned int m_count = 0;

  ConfigView<AgentConfig> m_config;

  std::mt19937& m_rng;

  std::normal_distribution<double> m_abilityDist;

  std::vector<Worker>      m_workers;
  std::vector<Adjudicator> m_adjudicators;
};
