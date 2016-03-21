/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Jaume Nin <jaume.nin@cttc.cat>
 */

#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/visualizer-module.h"
#include "ns3/radio-environment-map-helper.h"
#include "ns3/position-allocator.h"
#include "ns3/spectrum-module.h"


using namespace ns3;

/**
 * Sample simulation script for LTE + EPC. It instantiates several eNodeB,
 * attaches one UE per eNodeB starts a flow for each UE to  and from a remote host.
 * It also  starts yet another flow between each UE pair.
 */
NS_LOG_COMPONENT_DEFINE ("EpcFirstExample");
//float totalData=0;
//int totalFlow=0;
//void printStats(FlowMonitorHelper *flowmon_helper,Ptr<FlowMonitor> flowMon)
//{
//      Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon_helper->GetClassifier());
//      std::string proto;
//      Ptr<FlowMonitor> monitor = flowmon_helper->GetMonitor ();
//      std::map < FlowId, FlowMonitor::FlowStats > stats = monitor->GetFlowStats();
//      double totalTimeReceiving;
//      uint64_t totalPacketsReceived, totalPacketsDropped, totalBytesReceived;
//
//      totalBytesReceived = 0, totalPacketsDropped = 0, totalPacketsReceived = 0, totalTimeReceiving = 0;
//     for (std::map< FlowId, FlowMonitor::FlowStats>::iterator flow = stats.begin(); flow != stats.end(); flow++)
//      {
////    	totalFlow++;
//        Ipv4FlowClassifier::FiveTuple  t = classifier->FindFlow(flow->first);
//        switch(t.protocol)
//         {
//         case(6):
//             proto = "TCP";
//             break;
//         case(17):
//             proto = "UDP";
//             break;
//         default:
//             exit(1);
//         }
//         totalBytesReceived += (double) flow->second.rxBytes * 8;
//         totalTimeReceiving += flow->second.timeLastRxPacket.GetSeconds ();
//         totalPacketsReceived += flow->second.rxPackets;
//         totalPacketsDropped += flow->second.txPackets - flow->second.rxPackets;
//         if (true) {
//           std::cout << "FlowID: " << flow->first << " (" << proto << " "
//                     << t.sourceAddress << " / " << t.sourcePort << " --> "
//                     << t.destinationAddress << " / " << t.destinationPort << ")" << std::endl;
////           std::cout << "  Tx Bytes: " << flow->second.txBytes << std::endl;
////           std::cout << "  Rx Bytes: " << flow->second.rxBytes << std::endl;
////           std::cout << "  Tx Packets: " << flow->second.txPackets << std::endl;
////           std::cout << "  Rx Packets: " << flow->second.rxPackets << std::endl;
//           std::cout <<flow->second.timeLastRxPacket.GetSeconds ()<<"\t";
////           std::cout << "  Lost Packets: " << flow->second.lostPackets << std::endl;
////           std::cout << "  Pkt Lost Ratio: " << ((double)flow->second.txPackets-(double)flow->second.rxPackets)/(double)flow->second.txPackets << std::endl;
//
//           std::cout<<"Throughput"<<flow->first<<":\t" << flow->second.rxBytes * 8.0 / (flow->second.timeLastRxPacket.GetSeconds()-flow->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps"<<std::endl;
////           totalData+=( ((double)flow->second.rxBytes*8) / (flow->second.timeLastRxPacket.GetSeconds ()) );
//           //           std::cout << "  Mean{Delay}: " << (flow->second.delaySum.GetSeconds()/flow->second.rxPackets) << std::endl;
////           std::cout << "  Mean{Jitter}: " << (flow->second.jitterSum.GetSeconds()/(flow->second.rxPackets)) << std::endl;
//
//         }
//       }
//      //Simulator::Schedule(Seconds(0.5),&printStats, flowmon_helper, monitor);
//}

