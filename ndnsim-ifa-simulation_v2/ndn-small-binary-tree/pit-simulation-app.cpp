/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "pit-simulation-app.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("PitSimulationApp");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(PitSimulationApp);

// register NS-3 type
TypeId
PitSimulationApp::GetTypeId()
{
  static TypeId tid = TypeId("PitSimulationApp").SetParent<ndn::App>().AddConstructor<PitSimulationApp>();
  return tid;
}

// Processing upon start of the application
void
PitSimulationApp::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();
    ndn::FibHelper::AddRoute(GetNode(), "/line/fine", m_face, 0);
    ndn::FibHelper::AddRoute(GetNode(), "/line/forged", m_face, 0);
}

// Processing when application is stopped
void
PitSimulationApp::StopApplication()
{
  // cleanup ndn::App
  ndn::App::StopApplication();
}

// Callback that will be called when Interest arrives
void
PitSimulationApp::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{
    // do default behavior first
    ndn::App::OnInterest(interest);

    nfd::pit::Pit& the_pit = GetNode()->GetObject<ndn::L3Protocol>()->getForwarder()->getPit();
    // std::cout << "Pit size: " << the_pit.size() << std::endl;
    if (the_pit.size() >= 100) {
        if (the_pit.find(*interest) != nullptr)
            the_pit.erase(the_pit.find(*interest).get());
    }
}

} // namespace ns3
