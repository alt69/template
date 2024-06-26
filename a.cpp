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
#define int long long

const ll mod = 998244353;
const ll INF = 1e18;



/* mint
int norm (int x) {
        if (x < 0) x += mod;
        if (x >= mod) x -= mod;
        return x;
}
template<class T>
T power(T a, int b) {
        T res = 1;
        for (; b; b /= 2, a *= a) {
                if (b & 1) res *= a;
        }
        return res;
}
struct Z {
        int x;
        Z(int x = 0) : x(norm(x)) {}
        int val() const {
                return x;
        }
        Z operator-() const {
                return Z(norm(mod - x));
        }
        Z inv() const {
                return power(*this, mod - 2);
        }
        Z &operator*=(const Z &rhs) {
                x = x * rhs.x % mod;
                return *this;
        }
        Z &operator+=(const Z &rhs) {
                x = norm(x + rhs.x);
                return *this;
        }
        Z &operator-=(const Z &rhs) {
                x = norm(x - rhs.x);
                return *this;
        }
        Z &operator/=(const Z &rhs) {
                return *this *= rhs.inv();
        }
        friend Z operator*(const Z &lhs, const Z &rhs) {
                Z res = lhs;
                res *= rhs;
                return res;
        }
        friend Z operator+(const Z &lhs, const Z &rhs) {
                Z res = lhs;
                res += rhs;
                return res;
        }
        friend Z operator-(const Z &lhs, const Z &rhs) {
                Z res = lhs;
                res -= rhs;
                return res;
        }
        friend Z operator/(const Z &lhs, const Z &rhs) {
                Z res = lhs;
                res /= rhs;
                return res;
        }
        friend std::istream &operator>>(std::istream &is, Z &a) {
                int v;
                is >> v;
                a = Z(v);
                return is;
        }
        friend std::ostream &operator<<(std::ostream &os, const Z &a) {
                return os << a.val();
        }
}; */

/* combi init
const int maxn = 501;
Z fact[maxn];
Z ifact[maxn];
Z C (int n, int r) {
        return fact[n] * ifact[r] * ifact[n - r];
}

        fact[0] = 1;
        for (int i = 1; i < maxn; i++) fact[i] = fact[i - 1] * i;
        ifact[maxn - 1] = Z(1) / fact[maxn - 1]; 
        for (int i = maxn - 2; i >= 0; i--) ifact[i] = ifact[i + 1] * (i + 1); 
*/

Z summation_i_power_k (int n, int k) {
        if (n == 0) return 0;
        Z a[k + 3];
        for (int i = 1; i <= k + 2; i++) a[i] = (a[i - 1] + power(Z(i), k));
        if (n <= k + 2) return a[n];
        Z totn = 1, tot = 1, ans = 0;
        for (int i = 1; i <= k + 2; i++) totn *= (n - i);
        for (int i = 2; i <= k + 2; i++) tot *= (1 - i);
        for (int i = 1; i <= k + 2; tot = tot * i / (i - k - 2), i++)
                ans += a[i] * totn / (n - i) / tot;
        return ans;
}

int inversemod998244353 (int n) {
        int b = 1;
        int a = mod - 2;
        while (a > 0) {
                if (a & 1) {
                        b = b * n;
                        b %= mod;
                }
                n = n * n;
                n %= mod;
                a >>= 1; 
        }
        return b;
}

struct SegtreeGCD {
        int size;
        vector<int> gcds;
        vector<pair<int, int>> lr;
        void init(int n) {
                size = 1;
                while (size < n) size *= 2;
                gcds.assign(2 * size - 1, 1);
                lr.resize(2 * size - 1);
        }
        

        void build(vector<int> &a, int x, int lx, int rx) {
                lr[x] = {lx, rx};
                if (rx - lx == 1) {
                        if (lx < sz(a)) gcds[x] = a[lx];
                        return;
                }
                int m = (lx + rx) / 2;
                build(a, 2 * x + 1, lx, m);
                build(a, 2 * x + 2, m, rx);
                gcds[x] = gcd(gcds[2 * x + 1], gcds[2 * x + 2]);
        }


        void build(vector<int> &a) {
                build(a, 0, 0, size);
        }
        void out(){
                cout << sz(gcds) << "\n";
                for(int i = 0; i< sz(gcds); i++) cout << gcds[i] << " ";
        }

        int get(int l, int r, int x, int lx, int rx, int yet) {
                if (rx - lx == 1) {
                        yet = gcd(yet, gcds[x]);
                        return yet;
                }
                if (lx >= l && rx <= r) {
                        yet = gcd(yet, gcds[x]);
                        return yet;
                }
                int m = (lx + rx) / 2;
                if(m > l) {
                        int y = get(l, r, 2 * x + 1, lx, m, yet);
                        yet = gcd(yet, y);
                }
                if (m < r) {
                        int y = get(l, r, 2 * x + 2, m, rx, yet);
                        yet = gcd(yet, y);
                }
                return yet;
                
        }

        int get(int l, int r, int b) {
                return get(l, r, 0, 0, size, b);
        }


};

struct SegtreeMax {
        int size;
        vector<int> maxv;
        void init (int n) {
                size = 1;
                while (size < n) size *= 2;
                maxv.assign (2 * size - 1, -INF);
        }        
 
        void build (vector<int> &a, int x, int lx, int rx) {
                if (rx - lx == 1) {
                        if (lx < sz(a)) maxv[x] = a[lx];
                        else maxv[x] = -INF;
                        return;
                }
                int m = (lx + rx) / 2;
                build (a, 2 * x + 1, lx, m);
                build (a, 2 * x + 2, m, rx);
                maxv[x] = max(maxv[2 * x + 1], maxv[2 * x + 2]);
        }
 
        void set (int i, int v, int x, int lx, int rx) {
                if (rx - lx == 1) {
                        maxv[x] = v;
                        return;
                }
                int m = (lx + rx)/ 2;
                if (i < m) {
                        set (i, v, 2 * x + 1, lx, m);
                } else {
                        set (i, v, 2 * x + 2, m, rx);
                }
                maxv[x] = max(maxv[2 * x + 1], maxv[2 * x + 2]);
        }
        void set (int i, int v) {
                set (i, v, 0, 0, size);
        }
 
        void build (vector<int> &a) {
                build (a, 0, 0, size);
        }
        void out () {
                cout << sz(maxv) << "\n";
                for(int i = 0; i < sz(maxv); i++) cout << maxv[i] << " ";
        }
 
        int get (int l, int r, int x, int lx, int rx) {
                
                if (lx >= r || l >= rx) return -INF;
                if (rx - lx == 1) {
                        return maxv[x];
                }
                if (lx >= l && rx <= r) {
                        return maxv[x];
                }
                int m = (lx + rx) / 2;
                int s1 = get (l, r, 2 * x + 1, lx, m);
                int s2 = get (l, r, 2 * x + 2, m, rx);
                return max(s1, s2);
                
        }
 
        int get (int l, int r) {
                return get (l, r, 0, 0, size);
        } 
 
};