int main (int argc, char *argv[])
{
  uint16_t numberOfeNodeBs = 4;
  uint16_t numberOfUEs = 20;
  double simTime = 10.0;
  double interPacketInterval = 1.0;
  std::string schedularType="ns3::RrFfMacScheduler";
  std::string speed="ns3::ConstantRandomVariable[Constant=0]";
  bool fullBufferFlag=true;
  uint32_t rBs=50;
  uint32_t packetSize=1500;

  { PyViz v; }//Require for visualization

  // Command line arguments
  CommandLine cmd;
  cmd.AddValue("simTime", "Total duration of the simulation [s])", simTime);
  //cmd.AddValue("interPacketInterval", "Inter packet interval [ms])", interPacketInterval);
  cmd.AddValue("schedularType","Type to the MAC Schedular",schedularType);
  cmd.AddValue("fullBufferFlag","Full buffer condition or not",fullBufferFlag);
  cmd.AddValue("speed","Speed of the UEs",speed);
  cmd.Parse(argc, argv);

  ConfigStore config;
  config.ConfigureDefaults ();

  if(!fullBufferFlag)
  {
        interPacketInterval=10;
  }

  std::cout<<"\nSchedular Type :: "<<schedularType;
  std::cout<<"\nFull buffer flag :: "<<fullBufferFlag;
  std::cout<<"\nSpeed :: "<<speed<<std::endl;




  NodeContainer ueNodes1,ueNodes2,ueNodes3,ueNodes4;
  NodeContainer enbNodes;
  enbNodes.Create(numberOfeNodeBs);
  ueNodes1.Create(numberOfUEs/4);
  ueNodes2.Create(numberOfUEs/4);
  ueNodes3.Create(numberOfUEs/4);
  ueNodes4.Create(numberOfUEs/4);
  //Placing eNB
  Ptr<ListPositionAllocator> enbpositionAlloc = CreateObject<ListPositionAllocator> ();
  enbpositionAlloc->Add (Vector(0, 0, 0));
  	enbpositionAlloc->Add (Vector(1000, 0, 0));
	enbpositionAlloc->Add (Vector(1000,1000, 0));
	enbpositionAlloc->Add (Vector(0,1000, 0));

  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator(enbpositionAlloc);
  mobility.Install(enbNodes);


  MobilityHelper ue1mobility;
  ue1mobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                          "X",DoubleValue(0.0),
                                          "Y",DoubleValue(0.0),
                                          "Rho",StringValue("ns3::UniformRandomVariable[Min=0|Max=500]"));
  ue1mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                 "Bounds", RectangleValue (Rectangle (-1500, 2500, -1500, 2500)),"Speed", StringValue (speed));
  ue1mobility.Install(ueNodes1);

  //Postion of UEs Attach to eNB2
  MobilityHelper ue2mobility;
  ue2mobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                    "X",DoubleValue(1000.0),
                                    "Y",DoubleValue(0.0),
                                    "Rho",StringValue("ns3::UniformRandomVariable[Min=0.0|Max=500.0]"));
   ue2mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                 "Bounds", RectangleValue (Rectangle (-1500, 2500, -1500, 2500)),"Speed", StringValue (speed));
  ue2mobility.Install(ueNodes2);

  //Postion of UEs Attach to eNB3
  MobilityHelper ue3mobility;
  ue3mobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                    "X",DoubleValue(1000.0),
                                    "Y",DoubleValue(1000.0),
                                    "Rho",StringValue("ns3::UniformRandomVariable[Min=0.0|Max=500.0]"));
  ue3mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                 "Bounds", RectangleValue (Rectangle (-1500, 2500, -1500, 2500)),"Speed", StringValue (speed));
  ue3mobility.Install(ueNodes3);

  //Postion of UEs Attach to eNB4
  MobilityHelper ue4mobility;
  ue4mobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                    "X",DoubleValue(0.0),
                                    "Y",DoubleValue(1000.0),
                                    "Rho",StringValue("ns3::UniformRandomVariable[Min=0.0|Max=500.0]"));
  ue4mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                 "Bounds", RectangleValue (Rectangle (-1500, 2500, -1500, 2500)),"Speed", StringValue (speed));
  ue4mobility.Install(ueNodes4);



  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  Ptr<PointToPointEpcHelper>  epcHelper = CreateObject<PointToPointEpcHelper> ();
  lteHelper->SetEpcHelper (epcHelper);
  lteHelper->SetEnbDeviceAttribute("DlBandwidth",UintegerValue(rBs));
  lteHelper->SetEnbDeviceAttribute("UlBandwidth",UintegerValue(rBs));
  lteHelper->SetSchedulerType (schedularType);

  lteHelper->SetHandoverAlgorithmType ("ns3::A2A4RsrqHandoverAlgorithm");
  lteHelper->SetHandoverAlgorithmAttribute ("ServingCellThreshold",UintegerValue (15));
  lteHelper->SetHandoverAlgorithmAttribute ("NeighbourCellOffset",UintegerValue (1));

  Ptr<Node> pgw = epcHelper->GetPgwNode ();
  Ptr<ListPositionAllocator> pgwpositionAlloc = CreateObject<ListPositionAllocator> ();
  pgwpositionAlloc->Add(Vector(300,300,0));
  mobility.SetPositionAllocator(pgwpositionAlloc);
  mobility.Install(pgw);

   // Create a single RemoteHost
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);

  Ptr<ListPositionAllocator> rHpositionAlloc = CreateObject<ListPositionAllocator> ();
  rHpositionAlloc->Add(Vector(500,300,0));
  mobility.SetPositionAllocator(rHpositionAlloc);
  mobility.Install(remoteHostContainer);



  // Create the Internet
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("1024Mb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (3000));
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);

  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);


  // interface 0 is localhost, 1 is the p2p device
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

  // Install LTE Devices to the nodes
  NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);
  for(int i=0;i<numberOfeNodeBs;i++)
  {
	  Ptr<LteEnbPhy> enbPhy=enbLteDevs.Get(i)->GetObject<LteEnbNetDevice>()->GetPhy();
	  enbPhy->SetTxPower(30);
  }

  NetDeviceContainer ueLteDevs1 = lteHelper->InstallUeDevice (ueNodes1);
  NetDeviceContainer ueLteDevs2 = lteHelper->InstallUeDevice (ueNodes2);
  NetDeviceContainer ueLteDevs3 = lteHelper->InstallUeDevice (ueNodes3);
  NetDeviceContainer ueLteDevs4 = lteHelper->InstallUeDevice (ueNodes4);

  // Install the IP stack on the UEs
  internet.Install (ueNodes1);
  internet.Install (ueNodes2);
  internet.Install (ueNodes3);
  internet.Install (ueNodes4);

  Ipv4InterfaceContainer ueIpIface1;
  Ipv4InterfaceContainer ueIpIface2;
  Ipv4InterfaceContainer ueIpIface3;
  Ipv4InterfaceContainer ueIpIface4;

  ueIpIface1= epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs1));
  ueIpIface2= epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs2));
  ueIpIface3= epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs3));
  ueIpIface4= epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs4));

  // Assign IP address to UEs, and install applications
  for (uint32_t u = 0; u < ueNodes1.GetN (); ++u)
    {
      Ptr<Node> ueNode = ueNodes1.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }
  for (uint32_t u = 0; u < ueNodes2.GetN (); ++u)
    {
      Ptr<Node> ueNode = ueNodes2.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }
  for (uint32_t u = 0; u < ueNodes3.GetN (); ++u)
    {
      Ptr<Node> ueNode = ueNodes2.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }
  for (uint32_t u = 0; u < ueNodes4.GetN (); ++u)
    {
      Ptr<Node> ueNode = ueNodes4.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }


  // Attach all UEs to eNodeB
  lteHelper->Attach(ueLteDevs1,enbLteDevs.Get(0));
  lteHelper->Attach(ueLteDevs2,enbLteDevs.Get(1));
  lteHelper->Attach(ueLteDevs3,enbLteDevs.Get(2));
  lteHelper->Attach(ueLteDevs4,enbLteDevs.Get(3));


