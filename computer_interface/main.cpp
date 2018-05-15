#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "gCode.cpp"
#include "usbCommunication.cpp"

/*
Program flow:
1- open arquive
2- start com
3- get command from gcode line
4- send command
5- get ping respond from arduino when complete
6- do steps 3-5 until end of file
7- close com
*/

int main (){

    ifstream gCodeFile;
    fstream arduino("/dev/ttyUSB0", ios::in | ios::out);
    string command;

    openGCodeFile(&gCodeFile);
    startUSBCommunication();
    
    while(!command.end()){
        setMessageToSend(gCodeFile, &command);

        sendMessage(command, arduino);
        while(!isTaskDone()){}
    }
    

    return 0;
}