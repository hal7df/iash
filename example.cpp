/* iash example
 *
 * This program demonstrates the use of the
 * iash api.
 *
 * * Copyright (C) 2014  Paul Bonnen
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include "iash/iash.h"
using namespace std;

int main ()
{
    int x;
    vector<string> cmdLine;
    vector<string> ops;
    iash shell ("example",true);

    do {
        cmdLine = shell.getCmdLine();
        ops = shell.getOptions();

        if (!cmdLine.empty() && cmdLine[0] != "exit")
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
