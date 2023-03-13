#oled_ssd1306
set(oled_ssd1306_source 
"oled/ssd1306/oled_ssd1306.c"
"oled/ssd1306/oled_ssd1306_spi_port.c"
"oled/ssd1306/oled_ssd1306_i2c_port.c"
)
set(oled_ssd1306_include 
"oled/ssd1306"
)
set(oled_ssd1306_requires 
drivers
)

#lcd_ili9341
set(lcd_ili9341_source 
"lcd/ili9341/lcd_ili9341.c"
"lcd/ili9341/lcd_ili9341_port.c"
)
set(lcd_ili9341_include 
"lcd/ili9341"
)
set(lcd_ili9341_requires 
drivers
)


idf_component_register(SRCS ${oled_ssd1306_source}
                            ${lcd_ili9341_source}
                       INCLUDE_DIRS ${oled_ssd1306_include}
                                    ${lcd_ili9341_include}
                       REQUIRES ${oled_ssd1306_requires}
                                ${lcd_ili9341_requires}
                       )

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
