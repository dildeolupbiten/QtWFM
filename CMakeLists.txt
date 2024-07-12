cmake_minimum_required(VERSION 3.5)

project(QtWFM VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(Qt6_DIR "C:/Qt6.7.1/lib/cmake/Qt6")
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/Qt6.7.1")
    set(Qt6Widgets_DIR "C:/Qt6.7.1/lib/cmake/Qt6Widgets")
elseif(UNIX)
    set(Qt6_DIR /opt/Qt6.7.1/lib/cmake/Qt6)
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /opt/Qt6.7.1)
    set(Qt6Widgets_DIR /opt/Qt6.7.1/lib/cmake/Qt6Widgets)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/activities.cpp
        src/analysis.cpp
        src/analysis_results.cpp
        src/analysis_worker.cpp
        src/break_plan.cpp
        src/break_plan_results.cpp
        src/break_plan_worker.cpp
        src/consts.cpp
        src/datetime.cpp
        src/inputs.cpp
        src/main.cpp
        src/msgbox.cpp
        src/results.cpp
        src/shifts.cpp
        src/shift_plan.cpp
        src/shift_plan_results.cpp
        src/shift_plan_worker.cpp
        src/table.cpp
        src/trend.cpp
        src/utils.cpp
        src/widget.cpp
        include/activities.hpp
        include/analysis.hpp
        include/analysis_results.hpp
        include/analysis_worker.hpp
        include/break_plan.hpp
        include/break_plan_results.hpp
        include/break_plan_worker.hpp
        include/consts.hpp
        include/datetime.hpp
        include/inputs.hpp
        include/msgbox.hpp
        include/lib.hpp
        include/results.hpp
        include/shifts.hpp
        include/shift_plan.hpp
        include/shift_plan_results.hpp
        include/shift_plan_worker.hpp
        include/table.hpp
        include/trend.hpp
        include/utils.hpp
        include/widget.hpp
        lib/rapidxml/rapidxml.hpp
        lib/rapidxml/rapidxml_iterators.hpp
        lib/rapidxml/rapidxml_print.hpp
        lib/rapidxml/rapidxml_utils.hpp
        qss/resources.qrc
)

qt_add_executable(QtWFM
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

set_target_properties(QtWFM PROPERTIES
    LINK_FLAGS "-static-libgcc -static-libstdc++"
)

target_link_libraries(QtWFM PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(QtWFM PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS QtWFM
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QtWFM)
endif()
