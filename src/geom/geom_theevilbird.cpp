const long double PI = 3.1415926535897932384626433832795;
const long double EPS = 1e-7;

/*
 * Rotate relative to the origin, a - rotation angle:
 * new_x = x cos(a) - y sin(a)
 * new_y = x sin(a) + y cos(a)
 *
 * new_x = x cos(a) + y sin(a)
 * new_y = x sin(a) - y cos(a)
 *
 *
 * rad = degree * PI / 180
 * degree = rad * 180 / PI
*/

struct Point {
    ll x = 0, y = 0;
    // int id = -1;

    Point() : x(0), y(0) {}
    Point(ll _x, ll _y) : x(_x), y(_y) {}

    bool operator ==(const Point other) const {
        return x == other.x && y == other.y;
    }

    void operator *=(ll k) {
        x *= k;
        y *= k;
    }
};

istream &operator>>(istream &is, Point &point) {
    is >> point.x >> point.y;
    return is;
}

ostream &operator<<(ostream &os, const Point &point) {
    os << point.x << ' ' << point.y;
    return os;
}

struct Vec {
    ll x, y;

    Vec() : x(0), y(0) {}
    Vec(ll _x, ll _y) : x(_x), y(_y) {}
    Vec(Point a) : x(a.x), y(a.y) {}
    Vec(Point a, Point b) : x(b.x - a.x), y(b.y - a.y) {}

    ld len() const {
        return sqrtl(x * x + y * y);
    }

    ll len_sq() const {
        return (x * x + y * y);
    }

    Vec operator *(ll k) const {
        return {x * k, y * k};
    }

    void operator *=(ll k) {
        x *= k;
        y *= k;
    }

    Vec operator +(const Vec other) const {
        return {x + other.x, y + other.y};
    }

    Vec operator -(const Vec other) const {
        return {x - other.x, y - other.y};
    }

    bool operator ==(const Vec other) const {
        return x == other.x && y == other.y;
    }

    bool operator !=(const Vec other) const {
        return !(*this == other);
    }

    ll operator *(const Vec other) const { // dot product
        return x * other.x + y * other.y;
    }

    ll operator %(const Vec other) const { // cross product
        return x * other.y - y * other.x;
    }
};

istream &operator>>(istream &is, Vec &vec) {
    is >> vec.x >> vec.y;
    return is;
}

ostream &operator<<(ostream &os, const Vec &vec) {
    os << vec.x << ' ' << vec.y;
    return os;
}

typedef vector<Point> Polygon;
typedef vector<Vec> vPolygon;

bool cmp_vectors(Vec a, Vec b) {
    if ((a.y < 0 || (a.y == 0 && a.x < 0)) &&
        (b.y > 0 || (b.y == 0 && b.x > 0))) {
        return true;
    }
    if ((b.y < 0 || (b.y == 0 && b.x < 0)) &&
        (a.y > 0 || (a.y == 0 && a.x > 0))) {
        return false;
    }
    return (a % b > 0 || (a % b == 0 && a.len_sq() < b.len_sq()));
}

int get_sign(ld kek) {
    if (kek > 0) return 1;
    if (kek < 0) return -1;
    return 0;
}

Polygon build_convex_hull(Polygon &a) {
    int n = sz(a);
    for (int i = 1; i < n; ++i) {
        if ((a[i].y < a[0].y) || (a[i].y == a[0].y && a[i].x < a[0].x)) swap(a[0], a[i]);
    }
    sort(a.begin() + 1, a.end(), [&](Point A, Point B) {
        Vec oa(a[0], A), ob(a[0], B);
        if ((oa % ob) == 0) return oa.len_sq() < ob.len_sq();
        return (oa % ob) > 0;
    });
    Polygon hull = {a[0]};
    for (int i = 1; i < n; ++i) {
        while (sz(hull) >= 2) {
            Vec ab(hull[sz(hull) - 2], hull[sz(hull) - 1]), bp(hull[sz(hull) - 1], a[i]);
            if ((ab % bp) <= 0) {
                hull.pop_back();
            }
            else {
                break;
            }
        }
        hull.emplace_back(a[i]);
    }
    return hull;
}

ll area_of_polygon(Polygon &poly) {
    ll res = 0;
    int n = sz(poly);
    for (int i = 2; i < n; ++i) {
        Vec ab(poly[0], poly[i - 1]), ac(poly[0], poly[i]);
        res += (ab % ac);
    }
    // don't forget to divide the result by 2!
    return res;
}

