#include "iash.h"

iash::iash (string app_nm, bool useInPrompt)
{
    fstream fs;
    stringstream ss (ios::in | ios::out);

    cout<<"iash version 0.2 'Firmament' initializing..."<<endl;

    m_appName = app_nm;
    f_useAppNameInPrompt = useInPrompt;

    ss<<CrossLib::getConfigDir()<<m_appName<<'/';

    setEnv("IASH_CONFIG_PATH", ss.str());

    ss.clear();
    ss.str("");

    if (CrossLib::isdir(getEnv_string("IASH_CONFIG_PATH").c_str()))
        CrossLib::mkdir(getEnv_string("IASH_CONFIG_PATH").c_str());

    ss<<getEnv_string("IASH_CONFIG_PATH")<<"iashenv";

    if (CrossLib::isfile(ss.str().c_str()))
    {
        if (!loadEnv())
        {
            cout<<"Error opening config file. Starting default environment."<<endl;

            setEnv("IASH_APP_NAME",m_appName);
            setEnv("IASH_APP_NAME_IN_PROMPT",f_useAppNameInPrompt);
            setEnv("IASH_SYNC_ENV", true);
            setEnv("IASH_DEBUG_ACTIVE", false);
        }
    }
    else
    {
        setEnv("IASH_APP_NAME", m_appName);
        setEnv("IASH_APP_NAME_IN_PROMPT", f_useAppNameInPrompt);
        setEnv("IASH_SYNC_ENV", true);
        setEnv("IASH_DEBUG_ACTIVE", false);

        if (!saveEnv())
            cout<<"Error creating configuration. Please check your filesystem."<<endl;
    }

}

iash::~iash ()
{
    if (!saveEnv())
        cout<<"Error saving environment. Please check your filesystem."<<endl;
}

vector<string> iash::getCmdLine()
{
    string raw;
    vector<string> cmdLine;

    clear();

    updateAttached();

    do
    {
        if (getEnv_bool("IASH_DEBUG_ACTIVE"))
            cout<<"iash> ";
        else
        {
            if (f_useAppNameInPrompt)
                cout<<m_appName<<"> ";
            else
                cout<<">>> ";
        }

        getline(cin,raw);
    } while (raw == "");

    cmdLine = parseCmdLine(raw);

    m_cmdLine = cmdLine;

    if (cmdLine[0] == "iash")
    {
        if (cmdLine.size() > 1)
            debugConsole(cmdLine);
        else
            debugConsole();
    }

    return cmdLine;
}