/* Segtree to get first element before current element with diff(max - min) > d
struct SegtreeMm {

        int size;
        vector<int> minv;
        vector<int> maxv;

        void init (int n) {
                size = 1;
                while (size < n) size *= 2;
                minv.assign(2 * size - 1, INF);
                maxv.assign(2 * size - 1, -INF);
        }        
 
        void build (vector<int> &a, int x, int lx, int rx) {
                if (rx - lx == 1) {
                        if (lx < sz(a)) minv[x] = a[lx], maxv[x] = a[lx];
                        else minv[x] = INF, maxv[x] = -INF;
                        return;
                }
                int m = (lx + rx) / 2;
                build (a, 2 * x + 1, lx, m);
                build (a, 2 * x + 2, m, rx);
                minv[x] = min(minv[2 * x + 1], minv[2 * x + 2]);
                maxv[x] = max(maxv[2 * x + 1], maxv[2 * x + 2]);
        }
 
        void set (int i, int v, int x, int lx, int rx) {

                if (rx - lx == 1) {
                        minv[x] = v;
                        maxv[x] = v;
                        return;
                }

                int m = (lx + rx)/ 2;
                if (i < m) set (i, v, 2 * x + 1, lx, m);
                else set (i, v, 2 * x + 2, m, rx);

                minv[x] = min(minv[2 * x + 1], minv[2 * x + 2]);
                maxv[x] = max(maxv[2 * x + 1], maxv[2 * x + 2]);
        }
        void set (int i, int v) {
                set (i, v, 0, 0, size);
        }
 
        void build (vector<int> &a) {
                build (a, 0, 0, size);
        }
 
        int getmin (int l, int r, int x, int lx, int rx) {                
                if (lx >= r || l >= rx) return INF;
                if (rx - lx == 1) {
                        return minv[x];
                }
                if (lx >= l && rx <= r) {
                        return minv[x];
                }
                int m = (lx + rx) / 2;
                int s1 = getmin (l, r, 2 * x + 1, lx, m);
                int s2 = getmin (l, r, 2 * x + 2, m, rx);
                return min (s1, s2);                
        }
 
        int getmin (int l, int r) {
                return getmin (l, r, 0, 0, size);
        }

        int getmax (int l, int r, int x, int lx, int rx) {
                
                if (lx >= r || l >= rx) return -INF;
                if (rx - lx == 1) {
                        return maxv[x];
                }
                if (lx >= l && rx <= r) {
                        return maxv[x];
                }
                int m = (lx + rx) / 2;
                int s1 = getmax (l, r, 2 * x + 1, lx, m);
                int s2 = getmax (l, r, 2 * x + 2, m, rx);
                return max (s1, s2);
                
        }

        int getmax (int l, int r) {
                return getmax (l, r, 0, 0, size);
        }
        
        int bef (int i, int diff) {
                int cur = i + size - 1;
                int mn = minv[cur], mx = maxv[cur];
                bool f = false;
                while (cur > 0) {
                        if ((cur & (cur + 1)) == 0) {
                                f = true;
                                break;
                        }
                        if (cur & 1 == 1) cur = cur / 2;
                        else if (maxv[cur - 1] - min(mn, minv[cur - 1]) <= diff && max(mx, maxv[cur - 1]) - minv[cur - 1] <= diff) {
                                mn = min(mn, minv[cur - 1]);
                                mx = max(mx, maxv[cur - 1]);
                                cur = (cur - 1) / 2;
                        }
                        else {
                                cur = cur - 1;
                                break;
                        }
                }
                if (f) {
                        if (maxv[cur * 2 + 1] - min(mn, minv[cur * 2 + 1]) <= diff) return -1;
                        cur = cur * 2 + 1;
                }
                while (cur * 2 + 2 < 2 * size - 1) {
                        if (maxv[cur * 2 + 2] - min(mn, minv[cur * 2 + 2]) <= diff && max(mx, maxv[cur * 2 + 2]) - minv[cur * 2 + 2] <= diff) {
                                mn = min(mn, minv[cur * 2 + 2]);
                                mx = max(mx, maxv[2 * cur + 2]);
                                cur = cur * 2 + 1;
                        }
                        else cur = cur * 2 + 2;
                }
                return cur - size + 1;
        }
 
}; */

struct SegtreeSum {

        int size;
        vector<int> sums;
        void init(int n) {
                size = 1;
                while (size < n) size *= 2;
                sums.assign(2 * size - 1, 0);
        }        
 
        void build (vector<int> &a, int x, int lx, int rx) {
                if (rx - lx == 1) {
                        if (lx < sz(a)) sums[x] = a[lx];
                        else sums[x] = 0;
                        return;
                }
                int m = (lx + rx) / 2;
                build(a, 2 * x + 1, lx, m);
                build(a, 2 * x + 2, m, rx);
                sums[x] = sums[2 * x + 1] + sums[2 * x + 2];
        }

        void build (vector<int> &a) {
                build(a, 0, 0, size);
        }
 
        void set (int i, int v, int x, int lx, int rx) {
                if (rx - lx == 1) {
                        sums[x] = v;
                        return;
                }
                int m = (lx + rx)/ 2;
                if (i < m) set(i, v, 2 * x + 1, lx, m);
                else set(i, v, 2 * x + 2, m, rx);
                sums[x] = sums[2 * x + 1] + sums[2 * x + 2];
        }

        void set (int i, int v) {
                set(i, v, 0, 0, size);
        }
 
        int sum (int l, int r, int x, int lx, int rx) {                
                if (lx >= r || l >= rx) return 0;
                if (rx - lx == 1) return sums[x];
                if (lx >= l && rx <= r) return sums[x];
                int m = (lx + rx) / 2;
                int s1 = sum(l, r, 2 * x + 1, lx, m);
                int s2 = sum(l, r, 2 * x + 2, m, rx);
                return s1 + s2;                
        }
 
        int sum (int l, int r) {
                return sum(l, r, 0, 0, size);
        }
 
};

struct SegtreeFirstAbove {
        int size;
        vector<int> vals;
        vector<pair<int, int>> lr;
        void init(int n) {
                size = 1;
                while (size < n) size *= 2;
                vals.assign(2 * size - 1, 1);
                lr.resize(2 * size - 1);
        }
        

        void build(vector<int> &a, int x, int lx, int rx) {
                lr[x] = {lx, rx};
                if (rx - lx == 1) {
                        if (lx < sz(a)) vals[x] = a[lx];
                        return;
                }
                int m = (lx + rx) / 2;
                build(a, 2 * x + 1, lx, m);
                build(a, 2 * x + 2, m, rx);
                vals[x] = max(vals[2 * x + 1], vals[2 * x + 2]);
        }


        void build(vector<int> &a) {
                build(a, 0, 0, size);
        }
        
        void set(int i, int v, int x) {
                vals[x] = max(vals[2 * x + 1], vals[2 * x + 2]);
                if (x == 0) return;
                set(i, v, (x - 1)/2);
        }

