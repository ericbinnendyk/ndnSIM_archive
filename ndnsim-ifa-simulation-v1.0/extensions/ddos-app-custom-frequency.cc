/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2012 PARC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ddos-app-custom-frequency.h"
#include <ns3/point-to-point-module.h>

NS_LOG_COMPONENT_DEFINE ("DdosAppCustomFrequency");

NS_OBJECT_ENSURE_REGISTERED (DdosAppCustomFrequency);

TypeId
DdosAppCustomFrequency::GetTypeId ()
{
  static TypeId tid = TypeId ("DdosAppCustomFrequency")
    .SetParent<App> ()
    .AddConstructor<DdosAppCustomFrequency> ()

    .AddAttribute ("AvgGap", "AverageGap",
		   StringValue ("1ms"),
                   MakeTimeAccessor (&DdosAppCustomFrequency::m_avgGap),
                   MakeTimeChecker ())
    .AddAttribute ("Prefix","Name of the Interest",
                   StringValue ("/"),
                   MakeNameAccessor (&DdosAppCustomFrequency::m_prefix),
                   MakeNameChecker ())
    .AddAttribute ("LifeTime","Interest lifetime",
                   StringValue ("1s"),
                   MakeTimeAccessor (&DdosAppCustomFrequency::m_lifetime),
                   MakeTimeChecker ());

  return tid;
}

DdosAppCustomFrequency::DdosAppCustomFrequency ()
  : m_rand (0, std::numeric_limits<uint32_t>::max ())
  , m_jitter (0,1)
  , m_seq (0)
{
}

DdosAppCustomFrequency::~DdosAppCustomFrequency ()
{
}

void
DdosAppCustomFrequency::OnNack (const Ptr<const Interest> &interest)
{
  // immediately send new packet, without taking into account periodicity
  // m_nextSendEvent.Cancel ();
  // m_nextSendEvent = Simulator::Schedule (Seconds (0.001),
  //       				 &DdosAppCustomFrequency::SendPacket, this);
}

void
DdosAppCustomFrequency::OnData (const Ptr<const Data> &data)
{
  // who cares
}

void
DdosAppCustomFrequency::SendPacket ()
{
  m_seq++;
  // send packet
  Ptr<NameComponents> nameWithSequence = Create<NameComponents> (m_prefix);
  nameWithSequence->appendSeqNum (m_seq);

  Ptr<Interest> interest = Create<Interest> ();
  interest->SetNonce            (m_rand.GetValue ());
  interest->SetName             (nameWithSequence);
  interest->SetInterestLifetime (m_lifetime);
        
  NS_LOG_INFO ("> Interest for " << m_seq << ", lifetime " << m_lifetime.ToDouble (Time::S) << "s");

  m_face->ReceiveInterest (interest);
  m_transmittedInterests (interest, this, m_face);

  // std::cout << "Size: " << packet->GetSize () << std::endl;
  
  // NS_LOG_DEBUG (m_avgGap+MilliSeconds (m_rand.GetValue ()));
  Time nextTime = m_avgGap + Time::FromDouble (m_jitter.GetValue (), Time::US);
  NS_LOG_DEBUG ("next time: " << nextTime.ToDouble (Time::S) << "s");
  m_nextSendEvent = Simulator::Schedule (nextTime,
        				 &DdosAppCustomFrequency::SendPacket, this);
}

void
DdosAppCustomFrequency::StartApplication ()
{
  App::StartApplication ();
  SendPacket ();
}

void
DdosAppCustomFrequency::StopApplication ()
{
  m_nextSendEvent.Cancel ();

  // std::cerr << "# references before delayed stop: " << m_face->GetReferenceCount () << std::endl;
  Simulator::Schedule (Seconds (10.0), &DdosAppCustomFrequency::DelayedStop, this);
}

void
DdosAppCustomFrequency::DelayedStop ()
{
  // std::cerr << "# references after delayed stop: " << m_face->GetReferenceCount () << std::endl;
  App::StopApplication ();
}
