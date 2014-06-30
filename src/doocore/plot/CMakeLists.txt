add_library(dcPlot SHARED styles.h styles.cpp)
target_link_libraries(dcPlot ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS dcPlot DESTINATION lib)
install(FILES styles.h DESTINATION include/doocore/plot)


