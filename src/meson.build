configuration = configuration_data()
configuration.set('CMARK_EXPORT', '__attribute__ ((visibility ("default")))')
cmark_export_h = configure_file(output: 'cmark_export.h',
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
    'houdini_href_e.c',
    'houdini_html_e.c',
    'houdini_html_u.c',
    'cmark_ctype.c',
    cmark_export_h,
    cmark_version_h
]

config = configuration_data()
if get_option('commonmark')
    libcmark_src += [
        'commonmark.c'
    ]
    config.set('HAVE_COMMONMARK', '1')
endif

if get_option('html')
    libcmark_src += [
        'html.c'
    ]
    config.set('HAVE_HTML', '1')
endif

if get_option('htmlol')
    libcmark_src += [
        'htmlol.c'
    ]
    config.set('HAVE_HTMLOL', '1')
endif

if get_option('latex')
    libcmark_src += [
        'latex.c'
    ]
    config.set('HAVE_LATEX', '1')
endif

if get_option('man')
    libcmark_src += [
        'man.c'
    ]
    config.set('HAVE_LATEX', '1')
endif

if get_option('xml')
    libcmark_src += [
        'xml.c'
    ]
    config.set('HAVE_XML', '1')
endif

config_h = configure_file(input: 'config.h.in',
    format: 'cmake',
    output: 'config.h',
    configuration: config)
libcmark_src += [
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
pkg.generate(libcmark)

libcmark_static = static_library('cmark',
    libcmark_src,
    gnu_symbol_visibility: 'hidden',
    include_directories: [
        libcmark_inc
    ],
    install: true)
libcmark_static_dep = declare_dependency(link_with: libcmark_static,
    include_directories: libcmark_inc)

cmark = executable('cmark',
    'main.c',
    dependencies: [libcmark_static_dep])

install_headers(cmark_export_h)
install_headers(cmark_version_h)
install_headers('cmark.h')
