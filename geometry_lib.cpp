#include <iostream>
#include <fstream>
#include <stdio.h>

#include <vector>
#include <cmath>
#include <algorithm>

#define EPS 0.0000001
#define PI 3.14159265358979323846
#define PI2 6.283185307179586232

using namespace std;

#define range(i, j, k) for (int i = j; i < k; i++)
typedef long long int ll;
typedef int gmint;
typedef long double gmfloat;
typedef gmint gmvec;
typedef gmvec gmline;
gmint INF = 1000 * 1000 * 1000;


struct Vec;
struct Line;
Line line_pd(const Vec&, const Vec&);
Line line_2p(const Vec&, const Vec&);
pair<int, Vec> line_intersect(const Line&, const Line&);
gmvec dpr(const Vec&, const Vec&);
gmvec cpr(const Vec&, const Vec&);

template<typename tnum>
tnum abs(tnum n)
{
    if (n < 0) return -n;
    return n;
}

template<typename tnum1, typename tnum2>
bool eq(tnum1 a, tnum2 b) {
    return abs(a - b) < EPS;
}


struct Vec {
    gmvec x, y;

    Vec(): x(0), y(0) {}

    Vec(gmvec x0, gmvec y0): x(x0), y(y0) {}

    gmfloat len2() {
        return x * x + y * y;
    }

    gmfloat len() {
        return sqrt(len2());
    }

    Vec operator + (const Vec &v) const {
        return Vec(x + v.x, y + v.y);
    }

    Vec operator - (const Vec &v) const {
        return Vec(x - v.x, y - v.y);
    }

    Vec operator - () const {
        return Vec(-x, -y);
    }

    Vec operator * (gmint k) const {
        return Vec(gmvec(x * k), gmvec(y * k));
    }

    Vec operator * (gmfloat k) const {
        return Vec(gmvec(x * k), gmvec(y * k));
    }

    Vec dir() {
        gmfloat d = len();
        return Vec(gmvec(x / d), gmvec(y / d));
    }

    bool operator < (const Vec &v) const {
        return (y < v.y && eq(x, v.x) && !eq(y, v.y)) || x < v.x;
    }

    bool operator > (const Vec &v) const {
        return (y > v.y && eq(x, v.x) && !eq(y, v.y)) || x > v.x;
    }

    bool operator == (const Vec &v) const {
        return eq(x, v.x) && eq(y, v.y);
    }

    bool operator >= (const Vec &v) const {
        return (*this > v) || (*this == v);
    }

    bool operator <= (const Vec &v) const {
        return (*this < v) || (*this == v);
    }

    bool operator != (const Vec &v) const {
        return !(*this == v);
    }

    bool operator || (const Vec &v) const { // Parallel
        return eq(cpr(*this, v), 0);
    }

    void print() {
        cout << x << " " << y << endl;
    }
};

istream &operator >> (istream &in, Vec &a) {
    in >> a.x >> a.y;
    return in;
}

ostream &operator << (ostream &out, const Vec &a) {
    out << a.x << " " << a.y << "\n";
    return out;
}

struct Line {
    Vec A, B;
    gmline a, b, c;

    Line() {
        a = 0; b = 0; c = 0;
    }

    Line(gmline x, gmline y, gmline z) {
        a = x; b = y; c = z;
        // Two points on the line
        if (x != 0) {
            A = Vec(-z / x, 0);
        }
        if (y != 0) {
            A = Vec(0, -z / y);
        }
        B = A + dir();
    }

    Vec norm() const {
        return Vec(gmvec(a), gmvec(b));
    }

    Vec dir() const {
        return Vec(-gmvec(b), gmvec(a));
    }

    Vec v_norm(const Vec v) const {
        return line_intersect(*this, line_pd(v, norm())).second;
    }

    bool contains(const Vec &v) const {
        return eq(v.x * a + v.y * b, -c);
    }

    bool operator == (const Line l) const {
        return eq(a * l.b, b * l.a) && eq(b * l.c, c * l.b) && eq(a * l.c, c * l.a);
    }
};

istream &operator >> (istream &in, Line &l) {
    in >> l.a >> l.b >> l.c;
    l = Line(l.a, l.b, l.c);
    return in;
}

ostream &operator << (ostream &out, const Line &l) {
    out << l.a << " " << l.b << " " << l.c << "\n";
    return out;
}

Line line_pd(const Vec &p, const Vec &d) {
    if (d != Vec(0, 0)) {
        return Line(d.y, -d.x, d.x * p.y - d.y * p.x);
    }
    return Line();
}

Line line_2p(const Vec &v1, const Vec &v2) {
    if (!(v1 == v2)) {
        return Line(v2.y - v1.y, v1.x - v2.x, v1.y * (v2.x - v1.x) - v1.x * (v2.y - v1.y));
    }
    return Line();
}

// Line intersection: returns <true, Vec> if there's 1 point; otherwise returns <false, Vec>:
// Vec == Vec(0, 0) if none, Vec == Vec(1, 1) if infinity.
pair<bool, Vec> l_intersect(const Line &l1, const Line &l2)
{
    if (l1 == l2) {
        return make_pair(false, Vec(1, 1));
    } else if (l1.a * l2.b == l1.b * l2.a) {
        return make_pair(false, Vec(0, 0));
    } else {
        gmvec x = gmvec((-l2.b * l1.c + l1.b * l2.c) / (l1.a * l2.b - l2.a * l1.b));
        gmvec y = gmvec((-l1.a * l2.c + l2.a * l1.c) / (l1.a * l2.b - l2.a * l1.b));
        return make_pair(true, Vec(x, y));
    }
}

// Segment
struct Seg {
    Line l;
    Vec p1, p2;

