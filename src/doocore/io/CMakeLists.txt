add_library(IO SHARED MsgStream.cpp MsgStream.h EasyTuple.cpp EasyTuple.h)
target_link_libraries(IO ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS IO DESTINATION lib)
install(FILES MsgStream.h EasyTuple.h DESTINATION include/doocore/io)

