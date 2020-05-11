#include <iostream>
#include <vector>
#include <cstdio>
#define DEBUG
#ifdef DEBUG
#define RESET "\033[0m"
#define SET_COLOR "\033[36m"
#define SET_SUBSTR_COLOR "\033[31m"
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
void debugPrint(const string &str, size_t currInd, size_t val, size_t patternSize)
{
    currInd += patternSize;
    cout << (val == patternSize ? SET_SUBSTR_COLOR : SET_COLOR);
    for (size_t i = 0; i < val; i++)
        cout << str[i];
    cout << RESET;

    for (size_t i = val; i < currInd - val + 1; i++)
    {
        if (i == patternSize)
            cout << ' ';
        cout << str[i];
    }

    cout << (val == patternSize ? SET_SUBSTR_COLOR : SET_COLOR);
    for (size_t i = currInd - val + 1; i < currInd + 1; i++)
    {
        if (i == patternSize)
            cout << ' ';
        cout << str[i];
    }
    cout << RESET;

    for (size_t i = currInd + 1; i < str.size(); i++)
        cout << str[i];

    cout << " ---> " << "pi(" << currInd << ") = " << val << endl;
}
#endif

vector <size_t> getSubstringIndices(const string &p, const string &t)
{
    PrefixFunction prefixFunction(p, t);
    vector <size_t> substringIndices;
#ifdef DEBUG
    const string str = p + t;
#endif

    for (size_t i = 0; i < t.size(); i++)
    {
        size_t val = prefixFunction.nextValue();
#ifdef DEBUG
        debugPrint(str, i, val, p.size());
#endif
        //если значение п/ф совпало с длиной паттерна - кладём в вектор индекс начала вхождения паттерна
        if (val == p.size())
            substringIndices.push_back(i - p.size() + 1);
    }

    return substringIndices;
}

int main()
{
    string p, t;
    cin >> p >> t;

    auto result = getSubstringIndices(p, t);
    if (result.empty())
        cout << -1;
    else for (size_t i = 0; i < result.size(); i++)
        {
            cout << result[i];
            if (i + 1 != result.size())
                cout << ',';
        }
    cout << endl;
    return 0;
}
