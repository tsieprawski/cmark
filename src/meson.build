configuration = configuration_data()
configuration.set('CMARK_EXPORT', '__attribute__ ((visibility ("default")))')
cmark_version_h = configure_file(output: 'cmark_export.h',
    configuration: configuration)

configuration = configuration_data()
configuration.set_quoted('PROJECT_VERSION_MAJOR', '0')
configuration.set_quoted('PROJECT_VERSION_MINOR', '29')
configuration.set_quoted('PROJECT_VERSION_PATCH', '0')
configuration.set('CMARK_VERSION', 0x002900)
configuration.set_quoted('CMARK_VERSION_STRING', '0.29.0')
configuration.set('false', '0')
configuration.set('true', '1')
cmark_version_h = configure_file(output: 'cmark_version.h',
    configuration: configuration)

config_h = configure_file(input: 'config.h.in',
    format: 'cmake',
    output: 'config.h',
    configuration: configuration_data())

libcmark_inc = include_directories('.')
libcmark_src = [
    'cmark.c',
    'node.c',
    'iterator.c',
    'blocks.c',
    'inlines.c',
    'scanners.c',
    'utf8.c',
    'buffer.c',
    'references.c',
    'render.c',
    'man.c',
    'xml.c',
    'html.c',
    'commonmark.c',
    'latex.c',
    'houdini_href_e.c',
    'houdini_html_e.c',
    'houdini_html_u.c',
    'cmark_ctype.c',
    cmark_version_h,
    config_h
]
libcmark = shared_library('cmark',
    libcmark_src,
    gnu_symbol_visibility: 'hidden',
    include_directories: [
        libcmark_inc
    ],
    install: true)
libcmark_dep = declare_dependency(link_with: libcmark,
    include_directories: libcmark_inc)

cmark = executable('cmark',
    'main.c',
    dependencies: [libcmark_dep])
install_headers('cmark.h')
