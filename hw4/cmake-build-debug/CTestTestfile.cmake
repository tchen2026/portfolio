# CMake generated Testfile for 
# Source directory: /Users/tiffanychen/Desktop/cs 211/hw4
# Build directory: /Users/tiffanychen/Desktop/cs 211/hw4/cmake-build-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Test_ball_test "ball_test")
set_tests_properties(Test_ball_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/tiffanychen/Desktop/cs 211/hw4/.cs211/cmake/211commands.cmake;90;add_test;/Users/tiffanychen/Desktop/cs 211/hw4/CMakeLists.txt;23;add_test_program;/Users/tiffanychen/Desktop/cs 211/hw4/CMakeLists.txt;0;")
add_test(Test_model_test "model_test")
set_tests_properties(Test_model_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/tiffanychen/Desktop/cs 211/hw4/.cs211/cmake/211commands.cmake;90;add_test;/Users/tiffanychen/Desktop/cs 211/hw4/CMakeLists.txt;30;add_test_program;/Users/tiffanychen/Desktop/cs 211/hw4/CMakeLists.txt;0;")
subdirs(".cs211/lib/catch")
subdirs(".cs211/lib/ge211")
