add_library(dcStatistics SHARED general.h general.cpp)
target_link_libraries(dcStatistics ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS dcStatistics DESTINATION lib)
install(FILES general.h DESTINATION include/doocore/statistics)

