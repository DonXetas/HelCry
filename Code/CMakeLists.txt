cmake_minimum_required (VERSION 3.6.0)
project (Game VERSION 1.0 LANGUAGES CXX)

if (${CMAKE_GENERATOR} MATCHES "^Visual Studio")
	# Set startup project to launch Game.exe with this project
	configure_file ("Game.vcxproj.user.in" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vcxproj.user")
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

option(OPTION_PCH "Enable Precompiled headers" ON)

find_package (CryCommon REQUIRED)
find_package (CryAction REQUIRED)

set (SourceGroup_PluginMain
	"StdAfx.cpp"
	"StdAfx.h"
	"GamePlugin.cpp"
	"GamePlugin.h"
)
source_group("Main" FILES ${SourceGroup_PluginMain})

set (SourceGroup_EntityHelpers
	"Entities/Helpers/ISimpleExtension.h"
)
source_group("Entities" FILES ${SourceGroup_EntityHelpers})

set (SourceGroup_GameRules
	"GameRules/GameRules.h"
	"GameRules/GameRules.cpp"
)
source_group("GameRules" FILES ${SourceGroup_GameRules})


set (SourceGroup_Player
	"Player/ISimpleActor.h"
)
source_group("Player" FILES ${SourceGroup_Player})

set (SOURCE
	${SourceGroup_PluginMain}
	${SourceGroup_EntityHelpers}
	${SourceGroup_Player}
	${SourceGroup_GameRules}
)

if(OPTION_PCH AND MSVC)
	set (PRECOMPILED_SOURCE ${SOURCE})
	list(REMOVE_ITEM PRECOMPILED_SOURCE "StdAfx.cpp")

	set (PRECOMPILED_BINARY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pch)
	set_source_files_properties (${PRECOMPILED_SOURCE} PROPERTIES COMPILE_FLAGS "/Yu\"StdAfx.h\" /Fp\"${PRECOMPILED_BINARY}\"" OBJECT_DEPENDS "${PRECOMPILED_BINARY}")
	set_source_files_properties (StdAfx.cpp PROPERTIES COMPILE_FLAGS "/Yc\"StdAfx.h\" /Fp\"${PRECOMPILED_BINARY}\"" OBJECT_OUTPUTS "${PRECOMPILED_BINARY}")
endif (OPTION_PCH AND MSVC)

add_library(${PROJECT_NAME} MODULE ${SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE CryCommon CryAction)

if (WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE Ntdll)
endif(WIN32)