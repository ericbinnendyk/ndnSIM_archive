// app to print the number of interests of a certain type reaching node every second
// this app is used to find the fraction of all malicious interests reaching the producer

#include "interest_counter.hpp"

#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "helper/ndn-fib-helper.hpp"

NS_LOG_COMPONENT_DEFINE("ndn.InterestCounter");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(InterestCounter);

TypeId
InterestCounter::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ns3::ndn::InterestCounter")
      .SetGroupName("Ndn")
      .SetParent<App>()
      .AddConstructor<InterestCounter>()
      .AddAttribute("Prefix", "Prefix to count the arrivals of", StringValue("/"),
                    MakeNameAccessor(&InterestCounter::m_prefix), MakeNameChecker())
      .AddAttribute("Period", "Length of time (seconds) between reporting the number of new interests", StringValue("1.0"), MakeDoubleAccessor(&InterestCounter::m_period), MakeDoubleChecker<double>());
  return tid;
}

InterestCounter::InterestCounter()
{
  NS_LOG_FUNCTION_NOARGS();
}

// inherited from Application base class.
void
InterestCounter::StartApplication()
{
  NS_LOG_FUNCTION_NOARGS();
  App::StartApplication();

  interest_count = 0;
  FibHelper::AddRoute(GetNode(), m_prefix, m_face, 0);
  Simulator::Schedule(Seconds(m_period), &InterestCounter::PrintNumArrivals, this);
}

void
InterestCounter::StopApplication()
{
  NS_LOG_FUNCTION_NOARGS();

  App::StopApplication();
}

void
InterestCounter::OnInterest(shared_ptr<const Interest> interest)
{
  interest_count++;
}

void
InterestCounter::PrintNumArrivals()
{
    std::cout << interest_count << std::endl;
    interest_count = 0;
    Simulator::Schedule(Seconds(m_period), &InterestCounter::PrintNumArrivals, this);
}

} // namespace ndn
} // namespace ns3
