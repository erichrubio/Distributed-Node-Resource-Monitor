#include "NodeMonitor.h"
#include <iostream>
#include <fstream>
#include <lcm/lcm-cpp.hpp>

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


int main(){

  std::cout << "Current mem usage: " << NodeMonitor::getMemUsage() << std::endl;
  std::cout << "Current storage usage: " << NodeMonitor::getStorageUsage() << std::endl;

return 0;
} 
