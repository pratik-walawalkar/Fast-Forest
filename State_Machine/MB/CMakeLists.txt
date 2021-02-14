file(GLOB_RECURSE MAIN_SOURCES LIST_DIRECTORIES false *.h *.c)

target_sources(${PROJECT_NAME} 
                PUBLIC ${MAIN_SOURCES})

target_include_directories(${PROJECT_NAME}
                PUBLIC inc)

file(GLOB_RECURSE INCs LIST_DIRECTORIES true *.h)

set(incList "")
foreach(inc ${INCs})
    if(IS_DIRECTORY ${inc})
        if(${inc} MATCHES .*inc)
            target_include_directories(${PROJECT_NAME} PUBLIC ${inc})
            message(${inc})
        endif()
    endif()
endforeach()

    

