# --- Step 1: Create the Library ---

# Define a library target named "LibExample".
# We use INTERFACE because it only contains .hpp files (header-only).
# There are no .cpp files to compile here.
# TODO: Change to STATIC if you add .cpp files later.
add_library(LibExample INTERFACE)

# --- Step 2: Add Header Files to the Library ---

# This command explicitly lists the header files that belong to the library.
# This helps Visual Studio display them nicely in the Solution Explorer.
target_sources(LibExample
    PUBLIC
        list.hpp
        TSingleLinkedListTemplate.hpp
        TDoublyLinkedListTemplate.hpp
        TCircularDoublyLinkedListTemplate.hpp
        queue.hpp
        stack.hpp
)

# --- Step 3: Make Headers "Findable" ---

# This is the most important command here.
# It tells any other project that links to "LibExample" to add this
# directory (CMAKE_CURRENT_SOURCE_DIR) to its list of include paths.
# This is what allows you to write #include "list.hpp" in your main.cpp.
# Note: CMAKE_CURRENT_SOURCE_DIR is a built-in variable that points to the directory
target_include_directories(LibExample INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
#TODO: Change INTERFACE to PUBLIC if you add .cpp files later.

