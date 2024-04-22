cmake_minimum_required(VERSION 3.1...3.29) # Randomly picking lowest version I think might work


project(blitzENGINE
		VERSION 0.9.0
		DESCRIPTION "A cross-platform 2D game engine"
		LANGUAGES CXX)
		

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include_directories(${PROJECT_SOURCE_DIR}/include)


add_executable(blitz_engine)
