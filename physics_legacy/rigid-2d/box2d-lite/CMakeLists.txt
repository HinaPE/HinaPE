set(BOX2D_HEADER_FILES
        Arbiter.h
        Body.h
        Joint.h
        MathUtils.h
        World.h
        )

set(BOX2D_SOURCE_FILES
        Arbiter.cpp
        Body.cpp
        Collide.cpp
        Joint.cpp
        World.cpp
        )

add_library(box2d-lite STATIC ${BOX2D_SOURCE_FILES} ${BOX2D_HEADER_FILES})
target_include_directories(box2d-lite PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)