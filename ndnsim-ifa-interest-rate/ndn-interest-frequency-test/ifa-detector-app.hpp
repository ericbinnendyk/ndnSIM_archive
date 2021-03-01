// ifa-detector-app.hpp
// header file for ifa-detector-app.cpp

#ifndef IFA_DETECTOR_APP_H_
#define IFA_DETECTOR_APP_H_

#include <string>
#include <map>
#include "ns3/ndnSIM/apps/ndn-app.hpp"

namespace ns3 {

class IfaDetector : public ndn::App {

    public:
    static TypeId GetTypeId();

    virtual void StartApplication();
    virtual void OnInterest(std::shared_ptr<const ndn::Interest> interest) override;
    
    private:
    void ClearStats();
    bool IfaCheck();
    void SendIfaDetectionMessage(std::string badPrefix);
    void IfaDetect();
    double GiniImpurity(std::map<std::string, uint64_t> interestCounts);
    std::string IfaFindPrefix();
    std::map<std::string, uint64_t> interest_counts;
    double gi_curr;
    double gi_prev;
};

}

#endif
