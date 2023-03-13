#apps
set(apps_source 
"apps_init.c"
"apps_os_ports.c"
)
set(apps_include 
"."
)
set(apps_requires 
drivers
middlewares
peripherals
)

#app_idle
set(app_idle_source 
"app_idle/app_home_screen_idle_activity.c"
)
set(app_idle_include 
"app_idle"
)
set(app_idle_requires 
drivers
middlewares
peripherals
)

#app_preproc
set(app_preproc_source 
"app_preproc/app_preproc_activity.c"
)
set(app_preproc_include 
"app_preproc"
)
set(app_preproc_requires 
drivers
middlewares
peripherals
)


idf_component_register(SRCS ${apps_source}
                            ${app_idle_source}
                            ${app_preproc_source}
                       INCLUDE_DIRS ${apps_include}
                                    ${app_idle_include}
                                    ${app_preproc_include}
                       REQUIRES ${apps_requires}
                                ${app_idle_requires}
                                ${app_preproc_requires}
                       )

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
