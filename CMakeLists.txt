
if(CONFIG_ZCBOR)

zephyr_include_directories(
    ./include
    )

zephyr_sources(
    ./src/zcbor_encode.c
    ./src/zcbor_decode.c
    ./src/zcbor_common.c
    )

zephyr_sources_ifdef(CONFIG_ZCBOR_MGMT
    ./src/zcbor_mgmt.c
    ./src/error_rsp.c
    )

endif()