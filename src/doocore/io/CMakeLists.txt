add_library(dcIO SHARED MsgStream.cpp MsgStream.h EasyTuple.cpp EasyTuple.h)
target_link_libraries(dcIO ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS dcIO DESTINATION lib)
install(FILES MsgStream.h EasyTuple.h DESTINATION include/doocore/io)

