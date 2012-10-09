MESSAGE(STATUS "Looking for DooCore...")

SET(DooCore_FOUND FALSE)

IF(IS_DIRECTORY $ENV{DOOCORESYS})
  SET(DooCore_BASE $ENV{DOOCORESYS})
  SET(DooCore_FOUND TRUE)
  SET(DooCore_INCLUDES "${DooCore_BASE}/include")
  SET(DooCore_INCLUDE_DIR "${DooCore_BASE}/include")
  SET(DooCore_LIBRARY_DIR "${DooCore_BASE}/lib")
  SET(DooCore_LIBRARIES "-lToy -lPdf2WsStd -lConfig -lUtils")
  #SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${DooCore_LIBRARY_DIR})
  MESSAGE(STATUS "Looking for DooFit... - found at ${DooCore_BASE}")
ENDIF(IS_DIRECTORY $ENV{DOOCORESYS})
