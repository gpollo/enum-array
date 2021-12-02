#include <cassert>

#include <gpollo/enum-array.hpp>

enum class test { VALUE_1, VALUE_2, VALUE_3, VALUE_4, VALUE_5, VALUE_6 };

int main() {
    using array = gpollo::enum_array<test, int>;
    array a;

    std::fill(a.begin(), a.end(), 0);
    assert(a[test::VALUE_1] == 0);
    assert(a[test::VALUE_2] == 0);
    assert(a[test::VALUE_3] == 0);
    assert(a[test::VALUE_4] == 0);
    assert(a[test::VALUE_5] == 0);
    assert(a[test::VALUE_6] == 0);

    int i = 0;
    for (auto& value : a) {
        value = i++;
    }
    assert(a[test::VALUE_1] == 0);
    assert(a[test::VALUE_2] == 1);
    assert(a[test::VALUE_3] == 2);
    assert(a[test::VALUE_4] == 3);
    assert(a[test::VALUE_5] == 4);
    assert(a[test::VALUE_6] == 5);

    return 0;
}