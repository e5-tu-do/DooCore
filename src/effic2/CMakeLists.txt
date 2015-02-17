add_library(dcEffic2 SHARED 
effic/solvers.hpp src/solvers.cpp
effic/specfunc.hpp src/specfunc.cpp
effic/effic2.hpp src/effic2.cpp
)

target_link_libraries(dcEffic2 ${GSL_LIBRARIES})
install(TARGETS dcEffic2 DESTINATION lib)
install(FILES effic/effic2.hpp DESTINATION include/effic)


