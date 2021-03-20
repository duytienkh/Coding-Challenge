#include <bits/stdc++.h>
using namespace std;

class ArgsParser {
private:
    vector<tuple<string, string, string>> options;
    set<string> needParams;
    int n;
    vector<string> args;
public:
    ArgsParser() {
        n = 0;
        addOption("h", "Show this message");
    }
    bool parse(int argc, const char **argv) {
        n = argc;
        for (int i = 0; i < n; ++i) 
            args.push_back(string(argv[i]));
        for (string arg : args)
            if (arg == "-h") {
                help();
                return false;
            }
        return true;
    }
    
    void addOption(string command, string doc, string param = "") {
        options.emplace_back(command, doc, param);
        if (param != "")
            needParams.insert(command);
    }
    
    int get(string command) {
        bool needParam = needParams.count(command);
        command = "-" + command;
        for (int i = 0; i < n; ++i) {
            if (args[i] == command) {
                if (needParam) {
                    assert(i + 1 < n);
                    try {
                        return stoi(args[i + 1]);
                    }
                    catch (...) {
                        // cerr << "Expected args[" << i << "] is number, but found `" << args[i + 1] << "`";
                        return 0;
                    }
                }
                return 1;
            }
        }
        return 0;
    }
    void help() {
        cout << "usage: ";
        cout << args[0];
        for (auto [command, doc, param] : options) if (command != "h") {
            cout << " [-";
            cout << command;
            if (param != "")
                cout << " <" << param << ">";
            cout << "]";
        }
        cout << '\n';
        cout << "Commands:\n";
        for (auto [command, doc, param] : options) {
            string cmdHelpString = "-" + command;
            if (param != "")
                cmdHelpString += " <" + param + ">";
            cout << std::left << std::setw(15) << cmdHelpString;
            cout << doc << '\n';
        }
    }
};