        void set(int i, int v) {
                int x = i + size - 1;
                vals[x] -= v;
                set(i, v, (x - 1)/ 2);
        }

        int get(int v, int x, int lx, int rx) {
                if (rx - lx == 1) return lx;
                int m = (lx + rx) / 2;
                int a = vals[2 * x + 1];
                int b = vals[2 * x + 2];
                if(a >= v) return get(v, 2 * x + 1, lx, m);
                return get(v, 2 * x + 2, m, rx);
                
        }

        int get(int v) {
                if (vals[0] < v) return -1;
                return get(v, 0, 0, size);
        }

        void out(){
                cout << sz(vals) << "\n";
                for(int i = 0; i< sz(vals); i++) cout << vals[i] << " ";
                cout << "\n";
        }


};

/* LazyRangeSum Sum

struct lazy {
        int sum, lazyy;
};

struct SegtreeLazyRangeSum {
        int nn;
        int size;
        vector<lazy> vals;
        vector<pair<int, int>> lr;
        void init (int n) {
                nn = n;
                size = 1;
                while (size < n) size *= 2;
                vals.resize(2 * size - 1);
                lr.resize(2 * size - 1);
        }
        
        lazy merge (lazy a, lazy b) {
                return {a.sum + b.sum, 0};
        }

        void build (vector<int> &a, int x, int lx, int rx) {
                lr[x] = {lx, rx};
                if (rx - lx == 1) {
                        if (lx < sz(a)) vals[x] = {a[lx], 0};
                        else vals[x] = {0, 0};
                        return;
                }
                int m = (lx + rx) / 2;
                build(a, 2 * x + 1, lx, m);
                build(a, 2 * x + 2, m, rx);
                vals[x] = merge(vals[2 * x + 1], vals[2 * x + 2]);
        }

        void build (vector<int> &a) {
                build(a, 0, 0, size);
        }
        
        void RangeUpdate (int l, int r, int x, int lx, int rx, int v) {
                if (rx - lx == 1) {
                        vals[x].lazyy += v;
                        return;
                }
                if (lx >= l && rx <= r) {
                        vals[x].lazyy += v;
                        return;
                }
                int m = (lx + rx) / 2;
                if (m > l) {
                        RangeUpdate(l, r, 2 * x + 1, lx, m, v);
                }
                if (m < r) {
                        RangeUpdate(l, r, 2 * x + 2, m, rx, v);
                }
                vals[x].sum = vals[2 * x + 1].sum + vals[2 * x + 2].sum + vals[2 * x + 1].lazyy * (m - lx) + vals[2 * x + 2].lazyy * (rx - m);
                
        }

        void RangeUpdate (int l, int r, int v) {
                RangeUpdate(l, r, 0, 0, size, v);
        }

        int get (int i, int x, int lx, int rx) {
                if (rx - lx == 1) {
                        vals[x].sum += vals[x].lazyy;
                        vals[x].lazyy = 0;
                        return vals[x].sum;
                }
                int m = (lx + rx) / 2;
                int a = vals[x].lazyy;
                vals[2 * x + 1].lazyy += a;
                vals[2 * x + 2].lazyy += a;
                vals[x].lazyy = 0;
                vals[x].sum += (rx - lx) * a;

                if (i < m) return get(i, 2 * x + 1, lx, m);
                return get(i, 2 * x + 2, m, rx);
                
        }

        int get (int i) {
                
                return get(i, 0, 0, size);
        }

        int get(int l, int r, int x, int lx, int rx) {
                if (rx - lx == 1) {
                        vals[x].sum += vals[x].lazyy;
                        vals[x].lazyy = 0;
                        return vals[x].sum;
                }
                if (lx >= l && rx <= r) {
                        return vals[x].sum + vals[x].lazyy * (rx - lx);
                }
                int m = (lx + rx) / 2;
                int aa = 0, bb = 0;
                int a = vals[x].lazyy;
                vals[x].sum += (rx - lx) * a;
                vals[x].lazyy = 0;
                vals[2 * x + 1].lazyy += a; vals[2 * x + 2].lazyy += a;
                if (m > l) {
                        aa = get(l, r, 2 * x + 1, lx, m);
                }
                if (m < r) {
                        bb = get(l, r, 2 * x + 2, m, rx);
                }
                return aa + bb;

                
        }

        int get(int l, int r) {     

                return get(l, r, 0, 0, size);
        }

        void out () {
                
                for (int i = 0; i < sz(vals); i++) cout << vals[i].sum << " " << vals[i].lazyy << "  ";
                cout << "\n";
        }

        void outt () {
                for (int i = 0; i < nn; i++) cout << get(i, i + 1) << " ";
                cout << "\n";
        }


}; 
*/

/* LazyRangeSum Min
struct lazy {
        int val, lazyy;
};

struct SegtreeLazy {
        int size;
        vector<lazy> val;
        void init (int n) {
                size = 1;
                while (size < n) size *= 2;
                val.resize (2 * size - 1);
        }

        lazy merge (int x, int y) {
                return {min (val[x].val, val[y].val), 0};
        }        
        
        void propagate (int x) {
                val[2 * x + 1].val += val[x].lazyy;
                val[2 * x + 2].val += val[x].lazyy;
                val[2 * x + 1].lazyy += val[x].lazyy;
                val[2 * x + 2].lazyy += val[x].lazyy;
                val[x].lazyy = 0;
        }

        void build (vector<int> &a, int x, int lx, int rx) {
                if (rx - lx == 1) {
                        if (lx < sz(a)) val[x] = {a[lx], 0};
                        else val[x] = {INF, 0};
                        return;
                }
                int m = (lx + rx) / 2;
                build (a, 2 * x + 1, lx, m);
                build (a, 2 * x + 2, m, rx);
                val[x] = merge (2 * x + 1, 2 * x + 2);
        }

        void build (vector<int> &a) {
                build (a, 0, 0, size);
        }
        
        void RangeUpdate (int l, int r, int x, int lx, int rx, int v) {
                if (rx - lx == 1) {
                        val[x].val += v;
                        return;
                }
                if (lx >= l && rx <= r) {
                        val[x].val += v;
                        val[x].lazyy += v;
                        return;
                }
                int m = (lx + rx) / 2;
                propagate (x);
                if (m > l) {
                        RangeUpdate (l, r, 2 * x + 1, lx, m, v);
                }
                if (m < r) {
                        RangeUpdate (l, r, 2 * x + 2, m, rx, v);
                }
                val[x] = merge (2 * x + 1, 2 * x + 2);
                
        }

        void update (int l, int r, int v) {
                RangeUpdate (l, r, 0, 0, size, v);
        }

        int get (int l, int r, int x, int lx, int rx) {
                if (rx - lx == 1) {
                        return val[x].val;
                }
                if (lx >= l && rx <= r) {
                        return val[x].val;
                }
                int m = (lx + rx) / 2;
                propagate (x);
                int a1 = INF, a2 = INF;
                if (m > l) {
                        a1 = get (l, r, 2 * x + 1, lx, m);
                }
                if (m < r) {
                        a2 = get (l, r, 2 * x + 2, m, rx);
                }
                return min (a1, a2);                
        }

        int get (int l, int r) {
                return get (l, r, 0, 0, size);
        }
        void out () {                
                for (int i = 0; i < sz(val); i++) cout << val[i].val << " " << val[i].lazyy << "  ";
        }

};
*/

