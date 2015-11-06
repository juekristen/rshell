#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <string.h>
#include <errno.h>

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
            int status;
            pid_t pid;
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
                //cout << "before" << state << endl;
                //state = true;
                int err = errno;
                perror("execve failed");
                //cout << "after" << state << endl;
                exit(err);
                
                //printf("Child: I'm the child: %d\n", pid);
            }
            else if (pid > 0)
            {
                wait(&status);
                //cout << "status" << status << endl;
                
                if(WIFEXITED(status))
                {
                    if(status != 0)
                    {
                        state= true;
                    }
                }
                //printf("Parent: I'm the parent: %d\n", pid);
                // if( (lpid = wait(&status)) < 0)
                // {
                //     wait(0);
                //     state = true;
                //     exit(1);
                //     return;
                //     perror("wait");
                //     state = true;
                //     exit(1);
                // }

            }
            
//             if(pid != 0)
//         {
//             state = true;
//             cout << "state" << endl;
// }
            return;
        }
        
        void go(string cmd, bool &state)
        {
            pid_t pid;
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
                //cout << "before" << state << endl;
                //state = true;
                int err = errno;
                perror("execve failed");
                //cout << "after" << state << endl;
                exit(err);
                
                //printf("Child: I'm the child: %d\n", pid);
            }
            else if (pid > 0)
            {
                wait(&status);
                //cout << "status" << status << endl;
                
                if(WIFEXITED(status))
                {
                    if(status == 0)
                    {
                        return;
                    }
                    else if(status != 0)
                    {
                        state= true;
                    }
                }
                //return;
            
        }
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
                return true;
            }
            return false;
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
    bool states = false;
    cmd.start(lst);
    //ex.go("mkdir", "kjue");
    cout << 5 << endl;
    if(lst.size() != 0)
    {
        if(lst.size() <= 6)
        {
            ex.go(lst.at(0),lst.at(1),states = false);
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
                ex.go(lst.at(1), states = false);
                return 0;
            }
            if(lst.at(4) == "#")
            {
                ex.go(lst.at(4), states = false);
                return 0;
            }
            if(lst.at(2) == "||")
            {
                //state = false;
                ex.go(lst.at(0),lst.at(1), states);
                cout << states << endl;
                bool stat = connect.ors(states);
                cout << stat << endl;
                executed.push_back(stat);
                if(stat)
                {
                    ex.go(lst.at(3),lst.at(4),states = false);
                    executed.push_back(states);
                }
            }
            else if(lst.at(2) == "&&")
            {
                ex.go(lst.at(0), lst.at(1), states);
                cout << states << endl;
                bool stat = connect.ands(states);
                cout << stat << endl;
                executed.push_back(stat);
                if(!stat)
                {
                    cout << "true" << endl;
                    ex.go(lst.at(3),lst.at(4),states = false);
                    executed.push_back(states);
                }
            }
            else if(lst.at(2) == ";")
            {
                ex.go(lst.at(0), lst.at(1), states = false);
                executed.push_back(states);
                ex.go(lst.at(3), lst.at(4), states = false);
                executed.push_back(states);
            }
        }
    }
        if(lst.size() > 7)
        {
            cout << "second" << endl;
            //cout << lst.at(6) << endl;
            for (int i = 5; i < lst.size()-2; i = i + 3)
            {
                
                if(lst.at(i + 1) == "#")
                {
                    return 0;
                }
                if(lst.at(i+ 2) == "#")
                {
                    ex.go(lst.at(i + 1), states = false);
                    return 0;
                }
                if(lst.at(i) == "||")
                {
                    if(executed.at(executed.size()-1))
                    {
                        ex.go(lst.at(i + 1), lst.at(i + 2), states = false);
                        executed.push_back(states);
                    }
                }
                
                else if(lst.at(i) == "&&")
                {
                    cout << "and" << endl;
                    if(!executed.at(executed.size()-1))
                    {
                        ex.go(lst.at(i + 1), lst.at(i + 2), states = false);
                        executed.push_back(states);
                    }
                }
                 
                else if(lst.at(i) == ";")
                {
                    ex.go(lst.at(i + 1), lst.at(i + 2), states = false);
                    executed.push_back(states);
                }
                
            }
        }
    cout << 6 << endl;
    while(lst.size() == 0 || lst.at(0) != "exit")
    {
        lst.clear();
        cmd.start(lst);
        if(lst.size() != 0 && lst.at(0) != "exit")
        {
            if(lst.size() != 0)
    {
        if(lst.size() <= 6)
        {
            ex.go(lst.at(0),lst.at(1),states = false);
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
                ex.go(lst.at(1), states = false);
                return 0;
            }
            if(lst.at(4) == "#")
            {
                ex.go(lst.at(4), states = false);
                return 0;
            }
            if(lst.at(2) == "||")
            {
                //state = false;
                ex.go(lst.at(0),lst.at(1), states);
                cout << states << endl;
                bool stat = connect.ors(states);
                cout << stat << endl;
                executed.push_back(stat);
                if(stat)
                {
                    ex.go(lst.at(3),lst.at(4),states = false);
                    executed.push_back(states);
                }
            }
            else if(lst.at(2) == "&&")
            {
                ex.go(lst.at(0), lst.at(1), states);
                cout << states << endl;
                bool stat = connect.ands(states);
                cout << stat << endl;
                executed.push_back(stat);
                if(!stat)
                {
                    cout << "true" << endl;
                    ex.go(lst.at(3),lst.at(4),states = false);
                    executed.push_back(states);
                }
            }
            else if(lst.at(2) == ";")
            {
                ex.go(lst.at(0), lst.at(1), states = false);
                executed.push_back(states);
                ex.go(lst.at(3), lst.at(4), states = false);
                executed.push_back(states);
            }
        }
    }
        if(lst.size() > 7)
        {
            cout << "second" << endl;
            //cout << lst.at(6) << endl;
            for (int i = 5; i < lst.size()-2; i = i + 3)
            {
                
                if(lst.at(i + 1) == "#")
                {
                    return 0;
                }
                if(lst.at(i+ 2) == "#")
                {
                    ex.go(lst.at(i + 1), states = false);
                    return 0;
                }
                if(lst.at(i) == "||")
                {
                    if(executed.at(executed.size()-1))
                    {
                        ex.go(lst.at(i + 1), lst.at(i + 2), states = false);
                        executed.push_back(states);
                    }
                }
                
                else if(lst.at(i) == "&&")
                {
                    cout << "and" << endl;
                    if(!executed.at(executed.size()-1))
                    {
                        ex.go(lst.at(i + 1), lst.at(i + 2), states = false);
                        executed.push_back(states);
                    }
                }
                 
                else if(lst.at(i) == ";")
                {
                    ex.go(lst.at(i + 1), lst.at(i + 2), states = false);
                    executed.push_back(states);
                }
                
            }
        }
        }
        
    }
    cout << "done" << endl;
    
    return 0;
}