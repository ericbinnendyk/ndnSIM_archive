/* producer-logger.hpp */
/* inherits from producer class */

#ifndef PRODUCER_LOGGER_H
#define PRODUCER_LOGGER_H

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/ndnSIM/apps/ndn-producer.hpp"
#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/nstime.h"
#include "ns3/ptr.h"

namespace ns3 {
namespace ndn {

class ProducerLogger : public Producer {
public:
  static TypeId
  GetTypeId(void);

  ProducerLogger();

  // inherited from Producer
  virtual void
  OnInterest(shared_ptr<const Interest> interest);
};

} // namespace ndn
} // namespace ns3

#endif // PRODUCER_LOGGER_H
