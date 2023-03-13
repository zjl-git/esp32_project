#zlog
set(zlog_source 
"zlog/source/zlog.c"
"zlog/source/zlog_utils.c"
"zlog/source/zlog_ports.c"
"zlog/source/zlog_buf.c"
"zlog/source/zlog_async.c"
"zlog/demo/zlog_demo.c"
)
set(zlog_include 
"zlog/include"
"zlog/demo"
)
set(zlog_requires 
driver
)

idf_component_register(SRCS ${zlog_source}
                       INCLUDE_DIRS ${zlog_include}
                       REQUIRES ${zlog_requires}
                      )
target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
