cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project (Tetris)
set(
	HEADERS
	"TetrominoCor/TetrominoCor.h"
	"Dictionary/Dictionary.h"
	"TetrominoColor/TetrominoColor.h"
	"TetrominoWidth/TetrominoWidth.h"
	"Tetris/Tetris.h"
	"TetrisMap/TetrisMap.h"
)
set(
	SOURCE_CPP
	"Tetris/Tetris.cpp"
)
set(
	SOURCE_EXE
	"main.cpp"
)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_EXE} ${HEADERS} ${SOURCE_CPP})
set(SFML_STATIC_LIBRARIES TRUE)				
set(SFML_DIR "C:\\Libs\\SFML-x64\\SFML-2.5.1\\lib\\cmake\\SFML")
find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} sfml-system sfml-window sfml-graphics)
