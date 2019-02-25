//
//  main.cpp
//  SHREC19
//
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include <cmath>
#include <set>

using namespace std;

#define pp pair<int, int>

struct Point {
    double x, y, z;
    
    Point() {}
    Point(float x, float y, float z):x(x), y(y), z(z) {}
    
    void read(ifstream &cin) {
        cin >> x >> y >> z;
    }
    
    void write(ofstream &cout) {
        cout << x << ' ' << y << ' ' << z << endl;
    }
    
    double val() {
        return sqrt(x * x + y * y + z * z);
    }
    
    Point operator* (Point a) {
        return Point(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
    }
    
    double operator/ (Point a) {
        // Dot product
        return x * a.x + y * a.y + z * a.z;
    }
    
    Point operator- (Point a) {
        return Point(a.x - x, a.y - y, a.z - z);
    }
};
vector<Point> pt;

struct Face {
    vector<int> ver;
    Point norm;
    
    Face() {}
    Face(vector<int> ver):ver(ver) {}
    
    void read(ifstream &cin) {
        int n;
        cin >> n;
        ver.resize(n);
        for (int i = 0; i < n; ++i) cin >> ver[i];
        sort(ver.begin(), ver.end());
        norm = (pt[ver[0]] - pt[ver[1]]) * (pt[ver[0]] - pt[ver[2]]);
        
    }

    
};
vector<Face> face;

const double pi = acos(-1);

double Angle(Face a, Face b) {
    double cosa = abs(a.norm / b.norm) / (a.norm.val() * b.norm.val());
  //  cerr << "Angle: " << acos(cosa) << endl;
    return acos(cosa);
}

int nVer, nFace, nEdge;
vector< vector<int> > e;
map<pp, int> MAP;

set<double> vang;
bool Ok(int id) {
    for (int u = 0; u < e[id].size(); ++u) {
        
        for (int v = u + 1; v < e[id].size(); ++v) {
       //     cerr << "Pair " << u << ' ' << v << endl;
            double ang = Angle(face[e[id][u]], face[e[id][v]]);
   //         vang.insert(ang);
            if (ang >= pi / 10 && ang <= pi * 9/10) {
                return 1;
            } else {
         //       cerr << ang << endl;
            }
        }
    }
   // cerr << id << endl;
    return 0;
}

void Solve(string sin, string sout) {
    ifstream cin(sin);
    ofstream cout(sout);
    pt.clear();
    face.clear();
    e.clear();
    MAP.clear();
    
    string first;
    cin >> first;
    cout << first << endl;
    //
    cin >> nVer >> nFace >> nEdge;
    pt.resize(nVer);
    for (int i = 0; i < nVer; ++i) {
        pt[i].read(cin);
    }
    face.resize(nFace);
    int cnt = 0;
    for (int i = 0; i < nFace; ++i) {
        face[i].read(cin);
        for (int v1 = 0; v1 < 2; ++v1) {
            for (int v2 = v1 + 1; v2 < 3; ++ v2) {
                if(!MAP.count(pp(face[i].ver[v1], face[i].ver[v2]))) {
                    MAP[pp(face[i].ver[v1], face[i].ver[v2])] = cnt++;
                }
            }
        }
    }
    e.resize(cnt);
    for (int i = 0; i < nFace; ++i) {
        for (int v1 = 0; v1 < 2; ++v1) {
            for (int v2 = v1 + 1; v2 < 3; ++ v2) {
                int id = MAP[pp(face[i].ver[v1], face[i].ver[v2])];
                e[id].push_back(i);
            }
        }
    }
    vector<int> mark(cnt + 1), res(nVer + 1, 0);
    int ans = 0;
    for (int id = 0; id < cnt; ++id){
        /*   for (int f : e[id]) mark[f] = 1;
         for (int u : e[id]) {
         for (int v : face[u].ver) {
         //      cerr << v << ' ';
         res[v] = 1;
         }
         }*/
        if (Ok(id))
            mark[id] = 1;
    }
    for (auto x : MAP) {
        if (mark[x.second]) {
            if (!res[x.first.first]) {
                res[x.first.first] = 1;
                ++ans;
            }
            if (!res[x.first.second]) {
                res[x.first.second] = 1;
                ++ans;
            }
        }
    }
    //
    cout << ans << ' ' << 0 << ' ' << 0 << endl;
    cout << fixed << setprecision(5);
    for (int i = 0; i < nVer; ++i) if (res[i]) {
        pt[i].write(cout);
    }/*
      for (int i = 0; i < nFace; ++i) if (mark[i]) {
      cout << face[i].ver.size();
      for (int u : face[i].ver) cout << ' ' << u;
      cout << endl;
      }*/
    // for (double x : vang) cout << x << endl;
    cin.close();
    cout.close();
}
int main() {
    for (int i = 1; i <= 15; ++i) {
        string sin = "data-Text/" + to_string(i) + "t.off";
        string sout = "data-Text/output" + to_string(i) + ".off";
        Solve(sin, sout);
    }
}