ld perimeter_of_polygon(Polygon &poly) {
    ld res = 0;
    int n = sz(poly);
    for (int i = 0; i < n; ++i) {
        Vec v(poly[i], poly[(i + 1 == n ? 0 : i + 1)]);
        res += v.len();
    }
    return res;
}

ll diameter_of_polygon(Polygon &poly) {
    int n = sz(poly), x = 1;
    ll ans = 0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        while (true) {
            Vec ac(poly[i], poly[x]), ab(poly[i], poly[j]), cd(poly[x], poly[(x + 1) % n]);
            ans = max(ans, ac.len_sq());
            if ((ab % cd) <= 0) {
                break;
            }
            x++;
            x %= n;
        }
    }
    // don't forget to extract root!
    return ans;
}

ld angle(const Vec &a, const Vec &b) {
    return fabsl(atan2(a % b, a * b) / PI * 180);
}

ld angle_rad(const Vec &a, const Vec &b) {
    return (atan2(a % b, a * b));
}

ld angle_rad(const Point &a) {
    return atan2(a.y, a.x);
}

ld from_point_to_line(const Point &p, const Point &a, const Point &b) {
    Vec ba(b, a), ap(a, p);
    return fabs((ba % ap) / ba.len());
}

ld from_point_to_ray(const Point &p, const Point &a, const Point &b) {
    Vec ba(b, a), ap(a, p), ab(a, b);
    if ((ab * ap) < 0) {
        return ap.len();
    }
    else {
        return fabs((ba % ap) / ba.len());
    }
}

ld from_point_to_segment(const Point &p, const Point &a, const Point &b) {
    Vec ab(a, b), ap(a, p), bp(b, p), ba(b, a);
    if ((ab * ap) < 0) {
        return fabsl(ap.len());
    }
    else if ((ab * bp) > 0) {
        return fabsl(bp.len());
    }
    else {
        return fabsl((ba % ap) / ba.len());
    }
}

bool point_on_line(const Point &p, const Point &a, const Point &b) {
    Vec ab(a, b), ap(b, p);
    return (ab % ap) == 0;
}

bool point_on_ray(const Point &p, const Point &a, const Point &b) {
    Vec ab(a, b), ap(a, p);
    return ((ab % ap) == 0 && (ab * ap) > 0 || (a == p));
}

bool point_on_segment(const Point &p, const Point &a, const Point &b) {
    Vec ap(a, p), bp(b, p), ab(a, b);
    return ((ap * bp) <= 0 && (ap % ab) == 0);
}

bool point_in_angle(const Point &p, const Point &a, const Point &o, const Point &b) {
    Vec oa(o, a), ob(o, b), op(o, p);
    if ((oa % ob) < 0) swap(oa, ob);
    return ((oa % op) >= 0 && (ob % op) <= 0);
}

bool segment_intersection(const Point &a, const Point &b, const Point &c, const Point &d) {
    Vec ab(a, b), cd(c, d), ac(a, c), ad(a, d), cb(c, b), ca(c, a);
    if (get_sign((ab % ac)) * get_sign((ab % ad)) <= 0 && get_sign((cd % ca)) * get_sign((cd % cb)) <= 0) {
        ll x1 = max(min(a.x, b.x), min(c.x, d.x)), x2 = min(max(a.x, b.x), max(c.x, d.x));
        ll y1 = max(min(a.y, b.y), min(c.y, d.y)), y2 = min(max(a.y, b.y), max(c.y, d.y));
        return (x1 <= x2 && y1 <= y2);
    }
    return false;
}

bool rays_intersection(const Point &a, const Point &b, const Point &c, const Point &d) {
    Vec ab(a, b), cd(c, d);
    ll k = 1e6;
    ab *= k;
    cd *= k;
    Point nb(ab.x + a.x, ab.y + a.y), nd(cd.x + c.x, cd.y + c.y);
    return segment_intersection(a, nb, c, nd);
}

bool lines_intersection(const Point &a, const Point &b, const Point &c, const Point &d) {
    Vec ab(a, b), cd(c, d);
    return ((ab % cd) != 0);
}

