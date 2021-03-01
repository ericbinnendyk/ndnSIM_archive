#ifndef NDNSIM_EXAMPLES_INTEREST_MITIGATION_STRATEGY_HPP
#define NDNSIM_EXAMPLES_INTEREST_MITIGATION_STRATEGY_HPP

#include "face/face.hpp"
#include "fw/multicast-strategy.hpp"
#include "fw/algorithm.hpp"

namespace nfd {
namespace fw {

class InterestMitigationStrategy : public MulticastStrategy {
public:
    InterestMitigationStrategy(Forwarder& forwarder, const Name& name = getStrategyName());

    virtual ~InterestMitigationStrategy() override;

    void afterReceiveInterest(const FaceEndpoint& ingress, const Interest& interest, const shared_ptr<pit::Entry>& pitEntry) override;
    void afterReceiveData(const shared_ptr< pit::Entry > &pitEntry, const FaceEndpoint &ingress, const Data &data) override;

    static const Name& getStrategyName();

    void PerSecondReset();
private:
    std::vector<std::string> rate_limit_table;
    bool attackDetected = false;
    uint64_t rateLimit = 50;
    uint64_t numLimitedInterestsPerSec = 0;
};

}
}

#endif
