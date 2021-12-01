#include <cassert>

#include <fpm/enum-array.hpp>

enum class test { VALUE_1, VALUE_2, VALUE_3, VALUE_4, VALUE_5, VALUE_6 };

class obj {
   public:
    obj() = default;
    obj(int a, int b) : a_(a), b_(b) {}

    int sum() const {
        return a_ + b_;
    }

   private:
    int a_;
    int b_;
};

class container {
   public:
    using array = fpm::enum_array<test, obj>;
    array array_;

    container()
        : array_(array::safe_init(array::param<test::VALUE_1>(1, 10), array::param<test::VALUE_2>(2, 12),
                                  array::param<test::VALUE_3>(3, 13), array::param<test::VALUE_4>(4, 14),
                                  array::param<test::VALUE_5>(5, 15), array::param<test::VALUE_6>(6, 16))) {}
};

int main() {
    container c;

    assert(c.array_[test::VALUE_1].sum() == 11);
    assert(c.array_[test::VALUE_2].sum() == 14);
    assert(c.array_[test::VALUE_3].sum() == 16);
    assert(c.array_[test::VALUE_4].sum() == 18);
    assert(c.array_[test::VALUE_5].sum() == 20);
    assert(c.array_[test::VALUE_6].sum() == 22);

    return 0;
}