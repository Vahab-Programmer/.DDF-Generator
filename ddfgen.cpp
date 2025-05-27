#include <cstdlib>
#include <filesystem>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cctype>
#include <exception>
using namespace std;
namespace fs=filesystem;
string author="Vahab Programmer https://github.com/Vahab-Programmer";
string version="0.0.1";
void clscreen(){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}
bool isNum(string data){
    if (data.empty()) return false;
    for (char s:data){
        if (!isdigit(s))return false;
    };
    return true;
}
void getin(string prompt,string &ui){
    string input;
    clscreen();
    cout<<prompt;
    getline(cin,input);
    if (cin.fail()){
        cin.clear();
        cin.ignore(1000,'\n');
        getin(prompt,ui);
        return;
    }
    ui=input;
}
void getinbyint(string &ui,map <int,string>values){
    int input;
    clscreen();
    for (auto& pair:values){
        cout<<pair.first<<"."<<pair.second<<"\n";
    };
    cout<<"Select:";
    cin >>input;
    cin.ignore(1000,'\n');
    if (cin.fail()){
        cin.clear();
        cin.ignore(1000,'\n');
        getinbyint(ui,values);
        return;
    }
    if (values.find(input)==values.end()){
        getinbyint(ui,values);
        return;
    };
    ui=values[input];
}
int main(){
    string cabfn,odir,tdir,ddffn,comptype,mdisksz,mcabsz,output,tfile,dupf,dl;
    vector<string> includepath;
    getin("Name For CAB File:",cabfn);
    if (cabfn.empty())cabfn="output*.cab";
    else if(cabfn.ends_with(".cab")){
        cabfn=cabfn.substr(0,cabfn.size()-4);
        cabfn+="*";
        cabfn+=".cab";
    } else {
        cabfn+="*";
        cabfn+=".cab";
    }
    output+=".Set CabinetNameTemplate=";
    output+=cabfn;
    output+="\n";
    output+=".Set Cabinet=ON\n";
    getodir:
    getin("Output Dir:",odir);
    if (odir.empty())goto getodir;
    output+=".Set DiskDirectoryTemplate=";
    output+=odir;
    output+="\n";
    getin("Name For .DDF File:",ddffn);
    if (ddffn.empty())ddffn="output.ddf";
    else if (!ddffn.ends_with(".ddf"))ddffn+=".ddf";
    getinbyint(comptype,{{1,"LZX:21"},{2,"LZX:15"},{3,"MSZIP"},{4,"QUANTUM"},{5,"NONE"}});
    if (comptype!="NONE"){
        output+=".Set Compress=ON\n";
        output+=".Set CompressionType=";
        output+=comptype;
        output+="\n";
    } else {
        output+=".Set Compress=OFF\n";
    }
    getinbyint(dupf,{{1,"Duplicate Files"},{2,"No Duplicate"}});
    if (dupf=="Duplicate Files"){
        output+=".Set UniqueFiles=ON\n";
    } else {
        output+=".Set UniqueFiles=OFF\n";
    }
    getin("Disk Label(Enter None For Skip,Byte):",dl);
    if (!dl.empty()){
        output+=".Set DiskLabel=";
        output+=dl;
        output+="\n";
    }
    getin("Max Disk Size(Enter None For Skip,Byte):",mdisksz);
    if (isNum(mdisksz)){
        output+=".Set MaxDiskSize=";
        output+=mdisksz;
        output+="\n";
    }
    getin("Max Cabinet Size(Enter None For Skip,Byte)",mcabsz);
    if (isNum(mcabsz)){
        output+=".Set MaxCabinetSize=";
        output+=mcabsz;
        output+="\n";
        output+="\n";
    }
    getin("Enter File Path to add(Enter None For Skip):",tfile);
    while (!tfile.empty()){
        output+="\"";
        output+=tfile;
        output+="\"";"\n";
        output+="\"";"\n";
        getin("Enter File Path to add(Enter None For Skip):",tfile);
    }
    getin("Enter Path To Add(Enter None For Skip):",tdir);
    while (!tdir.empty()){
        includepath.push_back(tdir);
        getin("Enter Path To Add(Enter None For Skip):",tdir);
    };
    ofstream file(ddffn);
    file<<output;
    for (string& path:includepath){
        if (!fs::exists(path)) continue;
        cout<<"Extracting File Paths from "<<path<<"\n";
        try {
            for (const auto& entry:fs::recursive_directory_iterator(path,fs::directory_options::skip_permission_denied)){
                if (entry.is_regular_file()){
                    file<<"\""<<entry.path().string().c_str()<<"\""<<endl;
                }
            }
        } catch(const fs::filesystem_error& e){
        continue;
        }
    }
    file.flush();
    file.close();
    return 0;
}