//  lteHelper->AddX2Interface (enbNodes);


  // Install and start applications on UEs and remote host

  uint16_t dlPort = 1234;
  uint16_t ulPort = 2000;
  uint16_t otherPort = 3000;
  ApplicationContainer clientApps;
  ApplicationContainer serverApps;

  for (uint32_t u = 0; u < ueNodes1.GetN (); ++u)
    {
      ++ulPort;
      ++otherPort;
      PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
      PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
      PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), otherPort));
      serverApps.Add (dlPacketSinkHelper.Install (ueNodes1.Get(u)));
      serverApps.Add (ulPacketSinkHelper.Install (remoteHost));
      serverApps.Add (packetSinkHelper.Install (ueNodes1.Get(u)));
      UdpClientHelper dlClient (ueIpIface1.GetAddress (u), dlPort);
      dlClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
      dlClient.SetAttribute ("MaxPackets", UintegerValue(10000));
      dlClient.SetAttribute("PacketSize",UintegerValue(packetSize));
      clientApps.Add (dlClient.Install (remoteHost));
    }
    for (uint32_t u = 0; u < ueNodes2.GetN (); ++u)
      {
        ++ulPort;
        ++otherPort;
        PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
        PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
        PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), otherPort));
        serverApps.Add (dlPacketSinkHelper.Install (ueNodes2.Get(u)));
        serverApps.Add (ulPacketSinkHelper.Install (remoteHost));
        serverApps.Add (packetSinkHelper.Install (ueNodes2.Get(u)));
        UdpClientHelper dlClient (ueIpIface2.GetAddress (u), dlPort);
        dlClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
        dlClient.SetAttribute ("MaxPackets", UintegerValue(10000));
        dlClient.SetAttribute("PacketSize",UintegerValue(packetSize));
        clientApps.Add (dlClient.Install (remoteHost));
      }

      for (uint32_t u = 0; u < ueNodes3.GetN (); ++u)
        {
          ++ulPort;
          ++otherPort;
          PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
          PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
          PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), otherPort));
          serverApps.Add (dlPacketSinkHelper.Install (ueNodes3.Get(u)));
          serverApps.Add (ulPacketSinkHelper.Install (remoteHost));
          serverApps.Add (packetSinkHelper.Install (ueNodes3.Get(u)));
          UdpClientHelper dlClient (ueIpIface3.GetAddress (u), dlPort);
          dlClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
          dlClient.SetAttribute ("MaxPackets", UintegerValue(10000));
          dlClient.SetAttribute("PacketSize",UintegerValue(packetSize));
          clientApps.Add (dlClient.Install (remoteHost));
        }
        for (uint32_t u = 0; u < ueNodes4.GetN (); ++u)
          {
            ++ulPort;
            ++otherPort;
            PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
            PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
            PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), otherPort));
            serverApps.Add (dlPacketSinkHelper.Install (ueNodes4.Get(u)));
            serverApps.Add (ulPacketSinkHelper.Install (remoteHost));
            serverApps.Add (packetSinkHelper.Install (ueNodes4.Get(u)));
            UdpClientHelper dlClient (ueIpIface4.GetAddress (u), dlPort);
            dlClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
            dlClient.SetAttribute ("MaxPackets", UintegerValue(10000));
            dlClient.SetAttribute("PacketSize",UintegerValue(packetSize));
            clientApps.Add (dlClient.Install (remoteHost));
          }

  //-----------------REM Code------------------------
