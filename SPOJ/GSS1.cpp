#include <bits/stdc++.h>

using namespace std;

typedef unsigned long long ull;
typedef long double   ld;
typedef long long     ll;
typedef pair<int,int> pii;
typedef vector<int>   vi;
typedef vector<pii>   vpii;
typedef pair<ll,ll>   pll;
typedef vector<ll>    vl;
typedef vector<pll>   vpll;

#define gc getchar
#define lsb(x) (x&-x)
#define blt(x) __builtin_##x
#define clz blt(clz)
#define ctz blt(ctz)
#define ones blt(popcount)
#define gcd(x,y) __gcd(x,y)
#define lcm(x,y) (x/__gcd(x,y)*y)

#define rep(i,a,b) for(int i = a; i < b; ++i)
#define repq(i,a,b) for(int i = a; i <= b; ++i)

#define rev(i,a,b) for(int i = a; i >= b; --i)
#define revn(i,a,b) for(int i = a; i > b; --i)

#define all(x) x.begin(),x.end()
#define rall(x) x.rbegin(),x.rend()

#define pb push_back
#define eb emplace_back
#define mp make_pair
#define fi first
#define se second

int readln(char *w) {
  scanf(" %[^\n]\n", w);
  return strlen(w);
}

double eps = numeric_limits<double>::epsilon();
double PI = acos(-1);
int INF = 1e9;
ll MAX = 1e18;
int const N = 5e4 + 16;
int const M = 1e9 + 7;
int const M2 = 999999983;

template<class T> T read() {
  int c, z=0;
  T x;
  while(not isdigit(c = gc())) z=c;
  x = c-'0';
  while(isdigit(c = gc()))
    x = (x<<1) + (x<<3) + (c-'0');
  if(z == '-') x = -x;
  return x;
}

int n, m;

struct segment {
  int sum;
  int best;
  int prefix;
  int suffix;

  segment() {
    sum = prefix = suffix = best = 0;    
  }

  segment(int v) {
    sum = prefix = suffix = best = v;
  }

  void append(segment const& rg) {
    segment lf = *this;
    join(lf,rg);
  }

  void prepend(segment const& lf) {
    segment rg = *this;
    join(lf,rg);
  }

  void join(segment const& lf, segment const& rg) {
    sum = lf.sum + rg.sum;
    prefix = max(lf.prefix, lf.sum + rg.prefix);
    suffix = max(lf.suffix + rg.sum, rg.suffix);
    best = sum;
    best = max(best, lf.suffix + rg.prefix);
    best = max(best, max(prefix, suffix));
    best = max(best, max(lf.best, rg.best));
  }
} t[N<<1];

void build() {
  rep(i,0,n) {
    int x = read<int>();
    t[i+n] = segment(x);
  }

  revn(i,n-1,0)
    t[i].join(t[i<<1], t[i<<1|1]);
}

int query(int l, int r) {
  bool fl = false;
  bool fr = false;
  segment x, xl, xr;
  for(l+=n, r+=n; l <= r; l >>=1, r>>= 1) {
    if(l&1) {
      if(fl)
        xl.append(t[l]);
      else
        xl = t[l];
      ++l;
      fl = true;
    }
    if(not (r&1)) {

      if(fr)
        xr.prepend(t[r]);
      else
        xr = t[r];
      --r;
      fr = true;
    }
  }

  if(fl && fr)
    x.join(xl, xr);
  else if(fl)
    x = xl;
  else
    x = xr;
  return x.best;
}

int main() {
  n = read<int>();
  build();
  m = read<int>();  
  while(m--) {
    int l = read<int>() - 1;
    int r = read<int>() - 1;
    printf("%d\n", query(l,r));
  }
}