int point_in_polygon(const Point &p, const Polygon &poly) {
    // 0 - outside, 1 - inside, 2 - border;
    int n = sz(poly);
    if (point_in_angle(p, poly[n - 1], poly[0], poly[1])) {
        int l = 1, r = n - 1;
        while (r - l > 1) {
            int md = (l + r) / 2;
            if (point_in_angle(p, poly[md], poly[0], poly[l])) {
                r = md;
            }
            else {
                l = md;
            }
        }
        if (point_in_angle(p, poly[r], poly[0], poly[l]) &&
            point_in_angle(p, poly[0], poly[l], poly[r]) &&
            point_in_angle(p, poly[l], poly[r], poly[0])) {
            if (point_on_segment(p, poly[l], poly[r]) || 
                point_on_segment(p, poly[(l ? l - 1 : n - 1)], poly[l]) ||
                point_on_segment(p, poly[r], poly[(r + 1 == n ? 0 : r + 1)])) {
                return 2;
            }
            else {
                return 1;
            }
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}

int point_in_nonconvex_polygon(const Point &p, const Polygon &poly) {
    // 0 - outside, 1 - inside, 2 - border;
    int n = sz(poly);
    for (int i = 0; i < n; ++i) {
        if (point_on_segment(p, poly[i], poly[(i + 1) % n])) {
            return 2;
        }
    }
    ld s = 0.0;
    for (int i = 0; i < n; ++i) {
        Vec pa(p, poly[i]), pb(p, poly[(i + 1 == n ? 0 : i + 1)]);
        s += angle_rad(pa, pb);
    }
    if (s >= PI || s <= -PI) {
        return 1;
    }
    else {
        return 0;
    }
}

Polygon minkowski_sum(Polygon &a, Polygon &b) {
    // a[0], b[0]: y - max, y1 = y2 => x - max. Against clockwise
    int n = sz(a), m = sz(b);
    assert(n >= 3 && m >= 3);
    Point high_a = a[0], high_b = b[0];
    vPolygon va(n), vb(m);
    for (int i = 0; i < n; ++i) {
        va[i] = Vec(a[i], a[(i + 1) % n]);
    }
    for (int i = 0; i < m; ++i) {
        vb[i] = Vec(b[i], b[(i + 1) % m]);
    }
//    sort(all(va), cmp_vectors);
//    sort(all(vb), cmp_vectors);
    vPolygon vc;
    merge(all(va), all(vb), back_inserter(vc), cmp_vectors);
    Point high_c(high_a.x + high_b.x, high_a.y + high_b.y);
    Polygon c(sz(vc) + 1);
    c[0] = high_c;
    for (int i = 0; i < sz(c) - 1; ++i) {
        c[i + 1] = Point(c[i].x + vc[i].x, c[i].y + vc[i].y);
    }
    return c;
}

ld from_polygon_to_polygon(Polygon a, Polygon b) {
    for (auto &i: b) {
        i *= -1;
    }
    int pos = 0;
    for (int i = 1; i < sz(b); ++i) {
        if ((b[i].y > b[pos].y) ||
            (b[i].y == b[pos].y && b[i].x > b[pos].x)) {
            pos = i;
        }
    }
    rotate(b.begin(), b.begin() + pos, b.end());
    Polygon c = minkowski_sum(a, b);
    int n = sz(c);
    Point p(0, 0);
    ld ans = 1e20;
    for (int i = 0; i < n - 1; ++i) {
        ans = min(ans, from_point_to_segment(p, c[i], c[i + 1]));
    }
    return ans;
}

ll diameter_of_polygon_minkowski(Polygon &a) {
    Polygon ra = a;
    for (auto &i: ra) {
        i *= -1;
    }
    int pos = 0;
    for (int i = 1; i < sz(a); ++i) {
        if ((a[i].y > a[pos].y) ||
            (a[i].y == a[pos].y && a[i].x > a[pos].x)) {
            pos = i;
        }
    }
    rotate(a.begin(), a.begin() + pos, a.end());
    pos = 0;
    for (int i = 1; i < sz(a); ++i) {
        if ((ra[i].y > ra[pos].y) ||
            (ra[i].y == ra[pos].y && ra[i].x > ra[pos].x)) {
            pos = i;
        }
    }
    rotate(ra.begin(), ra.begin() + pos, ra.end());
    Polygon c = minkowski_sum(a, ra);
    int n = sz(c);
    ll ans = 0;
    for (int i = 0; i < n; ++i) {
        ans = max(ans, Vec(c[i]).len_sq());
    }
    // don't forget to extract root!
    return ans;
}