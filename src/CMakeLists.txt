find_package(Poco REQUIRED Net JSON)

include_directories(../include)

add_executable(fictional-octo-goggles main.cc)
target_link_libraries(fictional-octo-goggles PUBLIC Poco::Net Poco::JSON)