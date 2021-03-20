#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> cell;

bool AreNeighboring(cell a, cell b, int n) {  // Kiem tra 2 o co ke nhau
    return (a.first == b.first && ((a.second + 1) % n == b.second || (a.second + n - 1) % n == b.second)) ||
           (a.second == b.second && ((a.first + 1) % n == b.first || (a.first + n - 1) % n == b.first));
}

double SingleTestGrader(string inputFile, string outputFile) {  // Ham tinh diem cua 1 test
    fstream f;
    f.open(inputFile, std::fstream::in);
    if (f.is_open() == false)
        return 0;
    int n, m;
    f >> n >> m;
    map<cell, int> col;
    map<cell, bool> visited;
    for (int i = 1; i <= 2 * m; ++i) {
        int c, x, y;
        f >> c >> x >> y;
        x -= 1;
        y -= 1;
        col[{x, y}] = c;
    }
    f.close();
    f.open(outputFile, std::fstream::in);
    if (f.is_open() == false)
        return 0;
    vector<cell> path;
    int numCell = 0, numPair = 0, len;
    while (f >> len) {
        path.clear();
        numCell += len;
        ++numPair;
        for (int i = 0; i < len; ++i) {
            int x, y;
            f >> x >> y;
            x -= 1;
            y -= 1;
            path.push_back({x, y});
            if (visited[{x, y}])
                return 0;
            else
                visited[{x, y}] = true;
        }
        if (len < 2)
            return 0;
        else if (col[path[0]] != col[path.back()] || col[path[0]] == 0)
            return 0;
        for (int i = 1; i < path.size(); ++i) {
            if (!AreNeighboring(path[i], path[i - 1], n))
                return 0;
        }
    }
    f.close();
    cerr << setprecision(9) << numCell << '/' << n * n << ' ' << numPair << '/' << m << '\n';
    return 1.0 * (1.0 * numCell / (n * n) + 1) * numPair / m;
}