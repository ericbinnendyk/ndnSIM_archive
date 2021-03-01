/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2012-2013 University of California, Los Angeles
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

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

#include <boost/lexical_cast.hpp>

using namespace ns3;
using namespace ns3::ndn;
using namespace std;

#include "calculate-max-capacity.h"

uint32_t Run = 1;

void PrintTime (Time next, const string name)
{
  cerr << " === " << name << " " << Simulator::Now ().ToDouble (Time::S) << "s" << endl;
  Simulator::Schedule (next, PrintTime, next, name);
}

int main (int argc, char**argv)
{
  string topology = "topo-small-binary-tree";
  string folder = "tmp";
  
  CommandLine cmd;
  int seed = 1;
  std::string threshold = "0.015";
  cmd.AddValue("seed", "Random seed", seed);
  cmd.AddValue("threshold", "Threshold value of detection", threshold);
  cmd.Parse (argc, argv);

  string name = topology + "-eric";
  
  string results_file = "results/" + folder + "/" + name + ".txt";
  string meta_file    = "results/" + folder + "/" + name + ".meta";
  string graph_dot_file    = "results/" + folder + "/" + name + ".dot";
  string graph_pdf_file    = "results/" + folder + "/" + name + ".pdf";
  
  AnnotatedTopologyReader topologyReader ("", 1.0);
  topologyReader.SetFileName ("topologies/" + topology + ".txt");
  topologyReader.Read ();

  ns3::RngSeedManager::SetSeed(seed);

  NodeContainer everythingElse;
  for_each (NodeList::Begin(), NodeList::End(), [&] (Ptr<Node> node) {
      if (Names::FindName (node) != "NodeR2")
        everythingElse.Add(node);
  });

  StackHelper helper;
  helper.SetForwardingStrategy ("ns3::ndn::fw::BestRoute::InterestMitigationStrategy", "Threshold", threshold);
  helper.Install (everythingElse);

  NodeContainer justR2;
  justR2.Add(Names::Find<Node>("NodeR2"));

  StackHelper helper2;
  helper2.SetForwardingStrategy ("ns3::ndn::fw::BestRoute::InterestMitigationStrategyRoc", "Threshold", threshold);
  helper2.Install (justR2);

  topologyReader.ApplyOspfMetric ();
  
  GlobalRoutingHelper grouter;
  grouter.Install (topologyReader.GetNodes ());

  system (("mkdir -p \"results/" + folder + "\"").c_str ());
  ofstream os (meta_file.c_str(), ios_base::out | ios_base::trunc);
  
  grouter.AddOrigins ("/", Names::Find<Node>("NodeP1"));
  grouter.CalculateRoutes ();

  saveActualGraph (graph_dot_file, NodeContainer (topologyReader.GetNodes ()));
  system (("twopi -Tpdf \"" + graph_dot_file + "\" > \"" + graph_pdf_file + "\"").c_str ());
  cout << "Write effective topology graph to: " << graph_pdf_file << endl;

  AppHelper evilAppHelper ("DdosAppCustomFrequency");
  evilAppHelper.SetAttribute ("LifeTime", StringValue ("1s"));
    evilAppHelper.SetPrefix ("/forged/");
    evilAppHelper.SetAttribute ("AvgGap", TimeValue (Seconds (0.01)));
    auto icnAttack = evilAppHelper.Install(Names::Find<Node> ("NodeC1"));
    icnAttack.Start(Seconds(10.0));
  
  AppHelper goodAppHelper ("ns3::ndn::ConsumerZipfMandelbrot");
  goodAppHelper.SetAttribute ("LifeTime",  StringValue ("1s"));
  goodAppHelper.SetAttribute ("NumberOfContents", StringValue ("5000"));
    goodAppHelper.SetPrefix ("/fine");
    goodAppHelper.SetAttribute("q", DoubleValue(0.7));
    goodAppHelper.SetAttribute("s", DoubleValue(0.7));
    goodAppHelper.SetAttribute("Frequency", StringValue("50"));
    goodAppHelper.SetAttribute("Randomize", StringValue("uniform"));
    goodAppHelper.Install(Names::Find<Node> ("NodeC2"));
    goodAppHelper.Install(Names::Find<Node> ("NodeC3"));
    goodAppHelper.Install(Names::Find<Node> ("NodeC4"));

  AppHelper ph ("ns3::ndn::Producer");
  ph.SetPrefix ("/fine");
  ph.SetAttribute ("PayloadSize", StringValue("1100"));
  
  ph.Install (Names::Find<Node>("NodeP1"));

  L3RateTracer::InstallAll (results_file, Seconds (1.0));
  
  //Simulator::Schedule (Seconds (10.0), PrintTime, Seconds (10.0), name);

  Simulator::Stop (Seconds (20.5));
  Simulator::Run ();
  Simulator::Destroy ();
 
  L3RateTracer::Destroy ();

  return 0;
}
