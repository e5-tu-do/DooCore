add_library(dcIO SHARED Progress.cpp Progress.h MsgStream.cpp MsgStream.h EasyTuple.cpp EasyTuple.h Tools.cpp Tools.h)
target_link_libraries(dcIO ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS dcIO DESTINATION lib)
install(FILES Progress.h MsgStream.h EasyTuple.h Tools.h DESTINATION include/doocore/io)

