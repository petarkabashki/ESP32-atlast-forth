idf_component_register(SRCS "atlast.c" 
"atl_primitives/crypto_atl.c" 
"atl_primitives/u8g2_atl.c"
"atl_primitives/system_atl.c"

                    INCLUDE_DIRS . "atl_primitives"
                    REQUIRES freertos u8g2 crypto )