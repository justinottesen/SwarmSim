#include "Simulator.h"

#include <ranges>

Simulator::Simulator(const Params& params)
    : m_params(params)
    , m_rng(params.rng_seed)
    , m_agentManager(params.agent, m_rng)
    , m_contractManager(params.contract, m_rng) {
  LOG(INFO) << "Initializing simulation";

  LOG(TRACE) << "Seeded rng with value " << params.rng_seed;
}

void Simulator::step(unsigned int t) {
  LOG(TRACE) << "Running step " << t;
  // Update Contracts
  m_contractManager.step(t);
}

void Simulator::run() {
  LOG(INFO) << "Running simulation with total length " << m_params.simulation_length;
  for (unsigned int time = 0; time < m_params.simulation_length; ++time) { step(time); }
}
