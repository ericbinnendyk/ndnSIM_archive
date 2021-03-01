// This custom interest mitigation strategy can selectively limit the rate of forwarding of particular prefixes.
// It keeps track of a list of prefixes to use rate limiting.
// A prefix is added to this list by sending the node an interest of the form /stop/<prefix name>
// The IEEE paper describes sending an unnamed piece of data to warn 

#include "interest-mitigation-strategy.hpp"
#include "algorithm.hpp"
#include "common/logger.hpp"

namespace nfd {
namespace fw {

NFD_REGISTER_STRATEGY(InterestMitigationStrategy);

NFD_LOG_INIT(InterestMitigationStrategy);

void SendIfaDetectionMessage(std::string badPrefix);

InterestMitigationStrategy::InterestMitigationStrategy(Forwarder& forwarder, const Name& name)
  : Strategy(forwarder)
{
  this->setInstanceName(makeInstanceName(name, getStrategyName()));
    //IfaDetect();
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

// This is inspired by the code from the example scenario at
// https://github.com/cawka/ndnSIM-ddos-interest-flooding/blob/master/extensions/strategies/satisfaction-based-pushback.cc.
// That code appeared to be written for ndnSIM version 1.0 so I had to translate some of it to the current version.
void InterestMitigationStrategy::afterReceiveInterest(const FaceEndpoint& ingress, const Interest& interest,
                                                 const shared_ptr<pit::Entry>& pitEntry)
{
    //std::cout << "BLEH BLEH BLEH INTEREST RECEVED " << interest.getName() << "\n";
    if (hasPendingOutRecords(*pitEntry)) {
        // not a new Interest, don't forward
        return;
    }

    // deal with interests that are warning messages
    if (interest.getName().at(0) == name::Component("stop")) {
        // add prefix to warning table
        std::string interest_name = interest.getName().at(1).toUri();
        if (std::find(this->rate_limit_table.begin(), this->rate_limit_table.end(), interest_name) == this->rate_limit_table.end()) {
            this->rate_limit_table.push_back(interest.getName().at(1).toUri());
        }
        // send warning to neighbors (to do: actually send a new interest to be sent)
        const fib::Entry& fibEntry = this->lookupFib(*pitEntry);
        for (const auto& nexthop : fibEntry.getNextHops()) {
            Face& outFace = nexthop.getFace();
            if (!wouldViolateScope(ingress.face, interest, outFace) &&
                canForwardToLegacy(*pitEntry, outFace)) {
                this->sendInterest(pitEntry, FaceEndpoint(outFace, 0), interest);
            }
        }
        return;
    }

    // deal with interests that are non-warning messages
    std::string prefix = interest.getName().at(0).toUri();
    if (std::find(this->rate_limit_table.begin(), this->rate_limit_table.end(), prefix) != this->rate_limit_table.end()) {
        std::cout << "Limiting the rate of prefix " << interest.getName().at(0) << std::endl;
        // to do: actual rate limiting
        this->rejectPendingInterest(pitEntry);
    }
    else {
        // interest is okay, so send it
        // send it to all available routes (normally I would be just doing the "default behavior" for a forwarding strategy
        // but I can't seem to call afterReceiveInterest in the parent function
        // (calling Strategy::afterReceiveInterest(ingress, interest, pitEntry); gives a runtime error)
        // Maybe it should inherit from an existing forwarding strategy.
        const fib::Entry& fibEntry = this->lookupFib(*pitEntry);
        for (const auto& nexthop : fibEntry.getNextHops()) {
            Face& outFace = nexthop.getFace();
            if (!wouldViolateScope(ingress.face, interest, outFace) &&
                canForwardToLegacy(*pitEntry, outFace)) {
                this->sendInterest(pitEntry, FaceEndpoint(outFace, 0), interest);
            }
        }
        return;
    }
}

const Name&
InterestMitigationStrategy::getStrategyName()
{
  static Name strategyName("/localhost/nfd/strategy/interest-mitigation/%FD%01");
  return strategyName;
}

} // end namespace
} // end namespace
