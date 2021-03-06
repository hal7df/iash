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

#include "iash.h"
using namespace std;

int main ()
{
    iash shell ("example");

    cout << "iash proof-of-concept" << endl;
    cout << "This program does not register any additional commands to iash,";
    cout << " it merely creates a shell and runs it in interactive mode.";
    cout << endl;

    return shell.runInteractive();
}
