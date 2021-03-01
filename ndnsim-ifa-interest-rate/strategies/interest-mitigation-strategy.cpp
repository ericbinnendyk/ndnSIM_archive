// This custom interest mitigation strategy can selectively limit the rate of forwarding of particular prefixes.
// It keeps track of a list of prefixes to use rate limiting.
// A prefix is added to this list by sending the node an interest of the form /stop/<prefix name>

// This strategy is currently broken.
// I cannot seem to send notification interests or data from my detection app out to the router.
// The IEEE paper describes sending an unnamed piece of data, not an interest, to notify the forwarder about the malicious interest.
// In the future I may try to use this method instead.

#include "build/ns3/ptr.h"
#include "build/ns3/log.h"
#include "build/ns3/simulator.h"
#include "build/ns3/packet.h"

#include "interest-mitigation-strategy.hpp"
#include "algorithm.hpp"
#include "common/logger.hpp"

namespace nfd {
namespace fw {

NFD_REGISTER_STRATEGY(InterestMitigationStrategy);

NFD_LOG_INIT(InterestMitigationStrategy);

void SendIfaDetectionMessage(std::string badPrefix);

InterestMitigationStrategy::InterestMitigationStrategy(Forwarder& forwarder, const Name& name)
  : MulticastStrategy(forwarder)
{
  this->setInstanceName(makeInstanceName(name, getStrategyName()));
  ns3::Simulator::Schedule(ns3::Seconds(1.0), &InterestMitigationStrategy::PerSecondReset, this);
}

InterestMitigationStrategy::~InterestMitigationStrategy()
{
}

void InterestMitigationStrategy::PerSecondReset() {
    this->numLimitedInterestsPerSec = 0;
    ns3::Simulator::Schedule(ns3::Seconds(1.0), &InterestMitigationStrategy::PerSecondReset, this);
}

static bool
canForwardToNextHop(const Face& inFace, shared_ptr<pit::Entry> pitEntry, const fib::NextHop& nexthop)
{
  return !wouldViolateScope(inFace, pitEntry->getInterest(), nexthop.getFace()) &&
    canForwardToLegacy(*pitEntry, nexthop.getFace());
}

static bool
hasFaceForForwarding(const Face& inFace, const fib::NextHopList& nexthops, const shared_ptr<pit::Entry>& pitEntry)
{
  return std::find_if(nexthops.begin(), nexthops.end(), bind(&canForwardToNextHop, cref(inFace), pitEntry, _1))
         != nexthops.end();
}

void InterestMitigationStrategy::afterReceiveInterest(const FaceEndpoint& ingress, const Interest& interest,
                                                 const shared_ptr<pit::Entry>& pitEntry)
{
    if (hasPendingOutRecords(*pitEntry)) {
        // not a new Interest, don't forward
        return;
    }

    std::string prefix = interest.getName().at(0).toUri();
    //if (std::find(this->rate_limit_table.begin(), this->rate_limit_table.end(), prefix) != this->rate_limit_table.end()) {
    // the name table is currently broken, but I know that in my simulation the malicious prefix is /line/forged,
    // so I need to do this instead
    if (this->attackDetected && prefix == "line" && interest.getName().at(1).toUri() == "forged") {
        this->numLimitedInterestsPerSec += 1;
        if (this->numLimitedInterestsPerSec >= this->rateLimit) {
            this->rejectPendingInterest(pitEntry);
            return;
        }
    }
    // interest is okay, so send it as normal
    MulticastStrategy::afterReceiveInterest(ingress, interest, pitEntry);
}

void InterestMitigationStrategy::afterReceiveData(const shared_ptr< pit::Entry > &pitEntry, const FaceEndpoint &ingress, const Data &data) {
    // deal with warning messages
    if (data.getName().at(0) == name::Component("stop")) {
        this->attackDetected = true;
        MulticastStrategy::afterReceiveData(pitEntry, ingress, data);
        return;
    }
    MulticastStrategy::afterReceiveData(pitEntry, ingress, data);
}

const Name&
InterestMitigationStrategy::getStrategyName()
{
  static Name strategyName("/localhost/nfd/strategy/interest-mitigation/%FD%01");
  return strategyName;
}

} // end namespace
} // end namespace
