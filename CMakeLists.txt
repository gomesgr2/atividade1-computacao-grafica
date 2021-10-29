project(my_game)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp person.cpp balls.cpp)

enable_abcg(${PROJECT_NAME})
