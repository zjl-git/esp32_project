#oled_ssd1306
set(oled_ssd1306_source 
"oled/src/ssd1306/oled_ssd1306.c"
"oled/oled_ssd1306_port.c"
)
set(oled_ssd1306_include 
"oled/src/ssd1306"
"oled"
)
set(oled_ssd1306_requires 
drivers
)


idf_component_register(SRCS ${oled_ssd1306_source}
                       INCLUDE_DIRS ${oled_ssd1306_include}
                       REQUIRES ${oled_ssd1306_requires}
                       )

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
