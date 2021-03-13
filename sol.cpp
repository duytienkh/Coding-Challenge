#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> cell;

bool AreNeighboring(cell a, cell b, int n) { // Kiem tra 2 o co ke nhau
    return (a.first == b.first && ((a.second + 1) % n == b.second || (a.second + n - 1) % n == b.second)) ||
        (a.second == b.second && ((a.first + 1) % n == b.first || (a.first + n - 1) % n == b.first));
}

double SingleTestGrader(string inputFile, string outputFile) { // Ham tinh diem cua 1 test
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
        x -= 1; y -= 1;
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
            x -= 1; y -= 1;
            path.push_back({ x, y });
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
    cerr << setprecision(9) << numCell << '/' << n*n << ' ' << numPair << '/' << m << '\n';
    return 1.0 * (1.0 * numCell / (n * n) + 1) * numPair / m;
}

int MOD(int a, int n){
    if (a >= 0) return a % n;
    return n - (-a) % n;
}

pair<int, int> left(pair<int, int> p, int n){return {p.first, MOD(p.second - 1, n)};}
pair<int, int> right(pair<int, int> p, int n){return {p.first, MOD(p.second + 1, n)};}
pair<int, int> top(pair<int, int> p, int n){return {MOD(p.first - 1, n), p.second};}
pair<int, int> bottom(pair<int, int> p, int n){return {MOD(p.first + 1, n), p.second};}

int distance(pair<pair<int, int>, pair<int, int>> p, int n){
    pair<int, int> fi = p.first;
    pair<int, int> se = p.second;
    return min(abs(fi.first - se.first), n - abs(fi.first - se.first)) + min(abs(fi.second - se.second), n - abs(fi.second - se.second));
}

