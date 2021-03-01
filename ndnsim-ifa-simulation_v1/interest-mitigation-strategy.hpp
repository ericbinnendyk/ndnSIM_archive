#ifndef NDNSIM_EXAMPLES_INTEREST_MITIGATION_STRATEGY_HPP
#define NDNSIM_EXAMPLES_INTEREST_MITIGATION_STRATEGY_HPP

#include "face/face.hpp"
#include "fw/strategy.hpp"
#include "fw/algorithm.hpp"

NFD_LOG_INIT("InterestMitigationStrategy");

namespace nfd {
namespace fw {

class InterestMitigationStrategy : public Strategy {
public:
    InterestMitigationStrategy(Forwarder& forwarder, const Name& name = getStrategyName());

    virtual ~InterestMitigationStrategy() override;

    void afterReceiveInterest(const FaceEndpoint& ingress, const Interest& interest, const shared_ptr<pit::Entry>& pitEntry) override;

    static const Name& getStrategyName();
};

}
}

#endif
