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

struct Vec {
    ll x, y;

    Vec() : x(0), y(0) {}
    Vec(ll _x, ll _y) : x(_x), y(_y) {}

    ld len() const {
        return sqrtl(x * x + y * y);
    }

    ll len_sq() const {
        return (x * x + y * y);
    }

    Vec operator*(ll k) const {
        return {x * k, y * k};
    }

    void operator*=(ll k) {
        x *= k;
        y *= k;
    }

    Vec operator+(const Vec other) const {
        return {x + other.x, y + other.y};
    }

    Vec operator-(const Vec other) const {
        return {x - other.x, y - other.y};
    }

    bool operator==(const Vec other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vec other) const {
        return !(*this == other);
    }

    ll operator*(const Vec other) const {// dot product
        return x * other.x + y * other.y;
    }

    ll operator%(const Vec other) const {// cross product
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

typedef vector<Vec> Polygon;

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

int get_sign(ld x) {
    if (x < -EPS) return -1;
    if (EPS < x) return 1;
    return 0;
}

Polygon build_convex_hull(Polygon &a) {
    int n = sz(a);
    for (int i = 1; i < n; ++i) {
        if ((a[i].y < a[0].y) || (a[i].y == a[0].y && a[i].x < a[0].x)) swap(a[0], a[i]);
    }
    sort(a.begin() + 1, a.end(), [&](Vec A, Vec B) {
        Vec oa = A - a[0], ob = B - a[0];
        if ((oa % ob) == 0) return oa.len_sq() < ob.len_sq();
        return (oa % ob) > 0;
    });
    Polygon hull = {a[0]};
    for (int i = 1; i < n; ++i) {
        while (sz(hull) >= 2) {
            Vec ab = hull[sz(hull) - 1] - hull[sz(hull) - 2], bp = a[i] - hull[sz(hull) - 1];
            if ((ab % bp) <= 0) {
                hull.pop_back();
            } else {
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
        Vec ab = poly[i - 1] - poly[0], ac = poly[i] - poly[0];
        res += (ab % ac);
    }
    // don't forget to divide the result by 2!
    return res;
}

ld perimeter_of_polygon(Polygon &poly) {
    ld res = 0;
    int n = sz(poly);
    for (int i = 0; i < n; ++i) {
        Vec v = poly[(i + 1 == n ? 0 : i + 1)] - poly[i];
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
            Vec ac = poly[x] - poly[i], ab = poly[j] - poly[i], cd = poly[(x + 1) % n] - poly[x];
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

ld angle_rad(const Vec &a) {
    return atan2(a.y, a.x);
}

ld from_point_to_line(const Vec &p, const Vec &a, const Vec &b) {
    Vec ba = a - b, ap = p - a;
    return fabs((ba % ap) / ba.len());
}

ld from_point_to_ray(const Vec &p, const Vec &a, const Vec &b) {
    Vec ba = a - b, ap = p - a, ab = b - a;
    if ((ab * ap) < 0) {
        return ap.len();
    } else {
        return fabs((ba % ap) / ba.len());
    }
}

ld from_point_to_segment(const Vec &p, const Vec &a, const Vec &b) {
    Vec ab = b - a, ap = p - a, bp = p - b, ba = a - b;
    if ((ab * ap) < 0) {
        return fabsl(ap.len());
    } else if ((ab * bp) > 0) {
        return fabsl(bp.len());
    } else {
        return fabsl((ba % ap) / ba.len());
    }
}

bool point_on_line(const Vec &p, const Vec &a, const Vec &b) {
    Vec ab = b - a, ap = p - a;
    return (ab % ap) == 0;
}

bool point_on_ray(const Vec &p, const Vec &a, const Vec &b) {
    Vec ab = b - a, ap = p - a;
    return ((ab % ap) == 0 && (ab * ap) > 0) || (a == p);
}

bool point_on_segment(const Vec &p, const Vec &a, const Vec &b) {
    Vec ap = p - a, bp = p - b, ab = b - a;
    return (ap * bp) <= 0 && (ap % ab) == 0;
}

bool point_in_angle(const Vec &p, const Vec &a, const Vec &o, const Vec &b) {
    Vec oa = a - o, ob = b - o, op = p - o;
    if ((oa % ob) < 0) swap(oa, ob);
    return ((oa % op) >= 0 && (ob % op) <= 0);
}

bool segment_intersection(const Vec &a, const Vec &b, const Vec &c, const Vec &d) {
    Vec ab = b - a, cd = d - c, ac = c - a, ad = d - a, cb = b - c, ca = a - c;
    if (get_sign((ab % ac)) * get_sign((ab % ad)) <= 0 && get_sign((cd % ca)) * get_sign((cd % cb)) <= 0) {
        ll x1 = max(min(a.x, b.x), min(c.x, d.x)), x2 = min(max(a.x, b.x), max(c.x, d.x));
        ll y1 = max(min(a.y, b.y), min(c.y, d.y)), y2 = min(max(a.y, b.y), max(c.y, d.y));
        return (x1 <= x2 && y1 <= y2);
    }
    return false;
}

bool rays_intersection(const Vec &a, const Vec &b, const Vec &c, const Vec &d) {
    // TODO
}

bool lines_intersection(const Vec &a, const Vec &b, const Vec &c, const Vec &d) {
    Vec ab = b - a, cd = d - c;
    return ((ab % cd) != 0);
}

int point_in_polygon(const Vec &p, const Polygon &poly) {
    // 0 - outside, 1 - inside, 2 - border;
    int n = sz(poly);
    if (point_in_angle(p, poly[n - 1], poly[0], poly[1])) {
        int l = 1, r = n - 1;
        while (r - l > 1) {
            int md = (l + r) / 2;
            if (point_in_angle(p, poly[md], poly[0], poly[l])) {
                r = md;
            } else {
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
            } else {
                return 1;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

int point_in_nonconvex_polygon(const Vec &p, const Polygon &poly) {
    // 0 - outside, 1 - inside, 2 - border;
    int n = sz(poly);
    for (int i = 0; i < n; ++i) {
        if (point_on_segment(p, poly[i], poly[(i + 1) % n])) {
            return 2;
        }
    }
    ld s = 0.0;
    for (int i = 0; i < n; ++i) {
        Vec pa = poly[i] - p, pb = poly[(i + 1 == n ? 0 : i + 1)] - p;
        s += angle_rad(pa, pb);
    }
    if (s >= PI || s <= -PI) {
        return 1;
    } else {
        return 0;
    }
}

Polygon minkowski_sum(Polygon &a, Polygon &b) {
//    a[0], b[0]: y - max, y1 = y2 => x - max. Against clockwise
    int n = sz(a), m = sz(b);
    assert(n >= 3 && m >= 3);
    Vec high_a = a[0], high_b = b[0];
    Polygon va(n), vb(m);
    for (int i = 0; i < n; ++i) {
        va[i] = a[(i + 1) % n] - a[i];
    }
    for (int i = 0; i < m; ++i) {
        vb[i] = b[(i + 1) % m] - b[i];
    }
//    sort(all(va), cmp_vectors);
//    sort(all(vb), cmp_vectors);
    Polygon vc(sz(va) + sz(vb));
    merge(all(va), all(vb), vc.begin(), cmp_vectors);
    Vec high_c(high_a.x + high_b.x, high_a.y + high_b.y);
    Polygon c(sz(vc) + 1);
    c[0] = high_c;
    for (int i = 0; i < sz(c) - 1; ++i) {
        c[i + 1] = c[i] + vc[i];
    }
    return c;
}

ld from_polygon_to_polygon(Polygon a, Polygon b) {
    for (auto &i : b) {
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
    Vec p(0, 0);
    ld ans = 1e20;
    for (int i = 0; i < n - 1; ++i) {
        ans = min(ans, from_point_to_segment(p, c[i], c[i + 1]));
    }
    return ans;
}

ll diameter_of_polygon_minkowski(Polygon &a) {
    Polygon ra = a;
    for (auto &i : ra) {
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
        ans = max(ans, c[i].len_sq());
    }
    // don't forget to extract root!
    return ans;
}