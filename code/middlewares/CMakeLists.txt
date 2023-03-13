#zlog
set(zlog_source 
"zlog/src/source/zlog.c"
"zlog/src/source/zlog_utils.c"
"zlog/src/source/zlog_buf.c"
"zlog/src/source/zlog_async.c"
"zlog/zlog_ports.c"
)
set(zlog_include 
"zlog/src/include"
"zlog"
)
set(zlog_requires 
driver
)

#tinymaix
set(tinymaix_source 
"third_party/tinymaix/src/src/tm_stat.c"
"third_party/tinymaix/src/src/tm_model.c"
"third_party/tinymaix/src/src/tm_layers.c"
"third_party/tinymaix/src/src/tm_layers_O1.c"
"third_party/tinymaix/src/src/tm_layers_fp8.c"
"third_party/tinymaix/src/demo/mnist.c"
)
set(tinymaix_include 
"third_party/tinymaix/src/src"
"third_party/tinymaix/src/include"
"third_party/tinymaix/src/tmdl"
"third_party/tinymaix/src/demo"
)

#dsp
set(dsp_source            
"third_party/esp_dsp/src/modules/common/misc/dsps_pwroftwo.cpp"
"third_party/esp_dsp/src/modules/common/misc/aes3_tie_log.c" 
"third_party/esp_dsp/src/modules/dotprod/float/dsps_dotprod_f32_ae32.S"
"third_party/esp_dsp/src/modules/dotprod/float/dsps_dotprod_f32_m_ae32.S"
"third_party/esp_dsp/src/modules/dotprod/float/dsps_dotprode_f32_ae32.S"
"third_party/esp_dsp/src/modules/dotprod/float/dsps_dotprode_f32_m_ae32.S"
"third_party/esp_dsp/src/modules/dotprod/float/dsps_dotprod_f32_ansi.c"
"third_party/esp_dsp/src/modules/dotprod/float/dsps_dotprode_f32_ansi.c"
"third_party/esp_dsp/src/modules/dotprod/float/dsps_dotprod_f32_aes3.S"

"third_party/esp_dsp/src/modules/dotprod/fixed/dsps_dotprod_s16_ae32.S"
"third_party/esp_dsp/src/modules/dotprod/fixed/dsps_dotprod_s16_m_ae32.S"
"third_party/esp_dsp/src/modules/dotprod/fixed/dsps_dotprod_s16_ansi.c"

"third_party/esp_dsp/src/modules/dotprod/float/dspi_dotprod_f32_ansi.c"
"third_party/esp_dsp/src/modules/dotprod/float/dspi_dotprod_off_f32_ansi.c"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_s16_ansi.c"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_u16_ansi.c"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_s8_ansi.c"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_u8_ansi.c"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_off_s16_ansi.c"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_off_u16_ansi.c"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_off_s8_ansi.c"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_off_u8_ansi.c"

"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_s16_aes3.S"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_u16_aes3.S"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_off_s16_aes3.S"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_off_u16_aes3.S"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_s8_aes3.S"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_u8_aes3.S"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_off_u8_aes3.S"
"third_party/esp_dsp/src/modules/dotprod/fixed/dspi_dotprod_off_s8_aes3.S"
                    
"third_party/esp_dsp/src/modules/matrix/float/dspm_mult_3x3x1_f32_ae32.S"
"third_party/esp_dsp/src/modules/matrix/float/dspm_mult_3x3x3_f32_ae32.S"
"third_party/esp_dsp/src/modules/matrix/float/dspm_mult_4x4x1_f32_ae32.S"
"third_party/esp_dsp/src/modules/matrix/float/dspm_mult_4x4x4_f32_ae32.S"
"third_party/esp_dsp/src/modules/matrix/float/dspm_mult_f32_ae32.S"
"third_party/esp_dsp/src/modules/matrix/float/dspm_mult_f32_aes3.S"
"third_party/esp_dsp/src/modules/matrix/float/dspm_mult_f32_ansi.c"
"third_party/esp_dsp/src/modules/matrix/fixed/dspm_mult_s16_ae32.S"
"third_party/esp_dsp/src/modules/matrix/fixed/dspm_mult_s16_m_ae32_vector.S"
"third_party/esp_dsp/src/modules/matrix/fixed/dspm_mult_s16_m_ae32.S"
"third_party/esp_dsp/src/modules/matrix/fixed/dspm_mult_s16_ansi.c"
"third_party/esp_dsp/src/modules/matrix/fixed/dspm_mult_s16_aes3.S"
"third_party/esp_dsp/src/modules/matrix/mat/mat.cpp"
"third_party/esp_dsp/src/modules/math/mulc/float/dsps_mulc_f32_ansi.c"
"third_party/esp_dsp/src/modules/math/addc/float/dsps_addc_f32_ansi.c"
"third_party/esp_dsp/src/modules/math/mulc/fixed/dsps_mulc_s16_ansi.c"
"third_party/esp_dsp/src/modules/math/mulc/fixed/dsps_mulc_s16_ae32.S"
"third_party/esp_dsp/src/modules/math/add/float/dsps_add_f32_ansi.c"
"third_party/esp_dsp/src/modules/math/add/fixed/dsps_add_s16_ansi.c"
"third_party/esp_dsp/src/modules/math/add/fixed/dsps_add_s16_ae32.S"
"third_party/esp_dsp/src/modules/math/sub/float/dsps_sub_f32_ansi.c"
"third_party/esp_dsp/src/modules/math/mul/float/dsps_mul_f32_ansi.c"
"third_party/esp_dsp/src/modules/math/mul/fixed/dsps_mul_s16_ansi.c"
"third_party/esp_dsp/src/modules/math/mulc/float/dsps_mulc_f32_ae32.S"
"third_party/esp_dsp/src/modules/math/addc/float/dsps_addc_f32_ae32.S"
"third_party/esp_dsp/src/modules/math/add/float/dsps_add_f32_ae32.S"
"third_party/esp_dsp/src/modules/math/sub/float/dsps_sub_f32_ae32.S"
"third_party/esp_dsp/src/modules/math/mul/float/dsps_mul_f32_ae32.S"
"third_party/esp_dsp/src/modules/math/sqrt/float/dsps_sqrt_f32_ansi.c"

"third_party/esp_dsp/src/modules/fft/float/dsps_fft2r_fc32_ae32_.S"
"third_party/esp_dsp/src/modules/fft/float/dsps_fft2r_fc32_aes3_.S"
"third_party/esp_dsp/src/modules/fft/float/dsps_fft2r_fc32_ansi.c"
"third_party/esp_dsp/src/modules/fft/float/dsps_fft2r_fc32_ae32.c"
"third_party/esp_dsp/src/modules/fft/float/dsps_bit_rev_lookup_fc32_aes3.S"
"third_party/esp_dsp/src/modules/fft/float/dsps_fft4r_fc32_ansi.c"
"third_party/esp_dsp/src/modules/fft/float/dsps_fft4r_fc32_ae32.c"
"third_party/esp_dsp/src/modules/fft/float/dsps_fft2r_bitrev_tables_fc32.c"
"third_party/esp_dsp/src/modules/fft/float/dsps_fft4r_bitrev_tables_fc32.c"
"third_party/esp_dsp/src/modules/fft/fixed/dsps_fft2r_sc16_ae32.S"
"third_party/esp_dsp/src/modules/fft/fixed/dsps_fft2r_sc16_ansi.c"
"third_party/esp_dsp/src/modules/fft/fixed/dsps_fft2r_sc16_aes3.S"
                    
"third_party/esp_dsp/src/modules/dct/float/dsps_dct_f32.c"
"third_party/esp_dsp/src/modules/support/snr/float/dsps_snr_f32.cpp"
"third_party/esp_dsp/src/modules/support/sfdr/float/dsps_sfdr_f32.cpp"
"third_party/esp_dsp/src/modules/support/misc/dsps_d_gen.c"
"third_party/esp_dsp/src/modules/support/misc/dsps_h_gen.c"
"third_party/esp_dsp/src/modules/support/misc/dsps_tone_gen.c"
"third_party/esp_dsp/src/modules/support/view/dsps_view.cpp"
"third_party/esp_dsp/src/modules/windows/hann/float/dsps_wind_hann_f32.c"
"third_party/esp_dsp/src/modules/windows/blackman/float/dsps_wind_blackman_f32.c"
"third_party/esp_dsp/src/modules/windows/blackman_harris/float/dsps_wind_blackman_harris_f32.c"
"third_party/esp_dsp/src/modules/windows/blackman_nuttall/float/dsps_wind_blackman_nuttall_f32.c"
"third_party/esp_dsp/src/modules/windows/nuttall/float/dsps_wind_nuttall_f32.c"
"third_party/esp_dsp/src/modules/windows/flat_top/float/dsps_wind_flat_top_f32.c"
"third_party/esp_dsp/src/modules/conv/float/dsps_conv_f32_ansi.c"
"third_party/esp_dsp/src/modules/conv/float/dsps_conv_f32_ae32.S"
"third_party/esp_dsp/src/modules/conv/float/dsps_corr_f32_ansi.c"
"third_party/esp_dsp/src/modules/conv/float/dsps_corr_f32_ae32.S"
"third_party/esp_dsp/src/modules/conv/float/dsps_ccorr_f32_ansi.c"
"third_party/esp_dsp/src/modules/conv/float/dsps_ccorr_f32_ae32.S"
"third_party/esp_dsp/src/modules/iir/biquad/dsps_biquad_f32_ae32.S"
"third_party/esp_dsp/src/modules/iir/biquad/dsps_biquad_f32_ansi.c"
"third_party/esp_dsp/src/modules/iir/biquad/dsps_biquad_gen_f32.c"
"third_party/esp_dsp/src/modules/fir/float/dsps_fir_f32_ae32.S"
"third_party/esp_dsp/src/modules/fir/float/dsps_fird_f32_ae32.S"
"third_party/esp_dsp/src/modules/fir/float/dsps_fir_f32_ansi.c"
"third_party/esp_dsp/src/modules/fir/float/dsps_fir_init_f32.c"
"third_party/esp_dsp/src/modules/fir/float/dsps_fird_f32_ansi.c"
"third_party/esp_dsp/src/modules/fir/float/dsps_fird_init_f32.c"
"third_party/esp_dsp/src/modules/fir/fixed/dsps_fird_init_s16.c"
"third_party/esp_dsp/src/modules/fir/fixed/dsps_fird_s16_ansi.c"
"third_party/esp_dsp/src/modules/fir/fixed/dsps_fird_s16_ae32.S"
"third_party/esp_dsp/src/modules/fir/fixed/dsps_fir_s16_m_ae32.S"
"third_party/esp_dsp/src/modules/fir/fixed/dsps_fird_s16_aes3.S"
# EKF files
"third_party/esp_dsp/src/modules/kalman/ekf/common/ekf.cpp"
"third_party/esp_dsp/src/modules/kalman/ekf_imu13states/ekf_imu13states.cpp"
)

