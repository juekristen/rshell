#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <string.h>

using namespace std;
class CommandLine
{
    public:
        void start(vector <string> &vctr)
        {
            string cmd;
            char *command;
            cout << "$ ";
            getline(cin, cmd);
            command = new char [cmd.length()];
            cout << "1" << endl;
            for(int i = 0; i < cmd.size(); ++i)
            {
                if(cmd.size() == 0)
                return;
                command[i] = cmd[i];
            }
            cout << "2" << endl;
            command[cmd.size()] = '\0';
            //vector <char *> test;
            char *temp;
            temp = strtok (command," ;");
            cout << "3" << endl;
            if(cmd.size() == 0)
                {
                    return;
                }
            while (temp != '\0')
            {
                vctr.push_back(temp);
                temp = strtok ('\0', "  ;");
            }
            vctr.push_back("\0");
            cout << "4" << endl;
            int t = 0;
            for(vector<string>::iterator i = vctr.begin(); i != vctr.end();++i)
            {
                if((t + 1) % 3 == 0)
                {
                    if(*i != "||" && *i != "&&")
                    {
                        vctr.insert(i,";");
                    }
                }
                ++t;
            }
            cout << 5 << endl;
            
            //delete[] temp;
            return;
        }
};
class Execute
{
    public:
        void go(string cmd, string flag)
        {
            pid_t pid, lpid;
            int status;
            pid = fork();  
            const char* args[3] = { cmd.c_str(), flag.c_str(), '\0'} ;
            
            if(pid < 0)
            {                                                                                                                                  
                perror("fork failed");
                exit(1);                                                                                                                                        
            }
            else if( pid == 0 )
            {
                execvp( args[0], (char**)args); 
                perror("execve failed");
                //printf("Child: I'm the child: %d\n", pid);
            }
            else if (pid > 0)
            {
                //printf("Parent: I'm the parent: %d\n", pid);
                if( (lpid = wait(&status)) < 0)
                {
                  perror("wait");
                  exit(1);
                }

            }
            
        }

};

int main(int argc, char *argv[])
{
    vector <string> lst;
    CommandLine cmd;
    Execute ex;
    cmd.start(lst);
    //ex.go("mkdir", "kjue");
    cout << 5 << endl;
    if(lst.size() != 0)
    {
        for (int i = 0; i < lst.size()-2; i = i + 3)
        {
            ex.go(lst.at(i), lst.at(i+1));
            cout << lst.at(i) << endl;
        }
    }
    cout << 6 << endl;
    while(lst.size() == 0 || lst.at(0) != "exit")
    {
        lst.clear();
        cmd.start(lst);
        if(lst.size() != 0 && lst.at(0) != "exit")
        {
            for (int i = 0; i < lst.size()-1; ++i)
            {
                //ex.go(lst.at(i));
                cout << lst.at(i) << endl;
            }
        }
        
    }
    cout << "done" << endl;
    
    return 0;
}