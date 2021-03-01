#ifndef CUSTOM_ROUTER_APP_H_
#define CUSTOM_ROUTER_APP_H_

#include "ns3/ndnSIM/apps/ndn-app.hpp"

namespace ns3 {

/**
 * @brief A simple custom application
 *
 * This applications demonstrates how to send Interests and respond with Datas to incoming interests
 *
 * When application starts it "sets interest filter" (install FIB entry) for /prefix/sub, as well as
 * sends Interest for this prefix
 *
 * When an Interest is received, it is replied with a Data with 1024-byte fake payload
 */
class CustomRouterApp : public ndn::App {
public:
  // register NS-3 type "CustomRouterApp"
  static TypeId
  GetTypeId();

  // (overridden from ndn::App) Processing upon start of the application
  virtual void
  StartApplication();

  // (overridden from ndn::App) Processing when application is stopped
  virtual void
  StopApplication();

  virtual void
  OnInterest(std::shared_ptr<const ndn::Interest> interest);
  
private:
  int pitSize;
};

} // namespace ns3

#endif
