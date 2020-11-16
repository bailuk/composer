project('composer', 'c')


executable(
        meson.project_name(),
        'composer.c',
        'composer_gui.c',
        'composer_event.c',
        'composer_cfg.c',
        dependencies : [
                dependency('gtk+-3.0'),
                meson.get_compiler('c').find_library('xdo')
        ],
        install: true
)

