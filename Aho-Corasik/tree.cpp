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
#define LOGMAX 25

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


struct aho_carasic_forest {
    vector <node> nodes;
    vector <string> data;
    bool builded;

    aho_carasic_forest(): builded(0) {
        nodes.pb(node());
        loop(i,26) {
            nodes[0].next[i] = 0;
        }
        data.clear();
    }

    void modf(string& s) {
        loop(i,sz(s)) {
            s[i] -= 'a';
        }
    }

    void add (string& s) {
        assert(!builded);
        modf(s);
        data.pb(s);
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
                nodes.pb(node());
                nodes[cur].next[s[level]] = sz(nodes) - 1;
                nodes[cur].go[s[level]] = sz(nodes) - 1;
                nodes[sz(nodes) - 1].ch = s[level];
                nodes[sz(nodes) - 1].prev = cur;
                cur = sz(nodes) - 1;
            } else {
                cur = nodes[cur].next[s[level]];
            }
            ++level;
        }
    }

    void build_suff(int cur) {
        assert(!builded);
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
        assert(!builded);
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
        builded = true;
    }

    ll go_in(string s) {
        assert(builded);
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

    void add_full_forest(vector <string>& v) {
        assert(!builded);
        loop(i,sz(v)) {
            add(v[i]);
        }
    }
};


string conv_back(string s) {
    string h = "";
    loop(i,sz(s)) {
        h += ('a' + int(s[i]));
    }
    return h;
}



void solve() {
    ll n,m = 1;

    aho_carasic_forest bor;

    cin >> n;
    vector <string> strs(n);
    // loop(i,n) {
    //     string s;
    //     cin >> s;
    //     bor.add(s);
    // }

    loop(i,n) {
        cin >> strs[i];
    }
    // cout << "OK" << endl;

    // cout << "OK" << endl;
    bor.add_full_forest(strs);

    bor.build_acho_korasik();

    vector <string> vs(m);

    loop(i,m) {
        cin >> vs[i];
    }

    // cout << "OK" << endl;
    loop(i,m) {
        cout << bor.go_in(vs[i]) << endl;
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
       // cout << all << endl;
}

struct dynamic_aho_carasic {
    vector <string> data;
    aho_carasic_forest trees[LOGMAX];
    bitset <LOGMAX> bits;
    dynamic_aho_carasic() {
        bits.reset();
        data.clear();
    }

    ll go_in(string& s) {
        ll ans = 0;
        loop(i,LOGMAX) {
            if(bits.test(i)) {
                ans += trees[i].go_in(s);
            }
        }
        return ans;
    }

    void add(string& s) {
        data.pb(s);
        vector <string> added_data;
        added_data.pb(s);
        loop(i,LOGMAX) {
            if(!bits.test(i)) {
                bits.set(i,1);
                trees[i] = aho_carasic_forest();
                trees[i].add_full_forest(added_data);
                trees[i].build_acho_korasik();
                return;
            } else {
                bits.set(i,0);
                loop(j,sz(trees[i].data)) {
                    added_data.pb(conv_back(trees[i].data[j]));
                }
                trees[i] = aho_carasic_forest();
            }
        }
    }
};


void solve2() {
    ll q;
    string s;
    ll t;
    dynamic_aho_carasic in;
    dynamic_aho_carasic out;
    cin >> q;
    loop(i,q) {
        cin >> t >> s;
        if(t == 1) {
            in.add(s);
        }

        if(t == 2) {
            out.add(s);
        }

        if(t == 3) {
            cout << in.go_in(s) - out.go_in(s) << endl;
        }
        fflush(stdout);
    }
}

int main () {
    ios::sync_with_stdio(false);
    cin.tie(0);
    freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    // ll t;
    // cin >> t;
    // loop(i,t)
    // solve();
    solve2();
    return 0;
}

