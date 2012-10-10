add_library(System SHARED FileLock.cpp FileLock.h)
target_link_libraries(System ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS System DESTINATION lib)
install(FILES FileLock.h DESTINATION include/doocore/system)

