
// archiver that compress and decompress files from console


#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <clocale>
#include "Archiver.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    
    
    cout << "Welcome to Archiver" << endl << endl;
    
    if(argc > 1) {
        
        vector<string> files;
        string path = "";
        bool read_file = false;
        bool write_file = false;
        char type[6];
        memset(type, 0, 6);
        
        for (int i = 1; i < argc; i++) {
            
            if (strcmp(argv[i],"-pack") == 0) {
                strcpy(type,"pack");
                read_file = false;
                write_file = false;
            }
            if (strcmp(argv[i],"-unpack") == 0) {
                strcpy(type,"unpack");
                read_file = false;
                write_file = false;
            }
            if (strcmp(argv[i],"-path") == 0) {
                write_file = true;
                read_file = false;
                continue;
            }
            if (strcmp(argv[i],"-files") == 0) {
                read_file = true;
                write_file = false;
                continue;
            }
            
            if(write_file) {
                path.assign(argv[i]);
                
            }
            if(read_file) {
                files.push_back(string(argv[i]));
            }
        }
        
        Archiver *archiver = new Archiver(files, path);
        if (strcmp(type, "pack") == 0) {
            archiver->compress();
        }
        if (strcmp (type, "unpack") == 0) {
            archiver->decompress(files[0]);
        }
    }
    else cout<<"Include -pack/-unpack , -files, -path parameters"<<endl;
    
    return 0;
}
