idf_component_register(SRC_DIRS  "."
                    INCLUDE_DIRS "include"
                    REQUIRES EVE EW2024_Photobooth_Utils fatfs vfs sdmmc)

# see: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
target_compile_options(${COMPONENT_LIB} PUBLIC -Wno-error -Wno-format -Wno-pointer-sign  -w -Wunused-function)
