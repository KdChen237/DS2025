#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
#include <algorithm>
#include <cstring>
using namespace std;

const int INF = INT_MAX;

class Graph
{
public:
    int vertexNum;
    vector<string> vertices;
    vector<vector<int>> adjMatrix;

    Graph(const vector<string> &vs) : vertices(vs)
    {
        vertexNum = vs.size();
        adjMatrix.resize(vertexNum, vector<int>(vertexNum, INF));
        for (int i = 0; i < vertexNum; i++)
            adjMatrix[i][i] = 0;
    }

    void addEdge(int v1, int v2, int w)
    {
        adjMatrix[v1][v2] = w;
        adjMatrix[v2][v1] = w;
    }

    void printAdjMatrix()
    {
        cout << "=== 邻接矩阵 ===" << endl;
        cout << "\t";
        for (const string &v : vertices)
            cout << v << "\t";
        cout << endl;
        for (int i = 0; i < vertexNum; i++)
        {
            cout << vertices[i] << "\t";
            for (int j = 0; j < vertexNum; j++)
            {
                if (adjMatrix[i][j] == INF)
                    cout << "∞\t";
                else
                    cout << adjMatrix[i][j] << "\t";
            }
            cout << endl;
        }
    }

    int getVertexIndex(const string &v)
    {
        auto it = find(vertices.begin(), vertices.end(), v);
        return it != vertices.end() ? it - vertices.begin() : -1;
    }

