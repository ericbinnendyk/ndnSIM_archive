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

// custom-app.cpp

#include "custom-router-app.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("CustomRouterApp");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(CustomRouterApp);

// register NS-3 type
TypeId
CustomRouterApp::GetTypeId()
{
  static TypeId tid = TypeId("CustomRouterApp").SetParent<ndn::App>().AddConstructor<CustomRouterApp>();
  return tid;
}

// Processing upon start of the application
void
CustomRouterApp::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();
}

// Processing when application is stopped
void
CustomRouterApp::StopApplication()
{
  // cleanup ndn::App
  ndn::App::StopApplication();
}

// Callback that will be called when Interest arrives
void
CustomRouterApp::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{
    // do default behavior first
    ndn::App::OnInterest(interest);
    // now also implement PIT
    // however, I don't know how to get to the face associated with each node (or even the node associated with an app)
    // Update: This is no longer true, I found a pointer m_face
    if (m_face->getNPendingInterests() >= this.pitSize) {
        Ip::Nack nack;
        ndn::Face::put(nack); // Send a nack.
        // This command is meant to send a NACK to the node who requested data because the PIT is full, but I'm not sure if this is how to do it.
    }
    // another possible way to set a max PIT size. I don't know how to get the PIT associated with each node. It should be in class nfd::pit::Pit as described here: https://ndnsim.net/current/doxygen/classnfd_1_1pit_1_1Pit.html
    if (m_face->pit.size() >= this.pitSize) {
        node->face->pit.erase(node->face->pit.find(interest));
    }
}

} // namespace ns3
