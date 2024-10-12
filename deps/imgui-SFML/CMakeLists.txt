add_library(imgui_sfml
    imgui-SFML.cpp
)


target_link_libraries(imgui_sfml 
    ${CONAN_LIBS}
)