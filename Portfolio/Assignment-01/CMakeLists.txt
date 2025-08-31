set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(Assignment-01
    main.cpp
    TMainForm.cpp
    TMainForm.h
    frmMain.ui
 "TSongListModel.cpp" "TSong.h" )

target_link_libraries(Assignment-01
    PRIVATE
    SharedCode
    Qt6::Widgets
)

add_custom_command(TARGET Assignment-01 POST_BUILD
    COMMAND powershell -ExecutionPolicy Bypass -File "${CMAKE_SOURCE_DIR}/deploy.ps1" -BuildDir "$<TARGET_FILE_DIR:Assignment-01>"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Deploying Qt dependencies via PowerShell script..."
)