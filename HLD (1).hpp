/**
 * Author: islingr
 * Description: 
 */

struct hld {
	int n;
	vector<vector<int>> g;
	vector<int> nxt, in, sub, par;
	int tim;
	hld(const vector<vector<int>>& _g, int root)
		: n{(int)_g.size()}, g{_g}, nxt(n), in(n), sub(n), par(n), tim{0} {
		dfs_sz(root);
		nxt[root] = par[root] = root;
		dfs_hld(root);
	}

	void dfs_sz(int u) {
		sub[u] = 1;
		for (auto& v : g[u]) {	// & is important
			if (auto it = find(all(g[v]), u); it != g[v].end()) g[v].erase(it);
			par[v] = u;
			dfs_sz(v);
			sub[u] += sub[v];
			if (sub[v] > sub[g[u][0]]) swap(v, g[u][0]);
		}
	}

	void dfs_hld(int u) {
		in[u] = tim++;
		for (auto v : g[u]) {
			nxt[v] = v == g[u][0] ? nxt[u] : v;
			dfs_hld(v);
		}
	}

	bool is_ancestor(int u, int v) {
		// u is ancestor v
		return in[u]<=in[v] && in[v]<in[u]+sub[u];
	}
	int lca(int u, int v) {
		while (!is_ancestor(nxt[u], v)) u = par[nxt[u]];
		while (nxt[u] != nxt[v]) v = par[nxt[v]];
		return in[u] < in[v] ? u : v;
	}

	vector<pair<int, int>> get_ranges(int u, int v) {
		// the O(log n) ranges corresponding to the path (u, v) in order

		vector<pair<int, int>> L, R;
		// L = u -> before lca
		// R = before lca -> v
		while (!is_ancestor(nxt[u], v)) {
			L.emplace_back(in[nxt[u]], in[u]);
			u = par[nxt[u]];
		}
		while (nxt[u] != nxt[v]) {
			R.emplace_back(in[nxt[v]], in[v]);
			v = par[nxt[v]];
		}
		L.push_back(minmax({in[u], in[v]}));
		L.reserve(sz(L) + sz(R));
		L.insert(L.end(), rall(R));
		return L;
	}
};

