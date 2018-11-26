#---Make all the executables for data conversion
add_executable(urqmd2tc urqmd2tc.cxx)
target_link_libraries(urqmd2tc ModelTree)

add_executable(uioqgsm2tc uioqgsm2tc.cxx)
target_link_libraries(uioqgsm2tc ModelTree)