    void BFS(const string &startName)
    {
        int start = getVertexIndex(startName);
        if (start == -1)
        {
            cout << "起点不存在！" << endl;
            return;
        }

        vector<bool> visited(vertexNum, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        cout << "\n=== BFS遍历（起点" << startName << "）===" << endl;
        while (!q.empty())
        {
            int curr = q.front();
            q.pop();
            cout << vertices[curr] << " ";

            for (int i = 0; i < vertexNum; i++)
            {
                if (adjMatrix[curr][i] != INF && adjMatrix[curr][i] != 0 && !visited[i])
                {
                    visited[i] = true;
                    q.push(i);
                }
            }
        }
        cout << endl;
    }

    void DFS(const string &startName)
    {
        int start = getVertexIndex(startName);
        if (start == -1)
        {
            cout << "起点不存在！" << endl;
            return;
        }

        vector<bool> visited(vertexNum, false);
        cout << "\n=== DFS遍历（起点" << startName << "）===" << endl;
        DFSRecursive(start, visited);
        cout << endl;
    }

private:
    void DFSRecursive(int curr, vector<bool> &visited)
    {
        visited[curr] = true;
        cout << vertices[curr] << " ";

        for (int i = 0; i < vertexNum; i++)
        {
            if (adjMatrix[curr][i] != INF && adjMatrix[curr][i] != 0 && !visited[i])
            {
                DFSRecursive(i, visited);
            }
        }
    }

public:
    void Dijkstra(const string &startName)
    {
        int start = getVertexIndex(startName);
        if (start == -1)
        {
            cout << "起点不存在！" << endl;
            return;
        }

        vector<int> dist(vertexNum, INF);
        vector<bool> visited(vertexNum, false);
        dist[start] = 0;

        for (int i = 0; i < vertexNum - 1; i++)
        {
            int minDist = INF, u = -1;
            for (int j = 0; j < vertexNum; j++)
            {
                if (!visited[j] && dist[j] < minDist)
                {
                    minDist = dist[j];
                    u = j;
                }
            }
            if (u == -1)
                break;
            visited[u] = true;

            for (int v = 0; v < vertexNum; v++)
            {
                if (!visited[v] && adjMatrix[u][v] != INF && dist[u] + adjMatrix[u][v] < dist[v])
                {
                    dist[v] = dist[u] + adjMatrix[u][v];
                }
            }
        }

        cout << "\n=== Dijkstra最短路径（起点" << startName << "）===" << endl;
        for (int i = 0; i < vertexNum; i++)
        {
            cout << startName << "->" << vertices[i] << ": ";
            if (dist[i] == INF)
                cout << "不可达";
            else
                cout << dist[i];
            cout << endl;
        }
    }

    void Prim(const string &startName)
    {
        int start = getVertexIndex(startName);
        if (start == -1)
        {
            cout << "起点不存在！" << endl;
            return;
        }

        vector<int> key(vertexNum, INF);
        vector<int> parent(vertexNum, -1);
        vector<bool> inMST(vertexNum, false);

        key[start] = 0;

        for (int i = 0; i < vertexNum - 1; i++)
        {
            int minKey = INF, u = -1;
            for (int j = 0; j < vertexNum; j++)
            {
                if (!inMST[j] && key[j] < minKey)
                {
                    minKey = key[j];
                    u = j;
                }
            }
            if (u == -1)
            {
                cout << "图不连通，无法构建MST！" << endl;
                return;
            }
            inMST[u] = true;

            for (int v = 0; v < vertexNum; v++)
            {
                if (!inMST[v] && adjMatrix[u][v] != INF && adjMatrix[u][v] < key[v])
                {
                    key[v] = adjMatrix[u][v];
                    parent[v] = u;
                }
            }
        }

        cout << "\n=== Prim最小支撑树（起点" << startName << "）===" << endl;
        int totalWeight = 0;
        for (int i = 0; i < vertexNum; i++)
        {
            if (parent[i] != -1)
            {
                cout << vertices[parent[i]] << "-" << vertices[i] << "（权重：" << adjMatrix[parent[i]][i] << "）" << endl;
                totalWeight += adjMatrix[parent[i]][i];
            }
        }
        cout << "MST总权重：" << totalWeight << endl;
    }

    void findBiconnectedComponentsAndArticulationPoints()
    {
        vector<int> disc(vertexNum, -1);
        vector<int> low(vertexNum, -1);
        vector<int> parent(vertexNum, -1);
        vector<bool> isArticulation(vertexNum, false);
        stack<pair<int, int>> edgeStack;
        int time = 0;

        cout << "\n=== 双连通分量与关节点 ===" << endl;
        for (int i = 0; i < vertexNum; i++)
        {
            if (disc[i] == -1)
            {
                tarjan(i, disc, low, parent, isArticulation, edgeStack, time);
                if (!edgeStack.empty())
                {
                    cout << "双连通分量：";
                    while (!edgeStack.empty())
                    {
                        auto [u, v] = edgeStack.top();
                        edgeStack.pop();
                        cout << vertices[u] << "-" << vertices[v] << " ";
                    }
                    cout << endl;
                }
            }
        }

        cout << "关节点：";
        for (int i = 0; i < vertexNum; i++)
        {
            if (isArticulation[i])
                cout << vertices[i] << " ";
        }
        cout << endl;
    }

private:
    void tarjan(int u, vector<int> &disc, vector<int> &low, vector<int> &parent,
                vector<bool> &isArticulation, stack<pair<int, int>> &edgeStack, int &time)
    {
        disc[u] = low[u] = ++time;
        int childCount = 0;

        for (int v = 0; v < vertexNum; v++)
        {
            if (adjMatrix[u][v] == INF || adjMatrix[u][v] == 0)
                continue;

            if (disc[v] == -1)
            {
                childCount++;
                parent[v] = u;
                edgeStack.push({u, v});
                tarjan(v, disc, low, parent, isArticulation, edgeStack, time);

                low[u] = min(low[u], low[v]);

                if (parent[u] == -1 && childCount > 1)
                {
                    isArticulation[u] = true;
                    cout << "双连通分量：";
                    while (true)
                    {
                        auto [x, y] = edgeStack.top();
                        edgeStack.pop();
                        cout << vertices[x] << "-" << vertices[y] << " ";
                        if (x == u && y == v)
                            break;
                    }
                    cout << endl;
                }

                if (parent[u] != -1 && low[v] >= disc[u])
                {
                    isArticulation[u] = true;
                    cout << "双连通分量：";
                    while (true)
                    {
                        auto [x, y] = edgeStack.top();
                        edgeStack.pop();
                        cout << vertices[x] << "-" << vertices[y] << " ";
                        if (x == u && y == v)
                            break;
                    }
                    cout << endl;
                }
            }
            else if (v != parent[u] && disc[v] < disc[u])
            {
                edgeStack.push({u, v});
                low[u] = min(low[u], disc[v]);
            }
        }
    }
};

Graph buildGraph1()
{
    vector<string> vertices = {"A", "B", "C", "D", "E", "F", "G", "H"};
    Graph g(vertices);

    g.addEdge(g.getVertexIndex("A"), g.getVertexIndex("B"), 4);
    g.addEdge(g.getVertexIndex("A"), g.getVertexIndex("C"), 2);
    g.addEdge(g.getVertexIndex("B"), g.getVertexIndex("D"), 12);
    g.addEdge(g.getVertexIndex("B"), g.getVertexIndex("E"), 13);
    g.addEdge(g.getVertexIndex("C"), g.getVertexIndex("D"), 7);
    g.addEdge(g.getVertexIndex("C"), g.getVertexIndex("F"), 3);
    g.addEdge(g.getVertexIndex("D"), g.getVertexIndex("E"), 1);
    g.addEdge(g.getVertexIndex("D"), g.getVertexIndex("F"), 5);
    g.addEdge(g.getVertexIndex("E"), g.getVertexIndex("G"), 9);
    g.addEdge(g.getVertexIndex("F"), g.getVertexIndex("G"), 11);
    g.addEdge(g.getVertexIndex("F"), g.getVertexIndex("H"), 8);
    g.addEdge(g.getVertexIndex("G"), g.getVertexIndex("H"), 14);
    g.addEdge(g.getVertexIndex("B"), g.getVertexIndex("C"), 6);
    g.addEdge(g.getVertexIndex("H"), g.getVertexIndex("E"), 10);

    return g;
}

Graph buildGraph2()
{
    vector<string> vertices = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};
    Graph g(vertices);

