/* producer-logger.cpp */
/* inherits from producer class */
/* prints name of each interest reaching producer */

#include "producer-logger.hpp"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

#include "model/ndn-l3-protocol.hpp"
#include "helper/ndn-fib-helper.hpp"

#include <memory>

NS_LOG_COMPONENT_DEFINE("ProducerLogger");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(ProducerLogger);

TypeId
ProducerLogger::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ProducerLogger")
      .SetGroupName("Ndn")
      .SetParent<Producer>()
      .AddConstructor<ProducerLogger>();

  return tid;
}

ProducerLogger::ProducerLogger()
{
  NS_LOG_FUNCTION_NOARGS();
}

void
ProducerLogger::OnInterest(shared_ptr<const Interest> interest)
{
  // prints the number of seconds in the simulation
  // I got this code from a macro in log-macros-enabled.h
  ns3::TimePrinter printer = ns3::LogGetTimePrinter ();
  if (printer != 0)
    {
      (*printer)(std::cout);
      std::cout << " ";
    }

  std::cout << "receiving interest " << interest->getName() << std::endl;
  Producer::OnInterest(interest); // tracing inside
}

} // namespace ndn
} // namespace ns3
