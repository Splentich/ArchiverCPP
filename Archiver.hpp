
//  Archiver.hpp

#ifndef Archiver_hpp
#define Archiver_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <clocale>
#include <math.h>


using namespace std;

#endif /* Archiver_hpp */


class Archiver {
    
private:
    vector<string> files;
    string path;
    string binFile;
public:
    
    Archiver (vector<string> &vector, string str) {
        if (vector.size() > 0) {
            files.assign(vector.begin(), vector.end());
        }
        path = str+"\"";
        binFile = path+"binary.archiver";
    }
    
    void getInfo();
    void compress();
    void decompress(string binary);
    
    
    //static function to retrieve a filename from the path
    static string get_file_name(string pathName){
        return pathName.substr(pathName.find_last_of("\"")+1, pathName.size());
    }
    
};
