#include "NodeMonitor.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <lcm/lcm-cpp.hpp>
#include "nodeLCM/sysresource_t.hpp"


const std::string NodeMonitor::nodeID = "node_X";

NodeMonitor::NodeMonitor()
{
  std::cout << "Please enter an identifying tag for this node: ";
  std::cin >> nm.nodeID;
  
  //Initializes data in lastNetworkStats struct
  getNetworkUsage();

  //Initializes data in lastCpuStats struct
  getCpuUsage();

  /*
  lastNetworkStats.bytes_received = 0;
  lastNetworkStats.packets_received = 0;
  lastNetworkStats.bytes_sent = 0;
  lastNetworkStats.packets_sent = 0;

  lastCpuStats.nonidle = 0;
  lastCpuStats.total = 0;
  */

}

//grabs the first 3 lines from /proc/meminfo
//In order: MemTotal, MemFree, MemAvailable
//Discards MemFree, returns MemAvailable/MemTotal
double NodeMonitor::getMemUsage(){
  std::ifstream memFile ("/proc/meminfo");

  if(memFile.is_open()){
    std::string thisLine,memsize;
    int memNums[3];

    for(int i = 0; i < 3; i++){
      memFile >> thisLine >> memNums[i] >> memsize;
    }
    memFile.close();

    double memAvail = (double)memNums[2];
    double memTotal = (double)memNums[0];

    return (memAvail/memTotal);
  }
  else{
    std::cout << "(meminfo) File open failed";
  }
}

//Uses statfs() to pull system storage information
//Returns Available Blocks / Total Blocks
double NodeMonitor::getStorageUsage(){
  struct statfs buf;
  const char* path = "/home";
  int retval = statfs(path, &buf);

  if(statfs(path, &buf) == 0){
    return ((double)buf.f_bavail) /((double) buf.f_blocks);
  }
  else{
    std::cout << "error in statfs" << std::endl;
    return 0.0;
  }
}

cpu_usage NodeMonitor::getCurrentCpuStats(){
  std::ifstream cpufile("/proc/stat");
  if (cpufile.is_open())
  {
    // Pull "cpu" from file
    std::string garbage;
    cpufile >> garbage;
    int num;
    int index = 0;
    int nums[NUM_OF_CPU_COLS];
    while (cpufile >> num && index < NUM_OF_CPU_COLS)
      {
	nums[index] = num;
	index++;
      }
    int total = 0;
    int nonidle = 0;
    cpu_usage cpu;
    for(int i = 0; i < NUM_OF_CPU_COLS; i++)
      {
	// if not idle
	if(i != 3 && i != 4)
	  {
	    nonidle += nums[i];
	  }
	total += nums[i];
      }
    cpu.total = total;
    cpu.nonidle = nonidle;
    return cpu;
  }
  else
    {
      std::cout << "(/proc/stat) File open failed." << std::endl;
    }
}

double NodeMonitor::getCpuUsage(){
  cpu_usage currentStats;
  currentStats = getCurrentCpuStats();

  double total = (double)(currentStats.total - lastCpuStats.total);
  double nonidle = (double)(currentStats.nonidle - lastCpuStats.nonidle);

  lastCpuStats.total = currentStats.total;
  lastCpuStats.nonidle = currentStats.nonidle;

  return nonidle/total;
}

network_usage NodeMonitor::getNetworkUsage(){
  std::ifstream networkfile("/proc/net/dev");
  if (networkfile.is_open())
    {
      // Pull "cpu" from file
      std::string garbage;
      int index = 0;
      while(networkfile >> garbage && index < NUM_OF_NETWORK_JUNK_LINES)
	{ 
	  /* Remove unnecessary text */
	  index++;
	}
      
      index = 0;
      int num;
      int packetsReceived;
      int packetsSent;
      int bytesReceived;
      int bytesSent;
      while(networkfile >> num)
	{
	  if(index == RECEIVED_BYTES_INDEX)
	    { bytesReceived = num; }
	  if(index == RECEIVED_PACKETS_INDEX)
	    { packetsReceived = num; }
	  if(index == SENT_BYTES_INDEX)
	    { bytesSent = num; }
	  if(index == SENT_PACKETS_INDEX)
	    { packetsSent = num; }
	  
	  index++;
	}
      
      network_usage network;
      network.bytes_received = bytesReceived - lastNetworkStats.bytes_received;
      network.packets_received = packetsReceived - lastNetworkStats.packets_received;
      network.bytes_sent = bytesSent - lastNetworkStats.bytes_sent;
      network.packets_sent = packetsSent - lastNetworkStats.packets_sent;

      lastNetworkStats.bytes_received = bytesReceived;
      lastNetworkStats.packets_received = packetsReceived;
      lastNetworkStats.bytes_sent = bytesSent;
      lastNetworkStats.packets_sent = packetsSent;
      
      return network;
    }
  else
    {
      std::cout << "(/proc/net/dev) File open failed." << std::endl;
    }
}

void NodeMonitor::publishData(nodeLCM::sysresource_t &data){
  

}

void printStats(NodeMonitor &nm){
  std::cout << "Current mem usage: " << nm.getMemUsage() << std::endl;
  std::cout << "Current storage usage: " << nm.getStorageUsage() << std::endl;
  std::cout << "Current cpu usage: " << nm.getCpuUsage() << std::endl;
  std::cout << "Current network usage: " << std::endl;
  nm.getNetworkUsage().print();
}

int main(){
  
  lcm::LCM lcm;
  if(!lcm.good()){
    std::cout << "Error creating LCM connection" << std::endl;
    return 1;
  }

  NodeMonitor nm;
  

  printStats(nm);
  // Sleep for 5 seconds
  unsigned int seconds = 5;
  sleep(seconds);
  printStats(nm);
  sleep(seconds);
  printStats(nm);
  
  return 0;
} 
