#include "iash.h"

iash::iash (string app_nm, bool useInPrompt)
{
    cout<<"iash version 0.1 initializing..."<<endl;

    m_appName = app_nm;
    f_useAppNameInPrompt = useInPrompt;

    setenv("IASH_APP_NAME",m_appName);
    setenv("IASH_APP_NAME_IN_PROMPT",f_useAppNameInPrompt);
    setenv("IASH_SYNC_ENV", true);
}

vector<string> iash::getCmdLine()
{
    string raw;
    unsigned space, lastSpace;
    vector<string> cmdLine;

    clear();

    if (getenv_bool("IASH_SYNC_ENV"))
        updateAttached();

    do
    {
        if (f_useAppNameInPrompt)
            cout<<m_appName<<"> ";
        else
            cout<<">>> ";

        getline(cin,raw);
    } while (raw == "");

    lastSpace = 0;

    do
    {
        space = raw.find(' ',lastSpace);

        if (lastSpace == 0 && space == string::npos)
            cmdLine.push_back(raw);
        else if (space == string::npos)
            cmdLine.push_back(raw.substr(lastSpace));
        else
        {
            cmdLine.push_back(raw.substr(lastSpace,(space-lastSpace)));
            lastSpace = space+1;
        }

    }while(space != string::npos);

    if (cmdLine[0] == "iash")
    {
        if (cmdLine.size() > 1)
            debugConsole(cmdLine);
        else
            debugConsole();
    }
    m_cmdLine = cmdLine;

    return cmdLine;
}

vector<string> iash::getOptions(vector<string> cmd)
{
    int x,y;
    vector<string> options;

    for (x=0; x < cmd.size(); x++)
    {
        if (cmd.at(x).find('-') != string::npos)
        {
            if (cmd.at(x).find("--") != string::npos)
            {
                options.push_back(cmd.at(x).substr(cmd.at(x).find("--")+2));
            }
            else if (cmd.at(x).length() > 2)
            {
                for (y = 1; y < cmd.at(x).length(); y++)
                    options.push_back(std::string(1,cmd[x][y]));
            }
            else
                options.push_back(cmd.at(x).substr(1));
        }
    }

    return options;
}

void iash::cmdNotFound()
{
    cout<<m_cmdLine[0]<<": command not found"<<endl;
}

void iash::cmdNotFound_dbg()
{
    cout<<m_cmdLine[1]<<": command not found"<<endl;
}

void iash::clearScreen()
{
    #ifdef __unix__
        if (!cur_term)
        {
            int result;
            setupterm(NULL, STDOUT_FILENO, &result);
            if (result <= 0)
            return;
        }

        putp(tigetstr("clear"));
    #elif _WIN32
        HANDLE                     hStdOut;
          CONSOLE_SCREEN_BUFFER_INFO csbi;
          DWORD                      count;
          DWORD                      cellCount;
          COORD                      homeCoords = { 0, 0 };

          hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
          if (hStdOut == INVALID_HANDLE_VALUE) return;

          /* Get the number of cells in the current buffer */
          if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
          cellCount = csbi.dwSize.X *csbi.dwSize.Y;

          /* Fill the entire buffer with spaces */
          if (!FillConsoleOutputCharacter(
            hStdOut,
            (TCHAR) ' ',
            cellCount,
            homeCoords,
            &count
            )) return;

          /* Fill the entire buffer with the current colors and attributes */
          if (!FillConsoleOutputAttribute(
            hStdOut,
            csbi.wAttributes,
            cellCount,
            homeCoords,
            &count
            )) return;

          /* Move the cursor home */
          SetConsoleCursorPosition( hStdOut, homeCoords );
    #endif
}

void iash::setenv(string name, string value)
{
    int x;

    for (x = 0; x < name.length(); x++)
        name[x] = toupper(name[x]);

    if (m_env.find(name) == m_env.end())
        m_env[name] = value;
    else
        m_env.at(name) = value;
}

void iash::setenv(string name, bool value)
{
    if (value)
    {
        setenv(name,(string)"true");
    }
    else
    {
        setenv(name,(string)"false");
    }
}

string iash::getenv_string(string name)
{
    int x;

    for (x = 0; x < name.length(); x++)
        name[x] = toupper(name[x]);

    return m_env.at(name);
}

bool iash::getenv_bool(string name)
{
    if (getenv_string(name) == "true")
        return true;
    else
        return false;
}

void iash::rmenv(string name)
{
    int x;

    for (x = 0; x < name.length(); x++)
        name[x] = toupper(name[x]);

    if (name.find("IASH_") == string::npos)
        m_env.erase(name);
    else
        cout<<"Error: cannot remove required environment variable '"<<name<<"'."<<endl;
}

