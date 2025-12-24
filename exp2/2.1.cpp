#include <iostream>
#include <fstream>
#include <cstring>
#include <queue>
#include <vector>
#include <cctype>
#include <algorithm>
using namespace std;

typedef int Rank;

class Bitmap
{
private:
    unsigned char *M;
    Rank N;
    Rank _sz;
    void init(Rank n)
    {
        M = new unsigned char[N = (n + 7) / 8];
        memset(M, 0, N);
        _sz = 0;
    }
    void expand(Rank k)
    {
        if (k < 8 * N)
            return;
        Rank oldN = N;
        unsigned char *oldM = M;
        init(2 * k);
        memcpy(M, oldM, oldN);
        delete[] oldM;
    }

public:
    Bitmap() { init(8); }
    Bitmap(Rank n) { init(n); }
    Bitmap(const char *file, Rank n = 8)
    {
        init(n);
        FILE *fp = fopen(file, "rb");
        if (fp)
        {
            fread(M, sizeof(unsigned char), N, fp);
            fclose(fp);
        }
        _sz = 0;
        for (Rank k = 0; k < n; k++)
        {
            if (test(k))
                _sz++;
        }
    }
    ~Bitmap()
    {
        delete[] M;
        M = NULL;
        _sz = 0;
    }
    Rank size() { return _sz; }
    void set(Rank k)
    {
        expand(k);
        if (!test(k))
        {
            M[k >> 3] |= (0x80 >> (k & 0x07));
            _sz++;
        }
    }
    void clear(Rank k)
    {
        expand(k);
        if (test(k))
        {
            M[k >> 3] &= ~(0x80 >> (k & 0x07));
            _sz--;
        }
    }
    bool test(Rank k)
    {
        expand(k);
        return M[k >> 3] & (0x80 >> (k & 0x07));
    }
    void dump(const char *file)
    {
        FILE *fp = fopen(file, "wb");
        if (fp)
        {
            fwrite(M, sizeof(unsigned char), N, fp);
            fclose(fp);
        }
    }
    char *bits2string(Rank n)
    {
        expand(n - 1);
        char *s = new char[n + 1];
        s[n] = '\0';
        for (Rank i = 0; i < n; i++)
        {
            s[i] = test(i) ? '1' : '0';
        }
        return s;
    }
};

struct HuffNode
{
    char ch;
    int weight;
    HuffNode *left;
    HuffNode *right;

    HuffNode(char c = '\0', int w = 0, HuffNode *l = NULL, HuffNode *r = NULL)
    {
        ch = c;
        weight = w;
        left = l;
        right = r;
    }

    ~HuffNode()
    {
        if (left)
            delete left;
        if (right)
            delete right;
    }
};

class HuffTree
{
private:
    HuffNode *root;
    Bitmap *codeMap[26];
    int freq[26];

    struct NodeCompare
    {
        bool operator()(HuffNode *a, HuffNode *b)
        {
            return a->weight > b->weight;
        }
    };

    void generateCode(HuffNode *node, Bitmap *tempCode, int depth)
    {
        if (node == NULL)
            return;

        if (node->left == NULL && node->right == NULL)
        {
            int idx = toupper(node->ch) - 'A';
            codeMap[idx] = new Bitmap(depth);
            for (int i = 0; i < depth; i++)
            {
                if (tempCode->test(i))
                {
                    codeMap[idx]->set(i);
                }
                else
                {
                    codeMap[idx]->clear(i);
                }
            }
            return;
        }

        generateCode(node->left, tempCode, depth + 1);

        tempCode->set(depth);
        generateCode(node->right, tempCode, depth + 1);
        tempCode->clear(depth);
    }

    int getCodeLength(int idx)
    {
        if (codeMap[idx] == NULL)
            return 0;
        int maxValidBit = -1;

        for (int i = 0; i < 20; i++)
        {
            if (codeMap[idx]->test(i))
            {
                maxValidBit = i;
            }
        }

        if (maxValidBit == -1)
        {
            return 1;
        }
        else
        {
            return maxValidBit + 1;
        }
    }

public:
    HuffTree()
    {
        root = NULL;
        memset(freq, 0, sizeof(freq));
        for (int i = 0; i < 26; i++)
        {
            codeMap[i] = NULL;
        }
    }

