/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2013 University of California, Los Angeles
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

#include "load-stats.h"
#include "ns3/log.h"

// const double EXP_1  = (1-2.0/6.0);//exp (-1.0/5.0);  /* 1/exp(1sec/5sec) */
// const double EXP_2  = (1-2.0/31.0);//exp (-1.0/30.0); /* 1/exp(1sec/30sec) */
// const double EXP_3 = (1-2.0/61.0);//exp (-1.0/60.0); /* 1/exp(1sec/60sec) */

const double EXP_1 = exp (-1.0/5.0);  /* 1/exp(1sec/5sec) */
const double EXP_2 = exp (-1.0/30.0); /* 1/exp(1sec/30sec) */
const double EXP_3 = exp (-1.0/60.0); /* 1/exp(1sec/60sec) */

NS_LOG_COMPONENT_DEFINE ("ndn.LoadStats");

namespace ns3 {
namespace ndn {
namespace ndnSIM {

const double LoadStats::PRECISION = 0.0001;

LoadStats::LoadStats ()
  : counter_ (0)
  , zeroCount_ (0)
  , avg1_ (0)
  , avg2_ (0)
  , avg3_ (0)
  , m_currentSample (0)
{
  for (size_t i = 0; i < HISTORY_SAMPLES; i++)
    m_counters[i] = 0.0;
}

void
LoadStats::Step (double alpha) // alpha: weight for the new sample, e.g. 1/8
{
  avg1_ = alpha * counter_ + (1.0 - alpha) * avg1_;
  
  // // NS_LOG_FUNCTION (this);

  // // do magic
  // // avg1_ = EXP_1 * avg1_ + (1 - EXP_1) * counter_;
  // avg1_ = EXP_2 * avg1_ + (1 - EXP_2) * counter_;
  // // avg1_ = EXP_3 * avg1_ + (1 - EXP_3) * counter_;

  
  // // avg2_ = EXP_2 * avg2_ + (1 - EXP_2) * counter_;
  // // avg3_ = EXP_3 * avg3_ + (1 - EXP_3) * counter_;
  
  // avg1_ -= m_counters[m_currentSample];
  // m_counters[m_currentSample] = (1.0 / HISTORY_SAMPLES) * counter_;
  // avg1_ += m_counters[m_currentSample];

  // m_currentSample = (m_currentSample + 1) % HISTORY_SAMPLES;
    
  counter_ = 0;

  zeroCount_ = 0;
}
  

// void
// LoadStats::StepIfNonZeroCountOnly ()
// {
//   Step ();
//   // if (counter_ > 0)
//   //   Step ();
//   // else
//   //   {
//   //     // avg1_ = EXP_1 * avg1_;
//   //     avg1_ = 1;
//   //     zeroCount_ ++;
//   //   }
// }

LoadStats &
LoadStats::operator ++ (int)
{
  counter_ ++;
  return *this;
}

LoadStats &
LoadStats::operator -- (int)
{
  counter_ --;
  return *this;
}

LoadStats &
LoadStats::operator += (uint32_t amount)
{
  counter_ += amount;
  return *this;
}

LoadStats &
LoadStats::operator += (const LoadStats &stats)
{
  // NS_LOG_FUNCTION (this << &stats << stats.counter_);

  counter_ += stats.counter_;
  return *this;
}

LoadStats::stats_tuple
LoadStats::GetStats () const
{
  return stats_tuple (avg1_, avg2_, avg3_);
}

bool
LoadStats::IsZero () const
{  
  // return (zeroCount_ > 180 &&  // i.e., zero count for last 3 mins
  return (zeroCount_ > 90 &&  // i.e., zero count for last 3 mins
  // return (zeroCount_ > 30 &&  // i.e., zero count for last 3 mins
          counter_ == 0 &&
          avg1_ < PRECISION &&
	  avg2_ < PRECISION &&
	  avg3_ < PRECISION);
}

std::ostream &
operator << (std::ostream &os, const LoadStats &stats)
{
  os << stats.avg1_ << ", " << stats.avg2_ << ", " << stats.avg3_;
  return os;
}

} // namespace ndnSIM
} // namespace ndn
} // namespace ns3
