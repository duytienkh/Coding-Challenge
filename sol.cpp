#include <bits/stdc++.h>
#include "checker.h"
#include "solver.h"

using namespace std;

void singleSolve(int i, double& highest_score){
    string inp = "./input/input" + to_string(i) + ".txt";
    string out = "output.txt";
    solve(inp, out, 0);
    double new_score = SingleTestGrader("./input/input" + to_string(i) + ".txt", "output.txt");
    cerr << "new score: " << new_score << "\n";

    if (new_score > highest_score){
        cerr << highest_score << " -> " << new_score << "\n";
        highest_score = new_score;
        string srcFile = "output.txt";
        string desFile = "./save/output" + to_string(i) + ".txt";
        FILE* fSrc = fopen(srcFile.c_str(), "r");
        FILE* fDes = fopen(desFile.c_str(), "w");
        
        char buffer;
        while (fread(&buffer, 1, 1, fSrc) > 0){
            fwrite(&buffer, 1, 1, fDes);
        }

        fclose(fSrc);
        fclose(fDes);
    }
}

int main(int argc, char** argv){
    if (argc == 2 && argv[1][0] == 'c') {
        double sum = 0;
        ofstream f("result.txt");
        for (int i = 1; i <= 20; ++i) {
            if (i == 1)
                cerr << "N = 10\n";
            else if (i == 4)
                cerr << "N = 100\n";
            else if (i == 1)
                cerr << "N = 1000\n";
            double highest_score = SingleTestGrader("./input/input" + to_string(i) + ".txt", "./save/output" + to_string(i) + ".txt");
            string x = to_string(i);
            while (x.length() < 2) x = "0" + x;
            f << x << ": " << highest_score << '\n';
            sum += highest_score;
        }
        f << "sum = " << sum << '\n';
        return 0;
    }
    auto optimize_single = [](int i, int count) {
        cerr << "solving testcase " << i << "\n";
        double highest_score = SingleTestGrader("./input/input" + to_string(i) + ".txt", "./save/output" + to_string(i) + ".txt");
        for (int j = 0; j < count; ++j)
            singleSolve(i, highest_score);
        cerr << "testcase " << i << ", highest score: " << highest_score << '\n';
    };
    int count = 1;
    if (argc == 2)
        count = stoi(argv[1]);
    cerr << "repeat: " << count << '\n';
    if (argc == 1) {
        for (int i =  1; i <= 20; ++i) {
            optimize_single(i, count);
        }
    }
    if (argc == 3){
        int count = stoi(argv[2]);
        int i = stoi(argv[1]);
        optimize_single(i, count);
    }
}