add_library(dcStatistics SHARED general.h general.cpp
montecarlo/ErrorEstimator.h montecarlo/ErrorEstimator.cpp)
target_link_libraries(dcStatistics ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS dcStatistics DESTINATION lib)
install(FILES general.h DESTINATION include/doocore/statistics)
install(FILES montecarlo/ErrorEstimator.h DESTINATION
include/doocore/statistics/montecarlo)


