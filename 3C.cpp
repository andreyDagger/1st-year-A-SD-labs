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

#define int long long

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
    int sum;
    Node *l = 0, *r = 0;
    Node(int key) : key(key), prior(rand()), sum(key) {}
};

typedef pair<Node*, Node*> Pair;

Node* root_even = 0, *root_odd = 0;

int size(Node* v) {
    if (v) {
        return v->size;
    } else {
        return 0;
    }
}

int sum(Node* v) {
    if (v) {
        return v->sum;
    } else {
        return 0;
    }
}

void upd(Node* v) {
    v->size = size(v->l) + size(v->r) + 1;
    v->sum = sum(v->l) + sum(v->r) + v->key;
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

Node* ctrl_x(Node*& root, int l, int r) {
    if (l >= r) return 0;
    Pair q = split(root, r);
    Pair p = split(q.first, l);
    root = merge(p.first, q.second);
    return p.second;
}

void ctrl_v(Node*& root, Node* v, int l) {
    Pair p = split(root, l);
    root = merge(p.first, merge(v, p.second));
}

void insert(Node*& root, int x) {
    Node* t = new Node(x);
    root = merge(root, t);
}

void print(Node* v) {
    if (!v) return;
    print(v->l);
    cout << v->key << " ";
    print(v->r);
}

void solve(int n, int m) {
    root_odd = 0;
    root_even = 0;
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        if (i % 2 == 0) {
            insert(root_even, x);
        } else {
            insert(root_odd, x);
        }
    }
    while (m--) {
        int t, a, b; cin >> t >> a >> b;
        a--, b--;
        int le = (a + 1) / 2;
        int re = b / 2;
        int lo = a / 2;
        int ro = (b - 1) / 2;
        if (b == 0) ro = -1;
        Node* ve = ctrl_x(root_even, le, re + 1);
        Node* vo = ctrl_x(root_odd, lo, ro + 1);
        if (t == 1) {
            ctrl_v(root_odd, ve, lo);
            ctrl_v(root_even, vo, le);
        } else {
            int res = sum(ve) + sum(vo);
            cout << res << endl;
            ctrl_v(root_even, ve, le);
            ctrl_v(root_odd, vo, lo);
        }
    }
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    cout.precision(20);
    //freopen("input.txt", "r", stdin);
    //freopen("rmq.out", "w", stdout);

    for (int i = 1;; i++) {
        int n, m; cin >> n >> m;
        if (n == 0) break;
        if (i != 1) cout << "\n";
        cout << "Swapper " << i << ":\n";
        solve(n, m);
    }

    return 0;
}