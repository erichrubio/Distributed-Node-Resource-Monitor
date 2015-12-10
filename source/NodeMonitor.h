//Names: Zachary Barton
//       Eric Rubio
//Course: CS3281
//Section: Fall 2015

//This class gathers information from the Node where it is deployed,
//then sends that information back to the central node via LCM.

#ifndef NODE_MONITOR_H
#define NODE_MONITOR_H

#include <string>
#include <iostream>
#include <fstream>
#include <sys/vfs.h> //Used to obtain file system usage info
#include "nodeLCM/sysresource_t.hpp"
#include <lcm/lcm-cpp.hpp>

struct network_usage
{
  int packets_sent;
  int packets_received;
  int bytes_sent;
  int bytes_received;
  
  void print(){
    std::cout << "\tBytes received: " << bytes_received << std::endl;
    std::cout << "\tPackets received: " << packets_received << std::endl;
    std::cout << "\tBytes sent: " << bytes_sent << std::endl;
    std::cout << "\tPackets sent: " << packets_sent << std::endl;
  }
};

struct cpu_usage
{
  int nonidle;
  int total;
};

class NodeMonitor{
 private:	
  //LCM target URL
  //const LCM_DEFAULT URL = ""
  
  //string to identify current node
  static const std::string nodeID;

  //LCM struct, holds system info
  nodeLCM::sysresource_t data;

  // CPU constants
  static const int NUM_OF_CPU_COLS = 7;

  // CPU Stats
  cpu_usage lastCpuStats;
  
  // Network constants
  static const int NUM_OF_NETWORK_JUNK_LINES = 20;
  static const int RECEIVED_BYTES_INDEX = 0;
  static const int RECEIVED_PACKETS_INDEX = 1;
  static const int SENT_BYTES_INDEX = 8;
  static const int SENT_PACKETS_INDEX = 9;

  // Network Stats
  network_usage lastNetworkStats;

  // Returns cpu_usage containing cpu stats at time of method call
  cpu_usage getCurrentCpuStats();
  
 public:

  // Default constructor
  NodeMonitor();
		
  // Calculate memory usage % 
  // (MemAvailable / MemTotal)
  double getMemUsage();

  // Calculate storage usage % 
  // (Available Blocks / Total Blocks)
  double getStorageUsage();

  // Calculate CPU usage %
  // delta of (Non-idle processes)/(Total processes)
  double getCpuUsage();

  // Calculate network usage
  // Bytes sent & received, packets sent & received
  network_usage getNetworkUsage();

  // update data fields and send data via LCM
  void publishData(nodeLCM::sysresource_t &data);
};

#endif
