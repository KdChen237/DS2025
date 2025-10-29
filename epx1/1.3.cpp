#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
using namespace std;

int maxRect(vector<int> h)
{
    h.push_back(0);
    stack<int> st;
    int n = h.size();
    int best = 0;

    for (int i = 0; i < n; ++i)
    {
        while (!st.empty() && h[st.top()] > h[i])
        {
            int mid = st.top();
            st.pop();
            int left = st.empty() ? -1 : st.top();
            int width = i - left - 1;
            int area = h[mid] * width;
            if (area > best)
                best = area;
        }
        st.push(i);
    }
    return best;
}

int main()
{
    srand((unsigned)time(nullptr));

    vector<int> t1 = {2, 1, 5, 6, 2, 3};
    vector<int> t2 = {2, 4};
    cout << "例1输出: " << maxRect(t1) << endl; // 10
    cout << "例2输出: " << maxRect(t2) << endl; // 4

    cout << "\n10组随机测试：" << endl;
    for (int k = 1; k <= 10; ++k)
    {
        int len = rand() % 10 + 1;
        vector<int> a(len);
        for (int i = 0; i < len; ++i)
            a[i] = rand() % 10;

        cout << "第" << k << "组: [ ";
        for (int x : a)
            cout << x << " ";
        cout << "] → 最大面积 = " << maxRect(a) << endl;
    }
    return 0;
}
