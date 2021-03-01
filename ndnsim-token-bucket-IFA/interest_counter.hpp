#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ndn-app.hpp"
#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/double.h"

#ifndef NDN_INTEREST_COUNTER_H
#define NDN_INTEREST_COUNTER_H

namespace ns3 {
namespace ndn {

class InterestCounter : public App {
public:
  static TypeId
  GetTypeId(void);

  InterestCounter();

  // inherited from NdnApp
  virtual void
  OnInterest(shared_ptr<const Interest> interest);

  virtual void
  PrintNumArrivals();

protected:
  // inherited from Application base class.
  virtual void
  StartApplication(); // Called at time specified by Start

  virtual void
  StopApplication(); // Called at time specified by Stop

private:
  Name m_prefix;
  double m_period;
  uint64_t interest_count;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_INTEREST_COUNTER_H
