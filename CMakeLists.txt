cmake_minimum_required(VERSION 3.24)
project(Projet_ACL CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# Sources C de la partie Algo
set(ALGO_C_SOURCES
        Partie_Algo/code/src/list.c
        Partie_Algo/code/src/instance.c
        Partie_Algo/code/src/schedule.c
        Partie_Algo/code/src/state.c
        Partie_Algo/code/src/jackson.c
        Partie_Algo/code/src/algo_dyn.c
        Partie_Algo/code/src/fptas.c
        Partie_Algo/code/src/utils.c
)

set(SOURCE_FILES
        Partie_ACL/cpp/Outils/include/CsvOutils.h
)


add_executable(serveur
        Partie_ACL/cpp/Serveur.cpp
        ${ALGO_C_SOURCES}
)

add_custom_command(
        TARGET serveur POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Partie_ACL/Assets
        $<TARGET_FILE_DIR:serveur>/Assets
)

target_include_directories(serveur PRIVATE
        Partie_ACL
        Partie_ACL/cpp
        Partie_ACL/cpp/Outils/include
        Partie_ACL/cpp/lib
        Partie_ACL/cpp/Modeles/include
        Partie_ACL/cpp/Graphes/include
        Partie_Algo/code/include
)