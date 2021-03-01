// This file is unfinished.

// This is an implementation of the Expired PIT method described in the paper
// Mitigating Distributed Denial-of-Service Attacks in Named Data Networking
// by Vassilios Vassilakis et al.

#include "expired-pit.hpp"
#include "algorithm.hpp"
#include "common/logger.hpp"
#include "ns3/simulator.h"

using ns3::Simulator;
using ns3::Seconds;

namespace nfd {
namespace fw {

NFD_REGISTER_STRATEGY(ExpiredPit);

NFD_LOG_INIT(ExpiredPit);

ExpiredPit::ExpiredPit(Forwarder& forwarder, const Name& name)
  : MulticastStrategy(forwarder)
{
    setInstanceName(makeInstanceName(name, getStrategyName()));
    // Print name of interest before it expires
    auto f = [this](const pit::Entry& entry) {
        // let's make the first component of the name identify the owner.
        std::string owner = entry.getName().at(0).toUri();
        if (this->expiry_table.count(owner) == 0) {
            this->expiry_table[owner] = 1;
        }
        else {
            this->expiry_table[owner] += 1;
        }
    };
    forwarder.beforeExpirePendingInterest.connect(f);
    Simulator::Schedule(Seconds(1.0), &ExpiredPit::CollectStatistics, this);
}

void ExpiredPit::ClearTable() {
    this->expiry_table = std::map<std::string, uint64_t>();
}

void ExpiredPit::CollectStatistics() {
    for (std::pair<std::string, uint64_t> entry : this->expiry_table) {
        std::cout << entry.first << " has had " << entry.second << " timeouts in the last second." << std::endl;
    }
    ClearTable();
    Simulator::Schedule(Seconds(1.0), &ExpiredPit::CollectStatistics, this);
}

const Name&
ExpiredPit::getStrategyName()
{
  static Name strategyName("/localhost/nfd/strategy/expired-pit/%FD%01");
  return strategyName;
}

} // end namespace fw
} // end namespace nfd
