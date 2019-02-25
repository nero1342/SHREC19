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
        float temp;
        cin.read(reinterpret_cast<char*>(&temp), sizeof(float));
        x = temp;
        cin.read(reinterpret_cast<char*>(&temp), sizeof(float));
        y = temp;
        cin.read(reinterpret_cast<char*>(&temp), sizeof(float));
        z = temp;
        //        cerr << x << ' ' << y << ' ' << z << endl;
        //cin >> x >> y >> z;
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
        unsigned char cn;
        cin.read(reinterpret_cast<char*>(&cn), sizeof(unsigned char));
        //  cin >> n;
        n = (int) cn;
        ver.resize(n);
        for (int i = 0; i < n; ++i) {
            cin.read(reinterpret_cast<char*>(&ver[i]), sizeof(int));
        }
        sort(ver.begin(), ver.end());
        norm = (pt[ver[0]] - pt[ver[1]]) * (pt[ver[0]] - pt[ver[2]]);
        
    }
    
    
};
vector<Face> face;

const double pi = acos(-1);

double Angle(Face a, Face b) {
    double cosa = abs(a.norm / b.norm) / (a.norm.val() * b.norm.val());
    return acos(cosa);
}

int nVer, nFace, nEdge;
vector< vector<int> > e;
map<pp, int> MAP;

set<double> vang;
bool Ok(int id) {
    for (int u = 0; u < e[id].size(); ++u) {
        for (int v = u + 1; v < e[id].size(); ++v) {
            double ang = Angle(face[e[id][u]], face[e[id][v]]);
            if (ang >= pi / 10 && ang <= pi * 9/10) {
                return 1;
            } else {
            }
        }
    }
    return 0;
}


void ReadHeader(ifstream &cin, ofstream &cout) {
    string st;
    //ply
    getline(cin, st);
    //format binary_little_endian 1.0
    getline(cin, st);
    //comment VCGLIB generated
    getline(cin, st);
    //element vertex 50541
    cin >> st >> st >> nVer;
    getline(cin, st);
    //property float x
    getline(cin, st);
    //property float y
    getline(cin, st);
    //property float z
    getline(cin, st);
    //element face 99897
    cin >> st >> st >> nFace;
    getline(cin, st);
    //property list uchar int vertex_indices
    getline(cin, st);
    //end_header
    getline(cin, st);
}

void WriteHeader(ifstream &cin, ofstream &cout, int nVer, int nFace) {
    cout << "ply\n";
    cout << "format ascii 1.0\n";
    cout << "comment VCGLIB generated\n";
    cout << "element vertex " << nVer << endl;
    cout << "property float x\n";
    cout << "property float y\n";
    cout << "property float z\n";
    cout << "element face " << nFace << endl;
    cout << "property list uchar int vertex_indices\n";
    cout << "end_header\n";
}

void Solve(string sin, string sout) {
    //Reset
    cerr << sin << ' ' << sout << endl;
    pt.clear();
    face.clear();
    e.clear();
    MAP.clear();
    //File
    ifstream cin(sin);
    ofstream cout(sout);
    ReadHeader(cin, cout);
    //Process
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
    WriteHeader(cin, cout, ans, 0);
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
        string sin = "data-Binary/" + to_string(i) + ".ply";
        string sout = "data-Binary/output" + to_string(i) + ".ply";
        Solve(sin, sout);
    }
}
