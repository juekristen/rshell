#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <string.h>
#include <errno.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
using namespace std;
using namespace boost;

class CommandLine
{
    public:
        //takes user input and puts it into a vector
        void split(vector <string> &vctr, char seper, string cmd)
        {
            int state = 0;
            if(cmd.at(0) == '(' && seper == '(')
                 state = 1;
            if(cmd.at(cmd.size()-1) == ')' && seper == ')')
                state = 2;
            int a = 0,b = 0;
            char* sepa;
            sepa = &seper;
            string s1(1,seper);
            string s2(2,seper);
            char_separator<char> sep(sepa);
            tokenizer< char_separator<char> > tokens(cmd, sep);
            BOOST_FOREACH (const string& t, tokens) 
            {
                if(t != "/0")
                ++a;
            }
            BOOST_FOREACH (const string& t, tokens) 
            {
                ++b;
                
                //if(seper )
                if (state == 1)
                {
                    vctr.push_back(s1);
                }
                if(t != " ")
                vctr.push_back(t);
                if(state == 2)
                {
                    vctr.push_back(s1);
                }
                if(b != a)
                {
                    if(seper == ';' || seper == '#'|| seper == '(' || seper == ')')
                    vctr.push_back(s1);
                    else if(seper == '|' || seper == '&')
                    vctr.push_back(s2);
                }
                else if(seper == ')' && b == a && a != 1 && t != " " )
                    vctr.push_back(s1);
                
                
                //cout << t << "." << endl;
            }
        }
        
        void start(vector <string> &vctr)
        {
            string cmd;
            vector <string> temp;
            
            cout << "$ ";
            getline(cin, cmd);
            split(vctr,';',cmd);
            for(int i = 0; i < vctr.size(); ++i)
                split(temp,'|',vctr.at(i));
            vctr.clear();
            for(int i = 0; i < temp.size(); ++i)
                split(vctr, '&', temp.at(i));
            temp.clear();
            for(int i = 0; i < vctr.size(); ++i)
                split(temp, '(', vctr.at(i));
            vctr.clear();
            for(int i = 0; i < temp.size(); ++i)
                split(vctr, '#', temp.at(i));
            temp.clear();
            for(int i = 0; i < vctr.size(); ++i)
                split(temp, ')', vctr.at(i));
            temp.swap(vctr);
            return;
        }
};
class Execute
{
    public:
        // executes commands if there is a command and a flag also records the state
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
                    // if execution does not succeed then the state is true
                    if(status != 0)
                    {
                        state= true;
                    }
                }
            return;
            }
        }
        // executes a command without a flag and records state
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
                
            }
            else if (pid > 0)
            {
                wait(&status);
                if(WIFEXITED(status))
                {
                    if(status != 0)
                    {
                        state= true;
                    }
                }
            
            }
        }
        

};

class Connector
{
    public:
        // logic for or connector
        bool ors(bool one)
        {
            if(one)
            {
               return true;
            }
           return false; 
        }
        //logic for and connector
        bool ands(bool one)
        {
            if(one)
            {
                return true;
            }
            return false;
        }
        //logic for semicolon 
        bool semi(bool one)
        {
            return false;
        }
        // logic for hash function
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
    // executes first command line
    if(lst.size() != 0)
    {
        //  if the command line has less than two arguements
        if(lst.size() <= 6)
        {
            if(lst.at(0) == "exit")
                return 0;
            if(lst.at(0) == "#")
            {
                
            }
            else if(lst.size() == 2)
            {
                ex.go(lst.at(0), states = false);
            }
            else if(lst.at(1) == "#")
            {
                ex.go(lst.at(0), states = false);
            }
            else
            ex.go(lst.at(0),lst.at(1),states = false);
        }
        // if the command line has at least 2 commands
        if(lst.size() > 6)
        {
            if(lst.at(1) == "#")
            {
                ex.go(lst.at(0), states = false);
            }
            else if(lst.at(4) == "#")
            {
                ex.go(lst.at(3), states = false);
            }
            else if(lst.at(2) == "||")
            {
                //state = false;
                ex.go(lst.at(0),lst.at(1), states);
                //cout << states << endl;
                bool stat = connect.ors(states);
                //cout << stat << endl;
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
                //cout << states << endl;
                bool stat = connect.ands(states);
                //cout << stat << endl;
                executed.push_back(stat);
                if(!stat)
                {
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
        //if the command line has more than 2 commands
        if(lst.size() > 7)
        {
            //cout << lst.at(6) << endl;
            for (unsigned int i = 5; i < lst.size()-2; i = i + 3)
            {
                if(lst.at(i+ 2) == "#")
                {
                    ex.go(lst.at(i + 1), states = false);
                }
                else if(lst.at(i) == "||")
                {
                    if(executed.at(executed.size()-1))
                    {
                        ex.go(lst.at(i + 1), lst.at(i + 2), states = false);
                        executed.push_back(states);
                    }
                }
                
                else if(lst.at(i) == "&&")
                {
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
        
    //executes subsequent command lines
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
                    if(lst.at(0) == "#")
                    {
                        
                    }
                    else if(lst.at(1) == "#")
                    {
                        ex.go(lst.at(0), states = false);
                    }
                    else if(lst.size() == 2)
                    {
                        ex.go(lst.at(0), states = false);
                    }
                    else
                    ex.go(lst.at(0),lst.at(1),states = false);
                }
                if(lst.size() > 6)
                {
                    if(lst.at(1) == "#")
                    {
                        ex.go(lst.at(0), states = false);
                    }
                    else if(lst.at(4) == "#")
                    {
                        ex.go(lst.at(3), states = false);
                    }
                    else if(lst.at(2) == "||")
                    {
                        //state = false;
                        ex.go(lst.at(0),lst.at(1), states);
                        //cout << states << endl;
                        bool stat = connect.ors(states);
                        //cout << stat << endl;
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
                        //cout << states << endl;
                        bool stat = connect.ands(states);
                        //cout << stat << endl;
                        executed.push_back(stat);
                        if(!stat)
                        {
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
                //cout << lst.at(6) << endl;
                for (unsigned int i = 5; i < lst.size()-2; i = i + 3)
                {
                    if(lst.at(i+ 2) == "#")
                    {
                        ex.go(lst.at(i + 1), states = false);
                        return 0;
                    }
                    else if(lst.at(i) == "||")
                    {
                        if(executed.at(executed.size()-1))
                        {
                            ex.go(lst.at(i + 1), lst.at(i + 2), states = false);
                            executed.push_back(states);
                        }
                    }
                    
                    else if(lst.at(i) == "&&")
                    {
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
   
    return 0;
}