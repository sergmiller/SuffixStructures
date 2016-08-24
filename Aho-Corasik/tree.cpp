#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <queue>
#include <string>
#include <cmath>
#include <map>
#include <set>
#include <cassert>
#include <bitset>
#define MOD (long long)1e9 + 7
#define eps 1e-9
#define pb push_back
#define mp make_pair
#define ft first
#define sd second
#define sz(a) a.size()
#define loop(i, n) for(long long (i) = 0; (i) < (n) ; ++ (i))
#define loopn()
#define pii pair<ll,ll>
#define pll pair<long long,long long>
#define pld pair <long double ,long double>
#define vii vector<ll>
#define vll vector<long long> 

typedef long long ll;
typedef long double ld;

using namespace std;

#define pi (ld)3.141592653589793238462643383

/*@Sergey_Miller*/


struct node {
    int next[26];
    int go[26];
    int cnt_ends;
    int suff;
    int prev;
    char ch;
    int qnt_all_ends;
    node() : suff(-1), prev(-1), cnt_ends(0), qnt_all_ends(0), ch(0) {
        loop(i,26) {
            next[i] = -1;
            go[i] = -1;
        }
    }
};

node nodes[(ll)2e5 + 2];

ll all = 1;

void modf(string& s) {
    loop(i,sz(s)) {
        s[i] -= 'a';
    }
}

void add (string& s) {
    modf(s);
    // loop(i,sz(s)) {
    //     cout << (int)s[i];
    // }
    int level = 0;
    int cur = 0;
    while(1) {
        if(level == sz(s)) {
            ++nodes[cur].cnt_ends;
            return;
        }
        if(nodes[cur].next[s[level]] == cur || nodes[cur].next[s[level]] == -1) {
            nodes[cur].next[s[level]] = all;
            nodes[cur].go[s[level]] = all;
            nodes[all].ch = s[level];
            nodes[all].prev = cur;
            cur = all;
            ++all;
        } else {
            cur = nodes[cur].next[s[level]];
        }
        ++level;
    }
}

void build_suff(int cur) {
    if(nodes[cur].prev == 0) {
        nodes[cur].suff = 0;
        loop(i,26) {
            if(nodes[cur].next[i] == -1) {
                nodes[cur].go[i] = nodes[0].go[i];
            } 
            else {
                nodes[cur].go[i] = nodes[cur].next[i];
            }
        }
    } else {
        int suff_prev = nodes[nodes[cur].prev].suff;
        // cout << cur->c << endl;
        int ch = nodes[cur].ch;
        while(nodes[suff_prev].next[ch] == -1) {
            suff_prev = nodes[suff_prev].suff;
        }
        nodes[cur].suff = nodes[suff_prev].next[ch];
        loop(i,26) {
            if(nodes[cur].next[i] == -1) {
                nodes[cur].go[i] = nodes[nodes[cur].suff].go[i];
            } 
            else {
                nodes[cur].go[i] = nodes[cur].next[i];
            }
        }
    }
    nodes[cur].qnt_all_ends = nodes[nodes[cur].suff].qnt_all_ends + nodes[cur].cnt_ends;
}

void build_acho_korasik() {
    nodes[0].suff = 0;
    loop(i,26) {
        if(nodes[0].next[i] != -1) {
            nodes[0].go[i] = nodes[0].next[i];
        } else {
            nodes[0].go[i] = 0;
        }
    }

    queue <int> numb;
    numb.push(0);
    while(!numb.empty()) {
        int v = numb.front();
        // cout << v << endl;
        numb.pop();
        loop(i,26) {
            if(nodes[v].next[i] != -1 && nodes[v].next[i] != v) {
                build_suff(nodes[v].next[i]);
                numb.push(nodes[v].next[i]);
            }
        }
    }
}

ll go_in(string& s) {
    // cout << s << endl;
    ll ans = 0;
    int cur = 0;
    loop(i,sz(s)) {
        int x = s[i] - 'a';
        // cout << cur << " " << x << endl;
        cur = nodes[cur].go[x];
        ans += nodes[cur].qnt_all_ends;
    }
    return ans;
}

void solve() {
    loop(i,all) {
        nodes[i] = node();
    }

    all = 1;

    loop(i,26) {
        nodes[0].next[i] = 0;
    }

    ll n,m;

    cin >> m >> n;
    vector <string> vs(m);
    loop(i,m) {
        cin >> vs[i];
    }
    // cout << "OK" << endl;
    loop(i,n) {
        string s;
        cin >> s;
        add(s);
    }

    // cout << "OK" << endl;

    build_acho_korasik();

    // cout << "OK" << endl;
    loop(i,m) {
        cout << go_in(vs[i]) << endl;
        // cout << i << endl;
    }

    // loop(i,all) {
    //     cout << "*******************" << endl;
    //     cout << i << endl;
    //     cout << "go: " << endl;
    //     loop(j,26) {
    //         // if(nodes[i].next[j]) {
    //             cout << char('a' + j) << " -> " << nodes[i].go[j] << endl; 
    //         // }
    //     }
    //     cout << "link: " << nodes[i].suff << endl;
    //     cout << "all_ends: " << nodes[i].qnt_all_ends << endl;
    //     // cout << "cnt_ends: " << nodes[i].cnt_ends << endl;
    //    }
    //    cout << all << endl;
}

int main () {
    ios::sync_with_stdio(false);
    cin.tie(0);
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    ll t;
    cin >> t;
    loop(i,t)
    solve();
    return 0;
}