/* LazyRangeSum MinFreq

struct lazy {
        int min, freq;
        int lazyy;
};

struct SegtreeLazy {
        int size;
        vector<lazy> val;
        void init (int n) {
                size = 1;
                while (size < n) size *= 2;
                val.resize (2 * size - 1);
        }
        
        lazy merge (int x, int y) {
                if (val[x].min == val[y].min) return {val[x].min, val[x].freq + val[y].freq, 0};
                if (val[x].min < val[y].min) return {val[x].min, val[x].freq, 0};
                return {val[y].min, val[y].freq, 0};
        }
        void propagate (int x) {
                val[2 * x + 1].min += val[x].lazyy;
                val[2 * x + 2].min += val[x].lazyy;
                val[2 * x + 1].lazyy += val[x].lazyy;
                val[2 * x + 2].lazyy += val[x].lazyy;
                val[x].lazyy = 0;
        } 

        void build (vector<int> &a, int x, int lx, int rx) {
                if (rx - lx == 1) {
                        if (lx < sz(a)) val[x] = {a[lx], 1, 0};
                        else val[x] = {INF, 1, 0};
                        return;
                }
                int m = (lx + rx) / 2;
                build (a, 2 * x + 1, lx, m);
                build (a, 2 * x + 2, m, rx);
                val[x] = merge (2 * x + 1, 2 * x + 2);
        }

        void build (vector<int> &a) {
                build (a, 0, 0, size);
        }
        
        void RangeUpdate (int l, int r, int x, int lx, int rx, int v) {
                if (rx - lx == 1) {
                        val[x].min += v;
                        return;
                }
                if (lx >= l && rx <= r) {
                        val[x].min += v;
                        val[x].lazyy += v;
                        return;
                }
                propagate (x);
                int m = (lx + rx) / 2;
                if (m > l) {
                        RangeUpdate (l, r, 2 * x + 1, lx, m, v);
                }
                if (m < r) {
                        RangeUpdate (l, r, 2 * x + 2, m, rx, v);
                }
                val[x] = merge (2 * x + 1, 2 * x + 2);
                
        }

        void update (int l, int r, int v) {
                RangeUpdate (l, r, 0, 0, size, v);
        }

        array<int, 2> get (int l, int r, int x, int lx, int rx) {
                if (rx - lx == 1) {
                        return {val[x].min, val[x].freq};
                }
                if (lx >= l && rx <= r) {
                        return {val[x].min, val[x].freq};
                }
                int m = (lx + rx) / 2;
                propagate (x);
                array<int, 2> a1 = {INF, 0}, a2 = {INF, 0};

                if (m > l) {
                        a1 = get(l, r, 2 * x + 1, lx, m);
                }
                if (m < r) {
                        a2 = get(l, r, 2 * x + 2, m, rx);
                }
                if (a1[0] == a2[0]) return {a1[0], a1[1] + a2[1]};
                if (a1[0] < a2[0]) return {a1[0], a1[1]};
                return {a2[0], a2[1]};
        }

        array<int, 2> get (int l, int r) {    
                return get (l, r, 0, 0, size);
        }

        void out () {                
                for (int i = 0; i < sz(val); i++) cout << val[i].min << " " << val[i].freq << " " << val[i].lazyy << "  ";
        }


};  */

/* Persistent SetSum, get first pos j > 0 such that unique[0, j] > k
struct node {
        node *l = NULL;
        node *r = NULL;
        int val = 0;
};

inline void merge (node *x) {
        x->val = x->l->val + x->r->val;
}

void build (node *x, int lx, int rx) {
        if (rx - lx == 1) {
                x->val = 0;
                return;
        }
        int m = (lx + rx) / 2;
        node *l = new node;
        node *r = new node;
        x->l = l; x->r = r;
        build(l, lx, m);
        build(r, m, rx);
        merge(x);
}

void update (node *x, node *y, int i, int v, int lx, int rx) {
        if (rx - lx == 1) {
                x->val = v;
                return;
        }
        int m = (lx + rx) / 2;
        if (i < m) {
                x->r = y->r;
                x->l = new node;
                update(x->l, y->l, i, v, lx, m);
        }
        else {
                x->l = y->l;
                x->r = new node;
                update(x->r, y->r, i, v, m, rx);
        }
        merge(x);
}

int get (node *x, int lx, int rx, int k, int cyet) {
        if (rx - lx == 1) return lx;
        int m = (lx + rx) / 2;
        if (x->l->val + cyet <= k) {
                return get(x->r, m, rx, k, cyet + x->l->val);
        } 
        else {
                return get(x->l, lx, m, k, cyet);
        }
} */

/* Persistent SetSum
struct node {
        int l = -1;
        int r = -1;
        int val = 0;
};
const int maxn = 23e5;
node st[maxn];
int cur = 0;

void build (int x, int lx, int rx) {
        if (rx - lx == 1) return;
        int m = ((lx + rx) >> 1);
        st[x].l = cur++;
        st[x].r = cur++;
        build(st[x].l, lx, m);
        build(st[x].r, m, rx);
}

void add (int x, int y, int lx, int rx, int i) {
        if (rx <= lx) return;
        if (rx - lx == 1) {
                st[x].val = 1;
                return;
        }
        int m = ((lx + rx) >> 1);
        if (i < m) {
                st[x].l = cur++;
                st[x].r = st[y].r;
                add(st[x].l, st[y].l, lx, m, i);
        }
        else {
                st[x].r = cur++;
                st[x].l = st[y].l;
                add(st[x].r, st[y].r, m, rx, i);
        }
        st[x].val = st[st[x].r].val + st[st[x].l].val;
}

int get (int x, int lx, int rx, int l, int r) {
        if (lx >= r || l >= rx || rx <= lx) return 0;
        if (rx - lx == 1 || (lx >= l && rx <= r)) return st[x].val;
        int m = ((lx + rx) >> 1);
        return get(st[x].l, lx, m, l, r) + get(st[x].r, m, rx, l, r);
} */

/* Persistent SetMin
struct node {
        int l = -1;
        int r = -1;
        int val = INF;
};
const int maxn = 69e5;
vector<node> st(maxn);
int cur = 0;

void build (int x, int lx, int rx) {
        if (rx - lx == 1) return;
        int m = ((lx + rx) >> 1);
        st[x].l = cur++;
        st[x].r = cur++;
        build(st[x].l, lx, m);
        build(st[x].r, m, rx);
}

void add (int x, int y, int lx, int rx, int i, int j) {
        if (rx <= lx) return;
        if (rx - lx == 1) {
                st[x].val = j;
                return;
        }
        int m = ((lx + rx) >> 1);
        if (i < m) {
                st[x].l = cur++;
                st[x].r = st[y].r;
                add(st[x].l, st[y].l, lx, m, i, j);
        }
        else {
                st[x].r = cur++;
                st[x].l = st[y].l;
                add(st[x].r, st[y].r, m, rx, i, j);
        }
        st[x].val = min(st[st[x].r].val, st[st[x].l].val);
}

int get (int x, int lx, int rx, int l, int r) {
        if (lx >= r || l >= rx || rx <= lx) return INF;
        if (rx - lx == 1 || (lx >= l && rx <= r)) return st[x].val;
        int m = ((lx + rx) >> 1);
        return min(get(st[x].l, lx, m, l, r), get(st[x].r, m, rx, l, r));
}
 */

