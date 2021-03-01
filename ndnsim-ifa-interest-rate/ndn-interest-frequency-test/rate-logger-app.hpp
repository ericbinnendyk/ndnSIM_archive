// eric-app-test.hpp
// header file for eric-app-test.cpp

#ifndef RATE_LOGGER_APP_H_
#define RATE_LOGGER_APP_H_

#include <string>
#include <map>
#include "ns3/ndnSIM/apps/ndn-app.hpp"

namespace ns3 {

class RateLoggerApp : public ndn::App {

    public:
    static TypeId GetTypeId();

    virtual void StartApplication();
    virtual void OnInterest(std::shared_ptr<const ndn::Interest> interest) override;
    void CheckPit();
};

}

#endif
