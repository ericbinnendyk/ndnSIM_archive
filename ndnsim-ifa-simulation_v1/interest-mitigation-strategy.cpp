/* unfinished code to implement IFA mitigation */

#include "interest-mitigation-strategy.hpp"
#include "daemon/common/logger.hpp"

NFD_LOG_INIT("InterestMitigationStrategy");

namespace nfd {
namespace fw {

InterestMitigationStrategy::InterestMitigationStrategy(Forwarder& forwarder, const Name& name)
  : Strategy(forwarder)
{
  this->setInstanceName(makeInstanceName(name, getStrategyName()));
    IfaDetect();
}

InterestMitigationStrategy::~InterestMitigationStrategy()
{
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

std::string IfaFindPrefix(void)
{
    // to do: implement malicious prefix detection algorithm
    return "none";
}

bool IfaCheck(void)
{
    // to do: implement statistical IFA check
    return false;
}

void IfaDetect(void)
{
    if (IfaCheck()) {
        std::string badPrefix = IfaFindPrefix();
        SendIfaDetectionMessage(badPrefix);
    }
    // to do: schedule next IFA test 1 second in the future
}

// This is based on the code in satisfaction-based-pushbacks.cc from the example scenario at https://github.com/cawka/ndnSIM-scenario-template . That code appeared to be written for ndnSIM version 1.0 so I had to translate some of it to the current version.
// Sends an interest with name /<prefix>/stop where <prefix> is the prefix whose rate needs to be limited.
void SendIfaDetectionMessage(std::string badPrefix)
{
    Ptr<Name> name = Ptr<name>("stop");
    name.append(Ptr<name::Component>(badPrefix));
    Ptr<Interest> announceInterest = Create<Interest>();
    // SetScope and GetScope were only available in version 1.0 it seems
    //announceInterest->SetScope(0);
    announceInterest->setName(name);
    this->sendInterest(announceInterest, NULL, NULL, NULL); // I used NULL to express a function pointer that doesn't point to any function. Is there a better way?
    //NS_LOG("telling the neighboring nodes to mitigate the prefix " << badPrefix);
}

void InterestMitigationStrategy::afterReceiveInterest(const FaceEndpoint& ingress, const Interest& interest,
                                                 const shared_ptr<pit::Entry>& pitEntry)
{
    if (hasPendingOutRecords(*pitEntry)) {
        // not a new Interest, don't forward
        return;
    }

    /*if (interest->getName()->at(0) == name::Component("stop")) {
        // add prefix to warning table
        rate_limit_table.add(interest->getName()->at(1));
    }
    if (rate_limit_table.contains(interest->getName()->at(0))) {
        NS_LOG("Limiting the rate of prefix " << interest->getName()->at(0) << " on face " << face);
    }
    else {*/
        // interest is okay, so send it
        // can we do Strategy::afterReceiveInterest(ingress, interest, pitEntry); ?
        this->sendInterest(pitEntry, this->lookupFib(*pitEntry)->getNextHops(), interest);
    /*}*/
}

const Name&
RandomLoadBalancerStrategy::getStrategyName()
{
  static Name strategyName("ndn:/localhost/nfd/strategy/interest-mitigation");
  return strategyName;
}

} // end namespace
} // end namespace
