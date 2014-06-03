#include <iostream>
#include "iash/iash.h"
using namespace std;

int main ()
{
    int x;
    vector<string> cmdLine;
    vector<string> ops;
    iash shell ("example",true);

    cmdLine.push_back("init");

    do {
        cmdLine = shell.getCmdLine();
        ops = shell.getOptions();

        if (cmdLine[0] != "iash")
        {
            cout<<endl<<"Command: "<<cmdLine[0]<<endl<<endl<<"Arguments:"<<endl;

            for (x = 1; x < cmdLine.size(); x++)
                cout<<cmdLine[x]<<endl;

            cout<<endl<<"Options:"<<endl;

            for (x = 0; x < ops.size(); x++)
                cout<<ops[x]<<endl;

            cout<<endl;
        }

    } while (cmdLine[0] != "exit");

    cout<<"iash example exiting."<<endl;

    return 0;
}