void iash::setAppName(string app_nm)
{
    m_appName = app_nm;
    setenv("IASH_APP_NAME",app_nm);
}

void iash::useAppNameInPrompt(bool name)
{
    f_useAppNameInPrompt = name;
    setenv("IASH_APP_NAME_IN_PROMPT",name);
}

void iash::debugConsole()
{
    vector<string> cmdLine;

    m_appName = "iash";
    f_useAppNameInPrompt = true;

    setenv("IASH_SYNC_ENV",false);

    do {
        cmdLine = getCmdLine();
        cmdLine.insert(cmdLine.begin(),"iash");

        debugConsole(cmdLine);
    } while (cmdLine[1] != "exit" && cmdLine[1] != "quit");

    setenv("IASH_SYNC_ENV",true);

    m_appName = getenv_string("IASH_APP_NAME");
    f_useAppNameInPrompt = getenv_bool("IASH_APP_NAME_IN_PROMPT");
    clear();
}

void iash::debugConsole(vector<string> cmd)
{
    int x;

    if (cmd[1] == "cnf")
    {
        cout<<"DBG: IASH_CNF_TEST"<<endl;
        cmdNotFound_dbg();
    }
    else if (cmd[1] == "clear")
    {
        if (cmd.size() > 2)
        {
            if (cmd[2] == "screen")
                clearScreen();
            else if (cmd[2] == "buf" || cmd[2] == "buffer")
                clear();
            else
                cout<<cmd[2]<<": not a valid clear argument"<<endl;
        }
        else
            clear();
    }
    else if (cmd[1] == "disp")
    {
        if (cmd.size() > 2)
        {
            if (cmd[2] == "cmd")
            {
                for (x = 0; x < cmd.size(); x++)
                    cout<<cmd.at(x)<<endl;
            }
            else if (cmd[2] == "ops")
            {
                vector<string> ops;
                ops = getOptions(cmd);
                for (x = 0; x < ops.size(); x++)
                    cout<<ops.at(x)<<endl;
            }
            else if (cmd[2] == "env")
            {
                map<string,string>::iterator it;

                for (it = m_env.begin(); it != m_env.end(); ++it)
                    cout<<it->first<<' '<<it->second<<endl;
            }
            else
                cout<<cmd[2]<<": invalid "<<cmd[1]<<" argument"<<endl;
        }
        else
            cout<<cmd[1]<<": need an argument"<<endl;
    }
    else if (cmd[1] == "set")
    {
        if (cmd.size() > 2)
        {
            if (cmd[2] == "env")
            {
                if (cmd.size() == 5)
                {
                    if (cmd[3] == "-r")
                        rmenv(cmd[4]);
                    else
                        setenv(cmd[3],cmd[4]);
                }
                else
                    cout<<"Error: improper usage"<<endl<<"Usage: iash set env (-r) [NAME] [VALUE]"<<endl;
            }
            else if (cmd[2] == "appInPrompt")
            {
                if (cmd.size() > 3)
                {
                    if (cmd[3] == "on")
                        useAppNameInPrompt();
                    else if (cmd[3] == "off")
                        useAppNameInPrompt(false);
                    else
                        cout<<"Error: improper usage"<<endl<<"Usage: iash set appInPrompt <on/off> (optional)"<<endl;
                }
                else
                    useAppNameInPrompt();
            }
            else
                cout<<cmd[2]<<": invalid "<<cmd[1]<<" argument"<<endl;
        }
        else
            cout<<cmd[1]<<": need an argument"<<endl;
    }
    else if (cmd[1] == "app")
    {
        if (cmd.size() > 2)
        {
            if (cmd[2] == "quit" || cmd[2] == "exit")
                exit(EXIT_SUCCESS);
            else if (cmd[2] == "kill" || cmd[2] == "crash")
                exit(EXIT_FAILURE);
            else if (cmd[2] == "name" && cmd.size() == 4)
                setenv("IASH_APP_NAME",cmd[3]);
            else
                cout<<cmd[2]<<": invalid "<<cmd[1]<<" argument"<<endl;
        }
        else
            cout<<getenv_string("IASH_APP_NAME")<<endl;
    }
    else if (cmd[1] != "exit" && cmd[1] != "quit")
        cmdNotFound_dbg();
}

void iash::updateAttached()
{
    if (getenv_string("IASH_APP_NAME") != m_appName)
        m_appName = getenv_string("IASH_APP_NAME");

    if (getenv_bool("IASH_APP_NAME_IN_PROMPT") != f_useAppNameInPrompt)
        f_useAppNameInPrompt = getenv_bool("IASH_APP_NAME_IN_PROMPT");
}