set(dsp_include                
"third_party/esp_dsp/src/modules/dotprod/include"
"third_party/esp_dsp/src/modules/support/include"
"third_party/esp_dsp/src/modules/windows/include"
"third_party/esp_dsp/src/modules/windows/hann/include"
"third_party/esp_dsp/src/modules/windows/blackman/include"
"third_party/esp_dsp/src/modules/windows/blackman_harris/include"
"third_party/esp_dsp/src/modules/windows/blackman_nuttall/include"
"third_party/esp_dsp/src/modules/windows/nuttall/include"
"third_party/esp_dsp/src/modules/windows/flat_top/include"
"third_party/esp_dsp/src/modules/iir/include"
"third_party/esp_dsp/src/modules/fir/include"
"third_party/esp_dsp/src/modules/math/include"
"third_party/esp_dsp/src/modules/math/add/include"
"third_party/esp_dsp/src/modules/math/sub/include"
"third_party/esp_dsp/src/modules/math/mul/include"
"third_party/esp_dsp/src/modules/math/addc/include"
"third_party/esp_dsp/src/modules/math/mulc/include"
"third_party/esp_dsp/src/modules/math/sqrt/include"
"third_party/esp_dsp/src/modules/matrix/include"
"third_party/esp_dsp/src/modules/fft/include"
"third_party/esp_dsp/src/modules/dct/include"
"third_party/esp_dsp/src/modules/conv/include"
"third_party/esp_dsp/src/modules/common/include"
# EKF files
"third_party/esp_dsp/src/modules/kalman/ekf/include"
"third_party/esp_dsp/src/modules/kalman/ekf_imu13states/include"
)

set(dsp_priv_include_dirs
"third_party/esp_dsp/src/modules/dotprod/float"
"third_party/esp_dsp/src/modules/dotprod/fixed"
)


idf_component_register(SRCS ${zlog_source}
                            ${tinymaix_source}
                            ${dsp_source}
                       INCLUDE_DIRS ${zlog_include}
                                    ${tinymaix_include}
                                    ${dsp_include}
                       REQUIRES ${zlog_requires}
                       PRIV_INCLUDE_DIRS ${dsp_priv_include_dirs}
                      )
target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")


set_source_files_properties(${tinymaix_source}
        PROPERTIES COMPILE_FLAGS
        -Wno-unused-variable
        -Wmultichar
        -Wunused-but-set-variable
    )
set_source_files_properties(${tinymaix_include}
    PROPERTIES COMPILE_FLAGS
    -Wno-unused-variable
    -Wmultichar
    -Wunused-but-set-variable
)