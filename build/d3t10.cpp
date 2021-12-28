#include <bits/stdc++.h>
#include "sys/wait.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"
using namespace std;
using namespace fmt;
string HELP_STR="";

vector<string> prase(const string &a){
    vector<string> v;
    for (size_t i=0;i<a.size();){
        for (;i<a.size()&&a[i]==' ';i++);
        if (i<a.size()){
            int j=0;
            for (;j+i<a.size()&&a[j+i]!=' ';j++);
            v.push_back(a.substr(i,j));
            i=j;
        }
    }
    return v;
}

extern char **environ;

int exec(const string& file,
        const vector<string>& args,
        const map<string,string>& env){
    vector<string> v;
    vector<char*> arge(args.size()),enve(env.size());
    for (size_t i=0;i<args.size();i++)
        arge[i]=const_cast<char*>(args[i].c_str());
    for (auto& [p,q]:env) v.push_back(p+"="+q);
    for (size_t i=0;i<args.size();i++)
        enve[i]=const_cast<char*>(v[i].c_str());
    arge.push_back(0);
    enve.push_back(0);
    if (int id=fork()){
        wait(0);
        return 0;
    }
    else{
        execvpe(file.c_str(),&arge[0],&enve[0]);
    }
}
void loop(istream& is,ostream& os,map<string,string> env){
    string& workingdir=env["PWD"];
    auto ls=[&](){
        return exec("/bin/ls",{"ls"},env);
    };
    auto pwd=[&](){
        os<<workingdir<<endl;
    };
    while (1){
        os<<format("[{}:{}]$",env["USER"],workingdir);
        string str;
        getline(is,str);
        auto args=prase(str);
        for (auto i:args) os<<i<<endl;
        auto F=[&]()->optional<int>{
            if (args.size()>0){
                if (args[0]=="cd"){
                    if (args.size()==1){
                        pwd();
                        return 0;
                    }else{
                        
                    }
                }else if (args[0]=="quit"||
                          args[0]=="exit"||
                          args[0]=="bye"){
                    exit(0);
                }else{
                    return exec(args[0],args,env);
                }
            }
            return {};
        }();
        if (F){
        }else{
            os<<"Input Fail"<<endl;
        }
    }
}

int main(int argc,char *argv[]){
    vector<string> args{argv,argv+argc};
    map<string,string> env;
    for (char **c=environ;*c&&**c;c++){
        string line=*c;
        int w=line.find("=");
        env[line.substr(0,w)]=line.substr(w+1);
    }
    loop(cin,cout,move(env));
}