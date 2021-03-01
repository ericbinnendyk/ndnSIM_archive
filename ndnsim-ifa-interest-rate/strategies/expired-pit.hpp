#ifndef NDNSIM_EXAMPLES_INTEREST_MITIGATION_STRATEGY_HPP
#define NDNSIM_EXAMPLES_INTEREST_MITIGATION_STRATEGY_HPP

#include "face/face.hpp"
#include "fw/multicast-strategy.hpp"
#include "fw/algorithm.hpp"

namespace nfd {
namespace fw {

class ExpiredPit : public MulticastStrategy {
public:
    ExpiredPit(Forwarder& forwarder, const Name& name = getStrategyName());

    void beforeExpirePendingInterest(const FaceEndpoint& egress, const Interest& interest);

    void ClearTable();

    void CollectStatistics();

    static const Name& getStrategyName();
private:
    std::map<std::string, uint64_t> expiry_table;
};

}
}

#endif
