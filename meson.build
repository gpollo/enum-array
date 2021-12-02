project(
    'libenum-array', ['cpp'],
    default_options: ['cpp_std=c++17']
)

############################
# libmagic-enum dependency #
############################

libmagic_enum_subproject = subproject('libmagic-enum')

libmagic_enum_dep = libmagic_enum_subproject.get_variable('magic_enum_dep')

#################
# libenum-array #
#################

libenum_array_include = include_directories('include')

libenum_array_dep = declare_dependency(
    include_directories: libenum_array_include,
    dependencies: libmagic_enum_dep,
)

#########
# tests #
#########

if not meson.is_subproject()
    subdir('tests')
endif

#################
# format target #
#################

format_include = '`find include -type f -iname \'*.hpp\'`'
format_tests = '`find tests -type f -iname \'*.cpp\'`'
format_files = '@0@ @1@'.format(format_include, format_tests)

run_target('format',
    command: [
        'sh', '-c',
        'cd @0@ && clang-format --verbose -i @1@'.format(
            meson.source_root(),
            format_files,
        )
    ],
)

###############
# tidy target #
###############

tidy_include = '`find include -type f -iname \'*.hpp\'`'
tidy_tests = '`find tests -type f -iname \'*.cpp\'`'
tidy_files = '@0@ @1@'.format(tidy_include, tidy_tests)

run_target('tidy',
    command: [
        'bash', '-c',
        'cd @0@ && clang-tidy -p=@1@ @2@'.format(
            meson.source_root(),
            meson.current_build_dir(),
            tidy_files,
        )
    ],
)