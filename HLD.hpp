/**
 * Author: islingr, ghoul932(modified)
 * Description: 
 */

struct hld {
    int n; vector<vector<int>> g;
    vector<int> sz, par, depth, head, pos, rpos;
    hld(int n) : n(n), g(n), sz(n), par(n), depth(n), head(n), pos(n), invpos(n) {}
    void add_edge(int u, int v) {
        g[u].push_back(v); g[v].push_back(u);}
    void build(vector<int> roots = {0}) {
        int k = 0;
        for (int r : roots) {
            dfs_sz(r);head[r] = r;
            dfs_hld(r, k);} }
    void dfs_sz(int v) {
        sz[v] = 1;
        for (int &c : g[v]) {
            if (c == par[v]) continue;
            par[c] = v; depth[c] = depth[v] + 1;
            dfs_sz(c); sz[v] += sz[c];
            if (sz[c] > sz[g[v][0]] || g[v][0] == par[v]) swap(c, g[v][0]);
        }
    }
    void dfs_hld(int v, int &k) {
        pos[v] = k++; rpos[pos[v]] = v;
        for (int c : g[v]) {
            if (c == par[v]) continue;
            head[c] = (c == g[v][0] ? head[v] : c);
            dfs_hld(c, k);
        }
    }
    bool is_anc(int u, int v) {
        return pos[u] <= pos[v] && pos[v] < pos[u] + sz[u];}
    int lca(int u, int v) {
        while (!is_anc(head[u], v))
            u = par[head[u]];
        while (!is_anc(head[v], u))
            v = par[head[v]];
        return is_anc(u, v) ? u : v;
    }
    void decompose (int u, int v, vector<pair<int, int>> &up,
                         vector<pair<int, int>> &down) {
        while (!h.is_anc(h.head[u], v)) {
            up.emplace_back(h.pos[h.head[u]], h.pos[u]);
            u = h.par[h.head[u]];
        }
        while (h.head[u] != h.head[v]) {
            down.emplace_back(h.pos[h.head[v]], h.pos[v]);
            v = h.par[h.head[v]];
        }
        if (h.pos[u] < h.pos[v])
            down.emplace_back(h.pos[u], h.pos[v]);
        else
            up.emplace_back(h.pos[v], h.pos[u]);
        reverse(down.begin(), down.end());
    };
};