    Seg(const Vec &v1, const Vec &v2) {
        l = line_2p(v1, v2);
        if (v1 <= v2) {
            p1 = Vec(v1.x, v1.y);
            p2 = Vec(v2.x, v2.y);
        } else {
            p2 = Vec(v1.x, v1.y);
            p1 = Vec(v2.x, v2.y);
        }
    }

    bool env(const Vec &v) const { // Envelopes
        return p1 <= v && v <= p2;
    }

    Vec divide(gmfloat a, gmfloat b) { // Returns the point that divides the given segment
        return p1 + ((p2 - p1) * (a / (a + b)));
    }
};

// Intersection of two segments
// Returns either an empty vector, one intersection point, or two points defining the intersection segment
vector<Vec> intersect_segments(Vec a, Vec b, Vec c, Vec d) {
    vector<Vec> res;
    if (a > b) {
        swap(a, b);
    }
    if (c > d) {
        swap(c, d);
    }
    Vec d_1 = b - a;
    Vec d_2 = d - c;
    if (cpr(d_1, d_2) != 0) {
        gmfloat t_1 = cpr((c - a), d_2) / cpr(d_1, d_2);
        gmfloat t_2 = cpr((a - c), d_1) / cpr(d_2, d_1);
        if ((0 <= t_1) && (t_1 <= 1) && (0 <= t_2) && (t_2 <= 1)) {
            res.push_back(a + (d_1 * t_1));
        }
    }
    else if (cpr((c - a), d_1) == 0 && cpr((c - a), d_2) == 0) {
        if (a > c) {
            swap(a, c);
            swap(d_1, d_2);
        }
        if (c + d_2 < a + d_1) {
            res.push_back(c);
            if (c != c + d_2) {
                res.push_back(c + d_2);
            }
        }
        else if (a + d_1 >= c) {
            res.push_back(c);
            if (a + d_1 != c) {
                res.push_back(a + d_1);
            }
        }
    }
    return res;
}

bool v_cmp(Vec a, Vec b) {
    return cpr(a, b) > 0 || (eq(cpr(a, b), 0) && a.len2() < b.len2());
}

vector<Vec> convex_hull(vector<Vec> point_set) {
    int N = point_set.size();
    vector<Vec> polygon(N);
    for (int i = 0; i < N; i++) {
        polygon[i] = point_set[i];
    }

    // Choose a starting point (lowest of left-most points)
    Vec A = point_set[0];
    for (int i = 0; i < N; i++) {
        Vec X = point_set[i];
        if (X.x < A.x || (X.x == A.x && X.y < A.y)) {
            A = X;
        }
    }

    // Sort all diagonal vectors
    vector<Vec> diagonals;
    for (int i = 0; i < N; i++) {
        if (!(point_set[i] == A)) {
            diagonals.push_back(-A + polygon[i]);
        }
    }

    sort(diagonals.begin(), diagonals.end(), v_cmp);

    // Construct the hull using a stack
    vector<Vec> hull;
    hull.push_back(Vec(0, 0));
    for (int i = 0; i < diagonals.size(); i++) {
        if (hull.size() > 1) {
            int last = hull.size() - 1;
            double angle = cpr(-hull[last - 1] + hull[last], -hull[last] + diagonals[i]);
            if (eq(angle, 0)) {
                hull.pop_back();
            }
            while (!eq(angle, 0) && angle < 0) {
                hull.pop_back();
                last = hull.size() - 1;
                angle = cpr(-hull[last - 1] + hull[last], -hull[last] + diagonals[i]);
            }
        }
        hull.push_back(diagonals[i]);
    }
    for (int i = 0; i < hull.size(); i++) {
        hull[i] = hull[i] + A;
    }

    return hull;
}

bool in_polygon(const vector<Vec> &pol, const Vec &v)
{
    int average = 0; // Average between 0 and 1
    int steps = 10;
    for (int i = 0; i < steps; i++) {
        Vec distant(INF + abs(rand()) * INF, INF + abs(rand()) * INF);
        Seg line = Seg(v, distant);
        int cnt = 0;
        for (int j = 0; j < pol.size(); j++) {
            Vec a, b;
            a = pol[j];
            if (j == pol.size() - 1) {
                b = pol[0];
            } else {
                b = pol[j + 1];
            }

            Seg arc(a, b);
            vector<Vec> res = intersect_segments(line.p1, line.p2, arc.p1, arc.p2);
            if (res.size() > 0) {
                cnt++;
            }
        }
        average += cnt % 2;
    }
    return average / steps;
}

// Dot(scalar) and cross(vector) product
gmvec dpr(const Vec &v1, const Vec &v2) // (a, b) = x1 * x2 + y1 * y2
{
    return v1.x * v2.x + v1.y * v2.y;
}

gmvec cpr(const Vec &v1, const Vec &v2) // [a, b] = x1 * y2 - y1 * x2
{
    return v1.x * v2.y - v1.y * v2.x;
}

// Making angles universal - from 0 to 2pi
gmfloat uni(gmfloat angle) {
    while (angle < 0) {
        angle += PI2;
    }
    while (angle >= PI2) {
        angle -= PI2;
    }
    return angle;
}

// Calculate the angle between vectors
gmfloat at2(Vec a, Vec b) {
    gmfloat res = atan2l(cpr(a, b), dpr(a, b));
    res = -res;
    return uni(res);
}

int main()
{
    cout.precision(16);
    cout << fixed;

    Vec A, B, C, D;
    cin >> A.x >> A.y >> B.x >> B.y >> C.x >> C.y >> D.x >> D.y;
    intersect_segments(A, B, C, D);


    return 0;
}