void solve(string input, string output, bool showTable = 0){
    freopen(input.c_str(), "r", stdin);
    FILE* f = fopen(output.c_str(), "w");
    // INIT
    int n; // board size
    cin >> n;
    int m; // number of color couples
    cin >> m;
    vector<vector<int>> color(n, vector<int>(n, 0));
    vector<pair<pair<int, int>, pair<int, int>>> colorPosition(m + 1);

    for (int i = 0; i < 2 * m; i++){
        int c, row, col;
        cin >> c >> row >> col;
        color[row - 1][col - 1] = c;

        if (i % 2 == 0) colorPosition[c].second = {row - 1, col - 1};
        else colorPosition[c].first = {row - 1, col - 1};
    }
    // ~INIT

    // FIND PATH
    vector<vector<pair<int, int>>> path;
    vector<vector<pair<int, int>>> trace(n, vector<pair<int, int>>(n));
    vector<bool> existPath(m + 1, 0);
    // color list
    vector<int> colorList(m);
    for (int i = 0; i < m; i++) colorList[i] = i + 1;
    // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // shuffle(colorList.begin(), colorList.end(), std::default_random_engine(seed));
    sort(colorList.begin(), colorList.end(), [&](int i, int j) {
        auto& u = colorPosition[i];
        auto& v = colorPosition[j];
        return distance(u, n) < distance(v, n);
    });
    //for (auto p: colorList) cout << p << ' '; cout << "\n";
    // ~color list
    
    vector<vector<bool>> visited(n, vector<bool>(n, 0)); 
    for (int i: colorList) {
         if (visited[colorPosition[i].first.first][colorPosition[i].first.second] ||
            visited[colorPosition[i].second.first][colorPosition[i].second.second])
                continue; 
        // bfs
        queue<pair<int, int>> q;
        q.push(colorPosition[i].first);
        visited[colorPosition[i].first.first][colorPosition[i].first.second] = true;
        while (!q.empty()){
            pair<int, int> p = q.front();
            q.pop();
            pair<int, int> leftNode = left(p, n), rightNode = right(p, n), topNode = top(p, n), bottomNode = bottom(p, n);
            vector<pair<int, int>> nodeList;
            nodeList.push_back(leftNode);
            nodeList.push_back(rightNode);
            nodeList.push_back(topNode);
            nodeList.push_back(bottomNode);
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            shuffle(nodeList.begin(), nodeList.end(), std::default_random_engine(seed));
            for (auto node: nodeList){
                if (!visited[node.first][node.second]){
                    visited[node.first][node.second] = true;
                    q.push(node);
                    trace[node.first][node.second] = p;
                }
            }            

            if (visited[colorPosition[i].second.first][colorPosition[i].second.second]) {
                existPath[i] = 1;
                break;
            }
        }
        // ~bfs
        // color path
        if (existPath[i] == 1){    
            pair<int, int> secondPos = colorPosition[i].second;
            pair<int, int> firstPos = colorPosition[i].first;

            while (secondPos != firstPos){
                color[secondPos.first][secondPos.second] = i;
                secondPos = trace[secondPos.first][secondPos.second];
            }

            color[firstPos.first][firstPos.second] = i;
        }
        // ~color path
    }
    // ~FIND PATH
    for (int i: colorList){
        if (!existPath[i]) continue;
        // expand
        pair<int, int> src = colorPosition[i].first;
        pair<int, int> se = colorPosition[i].second, fi;
        while (src != se){
            fi = trace[se.first][se.second];
            pair<int, int> fTop = top(fi, n), fBottom = bottom(fi, n), fLeft = left(fi, n), fRight = right(fi, n);
            pair<int, int> sTop = top(se, n), sBottom = bottom(se, n), sLeft = left(se, n), sRight = right(se, n);
            if (se == right(fi, n) || se == left(fi, n)){ // in row
                vector<pair<pair<int, int>, pair<int, int>>> inRowPair;
                inRowPair.push_back({fTop, sTop});
                inRowPair.push_back({fBottom, sBottom});
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                shuffle(inRowPair.begin(), inRowPair.end(), std::default_random_engine(seed));
                if (!existPath[color[inRowPair[0].first.first][inRowPair[0].first.second]] && !existPath[color[inRowPair[0].second.first][inRowPair[0].second.second]]){
                    trace[se.first][se.second] = inRowPair[0].second;
                    trace[inRowPair[0].second.first][inRowPair[0].second.second] = inRowPair[0].first;
                    trace[inRowPair[0].first.first][inRowPair[0].first.second] = fi;
                    
                    color[inRowPair[0].second.first][inRowPair[0].second.second] = i;
                    color[inRowPair[0].first.first][inRowPair[0].first.second] = i;
                    continue;
                }
                if (!existPath[color[inRowPair[1].first.first][inRowPair[1].first.second]] && !existPath[color[inRowPair[1].second.first][inRowPair[1].second.second]]){
                    trace[se.first][se.second] = inRowPair[1].second;
                    trace[inRowPair[1].second.first][inRowPair[1].second.second] = inRowPair[1].first;
                    trace[inRowPair[1].first.first][inRowPair[1].first.second] = fi;
                    
                    color[inRowPair[1].second.first][inRowPair[1].second.second] = i;
                    color[inRowPair[1].first.first][inRowPair[1].first.second] = i;
                    continue;
                }
            }
            if (se == top(fi, n) || se == bottom(fi, n)){ // in column
                vector<pair<pair<int, int>, pair<int, int>>> inColPair;
                inColPair.push_back({fRight, sRight});
                inColPair.push_back({fLeft, sLeft});
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                shuffle(inColPair.begin(), inColPair.end(), std::default_random_engine(seed));

                if (!existPath[color[inColPair[0].first.first][inColPair[0].first.second]] && !existPath[color[inColPair[0].second.first][inColPair[0].second.second]]){
                    trace[se.first][se.second] = inColPair[0].second;
                    trace[inColPair[0].second.first][inColPair[0].second.second] = inColPair[0].first;
                    trace[inColPair[0].first.first][inColPair[0].first.second] = fi;
                    
                    color[inColPair[0].second.first][inColPair[0].second.second] = i;
                    color[inColPair[0].first.first][inColPair[0].first.second] = i;
                    continue;
                }
                if (!existPath[color[inColPair[1].first.first][inColPair[1].first.second]] && !existPath[color[inColPair[1].second.first][inColPair[1].second.second]]){
                    trace[se.first][se.second] = inColPair[1].second;
                    trace[inColPair[1].second.first][inColPair[1].second.second] = inColPair[1].first;
                    trace[inColPair[1].first.first][inColPair[1].first.second] = fi;
                    
                    color[inColPair[1].second.first][inColPair[1].second.second] = i;
                    color[inColPair[1].first.first][inColPair[1].first.second] = i;
                    continue;
                }
            }

            se = trace[se.first][se.second];            
        }
        // ~expand
        // trace
        if (existPath[i] == 1){    
            pair<int, int> secondPos = colorPosition[i].second;
            pair<int, int> firstPos = colorPosition[i].first;

            vector<pair<int, int>> tPath;
            while (secondPos != firstPos){
                tPath.push_back(secondPos);
                color[secondPos.first][secondPos.second] = i;
                secondPos = trace[secondPos.first][secondPos.second];
            }

            tPath.push_back(firstPos);
            color[firstPos.first][firstPos.second] = i;

            path.push_back(tPath);
        }
        // ~trace
    }
    // PRINT
    for (auto p: path){
        fprintf(f, "%d\n", p.size());
        for (auto q: p){
            fprintf(f, "%d %d ", q.first + 1, q.second + 1);
        }
        fprintf(f, "\n");
    }
    // ~PRINT

    // PRINT TABLE
    if (showTable){
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                if (color[i][j] > 0) cout << color[i][j];
                else cout << "_";
                cout << ' ';
            }
            cout << "\n";
        }
    }
    // ~PRINT TABLE
    fclose(f);
}

