add_library(IO SHARED MsgStream.cpp MsgStream.h)
target_link_libraries(IO ${ROOT_LIBRARIES} ${ROOFIT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS IO DESTINATION lib)
install(FILES MsgStream.h DESTINATION include/doocore/io)

