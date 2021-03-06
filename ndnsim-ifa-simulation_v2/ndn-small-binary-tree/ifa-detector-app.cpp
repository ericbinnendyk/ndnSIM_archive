// This app records the distribution of interests entering a router in a particular time frame and
// uses the Gini impurity measurement to detect the presence of an interest flooding attack (IFA)
// as described in the paper "A Gini Impurity-Based Interest Flooding Attack Defence Mechanism in NDN" by Zhi, Luo et al.

#include "ifa-detector-app.hpp"

// I don't know if I need all of these
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

//#include "ns3/ndnSIM/ndn-cxx/util/time.hpp"

NS_LOG_COMPONENT_DEFINE("IfaDetector");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(IfaDetector);
TypeId IfaDetector::GetTypeId()
{
    static TypeId tid = TypeId("IfaDetector").SetParent<ndn::App>().AddConstructor<IfaDetector>();
    return tid;
}

void IfaDetector::StartApplication()
{
    ndn::App::StartApplication();
    ndn::FibHelper::AddRoute(GetNode(), "/line/fine", m_face, 0);
    ndn::FibHelper::AddRoute(GetNode(), "/line/forged", m_face, 0);
    Simulator::Schedule(Seconds(1.0), &IfaDetector::IfaDetect, this);
    this->gi_curr = 0;
    this->gi_prev = 0;
}

void IfaDetector::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{
    // first do the default behavior of the app when it receives an interest
    ndn::App::OnInterest(interest);

    if (this->interest_counts.count(interest->getName().toUri()) == 0) {
        // add interest
        this->interest_counts[interest->getName().toUri()] = 1;
    }
    else {
        this->interest_counts[interest->getName().toUri()]++;
    }
}

std::string IfaDetector::IfaFindPrefix()
{
    // to do: implement malicious prefix detection algorithm
    return "/line/forged";
}

bool IfaDetector::IfaCheck()
{
    this->gi_prev = this->gi_curr;
    this->gi_curr = GiniImpurity(this->interest_counts);
    if (this->gi_prev == 0) {
        // not enough information, too early
        return false;
    }
    double relative_increase = (this->gi_curr - this->gi_prev)/this->gi_prev;
    //std::cout << relative_increase << std::endl;
    return (relative_increase > 0.01); // to do: replace threshold with a number that works.
}

void IfaDetector::SendIfaDetectionMessage(std::string badPrefix)
{
    std::shared_ptr<ndn::Name> interestName = std::make_shared<ndn::Name>(ndn::Name("stop"));
    interestName->append(ndn::name::Component(badPrefix));
    std::shared_ptr<ndn::Interest> stopInterest = std::make_shared<ndn::Interest>();
    stopInterest->setName(*interestName);
    // set nonce because apparently it uniquely identifies the interest.
    Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
    stopInterest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
    // consumer app also sets nonce, can be prefix, and interest lifetime. Do I need to set these?
    stopInterest->setCanBePrefix(false);
    ndn::time::milliseconds interestLifeTime(1000);
    stopInterest->setInterestLifetime(interestLifeTime);
    // log interest
    std::cout << "interest name: " << stopInterest->getName() << std::endl;
    m_transmittedInterests(stopInterest, this, m_face);
    // send interest
    m_appLink->onReceiveInterest(*stopInterest);
}

void IfaDetector::IfaDetect()
{
    if (IfaCheck()) {
        std::string badPrefix = IfaFindPrefix();
        SendIfaDetectionMessage(badPrefix);
    }
    ClearStats();
    Simulator::Schedule(Seconds(1.0), &IfaDetector::IfaDetect, this);
}

void IfaDetector::ClearStats()
{
    this->interest_counts = std::map<std::string, uint64_t>();
}

double IfaDetector::GiniImpurity(std::map<std::string, uint64_t> interestCounts)
{
    uint64_t totalInterests = 0;
    auto increaseTotal = [&totalInterests](std::pair<std::string, uint64_t> pair) { totalInterests += pair.second; };
    std::for_each(interestCounts.begin(), interestCounts.end(), increaseTotal);
    double value = 1.0;
    auto decreaseValue = [totalInterests, &value](std::pair<std::string, uint64_t> pair) {
        double freq = ((double) pair.second)/totalInterests;
        value -= freq*freq;
    };
    std::for_each(interestCounts.begin(), interestCounts.end(), decreaseValue);
    return value;
}

}
