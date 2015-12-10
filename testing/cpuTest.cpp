#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

int NUM_OF_CPU_COLS = 7;

struct cpu_usage
{
  int nonidle;
  int total;
};

cpu_usage initCpu;
cpu_usage finalCpu;


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
  initCpu = getCpuStats();
  double total, nonidle;
  int sleepTime = 10;
  for(int i = 0; i < 15; i++){
    sleep(sleepTime);
    finalCpu = getCpuStats();

    total = (double)(finalCpu.total - initCpu.total);
    nonidle = (double)(finalCpu.nonidle - initCpu.nonidle);
    initCpu.total = finalCpu.total;
    initCpu.nonidle = finalCpu.nonidle;

    std::cout << "CPU: " << nonidle/total << std::endl;
  }

  return 0;
}
