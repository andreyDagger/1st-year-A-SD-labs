#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <algorithm>
#include <set>
#include <array>
#include <cmath>
#include <map>
#include <unordered_map>
#include <chrono>
#include <random>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/rope>

using namespace std;
using namespace __gnu_pbds;
using namespace __gnu_cxx;

// #define int long long

const long long INF64 = 1e18;
const int INF32 = 2e9 + 2;
const int MOD = 1e9 + 7;
const double PI = acos(-1);
const double E = 2.7182818284590452353602874713527;
double eps = 1e-12;

using ll = long long;
using ull = unsigned long long;

mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());

int binpow(int a, int p, int mod) {
    if (!p) return 1;
    if (p & 1) return a * binpow(a, p - 1, mod) % mod;
    int temp = binpow(a, p >> 1, mod);
    return temp * temp % mod;
}

struct Node {
    int key, prior, size = 1;
    Node *l = 0, *r = 0;
    Node(int key) : key(key), prior(rng() % INT32_MAX) {}
};

Node* root;

typedef pair<Node*, Node*> Pair;

int size(Node* v) {
    if (v) {
        return v->size;
    } else {
        return 0;
    }
}

void upd(Node* v) {
    v->size = size(v->l) + size(v->r) + 1;
}

Pair split(Node* v, int k) {
    if (!v) {
        return {0, 0};
    }
    if (size(v->l) + 1 <= k) {
        Pair q = split(v->r, k - size(v->l) - 1);
        v->r = q.first;
        upd(v);
        return {v, q.second};
    } else {
        Pair q = split(v->l, k);
        v->l = q.second;
        upd(v);
        return {q.first, v};
    }
}

Node* merge(Node* l, Node* r) {
    if (!l) return r;
    if (!r) return l;
    if (l->prior > r->prior) {
        l->r = merge(l->r, r);
        upd(l);
        return l;
    } else {
        r->l = merge(l, r->l);
        upd(r);
        return r;
    }
}

Node* ctrl_x(int l, int r) {
    Pair q = split(root, r);
    Pair p = split(q.first, l);
    root = merge(p.first, q.second);
    return p.second;
}

void ctrl_v(Node* v) {
    root = merge(v, root);
}

void insert(int x) {
    Pair q = split(root, x);
    Node* t = new Node(x);
    root = merge(q.first, merge(t, q.second));
}

void print(Node* v) {
    if (!v) return;
    print(v->l);
    cout << v->key << " ";
    print(v->r);
}

void solve() {
    int n, m; cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        insert(i);
    }
    while (m--) {
        int l, r; cin >> l >> r;
        l--;
        Node* v = ctrl_x(l, r);
        ctrl_v(v);
//        print(root);
    }
    print(root);
}

// 1 2 3 4 5 6
// 2 3 4 1 5 6
// 4 1 5 2 3 6

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    cout.precision(20);
    //freopen("input.txt", "r", stdin);
    //freopen("rmq.out", "w", stdout);

    solve();

    return 0;
}