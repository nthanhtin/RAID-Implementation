#include "extractionraid1.h"
#include "writeraid1.h"

int main ( int argc, char *argv[] ){
        string command = string(argv[1]);
        string var = string(argv[2]);
        if (command == "-w"){
            cout<<"write";
            write(var);
        }    
        else if (command == "-e"){
            extract(var);  
        }
    return 0;
}
