#include <bits/stdc++.h>
using namespace std;


vector<int> cl;
vector<int> p;
int cycle_st, cycle_end;




bool dfs_cl (int v, vector<vector<int>>& g) {
    cl[v] = 1;


    for (size_t i=0; i<g[v].size(); ++i) {
        int to = g[v][i];


        if (cl[to] == 0 && g[v][i] != p[v]) {
            p[to] = v;
            if (dfs_cl (to, g))  return true;
        }


        else if (cl[to] == 1 && g[v][i] != p[v]) {
                cycle_end = v;
                cycle_st = to;
                return true;
        }
    }
    cl[v] = 2;
    return false;
}


void dfs(int f, int prev, const vector<vector<int>>& graph, vector<int>& used, vector<vector<int>>& tree, int& cnt) {


    if (used[f] != -1)
        return;


    used[f] = cnt;


    if (f != prev){
        tree[prev].push_back(f);
        tree[f].push_back(prev);
    }


    prev = f;
    cnt++;


    for( int to: graph[f] ) {
        dfs(to, prev, graph, used, tree,  cnt);
    }


}


int main(int argc, char *argv[]) {
    ifstream fin(argv[1]);
    int size;
    fin >> size;
    int matrix[size][size];


    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            fin >> matrix[i][j];
        }
    }


    vector<vector<int>> g(size);
    fin.close();


    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] == 1)
                g[i].push_back(j);
        }
    }


    vector<vector<int>> tree(size);
    vector<int> used(size);
    used.assign(size, -1);


    int f = 0;
    int cnt = 0;
    int prev = 0;


    dfs(f, prev, g, used, tree, cnt);
    vector<vector<int>> unused(size);
    vector<vector<int>> cycles;


    for (int i = 0; i < size; i++) {


        vector<int> vertexes_a(size);
        vertexes_a.assign(size, -1);


        vector<int> vertexes_b(size);
        vertexes_b.assign(size, -1);


        vector<int> difference;


        for (int x : g[i]) {
            vertexes_a[x] = 1;
        }


        for (int x : tree[i]) {
            vertexes_b[x] = 1;
        }


        for (int j = 0; j < size; j++) {
            if (vertexes_a[j] != vertexes_b[j])
                difference.push_back(j);
        }


        for (int x: difference) {
            vector<vector<int>> temp_g = tree;
            temp_g[i].push_back(x);
            temp_g[x].push_back(i);
            p.assign (size, -1);
            cl.assign (size, 0);
            cycle_st = -1;


            for (int a = 0; a < size; ++i) {
                if (dfs_cl (a, temp_g))
                    break;
            }


            vector<int> cycle;
            cycle.push_back (cycle_st);


            for (int v=cycle_end; v!=cycle_st; v=p[v])
                cycle.push_back (v);


            cycle.push_back (cycle_st);
            reverse (cycle.begin(), cycle.end());
            sort (cycle.begin(), cycle.end() - 1);
            cycles.push_back(cycle);
        }
    }


    sort(cycles.begin(), cycles.end());
    ofstream fout;
    fout.open(argv[1]);
    fout.clear();


    fout << size;
    fout << '\n';


    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            fout << matrix[i][j] << ' ';
        }
        fout << '\n';
    }


    fout << '\n';
    fout << "Text:";
    fout << '\n';


    for (int i = 0; i < cycles.size(); i++) {
        if (cycles[i+1] == cycles[i])
            continue;
        fout << "cycle: \n";
        for (int j = 0; j < cycles[i].size() - 1; j++)
            fout << cycles[i][j] << ' ';


        fout << '\n';
    }
    fout.close();
}