/* Toposort
bool f = false;
void dfsfortopo (int i, int state[], vector<int> &topo, vector<int> adj[]) {
        state[i] = 1;
        for (auto u : adj[i]) {
                if (state[u] == 1) f = true;
                if (f == true) return;
                if (state[u] == 0) dfsfortopo(u, state, topo, adj);
                if (f == true) return;
        }
        topo.push_back(i);
        state[i] = 2;
}
 
bool toposort (vector<int> &topo, int n, vector<int> adj[]) {
        //requires f for cycle detection, requires adjacency list of directed edges
        int state[n] = {0};
        for (int i = 0; i < n; i++) {
                if (state[i] == 0) dfsfortopo(i, state, topo, adj);
                if (f == true) return f;
        }
        reverse(all(topo));
        return false;
} */

/* SCC
void d1 (int x, vector<int> adj[], bool vis[], vector<int> &v) {
        vis[x] = true;
        for (auto u : adj[x]) {
                if (vis[u]) continue;
                d1 (u, adj, vis, v);
        }
        v.push_back(x);
}
void d2 (int xp, int x, vector<int> radj[], bool vis[], int p[]) {
        vis[x] = true; p[x] = xp;
        for (auto u : radj[x]) {
                if (vis[u]) continue;
                d2(xp, u, radj, vis, p);
        }
}
void scc (int n, vector<int> adj[], vector<int> radj[], int p[]) {
        vector<int> v; bool vis[n] = {0};
        for (int i = 0; i < n; i++) {
                if (vis[i]) continue;
                d1 (i, adj, vis, v);
        }
        reverse(v.begin(), v.end());
        fill(vis, vis + n, 0);
        for (auto u : v) {
                if (vis[u]) continue;
                d2 (u, u, radj, vis, p);
        }
} */

