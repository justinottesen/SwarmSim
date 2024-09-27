# Swarm Contract Simulator
The goal of this project is to build a general purpose simulator to model swarm contracts and test different parameters. This general model can be applied to specific situations and job types, being made more complex for these, but this is the most basic instructions.

## System Model
### Participants
All participants are solely motivated by profit. They will take whatever actions they believe will maximize profit. There are three main types of participants:
#### Clients
Clients are the creators of contracts, who stand to gain from jobs being done, but cannot do the jobs for whatever reason. When they identify a profitable job, they create a contract, and pay a worker to complete the job.
#### Workers
Workers are the acceptors of contracts. They accept contracts when they believe they can do the job outlined in the contract and make a profit from the payment offered.
#### Adjudicators
Adjudicators verify that workers complete the jobs. They will determine whether the worker gets paid out by the contract or not.

### Contracts
After identifying a profitable job, a client will create a smart contract on the Blockchain, publicly visible for all workers to see. A worker will accept the job, and attempt to complete the job before the deadline. Adjudicators will verify whether the job was completed or not, deciding whether the worker gets paid out or not.

Each contract has the following components:

#### Price
This is what the client is promising to pay upon completion of the job. This is locked into the smart contract upon posting to ensure the client cannot back down on payment.

#### Client Collatoral
This is what the client puts into the smart contract upon accepting. This penalizes clients who accept jobs without the goal of finishing them.

#### Number of Adjudicators
This is how many adjudicators must sign onto the contract for it to take effect. The reward will go to the majority vote of the adjudicators, and the adjudicators who voted in that majority will be rewarded.

#### Adjudicator Reward
This is what the adjudicator is promised as a reward for verifying the completion of the job.

#### Adjudicator Collatoral
This is what the adjudicator puts into the smart contract upon accepting. This penalizes adjudicators who incorrectly verify whether jobs are done or not.

## Simulation

### Simplest Model - First Iteration

#### Model Description

The only participants involved are workers and adjudicators. Contracts are created by the simiulation with some probability each simulation step. Each contract has a "difficulty" factor, and each worker has a "ability" factor. This introduces the idea that different workers have different costs.

The estimated cost for a worker to complete a job is as calculated as `est_cost = price * difficulty / ability`. These are simple abstract measures that will be improved upon in future iterations. Also for this iteration, adjudicators will have no cost to verify whether the job was completed or not.

Collatoral is set (for both workers and adjudicators) as a percentage of the contract price: `collatoral = price * collatoral_percentage`. For the simplest iteration, this percentage will be the same for both adjudicators and workers.

The adjudicator reward will be set at a percentage of the total contract value (`contract_value = worker_collatoral + adj_collatoral`). This percentage will be split between the adjudicators who vote with the majority. So, the total reward to be split among the majority vote adjudicators can be calculated as `adj_reward = contract_value * adj_cut`, and the reward of the worker is `reward = contract_value - adj_reward`. If the adjudicators vote instead against worker, the client will recieve the reward as an apology for not fulfilling the commitment.


#### Problems with this model
- Adjudicator cost to complete a job is assumed to be zero
- Contract difficulty and worker skill are scalar values, in future iterations, higher dimensionality encodings of "Problem difficulty" will be used.
- Clients are abstracted out of the picture, contracts are created by the simulation.
- In reality, choices are made off of estimations of cost
- In this model, workers only have one contract at a time.

## Remaining Questions

How is adjudicator reward & collatoral decided?

What to do with collatoral from incorrect adjudicator votes?

# Build & Run Instructions
Build the project by running `make` in the top level directory of the project. This should create an executable `bin/app`. This can be run either directly, or using `make run`.

`clang-format` is used to format the code. This can be done by running either `clang-format src/* -i`, or `make format`.

## Dependencies
- `clang` will be used to compile and link in the makefile, although `g++` can be used instead by modifying the makefile. **Regardless, the version must be new enough to support C++20**
- `make` is used to build the project. This is not necessary, but provides shortcuts to build and run commands
- `clang-format` is used to format the code. Version 20.0.0 was used on my machine, I cannot promise earlier versions will work. Again, this is not necessary, but appreciated if committing to the repository.