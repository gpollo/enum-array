#include <cassert>

#include <gpollo/enum-array.hpp>

enum class test { VALUE_1, VALUE_2, VALUE_3, VALUE_4, VALUE_5, VALUE_6 };

class obj {
   public:
    obj(int a) : a_(a), b_(a + 10) {}
    obj(int a, int b) : a_(a), b_(b) {}

    int sum() const {
        return a_ + b_;
    }

   private:
    int a_;
    int b_;
};

int main() {
    using array = gpollo::enum_array<test, obj>;

    auto a = array::safe_init(array::param<test::VALUE_1>(1, 10),
                              array::param<test::VALUE_2>(2),
                              array::param<test::VALUE_3>(3, 13),
                              array::param<test::VALUE_4>(4),
                              array::param<test::VALUE_5>(5, 15),
                              array::param<test::VALUE_6>(6));

    assert(a[test::VALUE_1].sum() == 11);
    assert(a[test::VALUE_2].sum() == 14);
    assert(a[test::VALUE_3].sum() == 16);
    assert(a[test::VALUE_4].sum() == 18);
    assert(a[test::VALUE_5].sum() == 20);
    assert(a[test::VALUE_6].sum() == 22);

    return 0;
}