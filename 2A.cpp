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

// #define int long long

const long long INF64 = 1e18;
const int INF32 = 2e9 + 2;
const int MOD = 1e9 + 7;
const double PI = acos(-1);
const double E = 2.7182818284590452353602874713527;
double eps = 1e-12;

using ull = unsigned long long;
using ll = long long;

using namespace std;

int sum_nodes = 0;

struct Segtree {
    int l, r, sum = 0;
    Segtree* left = 0;
    Segtree* right = 0;

    Segtree(int l , int r) : l(l), r(r) {
        sum_nodes++;
        int m = (l + r) / 2;
        if (r - l > 1) {
            left = new Segtree(l, m);
            right = new Segtree(m, r);
        }
    }

    void copyLeft() {
        if (left) {
            left = new Segtree(*left);
        }
    }

    void copyRight() {
        if (right) {
            right = new Segtree(*right);
        }
    }

    void add(int idx, int val) {
        sum += val;
        int m = (l + r) / 2;
        if (r - l > 1) {
            if (idx < m) {
                copyLeft();
                left->add(idx, val);
            } else {
                copyRight();
                right->add(idx, val);
            }
        }
    }
};

vector<Segtree*> versions;

void add(int idx, int val, int version) {
    Segtree* root = new Segtree(*versions[version]);
    root->add(idx, val);
    versions.emplace_back(root);
}

int get_kth(Segtree* l_tree, Segtree* r_tree, int k) {
    if (l_tree->r - l_tree->l == 1) {
        return l_tree->l;
    }
    int left_sum = r_tree->left->sum - l_tree->left->sum;
    if (left_sum < k) {
        return get_kth(l_tree->right, r_tree->right, k - left_sum);
    } else {
        return get_kth(l_tree->left, r_tree->left, k);
    }
}

int get_kth(int l, int r, int k) {
    return get_kth(versions[l], versions[r], k);
}

void solve() {
    int n; cin >> n;
    vector<int> a(n), coords(n);
    int l, m; cin >> a[0] >> l >> m;
    for (int i = 1; i < n; i++) {
        a[i] = (a[i - 1] * 1ll * l + m) % (1'000'000'000);
    }
    coords = a;
    sort(coords.begin(), coords.end());
    for (int i = 0; i < n; i++) {
        a[i] = lower_bound(coords.begin(), coords.end(), a[i]) - coords.begin();
    }
    versions.emplace_back(new Segtree(0, n));
    for (int i = 0; i < n; i++) {
        add(a[i], 1, i);
    }
    int B; cin >> B;
    ll res = 0;
    while (B--) {
        int g; cin >> g;
        int x_1, l_x, m_x, y_1, l_y, m_y, k_1, l_k, m_k;
        cin >> x_1>>l_x>> m_x>> y_1>> l_y>> m_y>> k_1>>l_k>>m_k;
        int i = min(x_1, y_1);
        int j = max(x_1, y_1);
        for (int idx = 0; idx < g; idx++) {
            res += coords[get_kth(i - 1, j, k_1)];
            x_1 = ((i - 1) * 1ll * l_x + m_x) % n + 1;
            y_1 = ((j - 1) * 1ll * l_y + m_y) % n + 1;
            i = min(x_1, y_1);
            j = max(x_1, y_1);
            k_1 = ((k_1 - 1) * 1ll * l_k + m_k) % (j - i + 1) + 1;
        }
    }
    cout << res << "\n";
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    cout.precision(20);
    //freopen("rmq.in", "r", stdin);
    //freopen("rmq.out", "w", stdout);
    int T = 1; //cin >> T;
    while (T--) {
        solve();
    }
    return 0;
}

// 10101010
// 1111