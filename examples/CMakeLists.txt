add_executable(bedrock_example example_usage.cpp)

target_link_libraries(bedrock_example PRIVATE bedrock_lib)

# Copiar biblioteca para diretório do executável
add_custom_command(TARGET bedrock_example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:bedrock_lib>
    $<TARGET_FILE_DIR:bedrock_example>
)
