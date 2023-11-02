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

const ll mod = 998244353;
const int INF = 1e9;

/* ----------------------------------------------------- GO DOWN ---------------------------------------------------------------------- */


template <const int MAX_N, typename flow_t, typename cost_t, flow_t FLOW_INF, cost_t COST_INF, const int SCALE = 16>
struct CostScalingMCMF {
#define zero_stl(v, sz) fill(v.begin(), v.begin() + (sz), 0)
        struct Edge {
                int v; flow_t c; cost_t d; int r;
                Edge() = default;
                Edge(int v, flow_t c, cost_t d, int r) : v(v), c(c), d(d), r(r) {}
        };
        vector<Edge> g[MAX_N];
        cost_t negativeSelfLoop;
        array<cost_t, MAX_N> pi, excess;
        array<int, MAX_N> level, ptr;
        CostScalingMCMF() { negativeSelfLoop = 0; }
                void clear() {
                        negativeSelfLoop = 0;
                        for (int i = 0; i < MAX_N; i++) g[i].clear();
                }
        void addEdge(int s, int e, flow_t cap, cost_t cost) {
                if (s == e) {
                        if (cost < 0) negativeSelfLoop += cap * cost;
                        return;
                }
                g[s].push_back(Edge(e, cap, cost, sz(g[e])));
                g[e].push_back(Edge(s, 0, -cost, sz(g[s]) - 1));
        }
        flow_t getMaxFlow(int V, int S, int T) {
        auto BFS = [&]() {
        zero_stl(level, V);
        queue<int> q; q.push(S); level[S] = 1;
        for (q.push(S); !q.empty(); q.pop()) {
                int v = q.front();
                for (const auto &e : g[v]) if (!level[e.v] && e.c) q.push(e.v), level[e.v] = level[v] + 1;
        }
        return level[T];
        };
        function<flow_t(int, flow_t)> DFS = [&](int v, flow_t fl) {
        if (v == T || fl == 0) return fl;
        for (int &i = ptr[v]; i < (int)g[v].size(); i++) {
                Edge &e = g[v][i];
                if (level[e.v] != level[v] + 1 || !e.c) continue;
                flow_t delta = DFS(e.v, min(fl, e.c));
                if (delta) {
                e.c -= delta; g[e.v][e.r].c += delta;
                return delta;
                }
        }
        return flow_t(0);
        };
        flow_t maxFlow = 0, tmp = 0;
        while (BFS()) {
        zero_stl(ptr, V);
        while ((tmp = DFS(S, FLOW_INF))) maxFlow += tmp;
        }
        return maxFlow;
        }
        pair<flow_t, cost_t> maxflow(int N, int S, int T) {
        flow_t maxFlow = 0;
        cost_t eps = 0, minCost = 0;
        stack<int, vector<int>> stk;
        auto c_pi = [&](int v, const Edge &edge) { return edge.d + pi[v] - pi[edge.v]; };
        auto push = [&](int v, Edge &edge, flow_t delta, bool flag) {
        delta = min(delta, edge.c);
        edge.c -= delta; g[edge.v][edge.r].c += delta;
        excess[v] -= delta; excess[edge.v] += delta;
        if (flag && 0 < excess[edge.v] && excess[edge.v] <= delta) stk.push(edge.v);
        };
        auto relabel = [&](int v, cost_t delta) { pi[v] -= delta + eps; };
        auto lookAhead = [&](int v) {
        if (excess[v]) return false;
        cost_t delta = COST_INF;
        for (auto &e : g[v]) {
                if (e.c <= 0) continue;
                cost_t cp = c_pi(v, e);
                if (cp < 0) return false;
                else delta = min(delta, cp);
        }
        relabel(v, delta);
        return true;
        };
        auto discharge = [&](int v) {
        cost_t delta = COST_INF;
        for (int i = 0; i < sz(g[v]); i++) {
                Edge &e = g[v][i];
                if (e.c <= 0) continue;
                cost_t cp = c_pi(v, e);
                if (cp < 0) {
                if (lookAhead(e.v)) {
                i--; continue;
                }
                push(v, e, excess[v], true);
                if (excess[v] == 0) return;
                } else delta = min(delta, cp);
        }
        relabel(v, delta);
        stk.push(v);
        };
        zero_stl(pi, N); zero_stl(excess, N);
        for (int i = 0; i < N; i++) for (auto &e : g[i]) minCost += e.c * e.d, e.d *= MAX_N + 1, eps = max(eps, e.d);
        maxFlow = getMaxFlow(N, S, T);
        while (eps > 1) {
        eps /= SCALE;
        if (eps < 1) eps = 1;
        stk = stack<int, vector<int>>();
        for (int v = 0; v < N; v++) for (auto &e : g[v]) if (c_pi(v, e) < 0 && e.c > 0) push(v, e, e.c, false);
        for (int v = 0; v < N; v++) if (excess[v] > 0) stk.push(v);
        while (stk.size()) {
                int top = stk.top(); stk.pop();
                discharge(top);
        }
        }
        for (int v = 0; v < N; v++) for (auto &e : g[v]) e.d /= MAX_N + 1, minCost -= e.c * e.d;
        minCost = minCost / 2 + negativeSelfLoop;
        return {maxFlow, minCost};
        }
};

const ll maxn = 16004;
const ll m2 = 201;

signed main() {

        ios::sync_with_stdio(0);
        cin.tie(0);
        cout.tie(0);

        int n, k;
        cin >> n >> k;
        int N = (1 << n);
        vector<int> a(N);
        for (int i = 0; i < N; i++) {
                //cin >> a[i];
                a[i] = rand((int)1e5, (int)1e6);
        }
        
        CostScalingMCMF<maxn, ll, ll, m2, (ll)1e18, 2> flow;

        vector<array<int, 3>> v;
        for (int i = 0; i < N; i++) {
                int x = __builtin_popcount(i);
                for (int j = 0; j < n; j++) {
                        if (i & (1 << j)) continue;
                        int i2 = (i ^ (1 << j));
                        if (x & 1) v.push_back({a[i] + a[i2], i, i2});
                        else v.push_back({a[i] + a[i2], i2, i});
                }
        }

        sort(all(v));
        reverse(all(v));
        vector<int> f(N, 0);

        int cur = 2;
        for (int i = 0; i < min(4000, sz(v)); i++) {
                int x = v[i][1], y = v[i][2], w = v[i][0];
                if (f[x] == 0) {
                        f[x] = cur++;
                        flow.addEdge(f[x] * 2, f[x] * 2 + 1, 1, 0);
                }
                if (f[y] == 0) {
                        f[y] = cur++;
                        flow.addEdge(f[y] * 2, f[y] * 2 + 1, 1, 0);
                }
                
                flow.addEdge(f[x] * 2 + 1, f[y] * 2, 1, -w);
                flow.addEdge(1, f[x] * 2, 1, 0);
                flow.addEdge(f[y] * 2 + 1, 2, 1, 0);

        }

        flow.addEdge(0, 1, k, 0);
        
        auto [x, y] = flow.maxflow(maxn, 0, 2);

        cout << -y;
       
}
