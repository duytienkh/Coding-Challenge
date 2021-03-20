#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int MOD(int a, int n) {
    if (a >= 0) return a % n;
    return n - (-a) % n;
}

pair<int, int> left(pair<int, int> p, int n){return {p.first, MOD(p.second - 1, n)};}
pair<int, int> right(pair<int, int> p, int n){return {p.first, MOD(p.second + 1, n)};}
pair<int, int> top(pair<int, int> p, int n){return {MOD(p.first - 1, n), p.second};}
pair<int, int> bottom(pair<int, int> p, int n){return {MOD(p.first + 1, n), p.second};}

int distance(pair<pair<int, int>, pair<int, int>> p, int n){
    auto fi = p.first;
    auto se = p.second;
    return min(abs(fi.first - se.first), n - abs(fi.first - se.first)) + min(abs(fi.second - se.second), n - abs(fi.second - se.second));
}


bool findPath(pair<int, int> s, pair<int, int> t, int cnt, int n, vector<vector<int>> &visited, vector<vector<pair<int, int>>> &trace) {
    queue<pair<int, int>> q;
    q.push(s);
    visited[s.first][s.second] = cnt;
    while (!q.empty()){
        pair<int, int> p = q.front();
        q.pop();
        vector<pair<int, int>> nodeList{left(p, n), right(p, n), top(p, n), bottom(p, n)};
        shuffle(nodeList.begin(), nodeList.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
        for (auto node: nodeList){
            if (visited[node.first][node.second] < cnt) {
                visited[node.first][node.second] = cnt;
                q.push(node);
                trace[node.first][node.second] = p;
            }
        }            

        if (visited[t.first][t.second] == cnt) {
            return true;
        }
    }
    return false;
}

void coloringPath(pair<int, int> s, pair<int, int> t, int currentColor, vector<vector<int>> &visited, vector<vector<pair<int, int>>> &trace, vector<vector<int>> &color) {
    pair<int, int> secondPos = t;
    pair<int, int> firstPos = s;

    while (secondPos != firstPos){
        color[secondPos.first][secondPos.second] = currentColor;
        visited[secondPos.first][secondPos.second] = INF; //mark as visted, and not re bfs
        secondPos = trace[secondPos.first][secondPos.second];
    }

    color[firstPos.first][firstPos.second] = currentColor;
    visited[firstPos.first][firstPos.second] = INF;
}

void expandPath(pair<int, int> s, pair<int, int> t, int currentColor, int n, vector<vector<pair<int, int>>> &trace, vector<vector<int>> &color, vector<bool> &existPath) {
    pair<int, int> src = s, se = t;
    pair<int, int> fi;
    while (src != se) {
        fi = trace[se.first][se.second];
        pair<int, int> fTop = top(fi, n), fBottom = bottom(fi, n), fLeft = left(fi, n), fRight = right(fi, n);
        pair<int, int> sTop = top(se, n), sBottom = bottom(se, n), sLeft = left(se, n), sRight = right(se, n);
        vector<pair<pair<int, int>, pair<int, int>>> adjPair;
        if (se == right(fi, n) || se == left(fi, n)){ // in row
            adjPair.push_back({fTop, sTop});
            adjPair.push_back({fBottom, sBottom});
        } else { // in col
            adjPair.push_back({fLeft, sLeft});
            adjPair.push_back({fRight, sRight});
        }
        shuffle(adjPair.begin(), adjPair.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
        bool expanded = 0;
        for (auto p: adjPair){
            auto f = p.first, s = p.second;
            if (!existPath[color[f.first][f.second]] && !existPath[color[s.first][s.second]]){
                trace[se.first][se.second] = s;
                trace[s.first][s.second] = f;
                trace[f.first][f.second] = fi;

                color[s.first][s.second] = color[f.first][f.second] = currentColor;

                expanded = 1;
                break;
            }
        }

        if (!expanded){
            se = trace[se.first][se.second];
        }
    }
}

vector<pair<int, int>> getPath(pair<int, int> s, pair<int, int> t, int currentColor, vector<vector<pair<int, int>>> &trace, vector<vector<int>> &color) {
    pair<int, int> secondPos = t;
    pair<int, int> firstPos = s;
    vector<pair<int, int>> tPath;
    while (secondPos != firstPos){
        tPath.push_back(secondPos);
        color[secondPos.first][secondPos.second] = currentColor;
        secondPos = trace[secondPos.first][secondPos.second];
    }

    tPath.push_back(firstPos);
    color[firstPos.first][firstPos.second] = currentColor;
    return tPath;
}

void readInput(string input, int &n, int &m, vector<vector<int>> &color, vector<pair<pair<int, int>, pair<int, int>>> &colorPosition) {
    ifstream cin(input);
    
    cin >> n;
    cin >> m;

    color = vector<vector<int>>(n, vector<int>(n, 0));
    colorPosition = vector<pair<pair<int, int>, pair<int, int>>>(m + 1);

    for (int i = 0; i < 2 * m; i++){
        int c, row, col;
        cin >> c >> row >> col;
        color[row - 1][col - 1] = c;

        if (i % 2 == 0) colorPosition[c].second = {row - 1, col - 1};
        else colorPosition[c].first = {row - 1, col - 1};
    }
}

void writeOutput(string output, vector<vector<pair<int, int>>> paths) {
    ofstream cout(output);

    for (auto p: paths){
        cout << p.size() << '\n';
        for (auto [x, y]: p){
            cout << x + 1 << ' ' << y + 1 << ' ';
        }
        cout << '\n';
    }
}

void solve(string input, string output, bool showTable = 0){
    vector<vector<int>> color;
    vector<pair<pair<int, int>, pair<int, int>>> colorPosition;
    int n, m; // board size, number of color couples

    readInput(input, n, m, color, colorPosition);

    // color list
    vector<int> colorList(m);
    iota(colorList.begin(), colorList.end(), 1); // Fill with 1 2 3 ... m
    
    // shuffle(colorList.begin(), colorList.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    sort(colorList.begin(), colorList.end(), [&](int i, int j) {
        auto& u = colorPosition[i];
        auto& v = colorPosition[j];
        return distance(u, n) < distance(v, n);
    });
    //for (auto p: colorList) cerr << p << ' '; cerr << "\n";

    // ~color list


    // FIND PATH

    vector<vector<pair<int, int>>> trace(n, vector<pair<int, int>>(n));
    vector<bool> existPath(m + 1, 0);
    vector<vector<int>> visited(n, vector<int>(n, 0)); 
    int cnt = 0;

    for (int i: colorList) {
        ++cnt;
        if (visited[colorPosition[i].first.first][colorPosition[i].first.second] > cnt ||
            visited[colorPosition[i].second.first][colorPosition[i].second.second] > cnt)
                continue; 

        if (findPath(colorPosition[i].first, colorPosition[i].second, cnt, n, visited, trace)) {
            existPath[i] = true;
            coloringPath(colorPosition[i].first, colorPosition[i].second, i, visited, trace, color);
        }
    }
    // ~FIND PATH

    vector<vector<pair<int, int>>> paths;
    for (int i: colorList){
        if (!existPath[i]) continue;
        expandPath(colorPosition[i].first, colorPosition[i].second, i, n, trace, color, existPath);

        paths.push_back(getPath(colorPosition[i].first, colorPosition[i].second, i, trace, color));
    }

    writeOutput(output, paths);

    // PRINT TABLE, only for debugging purpose
    if (showTable){
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                if (color[i][j] > 0) cerr << color[i][j];
                else cerr << "_";
                cerr << ' ';
            }
            cerr << "\n";
        }
    }
    // ~PRINT TABLE
}