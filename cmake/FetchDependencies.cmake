# Fetch and configure dependencies

message(STATUS "Configuring dependencies...")

FetchContent_Declare(
   wxWidgets
   GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
   GIT_SHALLOW ON
)

FetchContent_Declare(
   RtAudio
   GIT_REPOSITORY https://github.com/thestk/rtaudio.git
   GIT_TAG 6.0.1
)

# Make dependencies available
FetchContent_MakeAvailable(wxWidgets RtAudio)

message(STATUS "All dependencies ready!")
