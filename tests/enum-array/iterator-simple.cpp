#include <cassert>

#include <gpollo/enum-array.hpp>

enum class test { VALUE_1, VALUE_2, VALUE_3, VALUE_4, VALUE_5, VALUE_6 };

static void test_iterator(gpollo::enum_array<test, int>& array) {
    std::fill(array.begin(), array.end(), 0);
    assert(array[test::VALUE_1] == 0);
    assert(array[test::VALUE_2] == 0);
    assert(array[test::VALUE_3] == 0);
    assert(array[test::VALUE_4] == 0);
    assert(array[test::VALUE_5] == 0);
    assert(array[test::VALUE_6] == 0);

    int i = 0;
    for (auto& value : array) {
        value = i++;
    }
    assert(array[test::VALUE_1] == 0);
    assert(array[test::VALUE_2] == 1);
    assert(array[test::VALUE_3] == 2);
    assert(array[test::VALUE_4] == 3);
    assert(array[test::VALUE_5] == 4);
    assert(array[test::VALUE_6] == 5);
}

static void test_const_iterator(const gpollo::enum_array<test, int>& array) {
    int sum = 0;
    for (const auto& value : array) {
        sum += value;
    }

    assert(sum == 15);
}

int main() {
    gpollo::enum_array<test, int> array;

    test_iterator(array);
    test_const_iterator(array);

    return 0;
}