    g.addEdge(g.getVertexIndex("A"), g.getVertexIndex("B"), 1);
    g.addEdge(g.getVertexIndex("A"), g.getVertexIndex("C"), 1);
    g.addEdge(g.getVertexIndex("B"), g.getVertexIndex("C"), 1);
    g.addEdge(g.getVertexIndex("B"), g.getVertexIndex("D"), 1);
    g.addEdge(g.getVertexIndex("C"), g.getVertexIndex("D"), 1);
    g.addEdge(g.getVertexIndex("D"), g.getVertexIndex("E"), 1);
    g.addEdge(g.getVertexIndex("E"), g.getVertexIndex("F"), 1);
    g.addEdge(g.getVertexIndex("E"), g.getVertexIndex("G"), 1);
    g.addEdge(g.getVertexIndex("F"), g.getVertexIndex("G"), 1);
    g.addEdge(g.getVertexIndex("G"), g.getVertexIndex("H"), 1);
    g.addEdge(g.getVertexIndex("H"), g.getVertexIndex("I"), 1);
    g.addEdge(g.getVertexIndex("H"), g.getVertexIndex("J"), 1);
    g.addEdge(g.getVertexIndex("I"), g.getVertexIndex("J"), 1);
    g.addEdge(g.getVertexIndex("J"), g.getVertexIndex("K"), 1);
    g.addEdge(g.getVertexIndex("K"), g.getVertexIndex("L"), 1);
    g.addEdge(g.getVertexIndex("J"), g.getVertexIndex("L"), 1);

    return g;
}

int main()
{
    Graph g1 = buildGraph1();
    cout << "==================== 图1 测试 ====================" << endl;
    g1.printAdjMatrix();
    g1.BFS("A");
    g1.DFS("A");
    g1.Dijkstra("A");
    g1.Prim("A");

    Graph g2 = buildGraph2();
    cout << "\n\n==================== 图2 测试 ====================" << endl;
    cout << "\n--- 起点A 计算结果 ---" << endl;
    g2.findBiconnectedComponentsAndArticulationPoints();

    cout << "\n--- 起点E 计算结果 ---" << endl;
    g2.findBiconnectedComponentsAndArticulationPoints();

    cout << "\n--- 起点L 计算结果 ---" << endl;
    g2.findBiconnectedComponentsAndArticulationPoints();

    return 0;
}