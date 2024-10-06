#include "Agent.h"

#include "Logger.h"

AgentManager::AgentManager(const ConfigView<AgentConfig>&& config, std::mt19937& rng)
    : m_config(config)
    , m_rng(rng)
    , m_abilityDist(config.get<NormalDist>("ability_distribution")) {
  LOG(INFO) << "Initializing " << config.get<unsigned int>("num_workers") << " workers";
  while (m_count < config.get<unsigned int>("num_workers")) {
    LOG(TRACE) << "Adding worker with ID " << m_count;
    m_workers.emplace_back(m_count++, m_abilityDist(rng));
  }

  LOG(INFO) << "Initializing " << config.get<unsigned int>("num_adjudicators") << " adjudicators";
  while (m_count < config.get<unsigned int>("num_adjudicators") + config.get<unsigned int>("num_workers")) {
    LOG(TRACE) << "Adding adjudicator with ID " << m_count;
    m_adjudicators.emplace_back(m_count++);
  }
}
