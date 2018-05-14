#include <iostream>
#include <fstream>

using namespace std;

fstream read_file();
char * get_string_to_send(char *);

int main (){

    fstream file;
    cout<<"Hello World"<<endl;
    file = read_file();

    return 0;
}

fstream read_file(){

    cout<<"Insert name of the G-Code file:";

    char* name, *str;
    cin>>name;

    fstream file;
    file.open(name, fstream::in );

    return file;
}

char * get_string_to_send(char * file){

    char * string;

    for (int i =0; string[i] == '\n'; i++){
        string[i] = file[i];
    }

    return string;
}