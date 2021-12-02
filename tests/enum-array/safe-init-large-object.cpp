#include <cassert>

#include <gpollo/enum-array.hpp>

enum class test { VALUE_1, VALUE_2, VALUE_3, VALUE_4, VALUE_5, VALUE_6 };

template <unsigned int N>
class obj {
   public:
    obj(int a, int b) {
        for (unsigned int i = 0U; i < N; i++) {
            data_[i] = i * a + b;
        }
    }

    int sum() const {
        int32_t sum = 0U;
        for (unsigned int i = 0U; i < N; i++) {
            sum += data_[i];
        }
        return sum;
    }

   private:
    std::array<std::int32_t, N> data_;
};

int main() {
    using array = gpollo::enum_array<test, obj<2000>>;

    auto a = array::safe_init(array::param<test::VALUE_1>(1, 11),
                              array::param<test::VALUE_2>(2, 12),
                              array::param<test::VALUE_3>(3, 13),
                              array::param<test::VALUE_4>(4, 14),
                              array::param<test::VALUE_5>(5, 15),
                              array::param<test::VALUE_6>(6, 16));

    assert(a[test::VALUE_1].sum() == 2021000);
    assert(a[test::VALUE_2].sum() == 4022000);
    assert(a[test::VALUE_3].sum() == 6023000);
    assert(a[test::VALUE_4].sum() == 8024000);
    assert(a[test::VALUE_5].sum() == 10025000);
    assert(a[test::VALUE_6].sum() == 12026000);

    return 0;
}