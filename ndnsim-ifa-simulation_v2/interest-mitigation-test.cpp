/* interest-mitigation-test.cpp */
/* app to selectively limit the rate of a prefix when an IFA using that prefix has been detected */

#include "interest-mitigation-test.hpp"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

#include "model/ndn-l3-protocol.hpp"
#include "helper/ndn-fib-helper.hpp"

#include <memory>

NS_LOG_COMPONENT_DEFINE("InterestMitigation");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(InterestMitigation);

TypeId
InterestMitigation::GetTypeId(void)
{
  static TypeId tid = TypeId("InterestMitigation").SetParent<App>().AddConstructor<InterestMitigation>();

  return tid;
}

InterestMitigation::InterestMitigation(void)
{
  NS_LOG_FUNCTION_NOARGS();
}

virtual void InterestMitigation::OnStart(void)
{
    App::OnStart();
    IfaDetect();
}

virtual void InterestMitigation::IfaDetect(void)
{
    if (IfaCheck()) {
        std::string badPrefix = IfaFindPrefix();
        SendIfaDetectionMessage(badPrefix);
    }
    // to do: schedule next IFA test 1 second in the future
}

virtual bool InterestMitigation::IfaCheck(void)
{
    // to do: implement statistical IFA check
    return false;
}

virtual std::string InterestMitigation::IfaFindPrefix(void)
{
    // to do: implement malicious prefix detection algorithm
    return "none";
}

// This is based on the code in satisfaction-based-pushbacks.cc from the example scenario at https://github.com/cawka/ndnSIM-scenario-template . That code appeared to be written for ndnSIM version 1.0 so I had to translate some of it to the current version.
// Sends an interest with name /<prefix>/stop where <prefix> is the prefix whose rate needs to be limited.
virtual void InterestMitigation::SendIfaDetectionMessage(std::string badPrefix)
{
    Ptr<Name> name = Ptr<Name>(badPrefix);
    name.append(Ptr<name::Component>("stop"));
    Ptr<Interest> announceInterest = Create<Interest>();
    // SetScope and GetScope were only available in version 1.0 it seems
    //announceInterest->SetScope(0);
    announceInterest->setName(name);
    m_face->expressInterest(announceInterest, NULL, NULL, NULL); // I used NULL to express a function pointer that doesn't point to any function. Is there a better way?
    NS_LOG("telling the neighboring nodes to mitigate the prefix " << badPrefix);
}

// I guess that this was the function that got triggered when the interest reached the previous nodes
// to do: edit this one so that it becomes about receiving the interest warning of the bad prefix, and calling a function to filter that prefix
// If the received interest is of the form /<prefix>/stop, limit the rate of <prefix>
virtual void InterestMitigation::OnInterest(shared_ptr<Face> face, shared_ptr<const Interest> interest)
{
  if (/* interest->GetScope () != 0 && */ interest->getName()->at(1) != name::Component("stop"))
    App::OnInterest (face, interest);
  else
    //ApplyAnnouncedLimit (face, interest);
    NS_LOG("Limiting the rate of prefix " << interest->getName()->at(0) << " on face " << face);
    // to do: actually limit prefix rate
}
