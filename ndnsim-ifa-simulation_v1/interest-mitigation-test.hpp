/* interest-mitigation-test.hpp */

#ifndef INTEREST_MITIGATION_H_
#define INTEREST_MITIGATION_H_

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/nstime.h"
#include "ns3/ptr.h"

namespace ns3 {
namespace ndn {

class InterestMitigation : public App {
public:
  static TypeId GetTypeId(void);
  InterestMitigation();
  // inherited from App
  virtual void OnStart(void);
  virtual void IfaDetect(void);
  virtual bool IfaCheck(void);
  virtual std::string IfaFindPrefix(void);
  virtual void SendIfaDetectionMessage(std::string badPrefix);
  virtual void OnInterest(shared_ptr<Face> face, shared_ptr<const Interest> interest);
};

} // namespace ndn
} // namespace ns3

#endif // INTEREST_MITIGATION_H_
