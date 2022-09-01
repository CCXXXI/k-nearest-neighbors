#include "bits/stdc++.h"

using namespace std;

int constexpr inf = 0x3f3f3f3f;

// 辅助函数，相当于先push再pop
template<typename E>
void push_pop(vector<E> &v, E const &e) {
    if (e < v.front()) {
        pop_heap(v.begin(), v.end());
        v.back() = e;
        push_heap(v.begin(), v.end());
    }
}

template<typename Crd, typename Other>
class two_d_tree {
public:
    using crd_arr_t = array<Crd, 2>;

    class point {
    public:
        crd_arr_t crd;
        Other other;

        point(crd_arr_t crd_in, Other other_in) : crd(crd_in), other(std::move(other_in)) {}
    };

private:
    unsigned root = 0;
    vector<unsigned> lc, rc;
    // vector<bool> axis;
    bool beg = false;
    vector<point> points;

    // 以 [first, last) 中的点建树，返回此树的root
    unsigned build(unsigned first, unsigned last, bool r) { // NOLINT(misc-no-recursion)
        // auto r = choose_axis(first, last);
        auto num = last - first;
        auto mid = first + num / 2;
        if (num == 1) {
        } else if (num == 2) {
            // axis[mid] = r;
            (points[first].crd[r] <= points[mid].crd[r] ? lc[mid] : rc[mid]) = first;
        } else {
            auto b = points.begin();
            nth_element(b + first, b + mid, b + last,
                        [&](point const &x, point const &y) { return x.crd[r] < y.crd[r]; });
            // axis[mid] = r;
            lc[mid] = build(first, mid, !r);
            rc[mid] = build(mid + 1, last, !r);
        }
        return mid;
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantParameter"

    // 选择 [first, last) 中方差最大的维度
    [[nodiscard]] auto choose_axis(unsigned first, unsigned last) const {
        return variance(first, last, false) < variance(first, last, true);
    }

    // 计算 [first, last) 中，维度r的方差
    [[nodiscard]] auto variance(unsigned first, unsigned last, bool r) const {
        auto sum_x = 0.0f, sum_x2 = 0.0f;
        for (auto i = first; i != last; ++i) {
            auto tmp = static_cast<float>(points[i].crd[r]);
            sum_x += tmp;
            sum_x2 += tmp * tmp;
        }
        return sum_x2 - sum_x * sum_x / static_cast<float>(last - first);
    }

#pragma clang diagnostic pop

public:
    [[maybe_unused]] explicit two_d_tree(vector<point> &&points_in) : points(std::move(points_in)) {
        auto sz = points.size();
        beg = choose_axis(0, sz);
        // axis.resize(sz);
        lc.resize(sz, inf);
        rc.resize(sz, inf);
        root = build(0, sz, beg);
    }

private:
    class ret_t {
    public:
        double dis;
        Other other;

        auto operator<(ret_t const &a) const {
            return tie(this->dis, this->other) < tie(a.dis, a.other);
        }
    };

    ret_t none{numeric_limits<double>::infinity(), Other()};

    // 返回px的欧氏距离的平方，使用浮点数避免平方后溢出
    auto dis2(crd_arr_t p, unsigned x) {
        auto dis_x = static_cast<double>(p[0]) - static_cast<double>(points[x].crd[0]);
        auto dis_y = static_cast<double>(p[1]) - static_cast<double>(points[x].crd[1]);
        return static_cast<double>(lround(sqrt(dis_x * dis_x + dis_y * dis_y) * 1000)) / 1000;
    }

public:
    // 返回距离点p最近的k个点，欧氏距离
    auto knn(crd_arr_t p, unsigned k) {
        vector<ret_t> ret(k, none);
        function<void(unsigned, bool)> dfs = [&](unsigned x, bool r) {
            if (x != inf) {
                // auto r = axis[x];
                auto dis_sp = p[r] - points[x].crd[r];
                auto left = dis_sp <= 0;
                dfs(left ? lc[x] : rc[x], !r);
                auto tmp = ret_t{dis2(p, x), points[x].other};
                push_pop(ret, tmp);
                if (abs(dis_sp) <= ret.front().dis) {
                    dfs(left ? rc[x] : lc[x], !r);
                }
            }
        };
        dfs(root, beg);
        sort_heap(ret.begin(), ret.end());
        ret.erase(lower_bound(ret.begin(), ret.end(), none), ret.end());
        return ret;
    }
};


using name_t = array<char, 12>;

auto &operator<<(ostream &o, name_t const &a) {
    for (auto &&i: a) {
        o << i;
    }
    return o;
}

auto &operator>>(istream &in, name_t &a) {
    // 手动暴力展开
    cin.ignore(1, '\n');
    cin.get(a[0]);
    cin.get(a[1]);
    cin.get(a[2]);
    cin.get(a[3]);
    cin.get(a[4]);
    cin.get(a[5]);
    cin.get(a[6]);
    cin.get(a[7]);
    cin.get(a[8]);
    cin.get(a[9]);
    cin.get(a[10]);
    cin.get(a[11]);
    return in;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using kdt_t = two_d_tree<short, name_t>;
    auto m = 0, n = 0;
    cin >> m >> n;
    unordered_map<string, vector<kdt_t::point>> dat;
    name_t name;
    short x = 0;
    short y = 0;
    string key;
    while (m-- != 0) {
        cin >> name >> x >> y >> key;
        dat[key].emplace_back(array{x, y}, name);
    }
    unordered_map<string, kdt_t> trees;
    for (auto &&[key_, vec_]: dat) {
        trees.emplace(key_, std::move(vec_));
    }
    unsigned k = 0;
    while (n-- != 0) {
        cin >> x >> y >> key >> k;
        auto ans = trees.at(key).knn(array{x, y}, k);
        for (auto &&a: ans) {
            cout << a.other << " " << fixed << setprecision(3) << a.dis << "\n";
        }
    }
}
