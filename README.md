# enum-array

This library is a small experiment to try and create a data structure based on
an array that provides safe access using enumeration.

## Problem

We were using lots of `enum class` to access array members. Let's assume we
have the following enumerations that are linear and starts at zero:
```
enum class module { ... };
enum class channel { ... };

static constexpr auto MODULE_COUNT = ...;
static constexpr auto CHANNEL_COUNT = ...;
```

We would use these enumerations to define and access array like below. The
problem that we've encountered once was that we would swap the enumerations
in a 2D array. The program compiled without an error. Later, there would be
a bug.
```
std::array<std::array<int, CHANNEL_COUNT>, MODULE_COUNT> array;

void foo(module m, channel c) {
    /* compiles fine */
    array[static_cast<int>(c)][static_cast<int>(m)] = ...;
}
```

The goal of this library was to create a typed array where the keys, or the
indices, are an enumeration value. The compiler should generate an error if
the wrong type would be given.
```
using namespace gpollo;

enum_array<channel, enum_array<module, int>> array;

void foo(module m, channel c) {
    /* compiler error  */
    array[c][m] = ...;

    /* compiles fine  */
    array[m][c] = ...;
}
```

In addition, it is also possible to force the initialization of all members
like the following example. The idea is that if the enumeration changes, the
compiler does not silently compiles code that may have not been properly
initialized.
```
using namespace gpollo;

using module_array = enum_array<module, int>;

/* compiles files */
auto array = module_array::safe_init(
    module_array::param<module::MODULE_1>(1),
    module_array::param<module::MODULE_2>(2),
    module_array::param<module::MODULE_3>(3),
);

/* compiler error, duplicate key */
auto array = module_array::safe_init(
    module_array::param<module::MODULE_1>(1),
    module_array::param<module::MODULE_2>(2),
    module_array::param<module::MODULE_2>(2),
    module_array::param<module::MODULE_3>(3),
);

/* compiler error, missing key */
auto array = module_array::safe_init(
    module_array::param<module::MODULE_1>(1),
    module_array::param<module::MODULE_2>(2),
);
```

## Requirements

This library requires a compiler that support the
[`magic_enum`](https://github.com/Neargye/magic_enum/blob/master/doc/limitations.md)
library. This requires at least a C++17 capable compiler.

## Usage

This repository can be used as a Meson [subproject](https://mesonbuild.com/Subprojects.html).

You can also simply add the `include/` directory to your include directories.

You can look at the tests for examples.