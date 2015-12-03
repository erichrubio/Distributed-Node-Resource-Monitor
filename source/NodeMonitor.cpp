#include "NodeMonitor.h"
#include <iostream>
#include <fstream>
#include <unistd.h>


//grabs the first 3 lines from /proc/meminfo
//In order: MemTotal, MemFree, MemAvailable
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
  cpu_usage firstStats;
  firstStats = getCurrentCpuStats();

  // Sleep for one second
  unsigned int seconds = 1;
  sleep(seconds);

  cpu_usage secondStats;
  secondStats = getCurrentCpuStats();

  double total = (double)(secondStats.total - firstStats.total);
  double nonidle = (double)(secondStats.nonidle - firstStats.nonidle);

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
      network.bytes_received = bytesReceived;
      network.packets_received = packetsReceived;
      network.bytes_sent = bytesSent;
      network.packets_sent = packetsSent;
      
      return network;
    }
  else
    {
      std::cout << "(/proc/net/dev) File open failed." << std::endl;
    }
}

int main(){

	std::cout << "Current mem usage: " << NodeMonitor::getMemUsage() << std::endl;
	std::cout << "Current storage usage: " << NodeMonitor::getStorageUsage() << std::endl;
	std::cout << "Current cpu usage: " << NodeMonitor::getCpuUsage() << std::endl;
	std::cout << "Current network usage: " << std::endl;
	NodeMonitor::getNetworkUsage().print();

return 0;
} 
