# Link main file to executable name
add_executable(aircraft_transponder_top main_aircraft_transponder.cpp)


# Add required libraries and include directories
target_include_directories(aircraft_transponder_top PRIVATE "." "include" $ENV{CADMIUM})


# set c++ standard
target_compile_options(aircraft_transponder_top PUBLIC -std=gnu++2b)
