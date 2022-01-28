## set target project
file(GLOB target_src "*.h" "*.cpp") # look for source files
file(GLOB target_shaders "shaders/*.vert" "shaders/*.frag") # look for shaders
add_executable(${subdir} ${target_src} ${target_shaders})

## set link libraries
target_link_libraries(${subdir} ${libraries})

## add local source directory to include paths
target_include_directories(${subdir} PUBLIC ../../exercises/exercise_12/exercise_12_1_to_12_3)

## copy shaders folder to build folder
file(COPY ../../exercises/exercise_12/exercise_12_1_to_12_3/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

## copy models / textures
file(COPY ${CMAKE_SOURCE_DIR}/common/models/car DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/common/models/skybox DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/common/models/floor DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

## copy again at the time the current target gets compiled
add_custom_command(
        TARGET ${subdir} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ../../exercises/exercise_12/exercise_12_1_to_12_3/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMENT "Copying shaders" VERBATIM
)