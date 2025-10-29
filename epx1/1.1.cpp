#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;

class Complex
{
public:
    double real, imag;

    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    double mod() const
    {
        return sqrt(real * real + imag * imag);
    }

    bool operator<(const Complex &other) const
    {
        if (mod() == other.mod())
            return real < other.real;
        return mod() < other.mod();
    }

    bool operator==(const Complex &other) const
    {
        return real == other.real && imag == other.imag;
    }
};

ostream &operator<<(ostream &os, const Complex &c)
{
    os << "(" << c.real << "," << c.imag << ")";
    return os;
}

// 冒泡排序
void bubbleSort(vector<Complex> &v)
{
    int n = v.size();
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (v[j + 1] < v[j])
            {
                swap(v[j], v[j + 1]);
            }
        }
    }
}

// 归并排序
void merge(vector<Complex> &arr, int l, int mid, int r)
{
    vector<Complex> tmp;
    int i = l, j = mid + 1;

    while (i <= mid && j <= r)
    {
        if (arr[j] < arr[i])
            tmp.push_back(arr[j++]);
        else
            tmp.push_back(arr[i++]);
    }

    while (i <= mid)
        tmp.push_back(arr[i++]);
    while (j <= r)
        tmp.push_back(arr[j++]);

    for (int k = 0; k < tmp.size(); k++)
        arr[l + k] = tmp[k];
}

void mergeSort(vector<Complex> &arr, int l, int r)
{
    if (l >= r)
        return;
    int mid = (l + r) / 2;
    mergeSort(arr, l, mid);
    mergeSort(arr, mid + 1, r);
    merge(arr, l, mid, r);
}

// 唯一化
void uniqueComplex(vector<Complex> &v)
{
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
}

// 区间查找 [m1, m2)
vector<Complex> intervalSearch(const vector<Complex> &v, double m1, double m2)
{
    vector<Complex> ans;
    for (auto &c : v)
    {
        double m = c.mod();
        if (m >= m1 && m < m2)
            ans.push_back(c);
    }
    return ans;
}

int main()
{
    srand(time(NULL));
    int n = 2000;

    vector<Complex> data;

    for (int i = 0; i < n; i++)
    {
        double r = rand() % 50 - 25; // -25 ~ 24
        double im = rand() % 50 - 25;
        data.push_back(Complex(r, im));
    }

    cout << "原始复数数量: " << data.size() << endl;

    Complex target(3, 4);
    data.push_back(target);
    cout << "插入 (3,4)，当前个数: " << data.size() << endl;

    data.erase(remove(data.begin(), data.end(), target), data.end());
    cout << "删除所有 (3,4)，当前个数: " << data.size() << endl;

    uniqueComplex(data);
    cout << "唯一化后数量: " << data.size() << endl;

    // 顺序
    vector<Complex> v1 = data;
    sort(v1.begin(), v1.end());

    // 乱序
    vector<Complex> v2 = v1;
    shuffle(v2.begin(), v2.end(), default_random_engine(time(NULL)));

    // 逆序
    vector<Complex> v3 = v1;
    reverse(v3.begin(), v3.end());

    auto testSort = [&](vector<Complex> v, string name)
    {
        cout << "\n==== " << name << " ====" << endl;

        vector<Complex> b = v;
        vector<Complex> m = v;

        auto start = chrono::high_resolution_clock::now();
        bubbleSort(b);
        auto end = chrono::high_resolution_clock::now();
        auto bubbleTime = chrono::duration<double, micro>(end - start).count();
        cout << "冒泡排序耗时: " << bubbleTime << " 微秒" << endl;

        start = chrono::high_resolution_clock::now();
        mergeSort(m, 0, m.size() - 1);
        end = chrono::high_resolution_clock::now();
        auto mergeTime = chrono::duration<double, micro>(end - start).count();
        cout << "归并排序耗时: " << mergeTime << " 微秒" << endl;
    };

    testSort(v1, "顺序数据");
    testSort(v2, "乱序数据");
    testSort(v3, "逆序数据");

    // 区间查找

    double m1 = 5.0, m2 = 10.0;
    vector<Complex> result = intervalSearch(v1, m1, m2);

    cout << "\n模介于 [" << m1 << "," << m2 << ") 的复数数量: " << result.size() << endl;

    return 0;
}
