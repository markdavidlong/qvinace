
include_directories(
		${CMAKE_CURRENT_SOURCE_DIR}
 #  ${GTKMM_INCLUDE_DIRS}
)

add_subdirectory(core)
add_subdirectory(hardware)
add_subdirectory(gui)

set(VINACE_SOURCES 
	${CORE_SOURCE_FILES}
	${HARDWARE_SOURCE_FILES}
    ${GUI_SOURCE_FILES}
    main.cxx
    mainwindow.cxx
    mainwindow.h
    mainwindow.ui
)


add_executable(qvinace ${VINACE_SOURCES} )

target_link_libraries(qvinace Qt6::Widgets Qt6::Core Qt6::Quick Qt6::QuickWidgets)

set_target_properties(qvinace PROPERTIES AUTOMOC ON AUTORCC ON)
set_target_properties(qvinace PROPERTIES AUTOUIC ON)


