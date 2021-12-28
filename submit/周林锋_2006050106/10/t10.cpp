
#include <bits/stdc++.h>
#include "sys/wait.h"
#include <dirent.h>
#define FMT_HEADER_ONLY
#include "fmt/format.h"//:fmt:库
using namespace std;
using namespace fmt;

vector<string> prase(const string &a){//以空格解析参数
    vector<string> v;
    for (size_t i=0;i<a.size();){
        while (i<a.size()&&a[i]==' ') i++;
        int j=0;
        while (j+i<a.size()&&a[j+i]!=' ') j++;
        if (j>0) v.push_back(a.substr(i,j));
        i+=j;
    }
    return v;
}

extern char **environ;
#define debug(...) print(__VA_ARGS__)
#define debug(...) ;
int exec(const string& file,
        const vector<string>& args,
        const map<string,string>& env){//以参数args，环境变量env开新进程执行file
    debug("exec {}\n",file);
    vector<string> v;
    vector<char*> arge(args.size()),enve(env.size());
    for (size_t i=0;i<args.size();i++)
        arge[i]=const_cast<char*>(args[i].c_str());
    for (auto& [p,q]:env) v.push_back(p+"="+q);
    for (size_t i=0;i<args.size();i++)
        enve[i]=const_cast<char*>(v[i].c_str());
    arge.push_back(0);
    enve.push_back(0);
    if (int id=fork()){//fork出一个新进程
        wait(0);
        return 0;
    }
    else{
        return execvpe(file.c_str(),&arge[0],&enve[0]);//如果返回值为负数说明执行失败
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
        os<<format("[{}:{}]$",env["USER"],workingdir);//打印命令行前缀
        string str;
        getline(is,str);
        auto args=prase(str);
        for (auto i:args) debug("{}\n",i);
        auto F=[&]()->optional<int>{
            if (args.size()>0){
                if (args[0]=="cd"){//切换切换工作目录
                    if (args.size()==1){//不跟参数
                        pwd();
                        return 0;
                    }else{//跟参数，执行切换工作目录
                        string w=args[1][0]=='/'?args[1]:workingdir+"/"+args[1];
                        if (DIR* dir=opendir(w.c_str())){
                            chdir(w.c_str());
                            closedir(dir);
                            return 0;
                        }else{
                            os<<"Floder not exist!"<<endl;
                            return 1;
                        }
                    }
                }else if (args[0]=="quit"||
                          args[0]=="exit"||
                          args[0]=="bye"){//推出
                    exit(0);
                }else if (args[0]=="environ"){//打印环境变量
                    for (auto [p,q]:env)
                        os<<format("{}={}\n",p,q);
                    return 0;
                }else if (args[0]=="jobs"){//打印当前执行进程
                    args[0]="ps";
                }else if (args[0]=="help"){//打印帮助
                    os<<"帮助："<<endl;
                    os<<"进入目录：cd [directory]"<<endl;
                    os<<"显示执行的程序：job"<<endl;
                    os<<"显示环境变量：environ"<<endl;
                    return 0;
                }
                if (exec(args[0],args,env)){//执行程序返回非0，执行失败
                    os<<"Command Not Found!"<<endl;
                    exit(0);
                }
                return 0;
            }
            return {};
        }();
        char wd[1024];//替换环境变量PWD
        if (getcwd(wd,1024))
            workingdir=wd;
        else{
            os<<"Too long working path"<<endl; 
            exit(-1);
        } 
        if (F){
        }else{
            os<<"Input Fail"<<endl; 
        }
    }
}

int main(int argc,char *argv[]){
    vector<string> args{argv,argv+argc};
    map<string,string> env;
    for (char **c=environ;*c&&**c;c++){//解析环境变量
        string line=*c;
        int w=line.find("=");
        env[line.substr(0,w)]=line.substr(w+1);
    }
    loop(cin,cout,move(env));//开始事件循环
}