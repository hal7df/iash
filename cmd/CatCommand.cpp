#include "CatCommand.h"
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <string>
#include <cctype>

static void replaceAll (std::string &s, char c, const std::string &replace) {
    size_t pos = 0;
    while ((pos = s.find(c)) != std::string::npos) s.replace(pos, 1, replace);
}

const std::string CatCommand::getName () const { return "cat"; }

int CatCommand::run (const UserCommand *cmd) {
    long lnum = 1;
    std::vector<std::string> inputs = cmd->getArguments();
    std::ostream &os = cmd->getStdout();
    bool hadErrors = false;
    bool lastEmpty = false;

    bool showNums = cmd->hasOption('n') || cmd->hasOption('b');
    bool countContent = cmd->hasOption('b');
    bool showAll = cmd->hasOption('A');
    bool showEnds = showAll || cmd->hasOption('e') || cmd->hasOption('E'); 
    bool showCR = showAll || cmd->hasOption('v') || cmd->hasOption('e') || cmd->hasOption('t');
    bool showTabs = showAll || cmd->hasOption('t') || cmd->hasOption('T');
    bool squashSpace = cmd->hasOption('s');

    std::function<bool(const std::string&)> incr;

    if (countContent) {
        incr = [&lnum](const std::string &line) -> bool {
            return !all_of(line.begin(), line.end(), &isspace);
        };
    } else {
        incr = [&lnum](const std::string &line) -> bool {
            (void)line;
            return true;
        };
    }

    //Remove command name
    inputs.erase(inputs.begin());

    if (inputs.empty()) inputs.push_back("-");

    for (const std::string &fname : inputs) {
        //Skip empty filenames and options
        if (fname.empty() || (fname[0] == '-' && fname.size() != 1)) continue;

        std::istream *is = nullptr;
        std::string line;

        //Read from stdin
        if (fname[0] == '-') {
            is = &(cmd->getStdin());
            is->clear();
        } else is = new std::ifstream (fname.c_str());
        
        //Check validity of stdin
        if (!(*is)) {
            hadErrors = true;

            if (fname[0] == '-')
                std::cerr << "cat: error reading stdin\n";
            else
                std::cerr << "cat: " << fname << ": error opening file (may not exist)\n";

            continue;
        }

        while (std::getline(*is, line)) {
            bool isEmpty = (countContent || squashSpace) && all_of(line.begin(), line.end(), &isspace);

            //Squash extra whitespace
            if (squashSpace && isEmpty && lastEmpty) continue;

            //Add user-specified formatting
            if (showCR) replaceAll(line, '\r', "^M");
            if (showTabs) replaceAll(line, '\t', "^I");

            if (showNums) {
                os << std::setw(MARGIN_WIDTH);

                if (!(countContent && isEmpty)) os << (lnum++) << "  ";
                else os << ' ' << ' ';
            }

            os << line << (showEnds ? "$\n" : "\n");

            lastEmpty = isEmpty;
        }

        //Close file handle, if we're reading from file
        if (fname[0] != '-') delete is;
        
        //Try to clear EOF from stdin for next use 
        else is->clear();
    }

    return !!static_cast<int>(hadErrors);
}
