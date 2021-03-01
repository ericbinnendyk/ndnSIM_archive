#include "ns3/core-module.h" //I'm guessing this includes Config, CommandLine, and Simulator
#include "ns3/network-module.h" //I'm guessing this includes NodeContainer
#include "ns3/point-to-point-module.h" //I'm guessing this includes PointToPointHelper
#include "ns3/ndnSIM-module.h" //I'm guessing this includes ndn namespace but I'm not sure

// apparently you need to put it inside the ns3 namespace
namespace ns3 {
int main(int argc, char* argv[]) {
    // set bandwidth to 10 Mb/s
    Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
    // set other default parameters
    Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
    Config::SetDefault("ns3::QueueBase::MaxSize", StringValue("10p"));

    CommandLine cmd;
    cmd.Parse(argc, argv);

    NodeContainer nodes;
    nodes.Create(5);
    
    // link four nodes using something or other whose bandwidth we set to 10 Mb/s
    PointToPointHelper p2p;
    p2p.Install(nodes.Get(0), nodes.Get(1));
    p2p.Install(nodes.Get(1), nodes.Get(2));
    p2p.Install(nodes.Get(2), nodes.Get(3));
    p2p.Install(nodes.Get(3), nodes.Get(4));
    p2p.Install(nodes.Get(4), nodes.Get(0));
    
    // install NDN stack or whatever
    ndn::StackHelper ndnHelper;
    // apparently we normally need to calculate the routes before we run the simulation except here we're setting the default routes.
    ndnHelper.SetDefaultRoutes(true);
    ndnHelper.InstallAll();

    // forwarding strategy, chosen for each prefix. Let's just use multicast.
    ndn::StrategyChoiceHelper::InstallAll("/prefix1", "/localhost/nfd/strategy/multicast");
    ndn::StrategyChoiceHelper::InstallAll("/prefix2", "/localhost/nfd/strategy/multicast");
    ndn::StrategyChoiceHelper::InstallAll("/prefix3", "/localhost/nfd/strategy/multicast");
    ndn::StrategyChoiceHelper::InstallAll("/prefix4", "/localhost/nfd/strategy/multicast");
    
    // install a consumer requesting prefixes 1, 2, 3, and 4 to one node
    ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
    // not sure how to set multiple prefixes or if this will work
    consumerHelper.SetPrefix("/prefix1");
    consumerHelper.SetAttribute("Frequency", StringValue("50"));
    auto apps1 = consumerHelper.Install(nodes.Get(0));
    consumerHelper.SetPrefix("/prefix2");
    auto apps2 = consumerHelper.Install(nodes.Get(0));

    // stop consumer app 1 after 1 second
    apps1.Stop(Seconds(1.0));
    // stop consumer app 2 after 1.5 seconds
    apps2.Stop(Seconds(1.5));
    // install a producer producing a single prefix on each of the other four nodes
    // I think we need four producer helpers for this?
    ndn::AppHelper producerHelper1("ns3::ndn::Producer");
    ndn::AppHelper producerHelper2("ns3::ndn::Producer");
    ndn::AppHelper producerHelper3("ns3::ndn::Producer");
    ndn::AppHelper producerHelper4("ns3::ndn::Producer");
    producerHelper1.SetPrefix("/prefix1");
    producerHelper2.SetPrefix("/prefix2");
    producerHelper3.SetPrefix("/prefix3");
    producerHelper4.SetPrefix("/prefix4");
    // is this PayloadSize the size of what the producer produces?
    producerHelper1.SetAttribute("PayloadSize", StringValue("1024"));
    producerHelper2.SetAttribute("PayloadSize", StringValue("1024"));
    producerHelper3.SetAttribute("PayloadSize", StringValue("1024"));
    producerHelper4.SetAttribute("PayloadSize", StringValue("1024"));
    producerHelper1.Install(nodes.Get(1));
    producerHelper2.Install(nodes.Get(2));
    producerHelper3.Install(nodes.Get(3));
    producerHelper4.Install(nodes.Get(4));

    Simulator::Stop(Seconds(2.0));
    
    Simulator::Run();
    Simulator::Destroy();
    
    return 0;
}
}

int main(int argc, char* argv[])
{
    return ns3::main(argc, argv);
}
