test('fpm/enum-array/compile-failure-duplicate',
    compile_sh,
    args: [
        '--expect-failure',
        '--source-file',
        '@0@'.format(meson.current_source_dir() / 'compile-failure-duplicate.cpp'),
        '--meson-compiler-object',
        '@0@'.format(meson.get_compiler('cpp')),
        '--include',
        '@0@'.format(meson.source_root() / 'include/'),
        '--include',
        '@0@'.format(meson.source_root() / 'subprojects/libmagic-enum/magic_enum/include/'),
    ],
)

test('fpm/enum-array/compile-failure-missing',
    compile_sh,
    args: [
        '--expect-failure',
        '--source-file',
        '@0@'.format(meson.current_source_dir() / 'compile-failure-missing.cpp'),
        '--meson-compiler-object',
        '@0@'.format(meson.get_compiler('cpp')),
        '--include',
        '@0@'.format(meson.source_root() / 'include/'),
        '--include',
        '@0@'.format(meson.source_root() / 'subprojects/libmagic-enum/magic_enum/include/'),
    ],
)

test('fpm/enum-array/compile-failure-order',
    compile_sh,
    args: [
        '--expect-failure',
        '--source-file',
        '@0@'.format(meson.current_source_dir() / 'compile-failure-order.cpp'),
        '--meson-compiler-object',
        '@0@'.format(meson.get_compiler('cpp')),
        '--include',
        '@0@'.format(meson.source_root() / 'include/'),
        '--include',
        '@0@'.format(meson.source_root() / 'subprojects/libmagic-enum/magic_enum/include/'),
    ],
)

test('fpm/enum-array/different-constructors',
    executable('test-different-constructors', ['different-constructors.cpp'],
        dependencies: libfpm_dep,
    ),
)

test('fpm/enum-array/non-copyable',
    executable('test-non-copyable', ['non-copyable.cpp'],
        dependencies: libfpm_dep,
    ),
)

test('fpm/enum-array/object-attribute',
    executable('test-object-attribute', ['object-attribute.cpp'],
        dependencies: libfpm_dep,
    ),
)

test('fpm/enum-array/object',
    executable('test-object', ['object.cpp'],
        dependencies: libfpm_dep,
    ),
)

test('fpm/enum-array/simple',
    executable('test-simple', ['simple.cpp'],
        dependencies: libfpm_dep,
    ),
)