vector<string> iash::parseCmdLine(string raw)
{
    vector<string> cmdLine;
    unsigned space, lastSpace;

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

void iash::cmdNotFound_dbg(string cmd)
{
    cout<<cmd<<": command not found"<<endl;
}

void iash::setEnv(string name, string value)
{
    int x;

    name = convEnv(name);

    if (m_env.find(name) == m_env.end())
        m_env[name] = value;
    else
        m_env.at(name) = value;
}

void iash::setEnv(string name, bool value)
{
    if (value)
    {
        setEnv(name,(string)"true");
    }
    else
    {
        setEnv(name,(string)"false");
    }
}

string iash::getEnv_string(string name)
{
    int x;

    name = convEnv(name);

    return m_env.at(name);
}

bool iash::getEnv_bool(string name)
{
    if (getEnv_string(name) == "true")
        return true;
    else
        return false;
}

void iash::rmenv(string name)
{
    int x;

    name = convEnv(name);

    if (name.find("IASH_") == string::npos)
        m_env.erase(name);
    else
        cout<<"Error: cannot remove required environment variable '"<<name<<"'."<<endl;
}

bool iash::saveEnv()
{
    stringstream ss;

    ss<<getEnv_string("IASH_CONFIG_PATH")<<"iashenv";

    return saveEnv(ss.str());
}

bool iash::saveEnv(string filepath)
{
    ofstream fout;
    bool fileOpened;

    fout.open(filepath.c_str(), ios_base::trunc);

    if (fout.is_open())
    {
        fileOpened = true;

        map<string,string>::iterator it;

        for (it = m_env.begin(); it != m_env.end(); ++it)
        {
            if (it->first != "IASH_CONFIG_PATH")
                fout<<it->first<<' '<<it->second<<endl;
        }
        fout.close();
    }
    else
        fileOpened = false;

    return fileOpened;
}

bool iash::loadEnv()
{
    stringstream ss;

    ss<<getEnv_string("IASH_CONFIG_PATH")<<"iashenv";

    return loadEnv(ss.str());
}

bool iash::loadEnv(string filepath)
{
    ifstream fin;
    bool fileOpened;
    string name, value;
    int reqCount; //counts required environment variables

    fin.open(filepath.c_str());
    reqCount = 0;

    if (fin.is_open())
    {
        fileOpened = true;

        while (fin>>name>>value)
        {
            if (name.find("IASH_") != string::npos)
                reqCount++;

            setEnv(name,value);
        }

        if (reqCount < 4)
        {
            if (m_env.count("IASH_APP_NAME") == 0)
                setEnv("IASH_APP_NAME",m_appName);
            else if (m_env.count("IASH_APP_NAME_IN_PROMPT") == 0)
                setEnv("IASH_APP_NAME_IN_PROMPT", f_useAppNameInPrompt);
            else if (m_env.count("IASH_DEBUG_ACTIVE") == 0)
                setEnv("IASH_DEBUG_ACTIVE", false);
            else if (m_env.count("IASH_SYNC_ENV") == 0)
                setEnv("IASH_SYNC_ENV",true);
        }

        updateAttached();
        fin.close();
    }
    else
        fileOpened = false;

    return fileOpened;
}

void iash::setAppName(string app_nm)
{
    m_appName = app_nm;
    setEnv("IASH_APP_NAME",app_nm);
}

void iash::useAppNameInPrompt(bool name)
{
    f_useAppNameInPrompt = name;
    setEnv("IASH_APP_NAME_IN_PROMPT",name);
}

void iash::debugConsole()
{
    setEnv("IASH_DEBUG_ACTIVE",true);

    do {
        getCmdLine();
        m_cmdLine.insert(m_cmdLine.begin(),"iash");

        debugConsole(m_cmdLine);
    } while (m_cmdLine[1] != "exit" && m_cmdLine[1] != "quit");

    setEnv("IASH_DEBUG_ACTIVE",false);
    clear();
}

void iash::debugConsole(vector<string> cmd)
{
    int x;

    if (cmd[1] == "cnf")
    {
        cout<<"DBG: IASH_CNF_TEST"<<endl;
        cmdNotFound();
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
            else if (cmd[2] == "name")
                cout<<getEnv_string("IASH_APP_NAME")<<endl;
            else if (cmd[2] == "appInPrompt")
                cout<<boolalpha<<getEnv_bool("IASH_APP_NAME_IN_PROMPT")<<endl;
            else
                cout<<cmd[2]<<": invalid "<<cmd[1]<<" argument"<<endl;
        }
        else
            cout<<cmd[1]<<": need an argument"<<endl;
    }
    else if (cmd[1] == "env")
    {
        if (cmd.size() == 2)
        {
            map<string,string>::iterator it;

            for (it = m_env.begin(); it != m_env.end(); ++it)
                cout<<it->first<<' '<<it->second<<endl;
        }
        else if (cmd.size() >= 3)
        {
            if (cmd[2] == "save")
            {
                if (cmd.size() == 3)
                {
                     if (!saveEnv())
                         cout<<"Failed to save the environment. Check your filesystem."<<endl;
                     else
                         cout<<"Saved environment to default path."<<endl;
                }
                else if (cmd.size() == 4)
                {
                     if (!saveEnv(cmd[3]))
                         cout<<"Failed to save the environment. Check the path '"<<cmd[3]<<"'."<<endl;
                     else
                         cout<<"Saved environment to '"<<cmd[3]<<"'."<<endl;
                }
                else
                    cout<<cmd[1]<<' '<<cmd[2]<<": too many arguments"<<endl;
            }
            else if (cmd[2] == "load")
            {
                if (cmd.size() == 3)
                {
                    if (!loadEnv())
                         cout<<"Failed to load the environment. Check the path '"<<getEnv_string("IASH_CONFIG_PATH")<<"'."<<endl;
                    else
                         cout<<"Successfully loaded environment from default path."<<endl;
                }
                else if (cmd.size() == 4)
                {
                    if (!loadEnv(cmd[3]))
                         cout<<"Failed to load the environment. Check the path '"<<cmd[3]<<"'."<<endl;
                    else
                         cout<<"Successfully loaded environment from '"<<cmd[3]<<"'."<<endl;
                }
                else
                    cout<<cmd[1]<<' '<<cmd[2]<<": too many arguments"<<endl;
            }
            else if (cmd[2] == "-r" && doesEnvVarExist(cmd[3]))
            {
                int x;

                for (x = 3; x < cmd.size(); x++)
                {
                    if (doesEnvVarExist(cmd[x]))
                        rmenv(cmd[x]);
                    else
                        cout<<convEnv(cmd[x])<<": Environment variable not found."<<endl;
                }
            }
            else if (cmd[2] == "-d")
            {
                int x;

                for (x = 3; x < cmd.size(); x++)
                {
                    if (doesEnvVarExist(cmd[x]))
                        cout<<convEnv(cmd[x])<<' '<<getEnv_string(cmd[x])<<endl;
                    else
                        cout<<convEnv(cmd[x])<<": Environment variable not found."<<endl;
                }
            }
            else
            {
                if (cmd.size() == 3 && doesEnvVarExist(cmd[2]))
                    cout<<convEnv(cmd[2])<<' '<<getEnv_string(cmd[2])<<endl;
                else if (cmd.size() == 4)
                    setEnv(cmd[2],cmd[3]);
                else if (cmd.size() == 3)
                    cout<<"Error: "<<cmd[2]<<": Environment variable not found or improper "<<cmd[1]<<" argument"<<endl;
                else
                    cout<<"Error: improper usage"<<endl<<"Usage:"<<endl<<"\tiash env <load/save (FILE)> (-rd) (NAME) (VALUE)"<<endl;
            }
        }
    }
    else if (cmd[1] == "set")
    {
        if (cmd.size() > 2)
        {
            if (cmd[2] == "appInPrompt")
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
            else if (cmd[2] == "name" && cmd.size() == 4)
                setEnv("IASH_APP_NAME",cmd[3]);
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
            else
                cout<<cmd[2]<<": invalid "<<cmd[1]<<" argument"<<endl;
        }
        else
            cout<<cmd[1]<<": need an argument"<<endl;
    }
    else if (cmd[1] != "exit" && cmd[1] != "quit")
        cmdNotFound_dbg(cmd[1]);
}

bool iash::doesEnvVarExist(string name)
{
    return m_env.count(convEnv(name)) != 0;
}

string iash::convEnv(string name)
{
    for (int x = 0; x < name.length(); x++)
        name[x] = toupper(name[x]);

    return name;
}

void iash::updateAttached()
{
    if (getEnv_bool("IASH_SYNC_ENV"))
    {
        if (getEnv_string("IASH_APP_NAME") != m_appName)
            m_appName = getEnv_string("IASH_APP_NAME");

        if (getEnv_bool("IASH_APP_NAME_IN_PROMPT") != f_useAppNameInPrompt)
            f_useAppNameInPrompt = getEnv_bool("IASH_APP_NAME_IN_PROMPT");
    }
}
