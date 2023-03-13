set(srcs            "src/tm_stat.c"
                    "src/tm_model.c"
                    "src/tm_layers.c"
                    "src/tm_layers_O1.c"
                    "src/tm_layers_fp8.c"
                    "demo/mnist.c"
                    )



set(include_dirs    "src"
                    "include"
                    "tmdl"
                    "demo"
)

set(requires 
esp-dsp
)


idf_component_register(SRCS ${srcs}
                      INCLUDE_DIRS ${include_dirs}
                      REQUIRES ${requires})

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")

set_source_files_properties(${srcs}
        PROPERTIES COMPILE_FLAGS
        -Wno-unused-variable
        -Wmultichar
        -Wunused-but-set-variable
    )
