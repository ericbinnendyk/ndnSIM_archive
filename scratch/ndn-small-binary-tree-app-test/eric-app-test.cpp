#include "eric-app-test.hpp"

// I don't know if I need all of these
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("EricApp");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(EricApp);
TypeId EricApp::GetTypeId()
{
    static TypeId tid = TypeId("EricApp").SetParent<ndn::App>().AddConstructor<EricApp>();
    return tid;
}

void EricApp::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{
    // first do the default behavior of the app when it receives an interest
    ndn::App::OnInterest(interest);

    std::cerr << "We just received an interest named " << interest->getName() << std::endl;
}

void EricApp::OnData(std::shared_ptr<const ndn::Data> data)
{
    // first do the default behavior of the app when it receives a data
    ndn::App::OnData(data);

    std::cerr << "We just received a data named " << data->getName() << std::endl;
}

}
