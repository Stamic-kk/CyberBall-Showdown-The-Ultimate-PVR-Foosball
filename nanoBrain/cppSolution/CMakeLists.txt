# Copyright (c) 2019, NVIDIA CORPORATION. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  * Neither the name of NVIDIA CORPORATION nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.5)

project(max_min_loc_example)

find_package(vpi REQUIRED)
find_package(OpenCV REQUIRED)



add_executable(${PROJECT_NAME}.out src/main.cpp src/locator.cpp src/predictor.cpp src/camera.cpp)
target_include_directories(${PROJECT_NAME}.out PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}.out PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/uKal/src/)
target_include_directories(${PROJECT_NAME}.out PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/uKal/src/uLAPack/src)
#target_link_libraries(${PROJECT_NAME}.out vpi opencv_core opencv_imgproc opencv_imgcodecs opencv_videoio stdc++fs)
# add_definitions(-ULAPACK_USE_STATIC_ALLOC)
# target_compile_definitions(${PROJECT_NAME}.out PUBLIC ULAPACK_USE_STATIC_ALLOC)
target_link_libraries(${PROJECT_NAME}.out vpi ${OpenCV_LIBS}  stdc++fs)