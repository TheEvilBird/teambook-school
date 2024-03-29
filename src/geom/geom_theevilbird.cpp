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

template<typename T>
struct point_t {
    T x, y;

    point_t() : x(0), y(0) {}

    point_t(T _x, T _y) : x(_x), y(_y) {}

    T len_sq() const {
        return x * x + y * y;
    }

    ld len() const {
        return sqrtl(len_sq());
    }

    point_t operator*(T k) const {
        return {x * k, y * k};
    }

    void operator*=(T k) {
        x *= k;
        y *= k;
    }

    point_t operator+(const point_t<T> &other) const {
        return {x + other.x, y + other.y};
    }

    point_t operator-(const point_t<T> &other) const {
        return {x - other.x, y - other.y};
    }

    bool operator==(const point_t<T> &other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const point_t<T> &other) const {
        return !(*this == other);
    }

    T operator*(const point_t<T> &other) const {// dot product
        return x * other.x + y * other.y;
    }

    T operator%(const point_t<T> &other) const {// cross product
        return x * other.y - y * other.x;
    }
};

using Point = point_t<ll>;
using PointLD = point_t<ld>;

template<typename T>
istream &operator>>(istream &is, point_t<T> &vec) {
    is >> vec.x >> vec.y;
    return is;
}

template<typename T>
ostream &operator<<(ostream &os, const point_t<T> &vec) {
    os << vec.x << ' ' << vec.y;
    return os;
}

typedef vector<Point> Polygon;

