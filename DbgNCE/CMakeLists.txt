add_executable(dbg_alice WIN32 "${PROJECT_SOURCE_DIR}/DbgNCE/dbg_alice_main.cpp")

target_link_libraries(dbg_alice PRIVATE NCECommon)
target_link_libraries(dbg_alice PRIVATE dbghelp)
target_precompile_headers(dbg_alice REUSE_FROM NCE)
