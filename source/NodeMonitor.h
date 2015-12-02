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



class NodeMonitor{
	private:	
		//LCM target URL
		//const LCM_DEFAULT URL = ""

	public:
		
		//Calculate memory usage % 
		//(MemAvailable / MemTotal)
		static double getMemUsage();

		//Calculate storage usage % 
		//(Available Blocks / Total Blocks)
		static double getStorageUsage();

		//Calculate CPU usage %
		//formula here
		static double getCpuUsage();

		//Calculate network usage %
		//formula here
		static double getNetworkUsage();
	
};

#endif
