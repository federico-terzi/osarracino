add_executable (osarracino main.cpp GameManager.cpp GameManager.h profiles/PlayerProfile.h Player.h profiles/SimpleWhitePlayerProfile.h profiles/SimpleBlackPlayerProfile.h util/MemoryManager.cpp util/MemoryManager.h profiles/ParametrizedPlayerProfile.h profiles/FallbackWhitePlayerProfile.h profiles/FallbackBlackPlayerProfile.h)

target_link_libraries (osarracino libosarracino)