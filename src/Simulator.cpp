#include "Simulator.h"

#include <functional>
#include <ranges>

Simulator::Simulator(const Params& params)
    : m_params(params)
    , m_rng(params.rng_seed)
    , m_agentManager(params.agent, m_rng)
    , m_contractManager(params.contract, m_rng, m_metricManager) {
  LOG(INFO) << "Initializing simulation";

  LOG(TRACE) << "Seeded rng with value " << params.rng_seed;
}

void Simulator::step(unsigned int t) {
  LOG(INFO) << "Running step " << t;
  FrameMetrics& metrics = m_metricManager.getFrame(t);

  // Create contracts
  m_contractManager.createContracts(t);

  // Workers pick up new contracts
  for (Worker& w : m_agentManager.getWorkers()) {
    if (w.working(t)) { continue; }
    for (Contract& c : m_contractManager.getContracts()) {
      if (!c.available || !w.shouldAccept(c)) { continue; }
      w.accept(c);
      break;
    }
  }

  // Adjudicators pick up new contracts
  for (Adjudicator& a : m_agentManager.getAdjudicators()) {
    if (a.working(t)) { continue; }
    for (Contract& c : m_contractManager.getContracts()) {
      if (c.open_adjudicators == 0 || !a.shouldAccept(c)) { continue; }
      a.accept(c);
      break;
    }
  }

  // Tally finished contract info
  for (Contract& c : m_contractManager.getContracts()) {
    if (c.deadline == t) {
      if (c.available) {
        LOG(INFO) << "Contract " << c.ID << " expired (no worker picked it up)";
        metrics.increment(FrameMetrics::CONTRACTS_EXPIRED);
      } else if (c.open_adjudicators > 0) {
        LOG(INFO) << "Contract " << c.ID << " expired (missing " << c.open_adjudicators
                  << " adjudicators)";
        metrics.increment(FrameMetrics::CONTRACTS_EXPIRED);
      } else {
        LOG(INFO) << "Contract " << c.ID << " completed";
        metrics.increment(FrameMetrics::CONTRACTS_COMPLETED);
      }
    }
  }
}

void Simulator::run() {
  LOG(INFO) << "Running simulation with total length " << m_params.simulation_length;
  for (unsigned int time = 0; time < m_params.simulation_length; ++time) { step(time); }
}
