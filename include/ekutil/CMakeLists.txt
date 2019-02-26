add_library(ekutil INTERFACE
    compat.h memory.h meta.h numeric.h
    small_vector.h span.h string_view.h)
target_compile_features(ekutil INTERACE cxx_std_11)