/* LCA 
const int MAXN = 2e5 + 1;
int p[MAXN][19];
int lvl[MAXN];

void dfsforlca (int x, vector<int> adj[], int lv) {
        lvl[x] = lv;
        for (int i = 1; i < 19; i++) p[x][i] = p[p[x][i - 1]][i - 1];
        for (auto u : adj[x]) {
                if (u == p[x][0]) continue;
                p[u][0] = x;
                dfsforlca(u, adj, lv + 1);
        }
}

int getlca (int x, int y) {        
        if (lvl[y] < lvl[x]) swap(x, y);
        if (lvl[x] < lvl[y]) {
                int z = lvl[y] - lvl[x];
                for (int i = 0; i < 19; i++) {
                        if (z & 1) y = p[y][i];
                        z >>= 1;
                }
        }
        if (x == y) return x;
        for (int i = 18; i >= 0; i--) {
                if (p[x][i] == p[y][i]) continue;
                x = p[x][i]; y = p[y][i];
        }
        return p[x][0];
} /*

int binexp (int x, int n) { 
        int ans = 1; 
        while (n > 0) { 
                if (n & 1) {
                        ans = (ans * x) % P;
                } 
                x = (x * x) % P;
                n >>= 1; 
        } 
        return ans; 
}

struct segtree {
        int size;
        int N;
        vector<int> maxv;
        vector<pair<int, int>> lr;
        void init(int n) {
                N = n;
                size = 1;
                while (size < n) size *= 2;
                maxv.assign(2 * size - 1, -INF);
                lr.resize(2 * size - 1);
        }
        
 
        void build(vector<int> &a, int x, int lx, int rx) {
                lr[x] = {lx, rx};
                if (rx - lx == 1) {
                        if (lx < sz(a)) maxv[x] = a[lx];
                        else maxv[x] = -INF;
                        return;
                }
                int m = (lx + rx) / 2;
                build(a, 2 * x + 1, lx, m);
                build(a, 2 * x + 2, m, rx);
                maxv[x] = max(maxv[2 * x + 1], maxv[2 * x + 2]);
        }
 
        void set(int i, int v, int x, int lx, int rx) {
                if (rx - lx == 1) {
                        maxv[x] = v;
                        return;
                }
                int m = (lx + rx)/ 2;
                if (i < m) {
                        set(i, v, 2 * x + 1, lx, m);
                } else {
                        set(i, v, 2 * x + 2, m, rx);
                }
                maxv[x] = max(maxv[2 * x + 1], maxv[2 * x + 2]);
        }
        void set(int i, int v) {
                set(i, v, 0, 0, size);
        }
 
        void build(vector<int> &a) {
                build(a, 0, 0, size);
        }
        vector<int> out(){
                vector<int> v;
                for (int i = size - 1; i < N + size - 1; i++) v.push_back(maxv[i]);
                return v;
        }
 
        int get(int l, int r, int x, int lx, int rx) {
                
                if (lx >= r || l >= rx) return -INF;
                if (rx - lx == 1) {
                        return maxv[x];
                }
                if (lx >= l && rx <= r) {
                        return maxv[x];
                }
                int m = (lx + rx) / 2;
                int s1 = get(l, r, 2 * x + 1, lx, m);
                int s2 = get(l, r, 2 * x + 2, m, rx);
                return max(s1, s2);
                
        }
 
        int get(int l, int r) {
                return get(l, r, 0, 0, size);
        }
 
 
};

struct Seg2D {
        int size;
        int H;
        int N;
        vector<segtree> maxv;
        vector<pair<int, int>> lr;
        void init(int n) {
                N = n;
                size = 1;
                H = 1;
                while (size < n) size *= 2, H++;
                maxv.resize(2 * size - 1);
                lr.resize(2 * size - 1);
        }
        
        segtree merge (int x1, int x2, int h) {
                segtree seg;
                vector<int> v1 = maxv[x1].out();
                vector<int> v2 = maxv[x2].out();
                vector<int> v;
                for (int i = 0; i < sz(v1); i += h) {
                        for (int j = i; j < i + h; j++) v.push_back(v1[j]);
                        for (int j = i; j < i + h; j++) v.push_back(v2[j]);
                }
                seg.init(sz(v));
                seg.build(v);
                return seg;
        }
 
        void build (vector<vector<int>> &a, int x, int lx, int rx, int h) {
                lr[x] = {lx, rx};
                if (rx - lx == 1) {
                        segtree seg;
                        seg.init(sz(a[0]));
                        if (lx < sz(a)) seg.build(a[lx]);
                        else {
                                vector<int> v;
                                for (int i = 0; i < sz(a[0]); i++) v.push_back(-INF);
                                seg.build(v);
                        }
                        maxv[x] = seg;
                        return;
                }
                int m = (lx + rx) / 2;
                build(a, 2 * x + 1, lx, m, h / 2);
                build(a, 2 * x + 2, m, rx, h / 2);
                maxv[x] = merge(2 * x + 1, 2 * x + 2, h / 2);
        }
 
        void set (int i, int j, int v, int x, int lx, int rx, int h) {
                if (rx - lx == 1) {
                        maxv[x].set(j, v);
                        return;
                }
                int m = (lx + rx) / 2;
                if (i < m) {
                        set(i, j, v, 2 * x + 1, lx, m, h / 2);
                } else {
                        set(i, j, v, 2 * x + 2, m, rx, h / 2);
                }
                maxv[x].set(j * h + i - lx, v);
        }
        void set (int i, int j, int v) {
                set(i, j, v, 0, 0, size, size);
        }
 
        void build (vector<vector<int>> &a) {
                build(a, 0, 0, size, size);
        }
        
        void out (int x) {
                vector<int> v = maxv[x].out();
                cout << sz(v) << "\n";
                for (auto u : v) cout << u << " ";
                cout << "\n";
                //cout << N << " " << H << " ";
                //cout << maxv[x].get(0, N * H) << "\n";
        }
 
        int get (int l1, int r1, int l2, int r2, int x, int lx, int rx, int h) {
                
                if (lx >= r1 || l1 >= rx) return -INF;
                if (rx - lx == 1) {
                        return maxv[x].get(l2, r2);
                }
                if (lx >= l1 && rx <= r1) {
                        //cout << l1 << " " << r1 << " " << l2 << " " << r2 << " " << maxv[x].get(l2 * h, r2 * h) << endl;
                        return maxv[x].get(l2 * h, r2 * h);
                }
                int m = (lx + rx) / 2;
                int s1 = get(l1, r1, l2, r2, 2 * x + 1, lx, m, h / 2);
                int s2 = get(l1, r1, l2, r2, 2 * x + 2, m, rx, h / 2);
                return max(s1, s2);
                
        }
 
        int get (int l1, int r1, int l2, int r2) {
                return get(l1, r1, l2, r2, 0, 0, size, size);
        }
 
 
};

struct Dinic {
  struct Edge {
    int to, rev;
    int c, oc;
    int flow() { return max(oc - c, 0LL); } // if you need flows
  };
  vector<int> lvl, ptr, q;
  vector<vector<Edge>> adj;
  Dinic(int n) : lvl(n), ptr(n), q(n), adj(n) {}
  void addEdge(int a, int b, int c, int rcap = 0) {
    adj[a].push_back({b, sz(adj[b]), c, c});
    adj[b].push_back({a, sz(adj[a]) - 1, rcap, rcap});
  }
  int dfs(int v, int t, int f) {
    if (v == t || !f) return f;
    for (int& i = ptr[v]; i < sz(adj[v]); i++) {
      Edge& e = adj[v][i];
      if (lvl[e.to] == lvl[v] + 1)
        if (int p = dfs(e.to, t, min(f, e.c))) {
          e.c -= p, adj[e.to][e.rev].c += p;
          return p;
        }
    }
    return 0;
  }
  int calc(int s, int t) {
    int flow = 0; q[0] = s;
    for (int L = 0; L < 31; L++) do { // 'int L=30' maybe faster for random data
      lvl = ptr = vector<int>(sz(q));
      int qi = 0, qe = lvl[s] = 1;
      while (qi < qe && !lvl[t]) {
        int v = q[qi++];
        for (Edge e : adj[v])
          if (!lvl[e.to] && e.c >> (30 - L))
            q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
      }
      while (int p = dfs(s, t, LLONG_MAX)) flow += p;
    } while (lvl[t]);
    return flow;
  }
  bool leftOfMinCut(int a) { return lvl[a] != 0; }
};

/*  Modint
int norm(int x) {
        if (x < 0) {
                x += mod;
        }
        if (x >= mod) {
                x -= mod;
        }
        return x;
}
template<class T>
T power(T a, int b) {
        T res = 1;
        for (; b; b /= 2, a *= a) {
                if (b % 2) {
                res *= a;
        }       
        }
        return res;
}
struct Z {
        int x;
        Z(int x = 0) : x(norm(x)) {}
        int val() const {
                return x;
        }
        Z operator-() const {
                return Z(norm(mod - x));
        }
        Z inv() const {
                assert(x != 0);
                return power(*this, mod - 2);
        }
        Z &operator*=(const Z &rhs) {
                x = x * rhs.x % mod;
                return *this;
        }
        Z &operator+=(const Z &rhs) {
                x = norm(x + rhs.x);
                return *this;
        }
        Z &operator-=(const Z &rhs) {
                x = norm(x - rhs.x);
                return *this;
        }
        Z &operator/=(const Z &rhs) {
                return *this *= rhs.inv();
        }
        friend Z operator*(const Z &lhs, const Z &rhs) {
                Z res = lhs;
                res *= rhs;
                return res;
        }
        friend Z operator+(const Z &lhs, const Z &rhs) {
                Z res = lhs;
                res += rhs;
                return res;
        }
        friend Z operator-(const Z &lhs, const Z &rhs) {
                Z res = lhs;
                res -= rhs;
                return res;
        }
        friend Z operator/(const Z &lhs, const Z &rhs) {
                Z res = lhs;
                res /= rhs;
                return res;
        }
        friend std::istream &operator>>(std::istream &is, Z &a) {
                int v;
                is >> v;
                a = Z(v);
                return is;
        }
        friend std::ostream &operator<<(std::ostream &os, const Z &a) {
                return os << a.val();
        }
}; */

/* LCA with sum of values in the path
void dfsforlca2 (int x, vector<int> adj[], int lvl[], array<int, 20> p[], int val[], array<int, 20> val1[]) {
        lvl[x] = lv;
        vis[x] = true;
        val1[x][0] = val[x];
        for (int i = 1; i < 20; i++) p[x][i] = p[p[x][i - 1]][i - 1];
        for (int i = 1; i < 20; i++) val1[x][i] = val1[p[x][i - 1]][i - 1] + val1[x][i - 1];
        for (auto u : adj[x]) {
                if (lvl[u] > 0) continue;
                p[u][0] = x, lvl[u] = lvl[x] + 1;
                dfsforlca2 (u, adj, lvl, p, val, val1);
        }
}

int getlca2 (int x, int y, int lvl[], array<int, 20> p[], int val[], array<int, 20> val1[]) {        
        if (lvl[y] < lvl[x]) swap(x, y);
        int ans = 0;
        if (lvl[x] < lvl[y]) {
                int z = lvl[y] - lvl[x];
                for (int i = 0; i < 20; i++) {
                        if (z & 1) {
                                ans += val1[y][i];
                                y = p[y][i];
                        }
                        z >>= 1;
                }
        }
        if (x == y) {
                ans += val[x];
                return ans;
        }
        for (int i = 19; i >= 0; i--) {
                if (p[x][i] == p[y][i]) continue;
                ans += val1[x][i] + val1[y][i];
                x = p[x][i]; y = p[y][i];
        }
        return ans + val[x] + val[y] + val[p[x][0]];
} */

