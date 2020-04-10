#include <iostream>

#include <map>
#include <vector>
#include <stack>

using namespace std;


map<char, vector<pair<char, double>>> graph; // здесь хранится значение по ключу мы получаем доступ ко всем вершинам, в которые можем попасть из вершины - ключа
map<char, bool> visited; // список всех уже посещенных вершин


char from, to; // начальная и конечная вершина


void readGraph() // функция, которая считывает граф и помечает все вершины, как не посещенные
{
    char start, end;
    double distance;

    std::cin >> from >> to;

    while (cin >> start)
    {
        if (start == '!')
            break;
        cin >> end >> distance;

        graph[start].push_back(make_pair(end,distance));
        graph[end];
        visited[start] = false;
        visited[end] = false;
    }
}


void print(stack<char>& result) // рекурсивная функция, которая раскручивает стек, для получения пути от начально вершины в конечную
{
    if (result.empty())
        return;

    char tmp = result.top();
    result.pop();
    print(result);
    cout << tmp;
}

void greedySearch() // функция, которая реализует жадный поиск
{

    stack<char> way; // стек на котором будет хранится путь до текущей вершины
    stack<char> intermediateDataOutput; // стек для промежуточных данных

    way.push(from);
    char currPeak = way.top();

    //cout << "Intermediate way: \n";
    do // цикл, который работает пока на верху стека не окахется конечная вершина или не будет обойден весь граф
    {
        //intermediateDataOutput = way;
        //print(intermediateDataOutput);
        //cout << "\n";
        bool anyWay = false; // есть ли из текущей вершины, пути в другие еще не просмотренные верщины

        char nextPeak;
        double minDistance;


        if (graph[currPeak].empty()) // проверка на то, есть ли  пути вообще, если путей нет вершина помечается как посещенная
        {
            visited[currPeak] = true;

            way.pop();
            currPeak = way.top();
            continue;
        }

        for (int i = 0; i < graph[currPeak].size(); i++) //проверка на, то есть ли еще не посещенные вершины
        {
            if (!visited[graph[currPeak][i].first])
            {
                anyWay = true;
                nextPeak = graph[currPeak][i].first;
                minDistance = graph[currPeak][i].second;
                break;
            }
        }

        if (!anyWay) // если все вершины уже просмотренные, то вершина помечается как посещенная
        {
            visited[currPeak] = true;

            way.pop();
            currPeak = way.top();
            continue;
        }

        for (int i = 0; i < graph[currPeak].size(); i++) // поиск самого маленького ребра
        {
            if (!visited[graph[currPeak][i].first] && minDistance > graph[currPeak][i].second)
            {
                nextPeak = graph[currPeak][i].first;
                minDistance = graph[currPeak][i].second;
            }
        }

        way.push(nextPeak); //переходим в вершину путь до которой был самый короткий
        currPeak = way.top();

    }while (currPeak != to);

    cout << "Greedy search: ";
    print(way);

}


int main() {

    readGraph();

    greedySearch();

    return 0;
}
