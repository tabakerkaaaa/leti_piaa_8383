#include <iostream>
#include <unordered_map>
#include <set>
#include <queue>
#include <string>
#define DBG
using namespace std;

//структура вершины бора. Хранит символ, ссылки на родителя, на суффикс и на детей,
//а также индекс шаблона. Если строка не терминальная, хранится -1. Также для удобства в терминальных вершинах
//хранится длина шаблона.
struct Vertex {
    char ch;
    Vertex* parent = nullptr;
    Vertex* suffix = nullptr;
    unordered_map<char, Vertex*> child;
    int terminate = 0;
    int stringSize;
    vector<int> substringStart;
};

//класс бора
class Trie {
    Vertex root;
    char joker;
    int tempSize;
    int substringsNum = 0;
public:
    //метод для поиска строк в тексте, принимает текст.
    void findStrings(string text) {
        //начинаем поиск от корня бора
        Vertex* tmp = &root;
        int substringsEntries[text.size()+1];
        for (int i = 0; i <= text.size(); i++) {
            substringsEntries[i] = 0;
        }
        for (int i = 0; i < text.length(); i++) {
            //если можем перейти из текущей вершины, то делаем это, иначе пытаемся перейти из суффиксов
            if (tmp->child.find(text[i]) != tmp->child.end()) {
                tmp = tmp->child[text[i]];
                //если вершина терминальная, записываем индекс ее начала в тексте и возвращаемся
                //к корню чтобы избежать пересечения вхождений шаблонов
                if (tmp->terminate) {
                    for (auto j : tmp->substringStart) {
                        if (text.size() - i + j + tmp->stringSize > tempSize)
                            substringsEntries[i - tmp->stringSize - j + 2] += 1;
                    }
                }
                Vertex* suff = tmp;
                //также проверяем, не является ли терминальным один из суффиксов
                while (suff != &root) {
                    suff = suff->suffix;
                    if (suff->terminate) {
                        for (auto j : suff->substringStart) {
                            if (text.size() - i + j + suff->stringSize > tempSize)
                                substringsEntries[i - suff->stringSize - j + 2] += 1;
                        }
                    }
                }
            } else {
                //если мы не можем перейти из текущей вершины, проверяем, можем ли перейти из ее суффиксов
                while (tmp->child.find(text[i]) == tmp->child.end() && tmp != &root) {
                    tmp = tmp->suffix;
                }
                if (tmp->child.find(text[i]) != tmp->child.end())
                    i--;
            }
        }

        printEntries(substringsEntries, text.size());
    }
    //метод для вывода найденных вхождений в тексте

    void printEntries(const int* substringsEntries, int textSize) const {
#ifdef DBG
        cout << "midArray:\n";
        for (int i = 1; i <= textSize; i++) {
            cout << '\t' << substringsEntries[i] << endl;
        }
        cout << "\nAnswer:\n";
#endif
        int prev = 0;
        for (int i = 1; i <= textSize; i++) {
            if (substringsEntries[i] == substringsNum && (prev==0 || prev+tempSize <= i)) {
                prev = i;
                cout << i << endl;
            }
        }
    }
    //метод построения суффиксных ссылок в боре
    void initSuffixRef() {
        Vertex* vert;
        queue<Vertex*> vertexToVisit;
        vertexToVisit.push(&root);
        //запускается поиск в ширину, на каждом шаге для родителя текущей вершины суффиксная ссылка уже построена,
        //а значит остается только проверить можем ли из суффикса родителя перейти по символу текущей вершины,
        //если не можем то уменьшаем суффикс родителя и проверяем так пока не дойдем до корня
#ifdef DBG
        cout << "initialization of suffix links:\n";
#endif
        while (!vertexToVisit.empty()) {
            vert = vertexToVisit.front();
            vertexToVisit.pop();
#ifdef DBG
            if (vert == &root) {
                cout << "root:\n";
            } else {
                Vertex* t = vert;
                vector<char> str;
                while (t!=&root) {
                    str.push_back(t->ch);
                    t = t->parent;
                }
                for (auto k = str.rbegin(); k!=str.rend(); k++){
                    cout << *k;
                }
            }
            cout << "\tchilds: ";
            if (vert->child.empty()) {
                cout << "none";
            }
#endif
            for (auto &i : vert->child) {
                vertexToVisit.push(i.second);
#ifdef DBG
                cout << i.first;
#endif
            }
            Vertex* tmp = vert->parent;
#ifdef DBG
            cout << endl;
            if (vert != &root)
                cout << "\tsuffixLink: ";
#endif
            while (tmp) {
                tmp = tmp->suffix;
                if (tmp && tmp->child.find(vert->ch) != tmp->suffix->child.end()) {
                    vert->suffix = tmp->child[vert->ch];
#ifdef DBG
                    if (vert->suffix != &root) {
                        vector<char> suffix;
                        suffix.push_back(vert->ch);
                        while (tmp!=&root) {
                            suffix.push_back(tmp->ch);
                            tmp = tmp->parent;
                        }
                        for (auto k = suffix.rbegin(); k!=suffix.rend(); k++){
                            cout << *k;
                        }
                        cout << endl;
                    }
#endif
                    break;
                }
            }
#ifdef DBG
            if (vert->suffix == &root)
                cout << "root\n";
#endif
        }
    }
    void insert(const string& newString, int start) {
#ifdef DBG
        cout << "\nadd new string \"" << newString << "\" to trie\n";
        cout << "path in trie : root";
#endif
        Vertex* tmp = &root;
        for (auto i : newString) {
#ifdef DBG
            cout << "->";
#endif
            if (tmp->child.find(i) == tmp->child.end()) {
                tmp->child[i] = new Vertex;
                tmp->child[i]->parent = tmp;
                tmp->child[i]->ch = i;
                tmp->child[i]->suffix = &root;
#ifdef DBG
                cout << "(New)";
#endif
            }
            tmp = tmp->child[i];

#ifdef DBG
            cout << tmp->ch;
#endif
        }

#ifdef DBG
        cout << endl;
#endif
        //выставляем для терминальной вершины флаг о том, что она терминальная
        //записываем длину подстроки и индекс вхождения в шаблон
        tmp->terminate = 1;
        tmp->stringSize = newString.size();
        tmp->substringStart.push_back(start);
    }
    //метод считывания строк-шаблонов из потока ввода
    void readStrings() {
        string temp;
        cin >> temp;
        cin >> joker;
        tempSize = temp.size();
        int start = 0;
        bool sub = false;
        for (int i = 0; i < temp.size(); i++) {
            if (temp[i] != joker && !sub) {
                sub = true;
                start = i;
                if (i == temp.size() - 1) {
                    insert(temp.substr(start, 1), start);
                    substringsNum++;
                }
            } else if ((temp[i] == joker || i == temp.size() - 1) && sub) {
                sub = false;
                insert(temp.substr(start, i-start+(temp[i] == joker?0:1)), start);
                substringsNum++;
            }
        }

    }
};

int main() {
    string text;
    cin >> text;
    Trie trie;
    trie.readStrings();
    trie.initSuffixRef();
    trie.findStrings(text);
    return 0;
}
