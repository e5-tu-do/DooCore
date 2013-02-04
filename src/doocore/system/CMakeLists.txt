add_library(dcSystem SHARED FileLock.cpp FileLock.h Tools.cpp Tools.h)

target_link_libraries(dcSystem dcLUtils ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS dcSystem DESTINATION lib)
install(FILES FileLock.h Tools.h DESTINATION include/doocore/system)

