FILE(GLOB_RECURSE SRC
        "*.h"
        "*.pb.h"
        "*.pb.cc"
        "*.cpp"
        "*.hpp"
        )

#FILE(GLOB_RECURSE THIRD_PARTY
#        )

ADD_LIBRARY(app_core ${SRC})
SET_TARGET_PROPERTIES(app_core PROPERTIES LINKER_LANGUAGE CXX)
