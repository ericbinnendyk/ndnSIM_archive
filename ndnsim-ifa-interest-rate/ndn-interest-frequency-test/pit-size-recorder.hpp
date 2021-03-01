#ifndef PIT_SIZE_RECORDER_H_
#define PIT_SIZE_RECORDER_H_

#include <string>
#include <map>
#include "ns3/ndnSIM/apps/ndn-app.hpp"

namespace ns3 {

class PitSizeRecorder : public ndn::App {

    public:
    static TypeId GetTypeId();

    virtual void StartApplication();
    virtual void PrintPitSize();

    private:
    uint64_t numSecs;
};

}

#endif