/* DSU
int get (int x, vector<int> &p) {
        return p[x] = (x == p[x] ? x : get(p[x], p));
}

void merge (int x, int y, vector<int> &p, vector<int> &sz) {
        x = get(x, p), y = get(y, p);
        if (x == y) return;
        if (sz[x] < sz[y]) swap(x, y);
        sz[x] += sz[y];
        p[y] = x;
} */

/* Suffix Array
int norm (int x, int n) {
        if (x < 0) return x + n;
        if (x >= n) return x - n;
        return x;
}
 
vector<int> sort (string s) {
 
        int n = sz(s);
        int alphabet = 256;
        vector<int> p(n), c(n, 0), cnt(max(alphabet, n), 0);
        for (auto ch : s) cnt[(int)ch]++;
        for (int i = 1; i < alphabet; i++) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--) p[--cnt[(int)s[i]]] = i;
        int classes = 1;
        for (int i = 1; i < n; i++) {
                if (s[p[i]] != s[p[i - 1]]) classes++;
                c[p[i]] = classes - 1;
        }
 
        for (int h = 0; (1 << h) < n; h++) {
 
                vector<int> pn(n), cn(n, 0);
                for (int i = 0; i < n; i++) pn[i] = norm(p[i] - (1 << h), n);
                fill (cnt.begin(), cnt.begin() + classes, 0);
                for (int i = 0; i < n; i++) cnt[c[i]]++;
                for (int i = 1; i < classes; i++) cnt[i] += cnt[i - 1];
                for (int i = n - 1; i >= 0; i--) p[--cnt[c[pn[i]]]] = pn[i];
                classes = 1;
                for (int i = 1; i < n; i++) {
                        array<int, 2> a1 = {c[p[i]], c[norm(p[i] + (1 << h), n)]};
                        array<int, 2> a2 = {c[p[i - 1]], c[norm(p[i - 1] + (1 << h), n)]};
                        if (a1 != a2) classes++;
                        cn[p[i]] = classes - 1;
                }
                c.swap(cn);
 
        }
 
        return p;
 
}
 
vector<int> suffix_array (string s) {
        s += "$";
        vector<int> v = sort(s);
        v.erase(v.begin());
        return v;
}
 
vector<int> lps (vector<int> &p, string &s) {
        int n = sz(s);
        vector<int> lp(n - 1);
        int k = 0;
        int pos[n];
        for (int i = 0; i < n; i++) pos[p[i]] = i;
        for (int i = 0; i < n; i++) {
                if (pos[i] == n - 1) {
                        k = 0; continue;
                }
                int j = p[pos[i] + 1];
                while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
                lp[pos[i]] = k;
                if (k) k--;
        }
        return lp;
} */

/* MCMF
// CostScalingMCMF<MAXN, int, int, INF, INF> flow;
// flow.addEdge(u, v, capacity, cost);
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
}; */

struct Dinic {
        struct Edge {
                int to, rev;
                int c, oc;
                int flow() { return max(oc - c, 0LL); } // if you need flows
        };
        vector<int> lvl, ptr, q;
        vector<vector<Edge>> adj;
        Dinic(int n) : lvl(n), ptr(n), q(n), adj(n) {}
        void addEdge(int a, int b, int c, int rcap = 0) {
                adj[a].push_back({b, sz(adj[b]), c, c});
                adj[b].push_back({a, sz(adj[a]) - 1, rcap, rcap});
        }
        int dfs(int v, int t, int f) {
                if (v == t || !f) return f;
                for (int& i = ptr[v]; i < sz(adj[v]); i++) {
                Edge& e = adj[v][i];
                if (lvl[e.to] == lvl[v] + 1)
                        if (int p = dfs(e.to, t, min(f, e.c))) {
                        e.c -= p, adj[e.to][e.rev].c += p;
                        return p;
                        }
                }
                return 0;
        }
        int calc(int s, int t) {
                int flow = 0; q[0] = s;
                for (int L = 0; L < 31; L++) do { // 'int L=30' maybe faster for random data
                lvl = ptr = vector<int>(sz(q));
                int qi = 0, qe = lvl[s] = 1;
                while (qi < qe && !lvl[t]) {
                        int v = q[qi++];
                        for (Edge e : adj[v])
                        if (!lvl[e.to] && e.c >> (30 - L))
                        q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
                }
                while (int p = dfs(s, t, LLONG_MAX)) flow += p;
                } while (lvl[t]);
                return flow;
        }
        bool leftOfMinCut(int a) { return lvl[a] != 0; }
};

/* NTT
const ll mod = (119 << 23) + 1, root = 62; // = 998244353
// For p < 2^30 there is also e.g. 5 << 25, 7 << 26, 479 << 21
// and 483 << 21 (same root). The last two are > 10^9.
ll modpow (ll b, ll e) {
        ll ans = 1;
        for (; e; b = b * b % mod, e /= 2)
                if (e & 1) ans = ans * b % mod;
        return ans;
}
void ntt (vl &a) {
        int n = sz(a), L = 31 - __builtin_clz(n);
        static vl rt(2, 1);
        for (static int k = 2, s = 2; k < n; k *= 2, s++) {
                rt.resize(n);
                ll z[] = {1, modpow(root, mod >> s)};
                rep(i,k,2*k) rt[i] = rt[i / 2] * z[i & 1] % mod;
        }
        vi rev(n);
        rep(i,0,n) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
        rep(i,0,n) if (i < rev[i]) swap(a[i], a[rev[i]]);
        for (int k = 1; k < n; k *= 2)
                for (int i = 0; i < n; i += 2 * k) rep(j,0,k) {
                        ll z = rt[j + k] * a[i + j + k] % mod, &ai = a[i + j];
                        a[i + j + k] = ai - z + (z > ai ? mod : 0);
                        ai += (ai + z >= mod ? z - mod : z);
                }
}
vl conv (const vl &a, const vl &b) {
        if (a.empty() || b.empty()) return {};
        int s = sz(a) + sz(b) - 1, B = 32 - __builtin_clz(s), n = 1 << B;
        int inv = modpow(n, mod - 2);
        vl L(a), R(b), out(n);
        L.resize(n), R.resize(n);
        ntt(L), ntt(R);
        rep(i,0,n) out[-i & (n - 1)] = (ll)L[i] * R[i] % mod * inv % mod;
        ntt(out);
        return {out.begin(), out.begin() + s};
}
*/

