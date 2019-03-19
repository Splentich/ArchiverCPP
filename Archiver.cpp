
//  Archiver.cpp

#include "Archiver.hpp"

using namespace std;

//calculating the amount of digits in a number
int digits_in_number(double number) {
    return (number == 0) ? 1 : (int)ceil(log10(abs(number) + 0.5));
    }

// integer to string implementation
char* itoa(int num, char* str, int base) {
    int i = 0;
    int BASE = 10;
    bool isNegative = false;
    
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    
    if (num < 0 && base == BASE) {
        isNegative = true;
        num = -num;
    }
    
    while (num != 0) {
        int digit = num % base;
        str[i++] = (digit > 9) ? (digit - 10) + 'a' : digit + '0';
        num = num/base;
    }
    
    if (isNegative) {
        str[i++] = '-';
    }
    
    str[i] = '\0';
    reverse(str, i);
    return str;
    
};


void Archiver::getInfo() {
    

    basic_string<char> buffer = "";
    
    remove((this->path + "info.txt").c_str());
    
    FILE *info = fopen((this->path + "info.txt").c_str(), "a+");
    int sizeOfBytes = 0;
    
    for(vector<string>::iterator iterator = this->files.begin(); iterator != this->files.end(); iterator++){
        FILE *file = file = fopen((*iterator).c_str(), "rb");
        if (!file) break;
        
        fseek(file, 0, SEEK_END);
        int size = (int)ftell(file);
        
        string name = Archiver::get_file_name(*iterator);
        
        char *m_size = new char[digits_in_number(size) + 1];
        itoa(size, m_size, 10);
        fclose(file);
        
        sizeOfBytes += digits_in_number(size);
        sizeOfBytes += strlen(name.c_str());
        
        buffer.append(m_size);
        buffer.append("||");
        buffer.append(name);
        buffer.append("||");
        
        delete [] m_size;
    }
    
    sizeOfBytes = (int)buffer.size()+2;
    char *secondary_buffer = new char[digits_in_number(sizeOfBytes)];
    
    if(digits_in_number(sizeOfBytes) < 5) {
        fputs(string(5 - digits_in_number(sizeOfBytes), '0').c_str(), info);
    }
    fputs(secondary_buffer, info);
    fputs("||", info);
    fputs(buffer.c_str(), info);
    fclose(info);
}

//archiving files
void Archiver::compress() {
    
    char byte[1]; //buffer for 1 byte
    getInfo();
    
    FILE *file;
    FILE *main = fopen((this->binFile).c_str(), "wb");
    FILE *info = fopen((this->path + "info.txt").c_str(), "rb" );
    
    while (!feof(info)) {
        if (fread(byte, 1, 1, info) == 1) {
            fwrite(byte, 1, 1, main);
        }
        fclose(info);
        remove((this->path + "info.txt").c_str());
    }
    
    for (vector<string>::iterator iterator = this->files.begin(); iterator != this->files.end(); iterator++) {
        file = fopen((*iterator).c_str(), "rb");
        if (!file) {
            cout << *iterator << " not found!" << endl; break;
        } while (!feof(file)) {
            if (fread(byte, 1, 1, file) == 1) {
                fwrite(byte, 1, 1, main);
            }
        }
        cout << *iterator << " has been added to archive '" << this->binFile << "'." << endl;
        fclose(file);
    }
    fclose(main);
}

//unarchiving files
void Archiver::decompress(string binary) {
    
    char byte[1];
    
    FILE *bin = fopen(binary.c_str(), "rb");
    char block_size[5];
    fread(block_size, 1, 5, bin);
    int buff_to_digit = atoi(block_size);
    
    char *block = new char[buff_to_digit];
    fread(block, 1, buff_to_digit, bin);
    
    vector<string> tokens;
    char *tok = strtok(block, "||");
    int toks = 0;
    while (tok) {
        if (strlen(tok) == 0)
            break;
        tokens.push_back(tok);
        tok=strtok(NULL, "||");
        toks++;
    }
    //clearing junk
    if(toks % 2 == 1) {
        toks --;
    }
    
    int filesInArchive = toks/2;
    
    //unarchiving all files
    for (int i = 0; i < filesInArchive; i++) {
    
        const char* size = tokens[i * 2].c_str();
        const char* name = tokens[i * 2 + 1].c_str();
        char full_path[255];
        strcpy(full_path, this->path.c_str());
        strcat(full_path, name);
        int n = atoi(size);
        cout << " " << name << "has been unarchived into '" << this->path << "' ." << endl;
        FILE *currentFile = fopen(full_path, "wb");
        for (int k = 1; k < n; k++){
            if(fread(byte, 1, 1, bin) == 1) {
                fwrite(byte, 1, 1, currentFile);
            }
        }
        fclose(currentFile);
        
        delete [] size;
        delete [] name;
    }
    fclose(bin);
}
