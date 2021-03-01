These are the numbers of interests of each type reaching the producer per 1/10 sec with and without an IFA, on the simulation ndnQoS-with-IFA.

All of these metrics are obtained with 500 interests per second in the IFA and with the following settings:

       Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
       Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
       Config::SetDefault("ns3::QueueBase::MaxSize", StringValue("20p"));

The IFA is /prefix/forged, which means the producer can respond to it.