bool cmp_vectors(Point a, Point b) {
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
    sort(a.begin() + 1, a.end(), [&](Point A, Point B) {
        Point oa = A - a[0], ob = B - a[0];
        if ((oa % ob) == 0) return oa.len_sq() < ob.len_sq();
        return (oa % ob) > 0;
    });
    Polygon hull = {a[0]};
    for (int i = 1; i < n; ++i) {
        while (sz(hull) >= 2) {
            Point ab = hull[sz(hull) - 1] - hull[sz(hull) - 2], bp = a[i] - hull[sz(hull) - 1];
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
        Point ab = poly[i - 1] - poly[0], ac = poly[i] - poly[0];
        res += (ab % ac);
    }
    // don't forget to divide the result by 2!
    return res;
}

ld perimeter_of_polygon(Polygon &poly) {
    ld res = 0;
    int n = sz(poly);
    for (int i = 0; i < n; ++i) {
        Point v = poly[(i + 1 == n ? 0 : i + 1)] - poly[i];
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
            Point ac = poly[x] - poly[i], ab = poly[j] - poly[i], cd = poly[(x + 1) % n] - poly[x];
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

template<typename T>
ld angle(const point_t<T> &a, const point_t<T> &b) {
    return fabsl(atan2(a % b, a * b) / PI * 180);
}

template<typename T>
ld angle_rad(const point_t<T> &a, const point_t<T> &b) {
    return (atan2(a % b, a * b));
}

template<typename T>
ld angle_rad(const point_t<T> &a) {
    return atan2(a.y, a.x);
}

template<typename T>
point_t<ld> rotate(const point_t<T> &a, ld alpha) {
    return {a.x * cos(alpha) - a.y * sin(alpha), a.x * sin(alpha) + a.y * cos(alpha)};
}

ld from_point_to_line(const Point &p, const Point &a, const Point &b) { // point p, line ab
    Point ba = a - b, ap = p - a;
    return fabs((ba % ap) / ba.len());
}

ld from_point_to_ray(const Point &p, const Point &a, const Point &b) { // point p, ray ab
    Point ba = a - b, ap = p - a, ab = b - a;
    if ((ab * ap) < 0) {
        return ap.len();
    } else {
        return fabs((ba % ap) / ba.len());
    }
}

ld from_point_to_segment(const Point &p, const Point &a, const Point &b) { // point p, segment ab
    Point ab = b - a, ap = p - a, bp = p - b, ba = a - b;
    if ((ab * ap) < 0) {
        return fabsl(ap.len());
    } else if ((ab * bp) > 0) {
        return fabsl(bp.len());
    } else {
        return fabsl((ba % ap) / ba.len());
    }
}

bool point_on_line(const Point &p, const Point &a, const Point &b) { // point p, line ab
    Point ab = b - a, ap = p - a;
    return (ab % ap) == 0;
}

bool point_on_ray(const Point &p, const Point &a, const Point &b) { // point p, ray ab
    Point ab = b - a, ap = p - a;
    return ((ab % ap) == 0 && (ab * ap) > 0) || (a == p);
}

bool point_on_segment(const Point &p, const Point &a, const Point &b) { // point p, segment ab
    Point ap = p - a, bp = p - b, ab = b - a;
    return (ap * bp) <= 0 && (ap % ab) == 0;
}

bool point_in_angle(const Point &p, const Point &a, const Point &o, const Point &b) { // point p, angle aob
    Point oa = a - o, ob = b - o, op = p - o;
    if ((oa % ob) < 0) swap(oa, ob);
    return ((oa % op) >= 0 && (ob % op) <= 0);
}

bool segment_intersection(const Point &a, const Point &b, const Point &c, const Point &d) { // segment ab, segment, cd
    Point ab = b - a, cd = d - c, ac = c - a, ad = d - a, cb = b - c, ca = a - c;
    if (get_sign((ab % ac)) * get_sign((ab % ad)) <= 0 && get_sign((cd % ca)) * get_sign((cd % cb)) <= 0) {
        ll x1 = max(min(a.x, b.x), min(c.x, d.x)), x2 = min(max(a.x, b.x), max(c.x, d.x));
        ll y1 = max(min(a.y, b.y), min(c.y, d.y)), y2 = min(max(a.y, b.y), max(c.y, d.y));
        return (x1 <= x2 && y1 <= y2);
    }
    return false;
}

bool lines_intersection(const Point &a, const Point &b, const Point &c, const Point &d) { // line ab, line cd
    Point ab = b - a, cd = d - c, ac = c - a;
    return ((ab % cd) != 0 || (ab % ac == 0));
}

bool line_ray_intersection(const Point &a, const Point &b, const Point &c, const Point &d) { // line ab, ray cd
    if (!lines_intersection(a, b, c, d)) {
        return false;
    }
    Point ab = b - a, dp = d + (a - c), ac = c - a;
    if (get_sign(ab % (dp - a)) * get_sign(ab % ac) <= 0) {
        return true;
    }
    return false;
}

Point get_inf(const Point &a, const Point &b) { // returns inf point on ray ab
    if (a.x == b.x) {
        if (a.y < b.y) {
            return {a.x, INFLL};
        } else {
            return {a.x, -INFLL};
        }
    }
    if (a.x < b.x) {
        if (a.y < b.y) {
            return {INFLL, INFLL};
        } else if (a.y == b.y) {
            return {INFLL, a.y};
        } else {
            return {INFLL, -INFLL};
        }
    }
    // a.x > b.x
    if (a.y < b.y) {
        return {-INFLL, INFLL};
    } else if (a.y == b.y) {
        return {-INFLL, a.y};
    } else {
        return {-INFLL, -INFLL};
    }
}

bool rays_intersection(const Point &a, const Point &b, const Point &c, const Point &d) { // ray ab, ray cd
    if (line_ray_intersection(a, b, c, d) && line_ray_intersection(c, d, a, b)) {
        Point bp = get_inf(a, b), dp = get_inf(c, d);
        ll x1 = max(min(a.x, bp.x), min(c.x, dp.x)), x2 = min(max(a.x, bp.x), max(c.x, dp.x));
        ll y1 = max(min(a.y, bp.y), min(c.y, dp.y)), y2 = min(max(a.y, bp.y), max(c.y, dp.y));
        return (x1 <= x2 && y1 <= y2);
    }
    return false;
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
        Point pa = poly[i] - p, pb = poly[(i + 1 == n ? 0 : i + 1)] - p;
        s += angle_rad(pa, pb);
    }
    if (s >= PI || s <= -PI) {
        return 1;
    } else {
        return 0;
    }
}

Polygon minkowski_sum(Polygon &a, Polygon &b) {
//    a[0], b[0]: y - max, y1 = y2 => x - max. counter clockwise
    int n = sz(a), m = sz(b);
    assert(n >= 3 && m >= 3);
    Point high_a = a[0], high_b = b[0];
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
    Point high_c(high_a.x + high_b.x, high_a.y + high_b.y);
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
    Point p(0, 0);
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

ld from_segment_to_segment(const Point &a, const Point &b, const Point &c, const Point &d) { // segment ab, segment cd
    if (segment_intersection(a, b, c, d)) {
        return 0;
    }
    return min({from_point_to_segment(a, c, d), from_point_to_segment(b, c, d),
                from_point_to_segment(c, a, b), from_point_to_segment(d, a, b)});
}

bool segment_line_intersection(const Point &a, const Point &b, const Point &c, const Point &d) { // segment ab, line cd
    Point cd = d - c, ca = a - c, cb = b - c;
    if (get_sign(cd % ca) * get_sign(cd % cb) <= 0) {
        return 1;
    }
    return false;
}

bool ray_segment_intersection(const Point &a, const Point &b, const Point &c, const Point &d) { // ray ab, segment cd
    if (line_ray_intersection(c, d, a, b) && segment_line_intersection(c, d, a, b)) {
        Point bp = get_inf(a, b);
        ll x1 = max(min(a.x, bp.x), min(c.x, d.x)), x2 = min(max(a.x, bp.x), max(c.x, d.x));
        ll y1 = max(min(a.y, bp.y), min(c.y, d.y)), y2 = min(max(a.y, bp.y), max(c.y, d.y));
        return (x1 <= x2 && y1 <= y2);
    }
    return false;
}

ld from_segment_to_ray(const Point &a, const Point &b, const Point &c, const Point &d) { // segment ab, ray cd
    if (ray_segment_intersection(c, d, a, b)) {
        return 0;
    }
    return min({from_point_to_ray(a, c, d),
                from_point_to_ray(b, c, d),
                from_point_to_segment(c, a, b)});
}

ld from_segment_to_line(const Point &a, const Point &b, const Point &c, const Point &d) { // segment ab, line cd
    Point cd = d - c, ca = a - c, cb = b - c;
    if (get_sign(cd % ca) * get_sign(cd % cb) <= 0) {
        return 0;
    }
    return min(from_point_to_line(a, c, d), from_point_to_line(b, c, d));
}

ld from_ray_to_ray(const Point &a, const Point &b, const Point &c, const Point &d) { // ray ab, ray cd
    if (rays_intersection(a, b, c, d)) {
        return 0;
    }
    return min(from_point_to_ray(a, c, d), from_point_to_ray(c, a, b));
}

ld from_ray_to_line(const Point &a, const Point &b, const Point &c, const Point &d) { // ray ab, line cd
    if (line_ray_intersection(c, d, a, b)) {
        return 0;
    }
    return from_point_to_line(a, c, d);
}

ld from_line_to_line(const Point &a, const Point &b, const Point &c, const Point &d) { // line ab, line cd
    if (lines_intersection(a, b, c, d)) {
        return 0;
    }
    return from_point_to_line(a, c, d);
}

pii tangent_from_point(const Point &p, const Polygon &poly) {
//    returns id of tangent point: {left tangent, right tangent} (maybe {right, left}, but i believe no)
//    2**20 ~= 1e6, 2**17 ~= 1e5
    int n = sz(poly);
    int i_min = 0;
    int i_max = 0;
    for (int k = 17; k >= 0; --k) {
        {
            int l = (i_min + (1 << k)) % n;
            int r = ((i_min - (1 << k)) % n + n) % n;
            i_min = min({l, r, i_min}, [&poly, &p](int i, int j) {
                return ((poly[i] - p) % (poly[j] - p)) < 0 ||
                       (((poly[i] - p) % (poly[j] - p)) == 0 && (poly[i] - p).len_sq() < (poly[j] - p).len_sq());
            });
        }
        {
            int l = (i_max + (1 << k)) % n;
            int r = ((i_max - (1 << k)) % n + n) % n;
            i_max = max({l, r, i_max}, [&poly, &p](int i, int j) {
                return ((poly[i] - p) % (poly[j] - p)) < 0 ||
                       (((poly[i] - p) % (poly[j] - p)) == 0 && (poly[i] - p).len_sq() > (poly[j] - p).len_sq());
            });
        }
    }
    return {i_min, i_max};
}

template<typename T>
struct circle_t {
    point_t<T> c;
    T r;

    circle_t() {}

    circle_t(point_t<T> _c, T _r) : c(_c), r(_r) {}

    int point_in(const point_t<T> &a) {
        if (is_same<T, ll>()) {
            ll dist = (a - c).len_sq();
            if (dist > r * r) {
                return 0;
            } else if (dist < r * r) {
                return 1;
            } else {
                return 2;
            }
        } else {
            ld dist = (a - c).len();
            if (dist > r + EPS) {
                return 0;
            } else if (dist + EPS < r) {
                return 1;
            } else {
                return 2;
            }
        }
    }

    vector<PointLD> tangent_from_point(const point_t<T> &a) {
        PointLD p(a.x, a.y);
        PointLD vec(c.x - p.x, c.y - p.y);
        ld dist = vec.len();
        if (dist + EPS < r) {
            return {};
        }
        if (abs(r - dist) < EPS) {
            return {p};
        }
        vec.x /= dist;
        vec.y /= dist;
        ld k = sqrtl(dist * dist - r * r);
        ld alpha = atan2(r, k);
        PointLD t1 = p + rotate(vec, alpha) * k, t2 = p + rotate(vec, -alpha) * k;
        return {t1, t2};
    }
};

template<typename T>
istream &operator>>(istream &is, circle_t<T> &c) {
    is >> c.c >> c.r;
    return is;
}

/**
 * Description: half-plane intersection area
 * Time: O(N\log N)
 * Source: USACO
 * HALF PLANES: ax + by >= c, not ax + by + c >= 0
 */

using Half = array<ld, 3>; // half-plane, ax + by >= c
using vH = vector<Half>;

PointLD hp_point(const Half &h) { return {h[0], h[1]}; } // direction of half-plane
PointLD isect(const Half &h0, const Half &h1) { // Cramer's rule to intersect half-planes
    array<ld, 3> vals{};
    for (int i = -1; i <= 1; ++i) {
        int x = (i == 0 ? 2 : 0), y = (i == 1 ? 2 : 1);
        vals[1 + i] = h0[x] * h1[y] - h0[y] * h1[x];
    }
    assert(fabsl(vals[0]) > EPS);
    return {vals[1] / vals[0], vals[2] / vals[0]};
}

ld eval(const Half &h, ld x) { // evaluate half-plane at x-coordinate
    assert(fabsl(h[1]) > EPS);
    return (h[2] - h[0] * x) / h[1];
}

ld x_isect(const Half &h0, const Half &h1) { return isect(h0, h1).x; } // x-coordinate of intersection

vH construct_lower(PointLD x, vH planes) { // similar to convex hull (by duality)
    sort(all(planes), [](const Half &a, const Half &b) {
        return hp_point(a) % hp_point(b) > EPS;
    });
    vH res{{1, 0, x.x}}; // >= x.f
    planes.push_back({-1, 0, -x.y}); // <= x.s
    auto lst_x = [&](Half a, Half b) {
        if (fabsl(hp_point(a) % hp_point(b)) <= EPS) { // parallel half-planes, remove lower one
            return a[2] / a[1] <= b[2] / b[1] ? x.x : x.y;
        }
        return x_isect(a, b);
    };
    for (auto t : planes) {
        while (sz(res) > 1 && lst_x(res.back(), t) <= lst_x(res[sz(res) - 2], res.back())) {
            res.pop_back();
        }
        res.push_back(t);
    }
    return res;
}

ld isect_area(vH planes) {
    const ld BIG = 1e9;
    PointLD x{-BIG, BIG};
    planes.push_back({0, 1, -BIG}); // y >= -BIG
    planes.push_back({0, -1, -BIG}); // -y >= -BIG
    vH upper, lower;
    for (auto &t : planes) {
        if (fabsl(t[1]) <= EPS) { // vertical line
            ld quo = t[2] / t[0];
            if (t[0] > 0) {
                if (quo > x.x) x.x = quo;
            } else { // -x >=
                if (quo < x.y) x.y = quo;
            }
        } else if (t[1] > 0) {
            lower.push_back(t);
        } else {
            upper.push_back(t);
        }
    }
    if (x.x >= x.y) return 0;
    lower = construct_lower(x, lower);
    for (auto &t : upper) {
        t[0] *= -1;
        t[1] *= -1;
    }
    upper = construct_lower({-x.y, -x.x}, upper);
    for (auto &t : upper) {
        t[0] *= -1;
        t[1] *= -1;
    }
    reverse(all(upper));
    int iu = 1, il = 1;
    ld lst = x.x, lst_dif = eval(upper[1], lst) - eval(lower[1], lst);
    ld ans = 0;
    while (iu < sz(upper) - 1 && il < sz(lower) - 1) { // sweep vertical line through lower and upper hulls
        ld nex_upper = x_isect(upper[iu], upper[iu + 1]);
        ld nex_lower = x_isect(lower[il], lower[il + 1]);
        ld nex = min(nex_upper, nex_lower);
        ld nex_dif = eval(upper[iu], nex) - eval(lower[il], nex);
        auto avg_val = [](ld a, ld b) -> ld {
            if (a > b) swap(a, b);
            if (b <= 0) return 0;
            if (a >= 0) return (a + b) / 2;
            return b / (b - a) * b / 2;
        };
        ans += (nex - lst) * avg_val(lst_dif, nex_dif);
        assert(x.x <= nex && nex <= x.y);
        lst = nex, lst_dif = nex_dif;
        iu += fabsl(lst - nex_upper) <= EPS;
        il += fabsl(lst - nex_lower) <= EPS;
    }
    return ans;
}

Half plane_right(PointLD a, PointLD b) { // half-plane to right of a -> b
    return {b.y - a.y, a.x - b.x, (b.y - a.y) * a.x + (a.x - b.x) * a.y};
}

Half plane_through(PointLD p, PointLD dir) { // half-plane through p in direction dir
    return {dir.x, dir.y, p * dir};
}

