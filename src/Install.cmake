include(GNUInstallDirs)

install(TARGETS hexamine
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# For std::filesystem support used by blockplot
set_target_properties(hexamine PROPERTIES CXX_STANDARD 20)

# Installing is not easy, when we're dealing with shared libs
if(NOT LINK_DEPS_STATIC)
  set_target_properties(hexamine PROPERTIES
    INSTALL_RPATH $ORIGIN/../${CMAKE_INSTALL_LIBDIR}
  )

  set_target_properties(
    ImGui-SFML
    sfml-graphics sfml-system sfml-window
    PROPERTIES
    INSTALL_RPATH $ORIGIN
  )

  install(TARGETS
    ImGui-SFML
    sfml-graphics sfml-system sfml-window
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    NAMELINK_SKIP # don't need versionless .so's
  )
endif()

install(SCRIPT PostInstall.cmake)

include(CPack)
