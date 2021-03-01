// This app is supposed to log the names of all interest packets reaching node R1 as
// described in the topology file (the routing node at the root of the binary tree).
// Due to the topology of the network, all interests should reach this node.

#include "rate-logger-app.hpp"

// I don't know if I need all of these
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

//#include "ns3/ndnSIM/ndn-cxx/util/time.hpp"

NS_LOG_COMPONENT_DEFINE("RateLogger");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(RateLoggerApp);
TypeId RateLoggerApp::GetTypeId()
{
    static TypeId tid = TypeId("RateLogger").SetParent<ndn::App>().AddConstructor<RateLoggerApp>();
    return tid;
}

void RateLoggerApp::StartApplication()
{
    ndn::App::StartApplication();
    ndn::FibHelper::AddRoute(GetNode(), "/line/fine", m_face, 0);
    ndn::FibHelper::AddRoute(GetNode(), "/line/forged", m_face, 0);
}

void RateLoggerApp::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{
    // first do the default behavior of the app when it receives an interest
    ndn::App::OnInterest(interest);

    ns3::TimePrinter printer = ns3::LogGetTimePrinter ();
    std::cout << (int) Simulator::Now().GetSeconds() << '\t' << interest->getName() << std::endl;
}

// We want to add a function to find the Gini impurity.
// Here is the Python code for it:
/*
def gini_impurity(item_chances):
    value = 1.0
    for item in item_chances:
        value -= item[1]**2
    return value
*/
// First let me try scheduling a function for a specific time.
// Okay that worked

}
