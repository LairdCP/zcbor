
if(CONFIG_ZCBOR)

zephyr_include_directories(
    ./include
    )

zephyr_sources(
    ./src/zcbor_encode.c
    ./src/zcbor_decode.c
    ./src/zcbor_common.c
    )

endif()