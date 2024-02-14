cmake_minimum_required(VERSION 3.25)
project(ParallelTarjanProject
        DESCRIPTION "Parallel implementation of Tarjan algorithm"
        VERSION 0.1
        LANGUAGES C
        )



enable_testing()

add_executable(mpi_tarjan0 src/main.c src/graph.c src/node.c src/SCC.c src/SccStack.c src/stack.c src/tarjan.c src/TArray.c src/THTopen.c  src/TInfo_KV.c)
add_executable(mpi_tarjan1 src/main.c src/graph.c src/node.c src/SCC.c src/SccStack.c src/stack.c src/tarjan.c src/TArray.c src/THTopen.c  src/TInfo_KV.c)
add_executable(mpi_tarjan2 src/main.c src/graph.c src/node.c src/SCC.c src/SccStack.c src/stack.c src/tarjan.c src/TArray.c src/THTopen.c  src/TInfo_KV.c)
add_executable(mpi_tarjan3 src/main.c src/graph.c src/node.c src/SCC.c src/SccStack.c src/stack.c src/tarjan.c src/TArray.c src/THTopen.c  src/TInfo_KV.c)
add_executable(seq_tarjan0 src/main_seq.c src/graph.c src/node.c src/SCC.c src/SccStack.c src/stack.c src/tarjan.c src/TArray.c src/THTopen.c  src/TInfo_KV.c)
add_executable(seq_tarjan1 src/main_seq.c src/graph.c src/node.c src/SCC.c src/SccStack.c src/stack.c src/tarjan.c src/TArray.c src/THTopen.c  src/TInfo_KV.c)
add_executable(seq_tarjan2 src/main_seq.c src/graph.c src/node.c src/SCC.c src/SccStack.c src/stack.c src/tarjan.c src/TArray.c src/THTopen.c  src/TInfo_KV.c)
add_executable(seq_tarjan3 src/main_seq.c src/graph.c src/node.c src/SCC.c src/SccStack.c src/stack.c src/tarjan.c src/TArray.c src/THTopen.c  src/TInfo_KV.c)

target_include_directories(mpi_tarjan0 PRIVATE include)
target_include_directories(mpi_tarjan1 PRIVATE include)
target_include_directories(mpi_tarjan2 PRIVATE include)
target_include_directories(mpi_tarjan3 PRIVATE include)
target_include_directories(seq_tarjan0 PRIVATE include)
target_include_directories(seq_tarjan1 PRIVATE include)
target_include_directories(seq_tarjan2 PRIVATE include)
target_include_directories(seq_tarjan3 PRIVATE include)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
find_package(MPI REQUIRED)
if(MPI_C_FOUND)
        message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
        target_link_libraries(mpi_tarjan0 PUBLIC MPI::MPI_C)
        target_link_libraries(mpi_tarjan1 PUBLIC MPI::MPI_C)
        target_link_libraries(mpi_tarjan2 PUBLIC MPI::MPI_C)
        target_link_libraries(mpi_tarjan3 PUBLIC MPI::MPI_C)
endif()

target_compile_options(mpi_tarjan0 PRIVATE -O0)
target_compile_options(mpi_tarjan1 PRIVATE -O1)
target_compile_options(mpi_tarjan2 PRIVATE -O2)
target_compile_options(mpi_tarjan3 PRIVATE -O3)
target_compile_options(seq_tarjan0 PRIVATE -O0)
target_compile_options(seq_tarjan1 PRIVATE -O1)
target_compile_options(seq_tarjan2 PRIVATE -O2)
target_compile_options(seq_tarjan3 PRIVATE -O3)

# TEST
add_executable(tarjan_test test/test.c src/graph.c src/node.c src/SCC.c src/SccStack.c src/stack.c src/tarjan.c src/TArray.c src/THTopen.c  src/TInfo_KV.c)
target_include_directories(tarjan_test PRIVATE include)

add_test(NAME tarjan_test COMMAND tarjan_test)


#generazione grafi
add_custom_target(
        generate_graphs
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/script.py
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/script.py
)

#generazione misurazioni
add_custom_target(
        generate_measures
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/measures.bash ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS
                mpi_tarjan0
                mpi_tarjan1
                mpi_tarjan2
                mpi_tarjan3
                seq_tarjan0
                seq_tarjan1
                seq_tarjan2
                seq_tarjan3
)
#estrazione misure
find_program(PipEnv pipenv REQUIRED)

add_custom_target(
        install_pipenv
        COMMAND PIPENV_PIPFILE=${CMAKE_CURRENT_SOURCE_DIR}/Pipfile ${PipEnv} install --ignore-pipfile
        COMMENT "Installing pienv dependencies [${PipEnv} install]"
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Pipfile.lock
)

add_custom_target(
        extract_measures
        COMMAND PIPENV_PIPFILE=${CMAKE_CURRENT_SOURCE_DIR}/Pipfile ${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/extract.py
        COMMENT "Generating version header [${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/extract.py]"
        DEPENDS	
                    install_pipenv
                    ${CMAKE_CURRENT_SOURCE_DIR}/measures/extract.py
)