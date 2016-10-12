#include <bits/stdc++.h>

using namespace std;

int const N = 100 * 1000 + 16;

int n, m;
char w[N];
int len[N];
long long dp[N];
string code[256];

constexpr int index_of(char c) {
  return c=='-';
}

struct node {
  node* f = 0;
  set<int> mark;
  node* kids[2] = {};

  ~node() {
    for(auto x : kids) delete x;
  }
};

struct aho_corasick {
  int id = 0;
  node root;  

  void insert(char txt[]) {
    auto p = &root;
    for(int i = 0; txt[i]; ++i) {
      int idx = index_of(txt[i]);
      if(not p->kids[idx])
        p->kids[idx] = new node;
      p = p->kids[idx];
    }    
    len[++id] = strlen(txt);
    p->mark.insert(id);
  }

  void build() {
    queue<node*> q;
    q.push(&root);

    while(not q.empty()) {
      auto u = q.front(); q.pop();

      for(int i = 0; i < 2; ++i) {
        auto v = u->kids[i];
        if(not v) continue;
        q.push(v);

        auto f = u->f;

        while(f && not f->kids[i])
          f = f->f;

        v->f = f ? f->kids[i] : &root;

        if(v->f != &root)
          for(auto x : v->f->mark)
            v->mark.insert(x);
      }
    }
  }

  long long match() {
    auto p = &root;
    for(int i = 0; i < n; ++i) {
      int idx = index_of(w[i]);

      while(not p->kids[idx] && p->f)
        p = p->f;

      p = p->kids[idx] ? p->kids[idx] : &root;

      if(p != &root)
        for(auto x : p->mark) {
          int index = i - len[x] + 1;
          if(index == 0)
            dp[i]++;
          else if(dp[index-1])
            dp[i] += dp[index-1];
        }
    }
    return dp[n-1];
  }
};

int main() {
  code['A'] = ".-"  , code['B'] = "-...", code['C'] = "-.-.", code['D'] = "-.." , code['E'] = "."   , code['F'] = "..-.";
  code['G'] = "--." , code['H'] = "....", code['I'] = ".."  , code['J'] = ".---", code['K'] = "-.-" , code['L'] = ".-..";
  code['M'] = "--"  , code['N'] = "-."  , code['O'] = "---" , code['P'] = ".--.", code['Q'] = "--.-", code['R'] = ".-." ;
  code['S'] = "..." , code['T'] = "-"   , code['U'] = "..-" , code['V'] = "...-", code['W'] = ".--" , code['X'] = "-..-";
  code['Y'] = "-.--", code['Z'] = "--..";

  scanf(" %s", w);
  n = strlen(w);

  aho_corasick baka;
  scanf("%d", &m);
  for(int i = 0; i < m; ++i) {
    char y[128] = {}, z[128] = {};
    scanf(" %s", y);

    for(char c : y) if(c)
      strcat(z, code[c].c_str());

    baka.insert(z);
  }
  baka.build();
  printf("%lld\n", baka.match());
}
