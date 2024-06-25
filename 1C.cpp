#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <algorithm>
#include <set>

//#define int long long

using ull = unsigned long long;

using namespace std;

struct Segtree {
    int l, r, sum = 0;
    Segtree* left = 0;
    Segtree *right = 0;

    Segtree(int l, int r) : l(l), r(r) {
        if (r - l > 1) {
            int m = l + r >> 1;
            left = new Segtree(l, m);
            right = new Segtree(m, r);
        }
    }

    void copy() {
        if (left && right) {
            left = new Segtree(*left);
            right = new Segtree(*right);
        }
    }

    void add(int idx, int val) {
        copy();
        sum += val;
        int m = l + r >> 1;
        if (r - l > 1) {
            if (idx < m)
                left->add(idx, val);
            else
                right->add(idx, val);
        }
    }

    int get_sum(int L, int R) {
        if (L >= r || l >= R) return 0;
        if (l >= L && r <= R) return sum;
        int m = l + r >> 1;
        return left->get_sum(L, R) + right->get_sum(L, R);
    }
};

vector<Segtree*> versions;

void add(int idx, int val, int version) {
    Segtree *root = new Segtree(*versions[version]);
    root->add(idx, val);
    versions.emplace_back(root);
}

unsigned int a, b;
unsigned int cur = 0;

unsigned int nextRand17() {
    cur = cur * a + b;
    return cur >> 15;
}

unsigned int nextRand24() {
    cur = cur * a + b;
    return cur >> 8;
}

vector<pair<unsigned int, unsigned int>> full;

bool comp(const pair<int, int>& p1, const pair<int, int>& p2) {
    return p1 < p2;
}

unsigned int get_answer(unsigned int l, unsigned int r, unsigned int x) {
    int idx = upper_bound(full.begin(), full.end(), make_pair(x + 1, -1), comp) - full.begin() - 1;
    unsigned int res = versions[idx + 1]->get_sum(l, r + 1);
    return res;
}

unsigned int get(unsigned int l, unsigned  int r, unsigned int x, unsigned  int y) {
    unsigned int res = get_answer(l, r, y);
    if (x)
        res -= get_answer(l, r, x - 1);
    return res;
}

void solve() {
    unsigned int n = 1 << 17;
    full.resize(n);
    versions.push_back(new Segtree(0, n));
    int q; cin >> q >> a >> b;
    vector<unsigned int> f(n);
    for (int i = 0; i < n; i++) {
        f[i] = nextRand24();
//        f[i] = i;
        full[i] = {f[i], i};
    }
    sort(full.begin(), full.end());
    for (int i = 0; i < n; i++) {
        add(full[i].second, 1, i);
    }

    unsigned int res = 0;
    while (q--) {
        unsigned int l = nextRand17();
        unsigned int r = nextRand17();
        if (l > r) swap(l, r); // получили отрезок [l..r]
        unsigned int x = nextRand24();
        unsigned int y = nextRand24();
        if (x > y) swap(x, y); // получили отрезок [x..y]
//        unsigned l, r, x, y; cin >> l >> r >> x >> y;
        unsigned int c = get(l, r, x, y);
        b += c; // c –- ответ на данный запрос, для ответа на запросы в online
        res += c;
    }
    cout << res << "\n";
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    //freopen("permutation.in", "r", stdin);
    //freopen("permutation.out", "w", stdout);
    solve();
    return 0;
}