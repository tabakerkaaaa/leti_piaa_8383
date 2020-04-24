#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
#include <set>
#include <map>
using namespace std;

//структура ребра, хранит остаточную пропускную способность и поток, который можно пустить обратно
struct Edge {
    int resultedCapacity;
    int reversedFlow;
};

//остаточная сеть
map<char, map<char, Edge>> network;
//сет ребер
set<pair<char, char>> graph;
//вектор посещенных вершин
vector<bool> used;
char source, sink;

//функция для вывода найденного потока
void printFlow() {
  for (auto &i : graph) {
    cout << i.first << ' ' << i.second << ' ' << network[i.second][i.first].reversedFlow;

    cout << "(resulted capacity " << network[i.first][i.second].resultedCapacity
    << ", max capacity " << network[i.second][i.first].reversedFlow + network[i.first][i.second].resultedCapacity << ')';

    cout << endl;
  }
}

//функция чтение графа из консоли. Заполняет граф и остаточную сеть
void readGraph() {
    int n;
    char u, v;
    int weight;

    cin >> n;
    cin >> source >> sink;

    //128 - символов в таблице ascii 128
    used.resize(128);

    for (int i = 0; i < n; i++) {
        cin >> u >> v >> weight;
        graph.insert({u, v});
        network[u][v].resultedCapacity = weight;
        if (network.find(v) != network.end() && network[v].find(u) == network[v].end()){
            network[v][u].resultedCapacity = 0;
        }
    }
}

//Функция для обхода сети по правилу из варианта индивидуализации:
/*
 * Поиск не в глубину и не в ширину, а по правилу: каждый раз выполняется переход по дуге,
 * имеющей максимальную остаточную пропускную способность. Если таких дуг несколько,
 * то выбрать ту, которая была обнаружена раньше в текущем поиске пути.
 */
int networkTraversal(char v, int delta) {
    //если вершина уже была посещена, выходим из нее
    if (used[v])
        return 0;
    used[v] = true;

    //если текущая вершина - сток, выходим из нее
    if (v == sink)
        return delta;

    //множество смежных вершин, сортированное по остаточной пропускной способности
    set<pair<int, char>> toVisit;

    for (auto u : network[v]) {
        if (!used[u.first])
            toVisit.insert({max(u.second.resultedCapacity, u.second.reversedFlow), u.first});
    }
    //обходим вершины из множества в порядке убывания остаточной пропускной способности

    for (auto u = toVisit.rbegin(); u != toVisit.rend(); u++) {
        //если есть поток который можно пустить обратно,
        //находим минимальный вес ребра в пути и делаем это
        if (network[v][u->second].reversedFlow > 0) {
            int newDelta = networkTraversal(u->second, min(delta, network[v][u->second].reversedFlow));
            if (newDelta > 0) {
                network[u->second][v].resultedCapacity += newDelta;
                network[v][u->second].reversedFlow -= newDelta;
                return newDelta;
            }
        }
        //если остаточная пропускная способность больше нуля,
        //находим минимальный вес ребра в пути и пускаем поток по этому ребру
        if (network[v][u->second].resultedCapacity > 0) {
            int newDelta = networkTraversal(u->second, min(delta, network[v][u->second].resultedCapacity));
            if (newDelta > 0) {
                network[u->second][v].reversedFlow += newDelta;
                network[v][u->second].resultedCapacity -= newDelta;
                return newDelta;
            }
        }
    }
    return 0;
}

//запуск поиска алгоритмом Форда-Фалкерсона
void startFordFulkerson() {
  int flow = 0;
  int ans = 0;
  while (true) {
    //обнуляем вектор посещенных вершин
    used.clear();
    used.resize(128);
    //запускаем поиск в глубину
    flow = networkTraversal(source, INT_MAX);
    //если путь не найден - выходим

        cout << "==================\n";
        cout << "current flow = " << ans+flow << endl;
        printFlow();

    if (flow == 0 || flow == INT_MAX){

      cout << "<!New path not found!>\n";

      break;
    }
    //обновляем максимальный поток
    ans += flow;
  }

  cout << "Resulting max flow: ";
  cout << ans << endl;
  printFlow();
}

int main() {
    readGraph();
    startFordFulkerson();
    return 0;
}
