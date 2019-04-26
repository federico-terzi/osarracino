add_executable (osarracino main.cpp GameManager.cpp GameManager.h profiles/PlayerProfile.h Player.h profiles/SimpleWhitePlayerProfile.cpp profiles/SimpleWhitePlayerProfile.h profiles/SimpleBlackPlayerProfile.cpp profiles/SimpleBlackPlayerProfile.h)

target_link_libraries (osarracino libosarracino)