#include <vector>

struct Contract {

  Contract(double price_, unsigned int deadline_) : price(price_), deadline(deadline_) {}

  double price;
  unsigned int deadline;
};

typedef std::vector<Contract> Blockchain;
