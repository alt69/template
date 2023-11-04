//#pragma GCC optimize("O3")
//#pragma GCC optimize("Ofast")
//#pragma GCC optimize("unroll-loops")
//#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")


#include<bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;


struct custom_hash {
        static uint64_t splitmix64(uint64_t x) {
                // http://xorshift.di.unimi.it/splitmix64.c
                x += 0x9e3779b97f4a7c15;
                x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
                x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
                return x ^ (x >> 31);
        }

        size_t operator()(uint64_t x) const {
                static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
                return splitmix64(x + FIXED_RANDOM);
        }
};
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<typename T>
T rand(T a, T b){
    return uniform_int_distribution<T>(a, b)(rng);
}

typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
typedef tree<pair<int,int>, null_type, less<pair<int,int>>, rb_tree_tag, tree_order_statistics_node_update> ordered_multiset;
typedef long long ll;
typedef long double ld;
typedef vector<ll> vl;
typedef vector<int> vi;


#define rep(i, a, b) for(int i = a; i < b; i++)
#define all(x) begin(x), end(x)
#define sz(x) static_cast<int>((x).size())
//#define int long long

const int mod = 1e9 + 1;
const ll INF = 1e18;

/* ----------------------------------------------------- GO DOWN ---------------------------------------------------------------------- */

struct node {
        int mx = 0;
        int lazy = 0;
        int l = -1;
        int r = -1;
};

vector<node> nodes(25e6);
int cur = 0;

void upd (int x, int v) {
        nodes[x].mx += v;
        nodes[x].lazy += v;
} 
void create (int x, int m, int rx) {
        if (nodes[x].l == -1) {
                nodes[cur].mx = m - 1;
                nodes[x].l = cur++;
        }
        if (nodes[x].r == -1) {
                nodes[cur].mx = rx - 1;
                nodes[x].r = cur++;
        }
}
void propagate (int x) {
        upd(nodes[x].l, nodes[x].lazy);
        upd(nodes[x].r, nodes[x].lazy);
        nodes[x].lazy = 0;
}
void merge (int x) {
        nodes[x].mx = max(nodes[nodes[x].l].mx, nodes[nodes[x].r].mx);
}
void update (int x, int lx, int rx, int l, int r, int v) {
        if (rx - lx == 1) {
                upd(x, v);
                return;
        }
        if (lx >= l && rx <= r) {
                upd(x, v);
                return;
        }
        int m = (lx + rx) / 2;
        create(x, m, rx);
        propagate(x);
        if (m > l) update(nodes[x].l, lx, m, l, r, v);
        if (m < r) update(nodes[x].r, m, rx, l, r, v); 
        merge(x);
}

int get (int x, int lx, int rx, int t) {
        if (rx - lx == 1) return lx;
        int m = (lx + rx) / 2;
        create(x, m, rx);
        propagate(x);
        int l = nodes[x].l, r = nodes[x].r;
        if (nodes[l].mx >= t) return get(l, lx, m, t);
        return get(r, m, rx, t);
}

int get_t (int x, int lx, int rx, int i) {
        //cout << x << " " << lx << " " << rx << " " << nodes[x].mx << "\n";
        if (rx - lx == 1) return nodes[x].mx;
        int m = (lx + rx) / 2;
        int l = nodes[x].l, r = nodes[x].r;
        if (m > i) {
                if (l == -1) return i + nodes[x].lazy;
                propagate(x);
                return get_t(l, lx, m, i);
        }
        if (r == -1) return i + nodes[x].lazy;
        propagate(x);
        return get_t(r, m, rx, i);
}

const int maxn = 2e5;
int p[maxn], mx[maxn], mn[maxn], sz[maxn], mxi[maxn], mni[maxn];

int get (int x) {
        return p[x] = (p[x] == x ? x : get(p[x]));
}
void merge (int x, int y) {
        x = get(x); y = get(y);
        if (sz[x] < sz[y]) swap(x, y);
        p[y] = x;
        mn[x] = min(mn[x], mn[y]);
        mx[x] = max(mx[x], mx[y]);
        mxi[x] = max(mxi[x], mxi[y]);
        mni[x] = min(mni[x], mni[y]);
}

signed main() {

        ios::sync_with_stdio(0);
        cin.tie(0);
        cout.tie(0);

        int n;
        cin >> n;

        int lx = 0, rx = (1 << 30);
        int top = cur++;
        nodes[top].mx = rx - 1;

        int ans = 0;

        vector<int> ti(n);
        vector<int> qu[n];
        vector<int> tii(n);

        for (int i = 0; i < n; i++) {
                cin >> ti[i];
                tii[i] = i;
                int q;
                cin >> q;
                qu[i].resize(q);
                for (int j = 0; j < q; j++) cin >> qu[i][j];
        }

        sort(all(ti));
        ti.erase(unique(all(ti)), ti.end());
        map<int, int> pos;
        
        for (int i = 0; i < sz(ti); i++) {
                pos[ti[i]] = i;
                p[i] = i;
                mx[i] = ti[i];
                mn[i] = ti[i];
                mxi[i] = i;
                mni[i] = i;
                sz[i] = 1;
        }

        for (auto t : tii) {

                int x = get(top, lx, rx, t);
                int i = pos[t];
                i = get(i);
                cout << i << " ";
                int l = mn[i], r = mx[i];

                cout << l << " " << r << "?\n";

                if (l > 0) update(top, lx, rx, 0, l, 1);
                if (r < rx) update(top, lx, rx, r, rx, -1);
                
                l--; mn[i]--;
                r++; mx[i]++;
                int prev = mni[i] - 1;
                int nex = mxi[i] + 1;
                if (prev >= 0 && mn[i] <= mx[prev]) merge(i, prev);
                if (nex < sz(ti) && mx[i] >= mn[nex]) merge(i, nex);
 
                
                for (auto y : qu[i]) {
                        y = (y + ans) % mod;
                        cout << y << " ";
                        ans = get_t(top, lx, rx, y);
                        cout << ans << "\n";
                }


        }
        
}