void singleSolve(int i, double& highest_score){
    string inp = "input" + to_string(i) + ".txt";
    string out = "output.txt";
    solve(inp, out, 0);
    double new_score = SingleTestGrader("input" + to_string(i) + ".txt", "output.txt");
    cout << "new score: " << new_score << "\n";

    if (new_score > highest_score){
        cout << highest_score << " -> " << new_score << "\n";
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

void solveAll(){
    for (int i = 1; i <= 20; i++){
        string inp = "input" + to_string(i) + ".txt";
        string out = "output" + to_string(i) + ".txt";
        cout << "solving testcase " << i << "\n";
        solve(inp, out);
    }
}

int main(int argc, char** argv){
    // double sum = 0;
    // for (int i = 1; i <= 20; ++i) {
    //     double highest_score = SingleTestGrader("./save/input" + to_string(i) + ".txt", "./save/output" + to_string(i) + ".txt");
    //     string x = to_string(i);
    //     while (x.length() < 2) x = "0" + x;
    //     cout << x << ": " << highest_score << '\n';
    //     sum += highest_score;
    // }
    // cout << "sum = " << sum << '\n';
    // return 0;
    if (argc == 1) solveAll();
    if (argc == 3){
        cout << "solving testcase " << argv[1] << "\n";
        int count = stoi(argv[2]);
        int i = stoi(argv[1]);
        double highest_score = SingleTestGrader("./save/input" + to_string(i) + ".txt", "./save/output" + to_string(i) + ".txt");
        for (int r = 0; r < count; r++){
            singleSolve(stoi(argv[1]), highest_score);
        }
        cout << "testcase " << argv[1] << ", highest score: " << highest_score << '\n';
    }
}

/* Note:
N = 10
    01: 1.99
    02: 0.882
    03: 0.398
N = 100
    04: 1.9999
    05: 1.39769
    06: 0.9971
    07: 0.384663
    08: 0.207375
    09: 0.0637184
    10: 0.0369954
N = 1000
    11: 1.99999
    12: 0.999884
    13: 0.347721
    14: 0.203656
    15: 0.0912365
    16: 0.0592205
    17: 0.0198361
    18: 0.0100532
    19: 0.00566499
    20: 0.0029056
*/