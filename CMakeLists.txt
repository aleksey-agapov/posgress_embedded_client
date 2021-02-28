cmake_minimum_required (VERSION 2.6)

# Задать характеристики проекта "pg_sql"
project (pg_sql VERSION 0.0.1.0 LANGUAGES C CXX)


include_directories(config)
include_directories(control)
include_directories(db)
include_directories(gui)
include_directories(thread)
include_directories(utils)

# add_subdirectory(config)
# add_subdirectory(control)
# add_subdirectory(db)
# add_subdirectory(gui)
# add_subdirectory(thread)
# add_subdirectory(utils)

# Скомпилировать исполняемый файл "pg_sql" из pg_sql.cpp
# config/AppConfigControl.cpp \
# control/CommandControl.cpp \
# control/consoleinterface.cpp \
# control/About.cpp \
# control/DefaultSchema.cpp \
# control/History.cpp \
# control/Log.cpp \
# db/db_table_class_list.cpp \
# db/selector.cpp \
# db/sqlexec.cpp \
# gui/OutputForm.cpp \
# thread/log_thread.cpp \
# utils/apputils.cpp 


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall")
set(SOURCES pg_sql.cpp  config/AppConfigControl.cpp control/CommandControl.cpp control/consoleinterface.cpp  control/About.cpp  control/DefaultSchema.cpp  control/History.cpp  control/Log.cpp  db/db_table_class_list.cpp  db/selector.cpp  db/sqlexec.cpp  gui/OutputForm.cpp  thread/log_thread.cpp  utils/apputils.cpp )

add_executable(pg_sql $(SOURCES) )
target_link_libraries(${PROJECT_NAME} stdc++fs)
target_link_libraries(${PROJECT_NAME} pqxx)
target_link_libraries(${PROJECT_NAME} cpprest)
target_link_libraries(${PROJECT_NAME} pthread)

