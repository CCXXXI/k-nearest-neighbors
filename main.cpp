#pragma region base

#include "bits/stdc++.h"

using namespace std;

using i64 = int64_t;
using u64 = uint64_t;
using i32 = int32_t;
using u32 = uint32_t;
using i16 = int16_t;
using u16 = uint16_t;
using i8 = int8_t;
using u8 = uint8_t;

#define $ auto
#define $$ auto &&

$ constexpr inf = 0x3f3f3f3f;

$ ccxxxi() {
#ifdef ONLINE_JUDGE
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
#endif
}

#pragma endregion

#pragma region 2 - dimensional tree

// 辅助函数，相当于先push再pop
template <typename E> $ push_pop(vector<E> &v, const E &e) {
  if (e < v.front()) {
    pop_heap(v.begin(), v.end());
    v.back() = e;
    push_heap(v.begin(), v.end());
  }
}

template <typename Crd, typename Other> class _2dt {
public:
  using crd_arr_t = array<Crd, 2>;

  class point {
  public:
    crd_arr_t crd;
    Other other;

    point(const crd_arr_t &crd_in, Other other_in)
        : crd(crd_in), other(std::move(other_in)) {}
  };

private:
  u32 root_ = 0;
  vector<u32> lc_, rc_;
  // vector<bool> axis_;
  bool beg_ = false;
  vector<point> &points_;

  // 以 [first, last) 中的点建树，返回此树的root
  u32 build(const u32 &first, const u32 &last, const bool &r) {
    // auto const & r = choose_axis(first, last);
    auto const &num = last - first;
    auto const &mid = first + num / 2;
    if (num == 1) {
    } else if (num == 2) {
      // axis_[mid] = r;
      (points_[first].crd[r] <= points_[mid].crd[r] ? lc_[mid] : rc_[mid]) =
          first;
    } else {
      auto const &b = points_.begin();
      nth_element(
          b + first, b + mid, b + last,
          [&](const point &x, const point &y) { return x.crd[r] < y.crd[r]; });
      // axis_[mid] = r;
      lc_[mid] = build(first, mid, r ^ 1);
      rc_[mid] = build(mid + 1, last, r ^ 1);
    }
    return mid;
  }

  // 选择 [first, last) 中方差最大的维度
  $ choose_axis(const u32 &first, const u32 &last) const {
    return variance(first, last, false) < variance(first, last, true);
  }

  // 计算 [first, last) 中，维度r的方差
  $ variance(const u32 &first, const u32 &last, const bool &r) const {
    $ sum_x = 0.0f, sum_x2 = 0.0f;
    for ($ i = first; i != last; ++i) {
      auto const &tmp = static_cast<float>(points_[i].crd[r]);
      sum_x += tmp;
      sum_x2 += tmp * tmp;
    }
    return sum_x2 - sum_x * sum_x / static_cast<float>(last - first);
  }

public:
  // 以vector<point>初始化，之后外部不应修改此vector
  explicit _2dt(vector<point> &points_in) : points_(points_in) {
    auto const &sz = points_.size();
    beg_ = choose_axis(0, sz);
    // axis_.resize(sz);
    lc_.resize(sz, inf);
    rc_.resize(sz, inf);
    root_ = build(0, sz, beg_);
  }

private:
  class ret_t {
  public:
    double dis;
    Other other;
    $ operator<(const ret_t &a) const {
      return tie(this->dis, this->other) < tie(a.dis, a.other);
    }
  };

  ret_t none_{numeric_limits<double>::infinity(), Other()};

  // 返回px的欧氏距离的平方，使用浮点数避免平方后溢出
  $ dis2(const crd_arr_t &p, const u32 &x) {
    auto const &dis_x =
        static_cast<double>(p[0]) - static_cast<double>(points_[x].crd[0]);
    auto const &dis_y =
        static_cast<double>(p[1]) - static_cast<double>(points_[x].crd[1]);
    return static_cast<double>(static_cast<i32>(
               sqrt(dis_x * dis_x + dis_y * dis_y) * 1000 + 0.5)) /
           1000;
  }

public:
  // 返回距离点p最近的k个点，欧氏距离
  $ knn(const crd_arr_t &p, const u32 &k) {
    vector<ret_t> ret(k, none_);
    function<void(u32, bool)> dfs = [&](const u32 &x, const bool &r) {
      if (x != inf) {
        // auto const & r = axis_[x];
        auto const &dis_sp = p[r] - points_[x].crd[r];
        auto const &left = dis_sp <= 0;
        dfs(left ? lc_[x] : rc_[x], r ^ 1);
        auto const &tmp = ret_t{dis2(p, x), points_[x].other};
        push_pop(ret, tmp);
        if (abs(dis_sp) <= ret.front().dis) {
          dfs(left ? rc_[x] : lc_[x], r ^ 1);
        }
      }
    };
    dfs(root_, beg_);
    sort_heap(ret.begin(), ret.end());
    ret.erase(lower_bound(ret.begin(), ret.end(), none_), ret.end());
    return ret;
  }
};

#pragma endregion

using name_t = array<char, 12>;

auto &operator<<(ostream &o, const name_t &a) {
  for ($$ i : a) {
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
  ccxxxi();

  using kdt_t = _2dt<short, name_t>;
  $ m = 0, n = 0;
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
  for ($$[key_, vec_] : dat) {
    trees.emplace(key_, vec_);
  }
  u32 k = 0;
  while (n-- != 0) {
    cin >> x >> y >> key >> k;
    $$ ans = trees.at(key).knn(array{x, y}, k);
    for ($$ a : ans) {
      cout << a.other << " " << fixed << setprecision(3) << a.dis << "\n";
    }
  }
}
