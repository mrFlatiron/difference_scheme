TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += precompile_header

SOURCES += \
    src/containers/simple_vector.cpp \
    src/kernel/msr_matrix.cpp \
    src/main/main.cpp \
    src/kernel/difference_scheme_solver.cpp

HEADERS += \
    src/containers/simple_vector.h \
    src/kernel/msr_matrix.h \
    src/precompiled/precompiled_header.h \
    src/common/debug_utils.h \
    src/common/utils.h \
    src/kernel/difference_scheme_solver.h

PRECOMPILED_HEADER = src/precompiled/precompiled_header.h

INCLUDEPATH += src