    ~HuffTree()
    {
        if (root)
            delete root;
        for (int i = 0; i < 26; i++)
        {
            if (codeMap[i])
                delete codeMap[i];
        }
    }

    void countFrequency(const char *textPath)
    {
        ifstream file(textPath);
        if (!file.is_open())
        {
            cerr << "错误：无法打开文本文件！路径：" << textPath << endl;
            return;
        }

        char ch;
        while (file.get(ch))
        {
            if (isalpha(ch))
            {
                int idx = toupper(ch) - 'A';
                freq[idx]++;
            }
        }
        file.close();

        cout << "=== 《I have a dream》26个字母频率统计 ===" << endl;
        for (int i = 0; i < 26; i++)
        {
            if (freq[i] > 0)
            {
                cout << (char)('A' + i) << ": " << freq[i] << "次" << endl;
            }
        }
    }

    void buildTree()
    {
        priority_queue<HuffNode *, vector<HuffNode *>, NodeCompare> pq;

        for (int i = 0; i < 26; i++)
        {
            if (freq[i] > 0)
            {
                pq.push(new HuffNode('A' + i, freq[i]));
            }
        }

        while (pq.size() > 1)
        {
            HuffNode *leftNode = pq.top();
            pq.pop();
            HuffNode *rightNode = pq.top();
            pq.pop();

            HuffNode *parentNode = new HuffNode('\0', leftNode->weight + rightNode->weight, leftNode, rightNode);
            pq.push(parentNode);
        }

        if (!pq.empty())
        {
            root = pq.top();
        }
        else
        {
            cerr << "错误：无有效字母频率，无法构建哈夫曼树！" << endl;
        }
    }

    void generateHuffCode()
    {
        if (root == NULL)
        {
            cerr << "错误：哈夫曼树未构建，无法生成编码！" << endl;
            return;
        }

        Bitmap *tempCode = new Bitmap(20);
        generateCode(root, tempCode, 0);
        delete tempCode;

        cout << "\n=== 26个字母的哈夫曼编码表 ===" << endl;
        for (int i = 0; i < 26; i++)
        {
            if (codeMap[i] != NULL && freq[i] > 0)
            {
                int codeLen = getCodeLength(i);
                char *codeStr = codeMap[i]->bits2string(codeLen);
                cout << (char)('A' + i) << ": " << codeStr << "（长度：" << codeLen << "）" << endl;
                delete[] codeStr;
            }
        }
    }

    char *encodeWord(const char *word)
    {
        if (word == NULL || strlen(word) == 0)
        {
            return NULL;
        }

        int totalLen = 0;
        for (int i = 0; i < strlen(word); i++)
        {
            char ch = word[i];
            if (!isalpha(ch))
            {
                cerr << "警告：单词包含非字母字符 '" << ch << "'，已跳过！" << endl;
                continue;
            }
            int idx = toupper(ch) - 'A';
            if (codeMap[idx] == NULL)
            {
                cerr << "错误：字符 '" << ch << "' 无哈夫曼编码，编码失败！" << endl;
                return NULL;
            }
            totalLen += getCodeLength(idx);
        }

        char *wordCode = new char[totalLen + 1];
        wordCode[totalLen] = '\0';
        int pos = 0;

        for (int i = 0; i < strlen(word); i++)
        {
            char ch = word[i];
            if (!isalpha(ch))
                continue;
            int idx = toupper(ch) - 'A';
            int codeLen = getCodeLength(idx);
            char *codeStr = codeMap[idx]->bits2string(codeLen);

            strncpy(wordCode + pos, codeStr, codeLen);
            pos += codeLen;

            delete[] codeStr;
        }

        return wordCode;
    }
};

int main()
{
    HuffTree huffTree;

    huffTree.countFrequency("I_have_a_dream.txt");

    huffTree.buildTree();

    huffTree.generateHuffCode();

    const char *targetWords[] = {"dream", "freedom", "equality", "justice"};
    cout << "\n=== 单词哈夫曼编码结果 ===" << endl;
    for (int i = 0; i < 4; i++)
    {
        char *wordCode = huffTree.encodeWord(targetWords[i]);
        if (wordCode != NULL)
        {
            cout << targetWords[i] << ": " << wordCode << endl;
            delete[] wordCode;
        }
    }

    return 0;
}