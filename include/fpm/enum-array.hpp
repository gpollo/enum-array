#ifndef INCLUDE_FPM_ENUM_ARRAY_HPP
#define INCLUDE_FPM_ENUM_ARRAY_HPP

#include <algorithm>
#include <array>
#include <tuple>
#include <utility>

#include <magic_enum.hpp>

namespace fpm {

/**
 * This data structure defines an array where the key is a enumeration
 * value resulting in stronger typing when using the `operator[]` method.
 * Your enumeration must be supported by the library `magic_enum` [1].
 *
 * Consider a following 2D array,
 *
 * ```
 * std::array<std::array<data_type, TEMP_COUNT>, MODULE_COUNT> array;
 * ```
 *
 * You can access an element like this `array[m][t]`. Where `m` is the
 * module and `t` the temperature channel. If you accidentally swap `m`
 * and `t`, meaning you execute `array[t][m]`, your program will compile
 * fine, but the execution is most likely wrong and may result in program
 * crash.
 *
 * This data structure gives your the opportunity to replace your integer
 * keys with enumeration values. You may rewrite this array definition with
 * the following code.
 *
 * ```
 * enum class module { ... };
 * enum class temp { ... };
 * fpm::enum_array<module, std::enum_array<temp, data_type>> array;
 * ```
 *
 * You can still access an element like this `array[m][t]`, but now `m`
 * and `t` must be of enumeration type `module` and `temp`. If you swap
 * them, the compiler will give you a type error.
 *
 * [1] https://github.com/Neargye/magic_enum/blob/master/doc/limitations.md
 */
template <typename K, typename V>
class enum_array {
   private:
    static constexpr auto KEYS      = magic_enum::enum_values<K>();
    static constexpr auto KEY_COUNT = magic_enum::enum_count<K>();
    static constexpr auto N         = KEY_COUNT;

    template <K Key, typename... ConstructorArgs>
    struct param_tuple {
        std::tuple<ConstructorArgs...> constructor_args_;

        param_tuple(std::tuple<ConstructorArgs...>&& constructor_args)
            : constructor_args_(std::move(constructor_args)) {}

        /**
         * The implicit conversion is what calls the constructor.
         */
        operator V() {
            return std::make_from_tuple<V>(constructor_args_);
        }
    };

   public:
    /**
     * Inner data of the enumeration array. It should be private, but it breaks
     * safe initialisation in some cases. It should never be use directly.
     */
    std::array<V, N> do_not_use_me_directly_;

    /**
     * This method insures that all key-value pairs have been initialized
     * and that there are no duplicate key. The checks are at compile-time.
     *
     * ```
     * enum class test { VALUE_1, VALUE_2, VALUE_3, VALUE_4, VALUE_5, VALUE_6 };
     * using enum_array = fpm::enum_array<test, int>;
     *
     * // will compile
     * auto a1 = enum_array::safe_init(enum_array::param<test::VALUE_1>{1},
     *                                 enum_array::param<test::VALUE_2>{2},
     *                                 enum_array::param<test::VALUE_3>{3},
     *                                 enum_array::param<test::VALUE_4>{4},
     *                                 enum_array::param<test::VALUE_5>{5},
     *                                 enum_array::param<test::VALUE_6>{6});
     *
     * // won't compile, missing key "test::VALUE_4"
     * auto a2 = enum_array::safe_init(enum_array::param<test::VALUE_1>{1},
     *                                 enum_array::param<test::VALUE_2>{2},
     *                                 enum_array::param<test::VALUE_3>{3},
     *                                 enum_array::param<test::VALUE_5>{5},
     *                                 enum_array::param<test::VALUE_6>{6});
     *
     * // won't compile, two keys are not in order
     * auto a3 = enum_array::safe_init(enum_array::param<test::VALUE_1>{1},
     *                                 enum_array::param<test::VALUE_2>{2},
     *                                 enum_array::param<test::VALUE_4>{4},
     *                                 enum_array::param<test::VALUE_3>{3},
     *                                 enum_array::param<test::VALUE_5>{5},
     *                                 enum_array::param<test::VALUE_6>{6});
     * ```
     */
    template <typename... ExpectedParams>
    static enum_array<K, V> safe_init(ExpectedParams... expected_params) {
        using finished = std::integral_constant<bool, 0U == KEY_COUNT>;
        return impl_safe_init<0U>(finished(), expected_params...);
    }

    /**
     * Returns a parameter for the safe initialisation.
     */
    template <K Key, typename... ConstructorArgs>
    static param_tuple<Key, ConstructorArgs...> param(ConstructorArgs&&... constructor_args) {
        return param_tuple<Key, ConstructorArgs...>(std::forward_as_tuple(constructor_args...));
    }

    /**
     * Returns a reference to the element at with the specified key.
     */
    V& operator[](K key) noexcept {
        return do_not_use_me_directly_[static_cast<unsigned int>(key)];
    }

    /**
     * Returns a constant reference to the element at with the specified key.
     */
    const V& operator[](K key) const noexcept {
        return do_not_use_me_directly_[static_cast<unsigned int>(key)];
    }

    void fill(V value) noexcept {
        std::fill(do_not_use_me_directly_.begin(), do_not_use_me_directly_.end(), value);
    }

   private:
    /* safe initialisation implementation */

    template <unsigned int I, typename... ConstructorArgs>
    using expected = const param_tuple<KEYS[I], ConstructorArgs...>&;

    template <unsigned int I, typename... Converters>
    static enum_array<K, V> impl_safe_init(std::true_type /* finished */, Converters... converters) {
        return enum_array<K, V>{{converters...}};
    }

    template <unsigned int I, typename... ConstructorArgs, typename... OtherConstructorArgs, typename... Converters>
    static enum_array<K, V> impl_safe_init(std::false_type /* finished */,
                                           expected<I, ConstructorArgs...> current_constructor_args,
                                           OtherConstructorArgs... other_constructor_args,
                                           Converters... generated_converters) {
        using finished = std::integral_constant<bool, I + 1U == KEY_COUNT>;
        return impl_safe_init<I + 1U>(
            finished(), other_constructor_args..., generated_converters..., current_constructor_args);
    }

   public:
    class iterator {
       public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = V;
        using pointer           = V*;
        using reference         = V&;

        iterator(pointer ptr) : ptr_(ptr) {}

        /* clang-format off */
        reference operator* () const { return *ptr_; }
        pointer   operator->()       { return ptr_; }
        iterator& operator++()       { ptr_++; return *this; }  
        iterator  operator++(int)    { iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const iterator& a, const iterator& b) { return a.ptr_ == b.ptr_; };
        friend bool operator!= (const iterator& a, const iterator& b) { return a.ptr_ != b.ptr_; };
        /* clang-format on */

       private:
        pointer ptr_;
    };

    iterator begin() {
        return iterator(&do_not_use_me_directly_[0]);
    }

    iterator end() {
        return iterator(&do_not_use_me_directly_[N]);
    }
};

} /* namespace fpm */

#endif /* INCLUDE_FPM_ENUM_ARRAY_HPP */
