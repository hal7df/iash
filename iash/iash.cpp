#include "iash.h"

vector<string> iash::getCmdLine()
{
    string raw;
    unsigned space, lastSpace;
    vector<string> cmdLine;

    clear();

    getline(cin,raw);
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
            cmdLine.push_back(raw.substr(lastSpace,(space-lastSpace)+1));
            lastSpace = space+1;
        }

    }while(space != string::npos);

    m_cmdLine = cmdLine;

    return cmdLine;
}
