// ndn-small-binary-tree.cc
// This program simulates an interest flooding attack (IFA) on a network with a binary tree topology described in topo-small-binary-tree.txt
// The legitimate Interest requests follow a Zipf-Mandelbrot distribution with parameters q = 0.7 and s = 0.7 while the malicious requests are all distinct.
// A binary tree topology is one of the worst cases of suffering from an IFA, because all of the interests are forwarded to the same provider at the root of the tree.

// I was trying to install a custom app, "EricApp" on one of the routing nodes so that the names of the interests were printed. However, the function EricApp::OnInterest does not seem to be triggered when an interest arrives even though the actual data flow through the network seems to be working fine. (This is also a problem with the sample custom app scenario, ndn-custom-app.cpp.)
// I also could not set the PIT size or PIT timeout period as described in the paper. I simulated the latter by giving a lifetime to the malicious interests themselves.

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"

#include "ndn-cxx/interest.hpp"

namespace ns3 {

int main(int argc, char *argv[])
{
    // I got a message telling me to set this value when I ran my simulation
    ndn::Interest::setDefaultCanBePrefix(true);

    // parse command-line arguments
    CommandLine cmd;
    cmd.Parse(argc, argv);

    // read network topology from file
    // read from topo-small-binary-tree.txt with scaling 25
    AnnotatedTopologyReader topologyReader("", 25);
    topologyReader.SetFileName("scratch/topo-small-binary-tree.txt");
    topologyReader.Read();
    
    PointToPointHelper p2p;
    p2p.Install(Names::Find<Node>("NodeC2"), Names::Find<Node>("NodeR5"));

    // Install NDN stack on each node
    ndn::StackHelper ndnHelper;
    ndnHelper.InstallAll();

    // Choose forwarding strategy
    ndn::StrategyChoiceHelper::InstallAll("/line/fine", "/localhost/nfd/strategy/multicast");

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
    ndn::AppHelper consumerHelper("ns3::ndn::ConsumerZipfMandelbrot");
    consumerHelper.SetAttribute("NumberOfContents", StringValue("5000"));
    consumerHelper.SetAttribute("q", DoubleValue(0.7));
    consumerHelper.SetAttribute("s", DoubleValue(0.7));
    consumerHelper.SetPrefix("/line/fine");
    consumerHelper.SetAttribute("Frequency", StringValue("50"));

    // install application on three of four consumer nodes
    consumerHelper.Install(consumerNode2);
    consumerHelper.Install(consumerNode3);
    consumerHelper.Install(consumerNode4);

    // create application that requests 100 distinct malicious (unsatisfiable) interests per second from /line/forged
    ndn::AppHelper consumerHelper1("ns3::ndn::ConsumerCbr");
    consumerHelper1.SetPrefix("/line/forged");
    consumerHelper1.SetAttribute("Frequency", StringValue("100"));
    // The paper describes the interests as lasting 1 second in the PIT before the PIT lifetime expires, but I cannot find a way to set the lifetime of interests in the PIT so I am simulating this scenario by setting the lifetime of the interests themselves to be 1 second
    consumerHelper1.SetAttribute("LifeTime", TimeValue(Seconds(1.0)));
    auto icnAttack = consumerHelper1.Install(consumerNode1);

    // attack happens between 100 and 200 seconds
    icnAttack.Start(Seconds(100.0));
    icnAttack.Stop(Seconds(200.0));
    
    // install producer to respond with 1024 bits for each data response
    // producer recognizes responses from /line/fine
    ndn::AppHelper producerHelper("ns3::ndn::Producer");
    producerHelper.SetPrefix("/line/fine");
    producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
    producerHelper.Install(producer);
    
    // This custom app is supposed to print the names of all interests going into node R1, but it doesn't seem to work properly.
    ndn::AppHelper app1("EricApp");
    app1.Install(Names::Find<Node>("NodeR1"));
    
    // Add /prefix origins to ndn::GlobalRouter
    ndnGlobalRoutingHelper.AddOrigins("/line/fine", producer);

    // Calculate and install FIBs
    ndn::GlobalRoutingHelper::CalculateRoutes();

    // simulation time is 300 seconds
    Simulator::Stop(Seconds(10.0));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}

}

int main(int argc, char* argv[])
{
    return ns3::main(argc, argv);
}
