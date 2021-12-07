cmake_minimum_required(VERSION 3.12)
project(Final_Project)

include_directories(include)
link_directories(lib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set_source_files_properties(filename.c PROPERTIES LANGUAGE CXX )

add_executable(Final_Project main.cpp EditImage.cpp EditImage.h Crop.h PNGChunk.h upng.cpp upng.h lodepng.cpp lodepng.h Button.cpp Button.h)
target_link_libraries(Final_Project sfml-audio sfml-graphics sfml-system sfml-window sfml-network sfml-main)

# Copy single files
macro(resource_files files)
    foreach(file ${files})
        message(STATUS "  Copying resource ${file} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
        file(COPY ${file} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endforeach()
endmacro()

# Copy full directories
macro(resource_dirs dirs)
    foreach(dir ${dirs})
        # Replace / at the end of the path (copy dir content VS copy dir)
        string(REGEX REPLACE "/+$" "" dirclean "${dir}")
        file(GLOB files "${dirclean}/*.dll")
        message(STATUS "Copying directory ${dirclean}")
        foreach(file ${files})
            resource_files(${file})
        endforeach()
        message(STATUS "Done copying directory ${dirclean}")
    endforeach()
endmacro()

# Copy single files
macro(data_files files)
    foreach(file ${files})
        message(STATUS "  Copying resource ${file} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data")
        file(COPY ${file} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data)
    endforeach()
endmacro()

# Copy full directories
macro(data_dirs dirs)
    foreach(dir ${dirs})
        # Replace / at the end of the path (copy dir content VS copy dir)
        string(REGEX REPLACE "/+$" "" dirclean "${dir}")
        file(GLOB files "${dirclean}/*")
        message(STATUS "Copying directory ${dirclean}")
        foreach(file ${files})
            data_files(${file})
        endforeach()
        message(STATUS "Done copying directory ${dirclean}")
    endforeach()
endmacro()

resource_dirs(bin/)
data_dirs(data/)
