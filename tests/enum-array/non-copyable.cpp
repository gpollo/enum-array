#include <cassert>

#include <fpm/enum-array.hpp>

enum class test { VALUE_1, VALUE_2, VALUE_3, VALUE_4, VALUE_5, VALUE_6 };

class obj {
   public:
    obj() = delete;
    obj(int a, int b) : a_(a), b_(b) {}
    obj(const obj& other) = delete;
    obj(obj&& other)      = delete;
    obj& operator=(const obj& other) = delete;
    obj& operator=(obj&& other) = delete;

    int sum() const {
        return a_ + b_;
    }

   private:
    int a_;
    int b_;
};

int main() {
    using array = fpm::enum_array<test, obj>;

    auto a = array::safe_init(array::param<test::VALUE_1>(1, 10),
                              array::param<test::VALUE_2>(2, 12),
                              array::param<test::VALUE_3>(3, 13),
                              array::param<test::VALUE_4>(4, 14),
                              array::param<test::VALUE_5>(5, 15),
                              array::param<test::VALUE_6>(6, 16));

    assert(a[test::VALUE_1].sum() == 11);
    assert(a[test::VALUE_2].sum() == 14);
    assert(a[test::VALUE_3].sum() == 16);
    assert(a[test::VALUE_4].sum() == 18);
    assert(a[test::VALUE_5].sum() == 20);
    assert(a[test::VALUE_6].sum() == 22);

    return 0;
}