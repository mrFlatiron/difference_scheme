TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += precompile_header

SOURCES += \
    src/main/main.cpp \
    src/kernel/difference_scheme_solver.cpp \
    src/kernel/input/start_functions.cpp \
    src/kernel/scheme_point.cpp \
    src/kernel/scheme_iter_data.cpp \
    src/kernel/matrix_el.cpp \
    src/kernel/input/known_functions.cpp \
    src/common/math_utils.cpp \
    src/kernel/laspack_matrix.cpp \
    src/kernel/laspack_vector.cpp \
    src/3rd_party/laspack/eigenval.c \
    src/3rd_party/laspack/errhandl.c \
    src/3rd_party/laspack/factor.c \
    src/3rd_party/laspack/itersolv.c \
    src/3rd_party/laspack/matrix.c \
    src/3rd_party/laspack/mlsolv.c \
    src/3rd_party/laspack/operats.c \
    src/3rd_party/laspack/precond.c \
    src/3rd_party/laspack/qmatrix.c \
    src/3rd_party/laspack/rtc.c \
    src/3rd_party/laspack/vector.c \
    src/kernel/input/wanted_functions.cpp \
    src/kernel/solver_checker.cpp

HEADERS += \
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
    src/kernel/laspack_matrix.h \
    src/kernel/laspack_vector.h \
    src/3rd_party/laspack/copyrght.h \
    src/3rd_party/laspack/eigenval.h \
    src/3rd_party/laspack/elcmp.h \
    src/3rd_party/laspack/errhandl.h \
    src/3rd_party/laspack/factor.h \
    src/3rd_party/laspack/itersolv.h \
    src/3rd_party/laspack/lastypes.h \
    src/3rd_party/laspack/matrix.h \
    src/3rd_party/laspack/mlsolv.h \
    src/3rd_party/laspack/operats.h \
    src/3rd_party/laspack/precond.h \
    src/3rd_party/laspack/qmatrix.h \
    src/3rd_party/laspack/rtc.h \
    src/3rd_party/laspack/vector.h \
    src/3rd_party/laspack/version.h \
    src/kernel/input/wanted_functions.h \
    src/kernel/solver_checker.h

PRECOMPILED_HEADER = src/precompiled/precompiled_header.h

INCLUDEPATH += src


LIBS += -pthread
