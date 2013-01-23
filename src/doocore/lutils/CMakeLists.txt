add_library(dcLUtils SHARED lutils.cpp lutils.h)
target_link_libraries(dcLUtils dcIO ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS dcLUtils DESTINATION lib)
install(FILES lutils.h DESTINATION include/doocore/lutils)

