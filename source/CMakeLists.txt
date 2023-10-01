

set(SRC

    Main.cpp

    BotNeuralNet.cpp
    BotNeuralNet.h

    SDL.cpp
    SDL.h

    GUI.cpp
    GUI.h

    Settings.h
    Systems.h

    logic/BotNeuralNet.cpp
    logic/BotNeuralNet.h

    logic/NeuralNetRenderer.cpp
    logic/NeuralNetRenderer.h

    logic/Neuron.cpp
    logic/Neuron.h

    Simulation.cpp
    Simulation.h

    renderer/EnumRenderTypes.h
    renderer/EnumRenderTypes.cpp

    renderer/WorldRenderer.h
    renderer/WorldRenderer.cpp

    logic/WorldController.h
    logic/WorldController.cpp

    entity/Bot.cpp
    entity/Bot.h

    entity/Object.cpp
    entity/Object.h

    entity/Organics.cpp
    entity/Organics.h

    entity/Rock.cpp
    entity/Rock.h

    world/World.h 
    world/World.cpp

    world/EnumSeason.h
    world/EnumSeason.cpp

    MyTypes.cpp
    MyTypes.h

    Settings.h
    Systems.h

    ObjectSaver.cpp
    ObjectSaver.h
)

file(GLOB_RECURSE SRC
     "*.h"
     "*.cpp"
)

find_package(OpenGL REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SRC})

target_include_directories(${CMAKE_PROJECT_NAME} 
    PUBLIC
        ${IMGui_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${CMAKE_PROJECT_NAME} 
    PUBLIC
        ${IMGui_LIBRARIES}
        ${OPENGL_LIBRARIES}
        SDL2_ttf::SDL2_ttf-static
        imgui
        implot
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
