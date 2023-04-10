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

#flash
set(flash_source 
"flash/src/w25qxx/flash_w25qxx.c"
"flash/flash_w25qxx_port.c"
)
set(flash_include 
"flash/src/w25qxx"
"flash"
)
set(flash_requires 
drivers
)


idf_component_register(SRCS ${oled_ssd1306_source}
                            ${flash_source}
                       INCLUDE_DIRS ${oled_ssd1306_include}
                                    ${flash_include}
                       REQUIRES ${oled_ssd1306_requires}
                                ${flash_requires}
                       )

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
