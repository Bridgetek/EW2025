set(srcs "main.c")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "."
                    REQUIRES EVE EW2024_Photobooth_Utils EW2024_Photobooth)