/* FFT 
typedef complex<double> C;
typedef vector<double> vd;
void fft(vector<C>& a) {
        int n = sz(a), L = 31 - __builtin_clz(n);
        static vector<complex<long double>> R(2, 1);
        static vector<C> rt(2, 1);  // (^ 10% faster if double)
        for (static int k = 2; k < n; k *= 2) {
                R.resize(n); rt.resize(n);
                auto x = polar(1.0L, acos(-1.0L) / k);
                rep(i,k,2*k) rt[i] = R[i] = i&1 ? R[i/2] * x : R[i/2];
        }
        vi rev(n);
        rep(i,0,n) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
        rep(i,0,n) if (i < rev[i]) swap(a[i], a[rev[i]]);
        for (int k = 1; k < n; k *= 2)
                for (int i = 0; i < n; i += 2 * k) rep(j,0,k) {
                        // C z = rt[j+k] * a[i+j+k]; // (25% faster if hand-rolled)  /// include-line
                        auto x = (double *)&rt[j+k], y = (double *)&a[i+j+k];        /// exclude-line
                        C z(x[0]*y[0] - x[1]*y[1], x[0]*y[1] + x[1]*y[0]);           /// exclude-line
                        a[i + j + k] = a[i + j] - z;
                        a[i + j] += z;
                }
        
}
vd conv(const vd& a, const vd& b) {
        if (a.empty() || b.empty()) return {};
        vd res(sz(a) + sz(b) - 1);
        int L = 32 - __builtin_clz(sz(res)), n = 1 << L;
        vector<C> in(n), out(n);
        copy(all(a), begin(in));
        rep(i,0,sz(b)) in[i].imag(b[i]);
        fft(in);
        for (C& x : in) x *= x;
        rep(i,0,n) out[i] = in[-i & (n - 1)] - conj(in[i]);
        fft(out);
        rep(i,0,sz(res)) res[i] = imag(out[i]) / (4 * n);
        return res;
}
*/

/* mod fft
https://github.com/simonlindholm/fft-precision/blob/master/fft-precision.md)
#pragma once

#include "FastFourierTransform.h"

typedef vector<ll> vl;
template<int M> vl convMod(const vl &a, const vl &b) {
	if (a.empty() || b.empty()) return {};
	vl res(sz(a) + sz(b) - 1);
	int B=32-__builtin_clz(sz(res)), n=1<<B, cut=int(sqrt(M));
	vector<C> L(n), R(n), outs(n), outl(n);
	rep(i,0,sz(a)) L[i] = C((int)a[i] / cut, (int)a[i] % cut);
	rep(i,0,sz(b)) R[i] = C((int)b[i] / cut, (int)b[i] % cut);
	fft(L), fft(R);
	rep(i,0,n) {
		int j = -i & (n - 1);
		outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
		outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * n) / 1i;
	}
	fft(outl), fft(outs);
	rep(i,0,sz(res)) {
		ll av = ll(real(outl[i])+.5), cv = ll(imag(outs[i])+.5);
		ll bv = ll(imag(outl[i])+.5) + ll(real(outs[i])+.5);
		res[i] = ((av % M * cut + bv) % M * cut + cv) % M;
	}
	return res;
} */

/* Geometry

const ld eps = 1e-9;

struct point {
        ld x = 0;
        ld y = 0;
        point (ld x = 0, ld y = 0) : x(x), y(y) {}

        point &operator += (const point& other) {
                x += other.x; y += other.y; return *this;
        }
        point &operator -= (const point& other) {
                x -= other.x; y -= other.y; return *this;
        }
        point &operator /= (const ld& factor) {
                x /= factor; y /= factor; return *this;
        }
        point &operator *= (const ld& factor) {
                x *= factor; y *= factor; return *this;
        }
        friend bool operator == (const point &lhs, const point &rhs) {
                return abs(lhs.x - rhs.x) < eps && abs(lhs.y - rhs.y) < eps;
        }
        friend point operator / (const point &lhs, const ld &rhs) {
                point res = lhs;
                res /= rhs;
                return res;
        }
        friend point operator * (const point &lhs, const ld &rhs) {
                point res = lhs;
                res *= rhs;
                return res;
        }
        friend point operator + (const point &lhs, const point &rhs) {
                point res = lhs;
                res += rhs;
                return res;
        }
        friend point operator - (const point &lhs, const point &rhs) {
                point res = lhs;
                res -= rhs;
                return res;
        }
        ld mod () {
                return sqrtl(x * x + y * y);
        }
        friend ostream &operator<<(ostream &os, const point &a) {
                return os << a.x << " " << a.y;
        }
};

point anglebisector_vector (point x, point y) {  // vector towards bisector of 2 vectors
        x /= x.mod();
        y /= y.mod();
        point res = (x + y) / 2;
        res /= res.mod();
        return res;
}

ld crossp (point x, point y) {            // cross product of vectors 
        return x.x * y.y - x.y * y.x;     // negative if y lies to the right of x, otherwise positive
}

ld dist (point a, point b) {             
        b -= a;
        return b.mod();
}
ld dist (point a, point b, point x) {    // distance between point x and line segment joining a and b

        point dr1 = b - a;
        point dr2(dr1.y, -dr1.x);
        ld x1 = crossp(dr2, b - x), x2 = crossp(dr2, a - x);
        if (x1 * x2 > -eps) return min(dist(a, x), dist(b, x));

        return abs(((x.x - a.x) * (b.y - a.y) - (x.y - a.y) * (b.x - a.x)) / sqrtl((b.y - a.y) * (b.y - a.y) + (b.x - a.x) * (b.x - a.x)));

}

bool point_inside_polygon (point x, vector<point> p) {  //p is polygon, check if x lies inside p
        int c = 0;
        int n = sz(p);
        for (int i = 0; i < n; i++) {
                if (x == p[i]) return 0;
                int j = (i + 1) % n;
                if (abs(p[i].y - x.y) < eps && abs(p[j].y - x.y) < eps) {
                        if (min(p[i].x, p[j].x) < x.x + eps && max(p[i].x, p[j].x) > x.x - eps) return 0;
                        continue;
                }
                if (p[i].y > x.y - eps && p[j].y > x.y - eps) continue;
                if (p[i].y < x.y + eps && p[j].y < x.y + eps) continue;
                ld f1 = crossp(p[i] - x, p[j] - p[i]);
                if (abs(f1) < eps) return 0;
                ld f2 = p[j].y - p[i].y;
                if (f1 * f2 < 0) c += 1;
        }
        return c & 1;
} */

/* STACKSIZE 
void main_() {
    // implement your solution here
}

static void run_with_stack_size(void (*func)(void), size_t stsize) {
    char *stack, *send;
    stack = (char *)malloc(stsize);
    send = stack + stsize - 16;
    send = (char *)((uintptr_t)send / 16 * 16);
    asm volatile(
        "mov %%rsp, (%0)\n"
        "mov %0, %%rsp\n"
        :
        : "r"(send));
    func();
    asm volatile("mov (%0), %%rsp\n" : : "r"(send));
    free(stack);
}

run_with_stack_size(main_, 1024 * 1024 * 1024);   //inside main
*/

//7, 9, 21, 33, 87

signed main(){

        ios::sync_with_stdio(0);
        cin.tie(0);
        cout.tie(0); 
        
        //freopen("input.txt", "r", stdin);
        //freopen("output.txt", "w", stdout);

        /* string x;
        getline(cin, x); */

}
