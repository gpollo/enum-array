#include <cassert>

#include <fpm/enum-array.hpp>

enum class test { VALUE_1, VALUE_2, VALUE_3, VALUE_4, VALUE_5, VALUE_6 };

int main() {
    using array = fpm::enum_array<test, int>;

    auto a = array::safe_init(array::param<test::VALUE_1>(1),
                              array::param<test::VALUE_2>(2),
                              array::param<test::VALUE_3>(3),
                              array::param<test::VALUE_4>(4),
                              array::param<test::VALUE_5>(5),
                              array::param<test::VALUE_6>(6));

    assert(a[test::VALUE_1] == 1);
    assert(a[test::VALUE_2] == 2);
    assert(a[test::VALUE_3] == 3);
    assert(a[test::VALUE_4] == 4);
    assert(a[test::VALUE_5] == 5);
    assert(a[test::VALUE_6] == 6);

    return 0;
}