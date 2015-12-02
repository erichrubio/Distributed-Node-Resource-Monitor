#include <iostream>
#include <fstream>
#include <string>
#include <sys/vfs.h>


int main() {
        
    //System Memory info test
    std::ifstream memFile ("/proc/meminfo");

    if(memFile.is_open()){

        std::string thisLine,memsize;
        int memNums[3];
            
        for(int i = 0; i < 3; i++){
            memFile >> thisLine >> memNums[i] >> memsize;
            std::cout << thisLine << " " << memNums[i] << 
            " " << memsize << std::endl;
        }
    memFile.close();
    }
    else{
        std::cout << "(meminfo) File open failed";
    }

    struct statfs buf;
    const char* path = "/home";
    int retval = statfs(path, &buf);

    if(statfs(path, &buf) == 0){
        std::cout << "Total blocks: " << buf.f_blocks << std::endl;
        std::cout << "Free blocks: " << buf.f_bfree << std::endl;
        std::cout << "Avail blocks: " << buf.f_bavail << std::endl;
    }
    else{
        std::cout << "error in statfs" << std::endl;
    }
    

    return 0;
        
    }
