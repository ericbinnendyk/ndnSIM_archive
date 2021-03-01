// eric-app-test.hpp
// header file for eric-app-test.cpp

#ifndef CUSTOM_APP_H_
#define CUSTOM_APP_H_

#include "ns3/ndnSIM/apps/ndn-app.hpp"

namespace ns3 {

class EricApp : public ndn::App {

    public:
    static TypeId GetTypeId();

    virtual void StartApplication();
    virtual void OnInterest(std::shared_ptr<const ndn::Interest> interest) override;
    virtual void OnData(std::shared_ptr<const ndn::Data> data) override;
};

}

#endif
