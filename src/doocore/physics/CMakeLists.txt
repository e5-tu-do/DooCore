add_library(dcPhysics SHARED kinematic/kinematic.h kinematic/kinematic.cpp)
target_link_libraries(dcPhysics ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS dcPhysics DESTINATION lib)
install(FILES kinematic/kinematic.h DESTINATION include/doocore/physics/kinematic)