//  Ptr<RadioEnvironmentMapHelper> remHelper = CreateObject<RadioEnvironmentMapHelper> ();
//  remHelper->SetAttribute ("ChannelPath", StringValue ("/ChannelList/1"));
//  remHelper->SetAttribute ("OutputFile", StringValue ("rem.out"));
//  remHelper->SetAttribute ("XMin", DoubleValue (-1500.0));
//  remHelper->SetAttribute ("XMax", DoubleValue (2500.0));
//  remHelper->SetAttribute ("YMin", DoubleValue (-1500.0));
//  remHelper->SetAttribute ("YMax", DoubleValue (2500.0));
//  remHelper->SetAttribute ("Z", DoubleValue (0.0));
//  remHelper->Install ();
  //--------------------------------------------------------------------

  serverApps.Start (Seconds (0.01));
  clientApps.Start (Seconds (0.01));

  lteHelper->EnableTraces ();

//  FlowMonitorHelper flowmon;
//  Ptr<FlowMonitor> Monitor;
//  Monitor = flowmon.Install(ueNodes1);
//  Monitor = flowmon.Install(ueNodes2);
//  Monitor = flowmon.Install(ueNodes3);
//  Monitor = flowmon.Install(ueNodes4);
//  Monitor = flowmon.Install(remoteHostContainer);
//  Simulator::Schedule(Seconds(3),&printStats,&flowmon, Monitor);
 
 Simulator::Stop(Seconds(simTime));

  config.ConfigureAttributes();

  Simulator::Run();

//  printStats(&flowmon,Monitor);

  Simulator::Destroy();

//  std::cout<<"\nTotal data="<<totalData<<"\n";
//  std::cout<<"\nTotal Flow="<<totalFlow<<"\n";
//  std::cout<<"\nAverage Throughput ="<<(totalData/totalFlow)/8192<<"Mps\n";

  return 0;
}
