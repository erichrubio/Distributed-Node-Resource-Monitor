//Names: Zachary Barton
//       Eric Rubio
//Course: CS3281
//Section: Fall 2015

//This class gathers information from the Node where it is deployed,
//then sends that information back to the central node via LCM.

#ifndef NODE_MONITOR
#define NODE_MONITOR

#include <string>
#include <sys/vfs.h> //Used to obtain file system usage info



class NodeMonitor{
    private:
        //LCM target URL
        std::string lcm_url;
        //

    public:
        getCPUusage();
        getMemUsage();
        getStorageUsage();
        getNetworkUsage();

};

#endif NODE_MONITOR
