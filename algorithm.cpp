#include "algorithm.h"

algorithm::algorithm(std::vector<std::vector<std::string>> &p)
{
    algo = &distance::manhattan; //select distance algorithm
    for (int i = 0; i < p.size(); i++) {
        std::vector<int> gt;
        gt.resize(p[i].size(), 0);
        std::vector<int> st;
        st.resize(p[i].size(), 0);
        for (int j = 0; j < p[i].size(); j++) {
            if (p[i][j] == "E") {
                x2 = i;
                y2 = j;
            } else if (p[i][j] == "S") {
                x1 = i;
                y1 = j;
                st[j] = 1;
            } else if (p[i][j] == "O") {
                gt[j] = 1;
            }
        }
        s.push_back(st);
        g.push_back(gt);
    }
    this->p = p;
}

void algorithm::setAlgo(int (distance::*algo)(int, int, int, int)) {
    this->algo = algo;
}

std::vector<std::vector<std::string>> algorithm::start() {
    astar(x1,y1,x2,y2,p,g,s,algo);
    return p;
}

int algorithm::distance::chebyshev(int i, int j, int x2, int y2) {
    return std::max(abs(i - x2), abs(j - y2)); //雪比切夫距离
}
int algorithm::distance::bfs(int i, int j, int x2, int y2) {
    return 0; //雪比切夫距离
}
int algorithm::distance::euclidean(int i, int j, int x2, int y2) {
    return (int) (sqrt((i - x2) * (i - x2) + (j - y2) * (j - y2))); //欧几里德距离
}
int algorithm::distance::manhattan(int i, int j, int x2, int y2) {
    return abs(i - x2) + abs(j - y2); //曼哈顿距离
}

void algorithm::astar(int x1, int y1, int x2, int y2, std::vector<std::vector<std::string>> &p,
           const std::vector<std::vector<int>> g, std::vector<std::vector<int>> s,
           int(distance::*algo)(int, int, int, int)) {
    if (p.empty() || p[0].empty()) return;
    int M = p.size();
    int N = p[0].size();
    distance dis;
    int c = 0;
    s[x1][y1] = 1;
    std::priority_queue<Node> q;
    q.push({x1, y1, 0, (dis.*algo)(x1, y1, x2, y2)});
    bool exit = false;
    while (!q.empty()) {
        Node n = q.top();
        q.pop();
        for (int i = 0; i < 4; ++i) {
            int a = diri[i] + n.i;
            int b = dirj[i] + n.j;
            if (overflow(a, b, M, N) && s[a][b] != 1 && g[a][b] != 1) {
                c++;
                s[a][b] = 1;
                q.push({a, b, n.g + 1, (dis.*algo)(a, b, x2, y2)});
                p[a][b] = dir[i];
                if (a == x2 && b == y2) {
                    print(p,x2,y2);
                    std::cout << "ok " << c << " step" << std::endl;
                    exit = true;
                    break;
                }
            }
        }
        if (exit) {
            break;
        }
    }
    if (!exit) {
        std::cout << "failed" << std::endl;
    }

}

bool algorithm::overflow(int a, int b, int m, int n) {
    return a >= 0 && b >= 0 && a < m && b < n;
}
void algorithm::print(std::vector<std::vector<std::string> > &p, int x2, int y2) {
    int x = x2, y = y2;
    while (p[x][y] != "S") {
        if (p[x][y] == dir[0]) {
            p[x][y] = "A";
            x -= diri[0];
            y -= dirj[0];
        } else if (p[x][y] == dir[1]) {
            p[x][y] = "A";
            x -= diri[1];
            y -= dirj[1];
        } else if (p[x][y] == dir[2]) {
            p[x][y] = "A";
            x -= diri[2];
            y -= dirj[2];
        } else if (p[x][y] == dir[3]) {
            p[x][y] = "A";
            x -= diri[3];
            y -= dirj[3];
        }

    }
    p[x2][y2] = "E";
}
