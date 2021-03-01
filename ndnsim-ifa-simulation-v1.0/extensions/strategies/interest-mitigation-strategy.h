#ifndef SATISFACTION_BASED_INTEREST_ACCEPTANCE_H
#define SATISFACTION_BASED_INTEREST_ACCEPTANCE_H

//#include "ns3/random-variable.h"
#include "ns3/double.h"
#include "ns3/ndn-fib-entry.h"
#include "ns3/ndn-pit-entry.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-data.h"

#include "ns3/ndnSIM/model/fw/best-route.h"

#include "stats.h"

#include <algorithm>

namespace ns3 {
namespace ndn {
namespace fw {

// this is to be inherited from the BestRoute strategy located at src/ndnSIM/model/fw/best-route.cc
// all prefixes should follow this forwarding strategy
class InterestMitigationStrategy :
    public BestRoute
{
public:
  typedef BestRoute super;
  
  static TypeId GetTypeId ();

  static std::string GetLogName ();

  /**
   * @brief Default constructor
   */
  InterestMitigationStrategy ()
  {
    Simulator::Schedule(Seconds(1.0), &InterestMitigationStrategy::IfaDetect, this);
  }

protected:
  bool
  CanSendOutInterest (Ptr<Face> inFace,
                      Ptr<Face> outFace,
                      Ptr<const Interest> interest,
                      Ptr<pit::Entry> pitEntry);
  void DidReceiveUnsolicitedData(Ptr< Face > inFace, Ptr< const Data > data, bool didCreateCacheEntry);
  void ClearStats();
  double GiniImpurity(std::map<std::string, uint64_t> interestCounts);
  std::string IfaFindPrefix();
  bool IfaCheck();
  void IfaDetect();
  void SendWarning(std::string badPrefix);

  
private:
  static LogComponent g_log;
  
  double m_threshold;
  std::map<std::string, uint64_t> interest_counts;
  std::list<std::string> bad_prefixes;
  std::list<Ptr<Face>> arrival_faces; // list of faces on which interests have entered in the past
  double gi_curr;
  double gi_prev;
};

TypeId InterestMitigationStrategy::GetTypeId (void)
{
  static TypeId tid = TypeId ((super::GetTypeId ().GetName ()+"::InterestMitigationStrategy").c_str ())
    .SetGroupName ("Ndn")
    .template SetParent <super> ()
    .template AddConstructor <InterestMitigationStrategy> ()
    .AddAttribute ("Threshold", "Threshold of Gini impurity increase above which IFA is detected",
                            DoubleValue (0.001),
                            MakeDoubleAccessor (&InterestMitigationStrategy::m_threshold),
                            MakeDoubleChecker<double> ());
  return tid;
}

std::string InterestMitigationStrategy::GetLogName ()
{
  return super::GetLogName () + ".InterestMitigationStrategy";
}

bool InterestMitigationStrategy::CanSendOutInterest (Ptr<Face> inFace,
                                                                 Ptr<Face> outFace,
                                                                 Ptr<const Interest> interest,
                                                                 Ptr<pit::Entry> pitEntry)
{
  std::string init_prefix = interest->GetName().getPrefix(1).toUri();

    if (this->interest_counts.count(interest->GetName().toUri()) == 0) {
        // add interest
        this->interest_counts[interest->GetName().toUri()] = 1;
    }
    else {
        this->interest_counts[interest->GetName().toUri()]++;
    }
    if (std::find(arrival_faces.begin(), arrival_faces.end(), inFace) == arrival_faces.end()) {
      this->arrival_faces.push_back(inFace);
    }

  if (std::find(bad_prefixes.begin(), bad_prefixes.end(), init_prefix) != bad_prefixes.end()) {
      return false;
  }

  // should be PerOutFaceLimits
  return super::CanSendOutInterest (inFace, outFace, interest, pitEntry);
}

void InterestMitigationStrategy::ClearStats()
{
    this->interest_counts = std::map<std::string, uint64_t>();
}

double InterestMitigationStrategy::GiniImpurity(std::map<std::string, uint64_t> interestCounts)
{
    uint64_t totalInterests = 0;
    auto increaseTotal = [&totalInterests](std::pair<std::string, uint64_t> pair) { totalInterests += pair.second; };
    std::for_each(interestCounts.begin(), interestCounts.end(), increaseTotal);
    double value = 1.0;
    auto decreaseValue = [totalInterests, &value](std::pair<std::string, uint64_t> pair) {
        double freq = ((double) pair.second)/totalInterests;
        value -= freq*freq;
    };
    std::for_each(interestCounts.begin(), interestCounts.end(), decreaseValue);
    return value;
}

std::string InterestMitigationStrategy::IfaFindPrefix()
{
    // to do: implement malicious prefix detection algorithm
    return "/forged";
}

bool InterestMitigationStrategy::IfaCheck()
{
    this->gi_prev = this->gi_curr;
    this->gi_curr = GiniImpurity(this->interest_counts);
    if (this->gi_prev == 0) {
        // not enough information, too early
        return false;
    }
    double relative_increase = (this->gi_curr - this->gi_prev)/this->gi_prev;
    //std::cout << "The relative increase is " << relative_increase << std::endl;
    return (relative_increase > m_threshold);
}

void InterestMitigationStrategy::IfaDetect()
{
    if (IfaCheck()) {
        std::string badPrefix = IfaFindPrefix();
        if (std::find(bad_prefixes.begin(), bad_prefixes.end(), badPrefix) == bad_prefixes.end()) {
            bad_prefixes.push_back(badPrefix);
            SendWarning(badPrefix);
        }
    }
    ClearStats();
    Simulator::Schedule(Seconds(1.0), &InterestMitigationStrategy::IfaDetect, this);
}

void InterestMitigationStrategy::SendWarning(std::string badPrefix) {
  Ptr<Data> data = Create<Data> (Create<Packet> (1024));
  // give it an empty name so it can be recognized by anyone
  Ptr<Name> dataName = Create<Name> (Name("/"));
  data->SetName (dataName);

  data->SetSignature (0); // the producer also has the signature 0

  /*if (m_keyLocator.size () > 0)
    {
      data->SetKeyLocator (Create<Name> (m_keyLocator));
    }*/

  char buffer[1024];
  snprintf(buffer, 1024, "warning");
  // the Create function is a wrapper around new that handles the reference counting system.
  Ptr<Packet> packet = Create<Packet>(Packet((const uint8_t*) buffer, 1024));
  data->SetPayload(packet);

  for (Ptr<fib::Entry> entry = m_fib->Begin(); entry != m_fib->End(); entry = m_fib->Next(entry)) {
    //const fib::FaceMetric &metricFace, pitEntry->GetFibEntry ()->m_faces.get<fib::i_metric> ();
    /*std::for_each (entry.m_faces.get<fib::i_metric>.begin(), entry.m_faces.get<fib::i_metric>.end(), [data](const fib::FaceMetric &face_metric){
      face_metric.GetFace()->ReceiveData(data);
      //m_transmittedDatas (data, this, face);
    });*/
    BOOST_FOREACH (const fib::FaceMetric &metricFace, entry->m_faces.get<fib::i_metric> ())
    {
      metricFace.GetFace()->ReceiveData(data);
    }
  };
  for (Ptr<Face> face : arrival_faces) {
    face->ReceiveData(data);
  }
}

void InterestMitigationStrategy::DidReceiveUnsolicitedData(Ptr< Face > inFace, Ptr< const Data > data, bool didCreateCacheEntry) {
  if (data->GetName() == Name("/")) {
    // warn neighbors
    if (std::find(bad_prefixes.begin(), bad_prefixes.end(), "/forged") == bad_prefixes.end()) {
        bad_prefixes.push_back("/forged");
        SendWarning("/forged");
    }
  }
  else {
    super::DidReceiveUnsolicitedData(inFace, data, didCreateCacheEntry);
  }
}

} // namespace fw
} // namespace ndn
} // namespace ns3

#endif // SATISFACTION_BASED_INTEREST_ACCEPTANCE_H
