#pragma once

#include <random>

struct NormalDist {
  double mean;
  double stdev;
};

struct ContractParams {
  double       creation_prob = 0.5;
  NormalDist   price_dist{10, 1};
  NormalDist   difficulty_dist{1, 0.1};
  unsigned int num_adjudicators = 3;
  double       collateral_ratio = 1;
  unsigned int duration         = 10;
};

struct AgentParams {
  // Workers
  unsigned int num_workers = 10;
  NormalDist   ability_dist{1, 0.1};

  // Adjudicators
  unsigned int num_adjudicators = 10;
};

struct Params {
  // General
  unsigned int simulation_length = 60;
  unsigned int rng_seed          = 0;

  // Contracts
  ContractParams contract{};

  // Agenrs
  AgentParams agent{};
};
