
add_executable(hexamine hexamine.cpp blockplot.cpp)


# Yep, that's it!
target_link_libraries(hexamine
  PUBLIC
    ImGui-SFML::ImGui-SFML
)

include(Install.cmake)
