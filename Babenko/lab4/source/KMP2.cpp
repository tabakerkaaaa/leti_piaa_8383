#include <iostream>
#include <vector>
#include <cstdio>
#define DEBUG
#ifdef DEBUG
#define RESET "\033[0m"
#define SET_COLOR "\033[36m"
#endif

using namespace std;

class ConcatStrings {
    const string &p;
    const string &t;
public:
    ConcatStrings(const string &p, const string &t) : p(p), t(t) {}
    char operator[](size_t i)
    {
        if (i < p.size())
            return p[i];

        return t[(i - p.size()) % t.size()];
    }
};

class PrefixFunction {
    ConcatStrings str;
    size_t patternSize;
    size_t currIndex;
    vector <size_t> prefixValues;
public:
    PrefixFunction(const string& p, const string& t) : str(p, t), patternSize(p.size()), currIndex(p.size())
    {
        // выделяем память под размер паттерна
        prefixValues.resize(patternSize);
        prefixValues[0] = 0;

        for (size_t i = 1; i < patternSize; i++)
        {
            // берём значение вычисленное на предыдущем шаге
            auto k = prefixValues[i - 1];

            // пока новый последний символ суффикса не равен последнему символу префикса
            while (k > 0 && p[i] != p[k])
                // изменяем значение префикс-функции, на значение вычисленное п/ф для k-1
                k = prefixValues[k - 1];

            //если конец префикса стал равен новому концу суффикса, то увеличиваем k на один,
            //иначе не было найдено такого символа на конце префикса, равное концу суффикса
            prefixValues[i] = (p[i] == p[k]) ? k + 1 : 0;
        }
    }

    size_t nextValue()
    {
        static size_t k = 0;

        // пока новый последний символ суффикса не равен последнему символу префикса
        while (k > 0 && str[currIndex] != str[k])
            // изменяем значение префикс-функции, на значение вычисленное п/ф для k-1
            k = prefixValues[k-1];

        // если конец префикса стал равен новому концу суффикса, то увеличиваем k на один
        if (str[currIndex++] == str[k])
            k++;

        // если значение п/ф оказалось равным длине паттерна, тогда возвращаем её,
        // а значение префикс функции на тек. итерации изменяем на значение п/ф от k-1
        if (k == patternSize)
        {
            k = prefixValues[k-1];
            return patternSize;
        }

        return k;
    }
};

#ifdef DEBUG
void debugPrint(const string &str, size_t currInd, size_t val)
{
    currInd += str.size() / 3;
    cout << SET_COLOR;
    for (size_t i = 0; i < val; i++)
        cout << str[i];
    cout << RESET;

    for (size_t i = val; i < currInd - val + 1; i++)
    {
        if (i == str.size() / 3)
            cout << ' ';
        cout << str[i];
    }

    cout << SET_COLOR;
    for (size_t i = currInd - val + 1; i < currInd + 1; i++)
    {
        if (i == str.size() / 3)
            cout << ' ';
        cout << str[i];
    }
    cout << RESET;

    for (size_t i = currInd + 1; i < str.size(); i++)
        cout << str[i];

    cout << " ---> " << "pi(" << currInd << ") = " << val << endl;
}
#endif

int getShiftingIndex(const string &p, const string &t)
{
    if (p.size() != t.size())
        return -1;

    PrefixFunction prefixFunction(p, t);
#ifdef DEBUG
    const string str = p + t + t;
#endif

    for (size_t i = 0; i < 2 * t.size(); i++)
    {
        size_t val = prefixFunction.nextValue();
#ifdef DEBUG
        debugPrint(str, i, val);
#endif
        //если значение п/ф совпало с длиной паттерна - возвращаем индекс начала сдвига
        if (val == p.size())
            return int(i - p.size() + 1);
    }

    return -1;
}

int main()
{
    string p, t;

    cin >> t >> p;
    cout << getShiftingIndex(p, t) << endl;

    return 0;
}
