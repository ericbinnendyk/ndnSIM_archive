// Simulates an IFA with settings similar to the testing scenario described in the paper:
// iCAAP: information-Centric network Architecture for Application-specific Prioritization in Smart Grid

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {
    int
    main(int argc, char* argv[])
    {
        bool use_token_bucket = /*(argc > 1 && argv[1][0] == 't');*/ false;
       // setting default parameters for PointToPoint links and channels
       Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
       Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
       Config::SetDefault("ns3::QueueBase::MaxSize", StringValue("20p"));

       // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
       CommandLine cmd;
       cmd.Parse(argc, argv);

       // Creating nodes
       NodeContainer nodes;
       nodes.Create(6);

       // Connecting nodes using two links
       PointToPointHelper p2p;
       p2p.Install(nodes.Get(0), nodes.Get(3));
       p2p.Install(nodes.Get(1), nodes.Get(3));
       p2p.Install(nodes.Get(2), nodes.Get(3));
       p2p.Install(nodes.Get(3), nodes.Get(4));
       p2p.Install(nodes.Get(5), nodes.Get(3));

       // Install NDN stack on all nodes
       ndn::StackHelper ndnHelper;
       ndnHelper.SetDefaultRoutes(true);
       ndnHelper.InstallAll();

       // Setup dynamic routing
       ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
       ndnGlobalRoutingHelper.Install(nodes);

       // Choosing forwarding strategy
       ndn::StrategyChoiceHelper::Install(nodes.Get(0), "/prefix", "/localhost/nfd/strategy/multicast");
       ndn::StrategyChoiceHelper::Install(nodes.Get(1), "/prefix", "/localhost/nfd/strategy/multicast");
       ndn::StrategyChoiceHelper::Install(nodes.Get(2), "/prefix", "/localhost/nfd/strategy/multicast");
       if (use_token_bucket) {
           ndn::StrategyChoiceHelper::Install(nodes.Get(3), "/prefix", "/localhost/nfd/strategy/qos");
       }
       else {
           ndn::StrategyChoiceHelper::Install(nodes.Get(3), "/prefix", "/localhost/nfd/strategy/multicast");
       }
       ndn::StrategyChoiceHelper::Install(nodes.Get(4), "/prefix", "/localhost/nfd/strategy/multicast");
       ndn::StrategyChoiceHelper::Install(nodes.Get(5), "/prefix", "/localhost/nfd/strategy/multicast");

       // Installing applications

       // Consumer
       // All three legitimate consumers 
       ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
       // High priority Consumer 
       consumerHelper.SetPrefix("/prefix/typeI");
       consumerHelper.SetAttribute("Frequency", StringValue("90")); // 90 interests a second
       auto apps = consumerHelper.Install(nodes.Get(0));

       // Midium priority Consumer 
       consumerHelper.SetPrefix("/prefix/typeII");
       consumerHelper.SetAttribute("Frequency", StringValue("150")); // 150 interests a second
       apps = consumerHelper.Install(nodes.Get(1));

       // Low priortiy consumer
       consumerHelper.SetPrefix("/prefix/typeIII");
       consumerHelper.SetAttribute("Frequency", StringValue("300")); // 300 interests a second
       apps = consumerHelper.Install(nodes.Get(2));

       // IFA 
       consumerHelper.SetPrefix("/prefix/forged");
       consumerHelper.SetAttribute("Frequency", StringValue("1000")); // 10 interests a second
       apps = consumerHelper.Install(nodes.Get(5));

       if (use_token_bucket) {
           //Set token bucket values
           ndn::AppHelper tokenHelper("ns3::ndn::TokenBucketDriver");
           tokenHelper.SetAttribute("FillRate1", StringValue("2500"));
           tokenHelper.SetAttribute("Capacity1", StringValue("1000"));
           tokenHelper.SetAttribute("FillRate2", StringValue("2000"));
           tokenHelper.SetAttribute("Capacity2", StringValue("1000"));
           tokenHelper.SetAttribute("FillRate3", StringValue("800"));
           tokenHelper.SetAttribute("Capacity3", StringValue("1000"));
           //consumerHelper.Install(nodes.Get(0));                        // first node
           apps = tokenHelper.Install(nodes.Get(3));
      }

       // Producer
       ndn::AppHelper producerHelper("ns3::ndn::Producer");
       // Producer will reply to all requests starting with /prefix
       producerHelper.SetPrefix("/prefix");
       producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
       producerHelper.Install(nodes.Get(4)); // last node

       // App that counts the number of malicious interests reaching the producer
       ndn::AppHelper interestCounterHelper("ns3::ndn::InterestCounter");
       interestCounterHelper.SetPrefix("/prefix/typeIII");
       interestCounterHelper.SetAttribute("Period", DoubleValue(0.1));
       interestCounterHelper.Install(nodes.Get(4));

       ndnGlobalRoutingHelper.AddOrigin("/prefix", nodes.Get(4));
       ndn::GlobalRoutingHelper::CalculateAllPossibleRoutes();

       Simulator::Stop(Seconds(10.0));
       Simulator::Run();
       Simulator::Destroy();

       return 0;
   }

} // namespace ns3

    int
main(int argc, char* argv[])
{
    std::cout << argc << std::endl;
    return ns3::main(argc, argv);
}
