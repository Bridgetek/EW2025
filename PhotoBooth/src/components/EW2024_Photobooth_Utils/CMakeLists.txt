idf_component_register(SRC_DIRS  "."
                    INCLUDE_DIRS "include"
                    INCLUDE_DIRS "."
                    REQUIRES EVE
                    )
# see: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
target_compile_options(${COMPONENT_LIB} PUBLIC -Wno-error -Wno-format -w)
                    