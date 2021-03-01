// This app is supposed to log the names of all interest and data packets reaching node R1 as
// described in the topology file (the routing node at the root of the binary tree).
// Due to the topology of the network, all interests and data should reach this node.

// plan: first try calling OnInterest with your own interest, from StartApplication, which you know gets called.
// See if it prints. If so, try sending an interest to trigger some function or something. Not sure which one
// okay, OnInterest seems to be being called just fine when _I_ do it. Just not when it's done automatically.
// what if we program the app to send an interest (though this is a router node) and wait for data to come back. then when the data is received, we see if OnData is called.
// Okay, I can try to install the app on the producer node. That will narrow it down to one out of two possibilities. If it works on the producer node we know there's a problem with installing application on the router node. It it doesn't, then the problem is probably that my custom app is not overriding the default methods properly.
// Hmmm, seems it still doesn't work when I install it on the producer.
// Interesting, it looks like the ndn.Producer:OnInterest() call from the log file

// Tuesday, June 9
// Okay, we look at the built-in producer app class, and how that class inherits from the default app class. And why it works.
// I don't know. But it does indeed get called (I added my own print statements to the producer app class to show this.
// And at the end of the class code, it says: m_appLink->onReceiveData(*data); What does that do. What type is m_appLink?
// Okay I couldn't find much. Next I'm going to try to do text engineering of the output to gather statistics on tehe packets.
// I'm also going to try to install my app on the producer, because the producer app is definitely getting seen. Should be easier than installing on the router.
// And finally, I'm going to google my issues with the ndnSIM app.

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

void EricApp::StartApplication()
{
    ndn::App::StartApplication();
    ndn::FibHelper::AddRoute(GetNode(), "/line/fine", m_face, 0);
    ndn::FibHelper::AddRoute(GetNode(), "/line/forged", m_face, 0);
    std::cout << "Does _this_ get called automatically.\n";
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
