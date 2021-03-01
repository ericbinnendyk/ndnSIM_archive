#include "pit-size-recorder.h"
#include "ns3/point-to-point-module.h"

// I don't know if I need all of these
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/ndn-fib.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.h"

#include "ns3/random-variable-stream.h"

//#include "ns3/ndnSIM/ndn-cxx/util/time.hpp"

NS_LOG_COMPONENT_DEFINE("PitSizeRecorder");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(PitSizeRecorder);
TypeId PitSizeRecorder::GetTypeId()
{
    static TypeId tid = TypeId("PitSizeRecorder").SetParent<App>().AddConstructor<PitSizeRecorder>();
    return tid;
}

void PitSizeRecorder::StartApplication()
{
    App::StartApplication();
  Ptr<ndn::Fib> fib = GetNode ()->GetObject<ndn::Fib> ();

  Ptr<ndn::fib::Entry> fibEntry1 = fib->Add (ndn::Name("/fine"), m_face, 0);
  Ptr<ndn::fib::Entry> fibEntry2 = fib->Add (ndn::Name("/forged"), m_face, 0);

    this->numSecs = 0;
    Simulator::Schedule(Seconds(1.0), &PitSizeRecorder::PrintPitSize, this);
}

void PitSizeRecorder::PrintPitSize()
{
    this->numSecs += 1;
    std::cout << this->numSecs << "\t" << GetNode()->GetObject<ndn::L3Protocol>()->getForwarder()->getPit().size() << std::endl;
    Simulator::Schedule(Seconds(1.0), &PitSizeRecorder::PrintPitSize, this);
}
}