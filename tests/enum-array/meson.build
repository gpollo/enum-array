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

test('fpm/enum-array/iterator-simple',
    executable('test-iterator-simple', ['iterator-simple.cpp'],
        dependencies: libfpm_dep,
    ),
)

test('fpm/enum-array/safe-init-different-constructors',
    executable('test-safe-init-different-constructors', ['safe-init-different-constructors.cpp'],
        dependencies: libfpm_dep,
    ),
)

safe_init_large_object = executable('test-safe-init-large-object', ['safe-init-large-object.cpp'],
    dependencies: libfpm_dep,
    cpp_args: ['-fcallgraph-info=su', '-save-temps=cwd']
)

test('fpm/enum-array/safe-init-large-object', safe_init_large_object)

test('fpm/enum-array/safe-init-large-object-check-stack',
    check_stack_usage_sh,
    args: [
        '--callgraph-file',
        '@0@'.format(meson.build_root() / 'safe-init-large-object.cpp.ci'),
        '--function-name',
        'main',
        '--min-stack-usage',
        '49400',
        '--max-stack-usage',
        '49600',
    ],
    depends: safe_init_large_object,
)

test('fpm/enum-array/safe-init-non-copyable',
    executable('test-safe-init-non-copyable', ['safe-init-non-copyable.cpp'],
        dependencies: libfpm_dep,
    ),
)

test('fpm/enum-array/safe-init-object-attribute',
    executable('test-safe-init-object-attribute', ['safe-init-object-attribute.cpp'],
        dependencies: libfpm_dep,
    ),
)

test('fpm/enum-array/safe-init-object',
    executable('test-safe-init-object', ['safe-init-object.cpp'],
        dependencies: libfpm_dep,
    ),
)

test('fpm/enum-array/safe-init-simple',
    executable('test-safe-init-simple', ['safe-init-simple.cpp'],
        dependencies: libfpm_dep,
    ),
)