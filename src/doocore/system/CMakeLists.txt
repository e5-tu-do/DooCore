add_library(System SHARED FileLock.cpp FileLock.h Tools.cpp Tools.h)

target_link_libraries(System LUtils IO ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS System DESTINATION lib)
install(FILES FileLock.h Tools.h DESTINATION include/doocore/system)

