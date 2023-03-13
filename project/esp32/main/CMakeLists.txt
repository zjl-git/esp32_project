set(demo_bluedroid_source 
"./../../demo/demo_bluedroid/demo_ble_advertise.c"
"./../../demo/demo_bluedroid/demo_ble_ext_per_advertise.c"
"./../../demo/demo_bluedroid/demo_ble_gatt_server.c"
"./../../demo/demo_bluedroid/demo_ble_gatt_client.c"
)
set(demo_bluedroid_include 
"./../../demo/demo_bluedroid"
)

idf_component_register(SRCS "main.c"
                            ${demo_bluedroid_source}
                       INCLUDE_DIRS "."
                                    ${demo_bluedroid_include}
                       )

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
