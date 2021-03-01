// NOTE: On my computer, this file will only run if it is in the directory src/ndnSIM/examples/ (not scratch/), the ndn-small-binary-tree/ directory is in the same location, and the file topo-small-binary-tree.txt is in the src/ndnSIM/examples/topologies/ directory.

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"

#include "ns3/ndnSIM/NFD/daemon/fw/expired-pit.hpp"

#include "ndn-cxx/interest.hpp"

namespace ns3 {

int main(int argc, char *argv[])
{
    // I got a message telling me to set this value when I ran my simulation
    ndn::Interest::setDefaultCanBePrefix(true);
    // Apparently, I have to set this value early in my code for the visualizer to work
    ns3::PacketMetadata::Enable ();

    // setting default parameters for PointToPoint links and channels
    Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
    Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
    Config::SetDefault("ns3::QueueBase::MaxSize", StringValue("20p"));

    // parse command-line arguments
    CommandLine cmd;
    cmd.Parse(argc, argv);

    // read network topology from file
    // read from topo-small-binary-tree.txt with scaling 25
    AnnotatedTopologyReader topologyReader("", 25);
    topologyReader.SetFileName("src/ndnSIM/examples/topologies/topo-small-binary-tree.txt");
    topologyReader.Read();

    // Install NDN stack on each node
    ndn::StackHelper ndnHelper;
    ndnHelper.SetDefaultRoutes(true);
    ndnHelper.InstallAll();

    // Choose forwarding strategy
    // Apparently you're supposed to install custom strategies by using an InstallAll template with the type name
    // (see here: https://ndnsim.net/current/doxygen/classns3_1_1ndn_1_1StrategyChoiceHelper.html#a726f392b22b0acb1e450e21918240e50)
    // For me it works even if I just use the InstallAll function, though, probably because I'm writing my custom strategies in
    // the same directory as the official strategies.
    // Still, I have decided to use the template method.
    ndn::StrategyChoiceHelper::InstallAll("/nodec1", "/localhost/nfd/strategy/multicast");
    ndn::StrategyChoiceHelper::InstallAll("/nodec2", "/localhost/nfd/strategy/multicast");
    ndn::StrategyChoiceHelper::InstallAll("/nodec3", "/localhost/nfd/strategy/multicast");
    ndn::StrategyChoiceHelper::InstallAll("/nodec4", "/localhost/nfd/strategy/multicast");
    // My ExpiredPit strategy is unfinished but this should work after it's finished
    // Installing the stategy on NodeR4 for all interests. Hopefully this creates a single strategy object for all prefixes.
    // When completed, this stategy will be installed on all four peripheral routers.
    ndn::StrategyChoiceHelper::Install<nfd::fw::ExpiredPit>(Names::Find<Node>("NodeR4"), "/nodec1");
    ndn::StrategyChoiceHelper::Install<nfd::fw::ExpiredPit>(Names::Find<Node>("NodeR4"), "/nodec2");
    ndn::StrategyChoiceHelper::Install<nfd::fw::ExpiredPit>(Names::Find<Node>("NodeR4"), "/nodec3");
    ndn::StrategyChoiceHelper::Install<nfd::fw::ExpiredPit>(Names::Find<Node>("NodeR4"), "/nodec4");

    // Install global routing interface on each node
    // populate FIB
    ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
    ndnGlobalRoutingHelper.InstallAll();

    Ptr<Node> consumerNode1 = Names::Find<Node>("NodeC1");
    Ptr<Node> consumerNode2 = Names::Find<Node>("NodeC2");
    Ptr<Node> consumerNode3 = Names::Find<Node>("NodeC3");
    Ptr<Node> consumerNode4 = Names::Find<Node>("NodeC4");
    Ptr<Node> producer = Names::Find<Node>("NodeP1");

    // Use custom subclass of consumer helper for Zipf-Mandelbrot distributions
    // create application that requests up to 5000 distinct interests in Zipf-Mandelbrot distribution with parameters q = 0.7 and s = 0.7
    // under prefix /line/fine and at a rate of 50 interests/sec
    ndn::AppHelper consumerHelper1("ns3::ndn::ConsumerZipfMandelbrot");
    consumerHelper1.SetAttribute("NumberOfContents", StringValue("5000"));
    consumerHelper1.SetAttribute("q", DoubleValue(0.7));
    consumerHelper1.SetAttribute("s", DoubleValue(0.7));
    consumerHelper1.SetPrefix("/nodec2");
    // With a frequency value of 50, some packets are dropped at the producer even before the attack starts.
    // This can be changed by lowering the frequency of interests to, say, 20, or by lowering the producer payload size to something as low as 100. However, a frequency value of 50 interests/second matches what the paper gives.
    consumerHelper1.SetAttribute("Frequency", StringValue("50"));

    // install application on three of four consumer nodes
    consumerHelper1.Install(consumerNode2);

    ndn::AppHelper consumerHelper2("ns3::ndn::ConsumerZipfMandelbrot");
    consumerHelper2.SetAttribute("NumberOfContents", StringValue("5000"));
    consumerHelper2.SetAttribute("q", DoubleValue(0.7));
    consumerHelper2.SetAttribute("s", DoubleValue(0.7));
    consumerHelper2.SetPrefix("/nodec3");
    // With a frequency value of 50, some packets are dropped at the producer even before the attack starts.
    // This can be changed by lowering the frequency of interests to, say, 20, or by lowering the producer payload size to something as low as 100. However, a frequency value of 50 interests/second matches what the paper gives.
    consumerHelper2.SetAttribute("Frequency", StringValue("50"));

    // install application on three of four consumer nodes
    consumerHelper2.Install(consumerNode3);

    ndn::AppHelper consumerHelper3("ns3::ndn::ConsumerZipfMandelbrot");
    consumerHelper3.SetAttribute("NumberOfContents", StringValue("5000"));
    consumerHelper3.SetAttribute("q", DoubleValue(0.7));
    consumerHelper3.SetAttribute("s", DoubleValue(0.7));
    consumerHelper3.SetPrefix("/nodec4");
    // With a frequency value of 50, some packets are dropped at the producer even before the attack starts.
    // This can be changed by lowering the frequency of interests to, say, 20, or by lowering the producer payload size to something as low as 100. However, a frequency value of 50 interests/second matches what the paper gives.
    consumerHelper3.SetAttribute("Frequency", StringValue("50"));

    // install application on three of four consumer nodes
    consumerHelper3.Install(consumerNode4);

    // create application that requests 100 distinct malicious (unsatisfiable) interests per second from /nodec1
    ndn::AppHelper consumerHelper4("ns3::ndn::ConsumerCbr");
    consumerHelper4.SetPrefix("/nodec1");
    consumerHelper4.SetAttribute("Frequency", StringValue("100"));
    auto icnAttack = consumerHelper4.Install(consumerNode1);

    // attack happens between 10 and 20 seconds
    icnAttack.Start(Seconds(100.0));
    icnAttack.Stop(Seconds(200.0));

    // install producer to respond with 1024 bits for each data response
    // producer recognizes responses from /nodec1, /nodec2, /nodec3
    ndn::AppHelper producerHelper1("ns3::ndn::Producer");
    producerHelper1.SetPrefix("/nodec2");
    producerHelper1.SetAttribute("PayloadSize", StringValue("1024"));
    producerHelper1.Install(producer);
    ndn::AppHelper producerHelper2("ns3::ndn::Producer");
    producerHelper2.SetPrefix("/nodec3");
    producerHelper2.SetAttribute("PayloadSize", StringValue("1024"));
    producerHelper2.Install(producer);
    ndn::AppHelper producerHelper3("ns3::ndn::Producer");
    producerHelper3.SetPrefix("/nodec4");
    producerHelper3.SetAttribute("PayloadSize", StringValue("1024"));
    producerHelper3.Install(producer);

    // Add /prefix origins to ndn::GlobalRouter
    ndnGlobalRoutingHelper.AddOrigins("/nodec2", producer);
    ndnGlobalRoutingHelper.AddOrigins("/nodec3", producer);
    ndnGlobalRoutingHelper.AddOrigins("/nodec4", producer);

    // Calculate and install FIBs
    ndn::GlobalRoutingHelper::CalculateRoutes();

    // simulation time is 300 seconds
    Simulator::Stop(Seconds(300.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}

}

int main(int argc, char* argv[])
{
    return ns3::main(argc, argv);
}
