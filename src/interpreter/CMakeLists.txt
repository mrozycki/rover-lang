add_library(interpreter
    interpreter.cpp
)

target_include_directories(interpreter PUBLIC .)
target_link_libraries(interpreter PRIVATE lexer parser)