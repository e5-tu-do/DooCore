add_library(dcConfig SHARED EasyConfig.cpp EasyConfig.h Summary.cpp Summary.h)
target_link_libraries(dcConfig dcIO ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS dcConfig DESTINATION lib)
install(FILES EasyConfig.h Summary.h DESTINATION include/doocore/config)
