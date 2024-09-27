#include "Agent.h"

#include "Logger.h"

AgentManager::AgentManager(const AgentParams& params, std::mt19937& rng)
    : m_params(params)
    , m_rng(rng)
    , m_abilityDist(params.ability_dist.mean, params.ability_dist.stdev) {
  LOG(INFO) << "Initializing " << params.num_workers << " workers";
  while (m_count < params.num_workers) {
    LOG(TRACE) << "Adding worker with ID " << m_count;
    m_workers.emplace_back(m_count++, m_abilityDist(rng));
  }

  LOG(INFO) << "Initializing " << params.num_adjudicators << " adjudicators";
  while (m_count < params.num_adjudicators + params.num_workers) {
    LOG(TRACE) << "Adding adjudicator with ID " << m_count;
    m_adjudicators.emplace_back(m_count++);
  }
}
