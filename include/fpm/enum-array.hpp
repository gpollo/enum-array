#ifndef INCLUDE_FPM_ENUM_ARRAY_HPP
#define INCLUDE_FPM_ENUM_ARRAY_HPP

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
class enum_array : public std::array<V, magic_enum::enum_count<K>()> {
   private:
    static constexpr auto KEYS      = magic_enum::enum_values<K>();
    static constexpr auto KEY_COUNT = magic_enum::enum_count<K>();
    static constexpr auto N         = KEY_COUNT;

    template <K Key, typename... Args>
    struct param_tuple {
        std::tuple<Args...> args_;
        param_tuple(std::tuple<Args...>&& args) : args_(std::move(args)) {}
    };

   public:
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
    template <typename... Args>
    static enum_array<K, V> safe_init(Args... args) {
        enum_array<K, V> array;

        using finished = std::integral_constant<bool, 0U == KEY_COUNT>;
        init_array<0U>(finished(), array, args...);

        return array;
    }

    /**
     * Returns a parameter for the safe initialisation.
     */
    template <K Key, typename... Params>
    static param_tuple<Key, Params...> param(Params&&... args) {
        return param_tuple<Key, Params...>(std::forward_as_tuple(args...));
    }

    /**
     * Returns a reference to the element at with the specified key.
     */
    V& operator[](K key) noexcept {
        return static_cast<std::array<V, N>&>(*this)[static_cast<unsigned int>(key)];
    }

    /**
     * Returns a constant reference to the element at with the specified key.
     */
    const V& operator[](K key) const noexcept {
        return static_cast<const std::array<V, N>&>(*this)[static_cast<unsigned int>(key)];
    }

   private:
    /* safe initialisation implementation */

    template <unsigned int I, typename... Params>
    using expected_param = param_tuple<KEYS[I], Params...>;

    template <unsigned int I>
    static void init_array(std::true_type /* finished */, std::array<V, N>& array) {}

    template <unsigned int I, typename... Args, typename... Params>
    static void init_array(std::false_type /* finished */, std::array<V, N>& array,
                           const expected_param<I, Params...>& param, Args... args) {
        array[I] = std::make_from_tuple<V>(param.args_);

        using finished = std::integral_constant<bool, I + 1 == KEY_COUNT>;
        init_array<I + 1>(finished(), array, args...);
    }
};

} /* namespace fpm */

#endif /* INCLUDE_FPM_ENUM_ARRAY_HPP */
