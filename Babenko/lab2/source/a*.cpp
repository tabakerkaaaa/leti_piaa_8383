#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <queue>


using namespace std;

struct Cell{ // структура для хранения названия вершины, ее родителя и пути до нее
    char name;
    char parent;

    double rough;
};

struct Cmp{ // компоратор для очереди с приорететом
    bool operator()(const Cell& a, const Cell& b)
    {
        if (a.rough == b.rough)
        {
            return a.name < b.name;
        }
        return a.rough > b.rough;
    }

};

map<char, vector<pair<char, double > > > graph; // граф
map<char, bool> closeList; // уже просмотреннный вершины
map<char, pair<char, double> > realWay; // кратчайщие пути до вершин
char from, to; // начальная и конечная вершина
int n; // количество вершин снимаемых за раз


void readGraph(){ // функция, которая считывает граф
    char start, finish;
    double way;

    cout << "Введите количество вершин снимаемых с очереди за раз: ";
    cin >> n;

    std::cin >> from >> to;

    while (cin >> start)
    {
        if (start == '!')
            break;
        cin >> finish >> way;
        graph[start].push_back(make_pair(finish, way));
    }
}

void printWay(char a){ // функция которая востанавливает путь от конечной вершины до начальной,
    if (a == from)     // так как в кратчайщих путях мы так же храним из какой вершины мы в нее попали, то мы можем проследить весь путь от конца до началаa e
    {
        cout << a;
        return;
    }
    printWay(realWay[a].first);
    cout << a;
}


void aStar()
{
    vector <Cell> cells; // массив, в котором будут хранится n снятых вершин
    priority_queue <Cell, vector<Cell>, Cmp> openList; // открытый список, куда кладудтся все вершина, котоыре нужно рассмотреть, в верху очереди находится вершины с самым маленьким приоерететом

    openList.push(Cell{from, '\0', 0 + double(to - from)});

    cout << "Intermediate way:\n";

    while(!openList.empty()){ // цикл работает пока очередь не опустеет или с нее не будет снята конечная вершина
        for(auto& it : realWay)
        {
            cout << "minWay[" << it.first << "]: ";
            printWay(it.first);
            cout << ' ';
        }

        cout << '\n';
        if (openList.top().name == to) // если была снята конечная вершина, то алгоритм останавливается
        {
            cout << "A* result: ";
            printWay(to);
            cout << endl;
            return;
        }

        for (int i = 0 ; i < n && !openList.empty(); i++){ // вершины снимаются пока мы не снимим n вершин или очередь не опустеет
            Cell tmp = openList.top();                     // или если была встреченна конечная вершина, и она была не первая в очереди

            if (tmp.name == to) continue;

            cells.push_back(tmp);
            openList.pop();
        }

        for(int i = 0; i < cells.size(); i++) { // рассмотрение всех снятых верщин

            Cell currCell = cells[i];
            closeList[currCell.name] = true;

            for (int j = 0; j < graph[currCell.name].size(); j++) { // рассматриваем все смежные вершины
                pair<char, double> newCell = graph[currCell.name][j];

                if (closeList[newCell.first]) // если вершина уже была рассмотренна, то мы ее не рассматриваем
                    continue;

                if (realWay[newCell.first].second == 0 || realWay[newCell.first].second > realWay[currCell.name].second + newCell.second) // если вершина еще не была рассмотренна лии она все еще находится в открытом списке
                {                                                                                                                          // то мы проверяем короче ли новый найденный путь или нет, если да
                    realWay[newCell.first].second = realWay[currCell.name].second + newCell.second;                                        // то мы добавляем в очередь эту вершину, и запоминаем новый кратчайщий путь до нее,
                    realWay[newCell.first].first = currCell.name;                                                                          // если не была рассмотренна, то просто добавляем
                    openList.push(Cell{newCell.first, currCell.name, realWay[newCell.first].second + double(to - newCell.first)});
                }


            }
        }
        cells.clear();
    }
}


int main(){
    readGraph();

    aStar();

    return 0;
}
