#include <bits/stdc++.h>
#include "checker.h"
#include "solver.h"
#include "args_parser.h"

using namespace std;

const string INPUT_FOLDER = "./input/";
const string OUTPUT_FOLDER = "./save/";

void singleSolve(int i, double& highestScore){
    string inp = INPUT_FOLDER + "input" + to_string(i) + ".txt";
    string out = "output.txt";
    solve(inp, out, 0);
    double new_score = SingleTestGrader(INPUT_FOLDER + "input" + to_string(i) + ".txt", "output.txt");
    cerr << "new score: " << new_score << "\n";

    if (new_score > highestScore){
        cerr << highestScore << " -> " << new_score << "\n";
        highestScore = new_score;
        string srcFile = "output.txt";
        string dstFile = OUTPUT_FOLDER + "output" + to_string(i) + ".txt";
        ifstream src(srcFile, ios::in | ios::binary);
        ofstream dst(dstFile, ios::out | ios::binary);
        dst << src.rdbuf();
    }
}

void printAllResult(string resultFile) {
    double sum = 0;
    ofstream f(resultFile);
    for (int i = 1; i <= 20; ++i) {
        if (i == 1)
            cerr << "N = 10\n";
        else if (i == 4)
            cerr << "N = 100\n";
        else if (i == 1)
            cerr << "N = 1000\n";
        double highestScore = SingleTestGrader(INPUT_FOLDER + "input" + to_string(i) + ".txt", OUTPUT_FOLDER + "output" + to_string(i) + ".txt");
        f << std::setfill('0') << std::setw(2) << i << ": " << highestScore << '\n';
        sum += highestScore;
    }
    f << "sum = " << sum << '\n';
}


void optimizeSingleTest(int i, int count) {
    cerr << "solving testcase " << i << "\n";
    double highestScore = SingleTestGrader(INPUT_FOLDER + "input" + to_string(i) + ".txt", OUTPUT_FOLDER + "output" + to_string(i) + ".txt");
    for (int j = 0; j < count; ++j)
        singleSolve(i, highestScore);
    cerr << "testcase " << i << ", highest score: " << highestScore << '\n';
}

int main(int argc, const char** argv){
    ArgsParser parser;
    parser.addOption("r", "Get result of current outputs");
    parser.addOption("t", "Specify the test case will run", "test_id");
    parser.addOption("l", "Specify the number of time each test case will run ", "cnt");
    if (!parser.parse(argc, argv)) {
        return 0;
    }
    if (parser.get("r")) {
        printAllResult("result.txt");
        return 0;
    }

    int loop = max(1, parser.get("l"));
    cerr << "Loop: " << loop << '\n';
    int test_id = parser.get("t");

    if (test_id == 0) {
        for (int i =  1; i <= 20; ++i) {
            optimizeSingleTest(i, loop);
        }
    }
    else {
        optimizeSingleTest(test_id, loop);
    }
}