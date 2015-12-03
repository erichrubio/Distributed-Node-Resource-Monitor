#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

struct network_usage
{
  int packets_sent;
  int packets_received;
  int bytes_sent;
  int bytes_received;
};

int NUM_OF_NETWORK_JUNK_LINES = 20;
int RECEIVED_BYTES_INDEX = 0;
int RECEIVED_PACKETS_INDEX = 1;
int SENT_BYTES_INDEX = 8;
int SENT_PACKETS_INDEX = 9;

network_usage getNetworkUsage()
{
  std::ifstream cpufile("/proc/net/dev");
  if (cpufile.is_open())
  {
    // Pull "cpu" from file
    std::string garbage;
    int index = 0;
    while(cpufile >> garbage && index < NUM_OF_NETWORK_JUNK_LINES)
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
    while(cpufile >> num)
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
}

int NUM_OF_CPU_COLS = 7;

struct cpu_usage
{
  int nonidle;
  int total;
};

cpu_usage getCpuStats()
{
  std::ifstream cpufile("/proc/stat");
  if (cpufile.is_open())
  {
    // Pull "cpu" from file
    std::string garbage;
    cpufile >> garbage;

    int num;
    int index = 0;
    int nums[NUM_OF_CPU_COLS];
    while (cpufile >> num )
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
}

int main(int argc, char ** argv)
{
  network_usage network;
  network = getNetworkUsage();
  std::cout << "Bytes received: " << network.bytes_received << std::endl;
  std::cout << "Packets received: " << network.packets_received << std::endl;
  std::cout << "Bytes sent: " << network.bytes_sent << std::endl;
  std::cout << "Packets sent: " << network.packets_sent << std::endl;
  

  /*cpu_usage cpu1;
  cpu1 = getCpuStats();

  // Sleep for one second
  unsigned int seconds = 1;
  sleep(seconds);

  cpu_usage cpu2;
  cpu2 = getCpuStats();

  double total = (double)(cpu2.total - cpu1.total);
  double nonidle = (double)(cpu2.nonidle - cpu1.nonidle);

  std::cout << "CPU: " << nonidle/total << std::endl;*/

  return 0;
}
