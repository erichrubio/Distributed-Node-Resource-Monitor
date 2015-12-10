#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include "nodeLCM/sysresource_t.hpp"

class HandlerNode{
  public:
    ~HandlerNode() {}

    void handleMessage(const lcm::ReceiveBuffer* rbuf,
          const std::string& chan,
          const nodeLCM::sysresource_t* msg)
    {
      std::cout << "Received message on channel: " << chan << std::endl;
      //Print Node ID name
      std::cout << "Node ID: " << msg-> nodeID << std::endl;

      //Print Memory Usage
      std::cout << "Memory Usage: " << (msg->memUsage) * 100 << "%" << std:: endl;
      //Print Storage Usage
      std::cout << "Storage Usage: " << (msg->memUsage) * 100 << "%" << std::endl;

      //Print Cpu Usage
      std::cout << "CPU Usage: " << (msg->cpuUsage) * 100 << "%" << std::endl;

      //Print Network Usage
      //[BytesReceive, PacketsReceived, BytesSent, PacketsSent]
      std::cout << "Network Usage Stats:" << std::endl;
      std::cout << "\tBytes Received: " << msg->networkUsage[0] << std::endl;
      std::cout << "\tPackets Received: " << msg->networkUsage[1] << std::endl;
      std::cout << "\tBytes Sent: " << msg->networkUsage[2] << std::endl;
      std::cout << "\tPackets Sent: " << msg->networkUsage[3] << std::endl;
    //extra new lines for cleaner spacing in console    
    std::cout << std::endl << std::endl;

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

  while(0 == lcm.handle());

  return 0;



  return 0;
}
