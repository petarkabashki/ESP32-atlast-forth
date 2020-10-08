idf_component_register(SRCS "atlast.c" 
"primitives/crypto_atl.c" 
"primitives/u8g2_atl.c"
"primitives/system_atl.c"
                    INCLUDE_DIRS . "primitives"
                    REQUIRES freertos u8g2 crypto)