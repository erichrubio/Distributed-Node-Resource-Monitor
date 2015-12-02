//Names: Zachary Barton
//       Eric Rubio
//Course: CS3281
//Section: Fall 2015

//This class gathers information from the Node where it is deployed,
//then sends that information back to the central node via LCM.

#ifndef NODEMONITOR_H
#define NODEMONITOR_H

#include <string>
#include <sys/vfs.h> //Used to obtain file system usage info



class NodeMonitor{
	private:
		//LCM target URL
		std::string lcm_url;

		//Calculate memory usage % 
		//(MemAvailable / MemTotal)
		double getMemUsage();

		//Calculate storage usage % 
		//(Available Blocks / Total Blocks)
		double getStorageUsage();

		//Calculate CPU usage %
		//formula here
		double getCpuUsage();

		//Calculate network usage %
		//formula here
		double getNetworkUsage();
	

	public:


};

#endif
