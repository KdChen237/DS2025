#include <bits/stdc++.h>
using namespace std;
using clk = chrono::steady_clock;

// ================= 边界框结构体 =================
struct Box
{
    double x1, y1, x2, y2;
    double score;
};

// ================= IoU 计算 =================
double iou(const Box &a, const Box &b)
{
    double xx1 = max(a.x1, b.x1);
    double yy1 = max(a.y1, b.y1);
    double xx2 = min(a.x2, b.x2);
    double yy2 = min(a.y2, b.y2);
    double w = max(0.0, xx2 - xx1);
    double h = max(0.0, yy2 - yy1);
    double inter = w * h;
    double areaA = (a.x2 - a.x1) * (a.y2 - a.y1);
    double areaB = (b.x2 - b.x1) * (b.y2 - b.y1);
    double uni = areaA + areaB - inter;
    return uni <= 0 ? 0.0 : inter / uni;
}

// ================= 排序算法（按 score 降序） =================
void insertion_sort(vector<Box> &a)
{
    for (int i = 1; i < a.size(); ++i)
    {
        Box key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j].score < key.score)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

void merge_sort(vector<Box> &a)
{
    stable_sort(a.begin(), a.end(),
                [](const Box &x, const Box &y)
                {
                    return x.score > y.score;
                });
}

void quick_sort(vector<Box> &a)
{
    sort(a.begin(), a.end(),
         [](const Box &x, const Box &y)
         {
             return x.score > y.score;
         });
}

void heap_sort(vector<Box> &a)
{
    priority_queue<Box, vector<Box>,
                   function<bool(Box, Box)>>
        pq(
            [](Box x, Box y)
            { return x.score < y.score; });
    for (auto &b : a)
        pq.push(b);
    for (int i = 0; i < a.size(); ++i)
    {
        a[i] = pq.top();
        pq.pop();
    }
}

// ================= 基础 NMS =================
void nms(const vector<Box> &boxes, double thresh)
{
    int n = boxes.size();
    vector<bool> removed(n, false);
    for (int i = 0; i < n; ++i)
    {
        if (removed[i])
            continue;
        for (int j = i + 1; j < n; ++j)
        {
            if (!removed[j] && iou(boxes[i], boxes[j]) > thresh)
                removed[j] = true;
        }
    }
}

// ================= 数据生成 =================
vector<Box> gen_uniform(int N)
{
    mt19937 rng(2025);
    uniform_real_distribution<double> d(0, 1000), s(0, 1);
    vector<Box> v;
    for (int i = 0; i < N; ++i)
    {
        double x = d(rng), y = d(rng);
        v.push_back({x, y, x + 50, y + 50, s(rng)});
    }
    return v;
}

vector<Box> gen_clustered(int N)
{
    mt19937 rng(2025);
    normal_distribution<double> d(500, 100);
    uniform_real_distribution<double> s(0, 1);
    vector<Box> v;
    for (int i = 0; i < N; ++i)
    {
        double x = d(rng), y = d(rng);
        v.push_back({x, y, x + 50, y + 50, s(rng)});
    }
    return v;
}

// ================= 计时工具 =================
double time_ms(function<void()> f)
{
    auto st = clk::now();
    f();
    auto ed = clk::now();
    return chrono::duration<double, milli>(ed - st).count();
}

// ================= 主函数 =================
int main()
{
    vector<int> sizes = {100, 1000, 10000};
    double thresh = 0.5;

    for (int N : sizes)
    {
        cout << "\n数据规模 N = " << N << endl;

        vector<Box> data = gen_uniform(N);

        vector<pair<string, function<void(vector<Box> &)>>> algos = {
            {"插入排序", insertion_sort},
            {"归并排序", merge_sort},
            {"快速排序", quick_sort},
            {"堆排序", heap_sort}};

        for (auto &algo : algos)
        {
            vector<Box> a = data;
            double t1 = time_ms([&]()
                                { algo.second(a); });
            double t2 = time_ms([&]()
                                { nms(a, thresh); });
            cout << algo.first
                 << " | 排序时间(ms): " << t1
                 << " | NMS时间(ms): " << t2 << endl;
        }
    }
    return 0;
}
