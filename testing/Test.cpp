#include <iostream>
#include <fstream>
#include <string>


    int main() {
        
        //System Memory info test
        string lineIn;
        ifstream myfile ("/proc/meminfo");

        if(myfile.is_open()){

            while (getline(myfile,line)){
                std::cout << line << std::endl;
            }
        myfile.close();
        }
        else
            std::cout << "File open failed";
        return 0;
        
    }
