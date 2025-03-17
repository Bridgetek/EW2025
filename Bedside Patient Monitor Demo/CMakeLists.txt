
FILE(GLOB SRCS ../../common/application/*.c Src/*.c)
FILE(GLOB HDRS ../../common/application/*.h Hdr/*.h)
INCLUDE_DIRECTORIES("Hdr")

get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
string(FIND "${CMAKE_CURRENT_LIST_DIR}" "SampleApp" is_sampleapp )

if(${is_sampleapp} GREATER -1)
    SET(ProjectId "SampleApp_${ProjectId}" )
ENDIF()

MESSAGE(STATUS "Configuring target: ${ProjectId}")

project(${ProjectId} C CXX)

ADD_EXECUTABLE(${ProjectId} ${SRCS} ${HDRS})

# Deployment ###################################################################
SET(path_deploy "${CMAKE_BINARY_DIR}/deploy/${ProjectId}")
SET(path_exe "${path_deploy}/executable" )
SET(path_test "${CMAKE_CURRENT_SOURCE_DIR}/Test" )

IF (WIN32)
    SET(exe_filename "${ProjectId}.exe")
    SET(path_exe "${path_deploy}/executable/1/2" ) # it need to access ../../../Test/image...
ENDIF()

add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${path_deploy}" )
add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${path_exe}" )

if(EXISTS "${path_test}")
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${path_test}" "${path_deploy}/Test" )
    if(EXISTS "${path_test}/Flash/flash.bin" AND NOT EXISTS "${path_exe}/__Flash.bin")
        add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E rename  "${path_deploy}/Test/Flash/flash.bin" "${path_exe}/__Flash.bin")
    endif()
endif()

add_custom_command(TARGET "${ProjectId}"  POST_BUILD  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${ProjectId}> "${path_exe}/${exe_filename}")    
  
IF (WIN32)
  IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/MSVC/x64/libMPSSE.dll "${path_exe}")
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/MSVC/x64/LibFT4222-64.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/MSVC/x64/ftd2xx.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/Simulation/x64/bt8xxemu.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/Simulation/x64/mx25lemu.dll "${path_exe}")   
  ELSE()
      
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/MSVC/x86/libMPSSE.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/MSVC/x86/LibFT4222.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/MSVC/x86/ftd2xx.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/Simulation/x86/bt8xxemu.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/Simulation/x86/mx25lemu.dll "${path_exe}")   
  ENDIF()
ENDIF()


