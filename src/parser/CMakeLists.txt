add_library(parser 
  ast.cpp
  ast_printer.cpp
  parser.cpp
)

target_include_directories(parser PUBLIC .)
target_link_libraries(parser PRIVATE lexer)
