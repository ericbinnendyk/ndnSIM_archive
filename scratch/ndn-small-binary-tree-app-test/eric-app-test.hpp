// eric-app-test.hpp

#ifndef CUSTOM_APP_H_
#define CUSTOM_APP_H_

#include "ns3/ndnSIM/apps/ndn-app.hpp"

namespace ns3 {

class EricApp : public ndn::App {

    public:
    static TypeId GetTypeId();

    virtual void OnInterest(std::shared_ptr<const ndn::Interest> interest);
    virtual void OnData(std::shared_ptr<const ndn::Data> data);
};

}

#endif
