TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += precompile_header

SOURCES += \
    src/containers/simple_vector.cpp \
    src/kernel/msr_matrix.cpp \
    src/main/main.cpp \
    src/kernel/difference_scheme_solver.cpp \
    src/kernel/input/start_functions.cpp \
    src/kernel/scheme_point.cpp \
    src/kernel/scheme_iter_data.cpp \
    src/kernel/matrix_el.cpp \
    src/kernel/input/known_functions.cpp \
    src/common/math_utils.cpp \
    src/kernel/dqgmres_solver/msr_dqgmres_initializer.cpp \
    src/kernel/dqgmres_solver/msr_thread_dqgmres_solver.cpp \
    src/kernel/dqgmres_solver/msr_thread_handler.cpp \
    src/kernel/threads/thread_handler.cpp \
    src/kernel/threads/thread_vector_utils.cpp

HEADERS += \
    src/containers/simple_vector.h \
    src/kernel/msr_matrix.h \
    src/precompiled/precompiled_header.h \
    src/common/debug_utils.h \
    src/common/utils.h \
    src/kernel/difference_scheme_solver.h \
    src/kernel/input/start_functions.h \
    src/kernel/scheme_point.h \
    src/kernel/scheme_defs.h \
    src/kernel/scheme_iter_data.h \
    src/kernel/matrix_el.h \
    src/kernel/input/known_functions.h \
    src/common/math_utils.h \
    src/kernel/dqgmres_solver/msr_dqgmres_initializer.h \
    src/kernel/dqgmres_solver/msr_thread_dqgmres_solver.h \
    src/kernel/dqgmres_solver/msr_thread_handler.h \
    src/kernel/threads/thread_handler.h \
    src/kernel/threads/thread_vector_utils.h \
    src/containers/limited_deque.h

PRECOMPILED_HEADER = src/precompiled/precompiled_header.h

INCLUDEPATH += src

LIBS += -pthread
