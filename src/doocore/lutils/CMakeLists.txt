add_library(LUtils SHARED lutils.cpp lutils.h)
target_link_libraries(LUtils ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS LUtils DESTINATION lib)
install(FILES lutils.h DESTINATION include/doocore/lutils)

