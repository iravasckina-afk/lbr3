#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>  

using namespace std;

map<char, vector<char>> graph;
map<char, int> visited; // 0 - no, 1 - visiting, 2 - done

// DFS для поиска цикла
bool hasCycle(char v) {
    visited[v] = 1;

    for (char u : graph[v]) {
        if (visited[u] == 1) return true;
        if (visited[u] == 0 && hasCycle(u)) return true;
    }

    visited[v] = 2;
    return false;
}

// топологическая сортировка
vector<char> result;
void topoSort(char v) {
    visited[v] = 1;

    for (char u : graph[v]) {
        if (!visited[u]) topoSort(u);
    }

    result.push_back(v);
}

int main() {
    int n;
    cout << "Enter number of pairs: ";
    cin >> n;

    set<char> nodes;

    for (int i = 0; i < n; i++) {
        char a, b;
        cout << "Enter pair (a b): ";
        cin >> a >> b;

        graph[a].push_back(b);
        nodes.insert(a);
        nodes.insert(b);
    }

    // ===== Проверка на цикл =====
    for (char node : nodes) visited[node] = 0;

    for (char node : nodes) {
        if (visited[node] == 0) {
            if (hasCycle(node)) {
                cout << "\nSequence is CONTRADICTORY\n";
                return 0;
            }
        }
    }

    // ===== Топологическая сортировка =====
    for (char node : nodes) visited[node] = 0;

    for (char node : nodes) {
        if (!visited[node]) topoSort(node);
    }

    reverse(result.begin(), result.end());

    // ===== Проверка полноты =====
    bool full = true;

    for (int i = 0; i < result.size() - 1; i++) {
        char a = result[i];
        char b = result[i + 1];

        bool found = false;
        for (char x : graph[a]) {
            if (x == b) found = true;
        }

        if (!found) {
            full = false;
            break;
        }
    }

    if (full) {
        cout << "\nSequence is COMPLETE\n";
    } else {
        cout << "\nSequence is NOT COMPLETE\n";
    }

    cout << "\nOrder: ";
    for (char c : result) cout << c << " ";

    cout << endl;

    return 0;
}