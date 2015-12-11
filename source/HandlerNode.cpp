#include <iostream>
#include <map>
#include <unistd.h>
#include <lcm/lcm-cpp.hpp>
#include "nodeLCM/sysresource_t.hpp"
  

class HandlerNode{
  
private:
  
  static const double HIGH_MEMORY = 0.8;
  static const double HIGH_STORAGE = 0.8;
  static const double HIGH_CPU = 0.8;
  static const int HIGH_BYTES_RECEIVED = 1000;
  static const int HIGH_BYTES_SENT = 1000;

  std::map<std::string, nodeLCM::sysresource_t> nodeData;
  std::map<std::string, bool> nodeUpdates;

  void printNodeData(nodeLCM::sysresource_t msg)
  {
      //Print Node ID name
      std::cout << "Node ID: " << msg.nodeID << std::endl;

      //Print Memory Usage
      if(msg.memUsage > HIGH_MEMORY)
	{
	  std::cout << "ALERT: MEMORY USAGE IS HIGH" << std::endl;
	}
      std::cout << "Memory Usage: " << (msg.memUsage) * 100 << "%" << std:: endl;

      //Print Storage Usage
      if(msg.storageUsage > HIGH_STORAGE)
	{
	  std::cout << "ALERT: STORAGE USAGE IS HIGH" << std::endl;
	}
      std::cout << "Storage Usage: " << (msg.storageUsage) * 100 << "%" << std::endl;

      //Print Cpu Usage
      if(msg.cpuUsage > HIGH_CPU)
	{
	  std::cout << "ALERT: CPU USAGE IS HIGH" << std::endl;
	}
      std::cout << "CPU Usage: " << (msg.cpuUsage) * 100 << "%" << std::endl;

      //Print Network Usage
      //[BytesReceive, PacketsReceived, BytesSent, PacketsSent]
      std::cout << "Network Usage Stats:" << std::endl;
      if(msg.networkUsage[0] > HIGH_BYTES_RECEIVED)
	{
	  std::cout << "ALERT: NETWORK RECEIVING TOO MUCH DATA" << std::endl;
	}
      std::cout << "\tBytes Received: " << msg.networkUsage[0] << std::endl;
      std::cout << "\tPackets Received: " << msg.networkUsage[1] << std::endl;
      if(msg.networkUsage[2] > HIGH_BYTES_SENT)
	{
	  std::cout << "ALERT: NETWORK SENDING TOO MUCH DATA" << std::endl;
	}
      std::cout << "\tBytes Sent: " << msg.networkUsage[2] << std::endl;
      std::cout << "\tPackets Sent: " << msg.networkUsage[3] << std::endl;
    //extra new lines for cleaner spacing in console    
    std::cout << std::endl << std::endl;
  }
  
public:
    ~HandlerNode() {}

    void handleMessage(const lcm::ReceiveBuffer* rbuf,
          const std::string& chan,
          const nodeLCM::sysresource_t* msg)
    { 
      nodeData[msg->nodeID] = *msg;
      nodeUpdates[msg->nodeID] = true;
    }

  void outputNodeData()
  {
    for (std::map<std::string, bool>::iterator it = nodeUpdates.begin(); it != nodeUpdates.end(); ++it)
      {
	if(it->second) // if updated
	  {
	    printNodeData(nodeData[it->first]);
	  }
	else
	  {
	    std::cout << it->first << " is OFFLINE" << std::endl;
	    //extra new lines for cleaner spacing in console    
	    std::cout << std::endl << std::endl;
	  }
      }
  }

  void setUpdatesFalse()
  {
    for (std::map<std::string, bool>::iterator it = nodeUpdates.begin(); it != nodeUpdates.end(); ++it)
      {
	it->second = false;
      }
  }
 
};


int main() {
  lcm::LCM lcm;
  if(!lcm.good()){
    std::cout << "Error creating LCM connection" << std::endl;
    return 1;
  }

  HandlerNode handler;
  lcm.subscribe("NodeMonitor", &HandlerNode::handleMessage, &handler);

  while(true)
    {
      int timeout = lcm.handleTimeout(100);
      if(timeout == 0) // timed out
	{
	  // Note: all messages have been handled

	  // Output data
	  handler.outputNodeData();

	  // Set updated to false
	  handler.setUpdatesFalse();

	  unsigned int seconds = 5;
	  sleep(seconds);
	}
      else if(timeout > 0)
	{
	  // Continue handling messages (nothing to do here)
	}
    }
  
  return 0;

}
