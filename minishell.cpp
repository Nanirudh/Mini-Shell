#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/types.h>
#include<iostream>
#include<algorithm>
#include<vector>
#include<string>
#include<fstream>
#include <sys/wait.h>
#include <sstream>
#include<fcntl.h>
using namespace std;

void take_input();

void prin_side()
{

	char *buf;
    buf=(char *)malloc(100*sizeof(char));
    getcwd(buf,100);
    printf("%s:~$",buf);
    take_input();
}

void parse(string str)
{
    
    stringstream ss(str);

    vector<string>v;

    string sp;
    while(getline(ss,sp,'|'))
    {
       v.push_back(sp);
    }

    int p[2];
    pid_t pid;
    int  fd_in = 0;
    int ind=0;

    while(ind<v.size())
    {
        pipe(p);
        pid=fork();
    

        if (pid == 0)
        {
          dup2(fd_in, 0); //change the input according to the old one 
          if ((ind + 1) != v.size())
            dup2(p[1], 1);
          else
          {
             int pos=v[ind].find(">");
             if(pos!=-1)
             {
                int len=v[ind].length();
                string comm=v[ind].substr(0,pos);
                v[ind]=v[ind].substr(pos+1,len-pos-1);
               
                int fdout=open(v[ind].c_str(),O_WRONLY|O_CREAT);
                v[ind]=comm;
               
                int df=dup2(fdout,1);
                
             }
          }
          close(p[0]);
          //execvp((*cmd)[0], *cmd);
          char* arg[5];
          for(int i=0;i<5;i++)
            arg[i]=NULL;

          int cnt=0;
          stringstream st(v[ind]);
          string div[5];
          while(getline(st,div[cnt],' '))
          {
            arg[cnt]=const_cast<char*>(div[cnt].c_str());
            cnt++;
          }
          execvp(arg[0],arg);
          exit(EXIT_FAILURE);
        }
        else
        {
          wait(NULL);
          close(p[1]);
          fd_in = p[0]; //save the input for the next command
          ind++;
        }

    }
     

}

void take_input()
{
	string input_command;
    
    getline(cin,input_command);
    parse(input_command);
    

}

int main()
{

	prin_side();
	return 0;
}