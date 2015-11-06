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
                cout << 4.1 << endl;
                if((t + 1) % 3 == 0)
                {
                    if((vctr.size() - 1) % 3 == 0)
                    {
                        cout << vctr.size() << endl;
                        cout << 4.2 << endl;
                        if(t > vctr.size() - 2)
                        {
                            vctr.insert(i,";");
                            cout << 4.3 << endl;
                        }
                    }
                    else if(*i != "||" && *i != "&&")
                    {
                        cout << 4.4 << endl;
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
        void go(string cmd, string flag, bool &state)
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
                state = true;
                exit(1);
                //printf("Child: I'm the child: %d\n", pid);
            }
            else if (pid > 0)
            {
                //printf("Parent: I'm the parent: %d\n", pid);
                if( (lpid = wait(&status)) < 0)
                {
                    perror("wait");
                    state = true;
                    exit(1);
                }

            }
            
            state = false;
            return;
        }
        
        void go(string cmd, bool &state)
        {
            pid_t pid, lpid;
            int status;
            pid = fork();  
            const char* args[2] = { cmd.c_str(), '\0'} ;
            
            if(pid < 0)
            {                                                                                                                                  
                perror("fork failed");
                exit(1);                                                                                                                                        
            }
            else if( pid == 0 )
            {
                execvp( args[0], (char**)args); 
                perror("execve failed");
                state = true;
                exit(1);
                //printf("Child: I'm the child: %d\n", pid);
            }
            else if (pid > 0)
            {
                //printf("Parent: I'm the parent: %d\n", pid);
                if( (lpid = wait(&status)) < 0)
                {
                    //wait(1);
                    perror("wait");
                    state = true;
                    exit(1);
                }

            }
            
                state = false;
                return;
            
        }

};

class Connector
{
    public:
        bool ors(bool one)
        {
            if(one)
            {
               return true;
            }
           return false; 
        }
        //make sure logic is right
        bool ands(bool one)
        {
            if(one)
            {
                return false;
            }
            return true;
        }
        bool semi(bool one)
        {
            return false;
        }
        bool hash()
        {
            return true;
        }
};

int main(int argc, char *argv[])
{
    vector <string> lst;
    vector <bool> executed;
    CommandLine cmd;
    Execute ex;
    Connector connect;
    bool state;
    cmd.start(lst);
    //ex.go("mkdir", "kjue");
    cout << 5 << endl;
    if(lst.size() != 0)
    {
        if(lst.size() <= 6)
        {
            ex.go(lst.at(0),lst.at(1),state = false);
        }
        if(lst.size() > 6)
        {
            cout << lst.size() << endl;
            if(lst.at(0) == "#" || lst.at(3) == "#")
            {
                return 0;
            }
            if(lst.at(1) == "#")
            {
                ex.go(lst.at(1), state = false);
                return 0;
            }
            if(lst.at(4) == "#")
            {
                ex.go(lst.at(4), state = false);
                return 0;
            }
            if(lst.at(2) == "||")
            {
                ex.go(lst.at(0),lst.at(1), state = false);
                bool stat = connect.ors(state);
                executed.push_back(stat);
                if(stat)
                {
                    ex.go(lst.at(3),lst.at(4),state = false);
                    executed.push_back(state);
                }
            }
            else if(lst.at(2) == "&&")
            {
                ex.go(lst.at(0), lst.at(1), state = false);
                bool stat = connect.ands(state);
                //cout << stat << endl;
                executed.push_back(stat);
                if(stat)
                {
                    cout << "true" << endl;
                    ex.go(lst.at(3),lst.at(4),state = false);
                    executed.push_back(state);
                }
            }
            else if(lst.at(2) == ";")
            {
                ex.go(lst.at(0), lst.at(1), state = false);
                executed.push_back(state);
                ex.go(lst.at(3), lst.at(4), state = false);
                executed.push_back(state);
            }
        }
    }
    //     for (int i = 0; i < lst.size()-2; i = i + 3)
    //     {
            
    //         if(lst.at(i) == "#")
    //         {
    //             return 0;
    //         }
    //         if(lst.at(i+1) == "#")
    //         {
    //             ex.go(lst.at(i));
    //             return 0;
    //         }
    //         if(lst.at(i + 2) == "||")
    //         {
    //             bool stat = connect.ors(ex.go(lst.at(i), lst.at(i+1)));
    //             executed.push_back(stat);
    //             // if(!stat)
    //             // {
    //             //     executed.push_back()
    //             // }
    //         }
            
    //         else if(lst.at(i + 2) == "&&")
    //         {
    //             executed.push_back(connect.ands(ex.go(lst.at(i), lst.at(i+1))));
    //         }
             
    //         // else if(lst.at(i + 2) == ";")
    //         // {
    //         //     executed.push_back()
    //         // }
    //         cout << ex.go(lst.at(i), lst.at(i+1)) << endl;
    //         cout << lst.at(i) << endl;
    //     }
    
    // cout << 6 << endl;
    // while(lst.size() == 0 || lst.at(0) != "exit")
    // {
    //     lst.clear();
    //     cmd.start(lst);
    //     if(lst.size() != 0 && lst.at(0) != "exit")
    //     {
    //         for (int i = 0; i < lst.size() - 2; i = i + 3)
    //         {
    //             ex.go(lst.at(i), lst.at(i+1));
    //             cout << lst.at(i) << endl;
    //         }
    //     }
        
    // }
    // cout << "done" << endl;
    
    return 0;
}