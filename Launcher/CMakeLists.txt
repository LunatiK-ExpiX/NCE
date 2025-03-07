add_executable(NCE_Launcher WIN32
	${ALICE_CORE_SOURCES_LIST}
	"${PROJECT_SOURCE_DIR}/src/launcher/launcher_main.cpp"
	"${PROJECT_SOURCE_DIR}/src/NCE.rc")

target_compile_definitions(NCE_Launcher PUBLIC ALICE_NO_ENTRY_POINT)

target_link_libraries(NCE_Launcher PRIVATE NCECommon)
target_link_libraries(NCE_Launcher PRIVATE dbghelp)
target_precompile_headers(NCE_Launcher REUSE_FROM NCE)
