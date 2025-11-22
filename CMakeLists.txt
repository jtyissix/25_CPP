cmake_minimum_required(VERSION 3.28)
project(25_CPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


add_definitions(-DQT_NO_DEBUG_OUTPUT)
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

add_executable(${PROJECT_NAME} src/main.cpp
        src/Intro.cpp
        src/Intro.h
        src/GameProgress.h
        src/MyGame.cpp
        src/MyGame.h
        src/Items/Item.cpp
        src/Items/Item.h
        src/UI/HealthBar.cpp
        src/UI/HealthBar.h
        src/Items/Figure/Figure.cpp
        src/Items/Figure/Figure.h
        src/Items/Figure/FigureA.cpp
        src/Items/Figure/FigureA.h
        src/Items/Characters/Character.cpp
        src/Items/Characters/Character.h
        src/Items/Characters/Link.cpp
        src/Items/Characters/Link.h
        src/Items/Characters/link2.cpp
        src/Items/Characters/link2.h
        src/Items/Armors/Armor.cpp
        src/Items/Armors/Armor.h
        src/Items/Weapons/Weapon.cpp
        src/Items/Weapons/Weapon.h
        #src/Items/LegEquipments/LegEquipment.cpp
        #src/Items/LegEquipments/LegEquipment.h
        src/Items/Weapons/Rifle.cpp
        src/Items/Weapons/Rifle.h
        src/Items/Armors/BulletArmor.cpp
        src/Items/Armors/BulletArmor.h
        #src/Items/LegEquipments/WellWornTrousers.cpp
        #src/Items/LegEquipments/WellWornTrousers.h
        src/Scenes/Scene.cpp
        src/Scenes/Scene.h
        src/Scenes/BattleScene.cpp
        src/Scenes/BattleScene.h
        assets/assets.qrc
        src/Items/Maps/Map.cpp
        src/Items/Maps/Map.h
        src/Items/Maps/Battlefield.cpp
        src/Items/Maps/Battlefield.h
        src/Items/Mountable.cpp
        src/Items/Mountable.h
        src/Items/Figure/Figure.h
        src/Items/Figure/Figure.cpp
        src/Items/Armors/NormalArmor.cpp
        src/Items/Armors/NormalArmor.h
        src/Items/Figure/FigureA_KneeDown.cpp
        src/Items/Figure/FigureA_KneeDown.h src/Items/Figure/FigureA_KneeDown.cpp
        src/Items/Characters/link2.h src/Items/Characters/link2.cpp
        src/Items/Figure/figureb.h src/Items/Figure/figureb.cpp
        src/Items/Figure/figureb_kneedown.h src/Items/Figure/figureb_kneedown.cpp
        src/Items/Figure/figurea_hand.h src/Items/Figure/figurea_hand.cpp
        src/Items/Figure/figureb_hand.h src/Items/Figure/figureb_hand.cpp
        src/FallingController/fallingcontroller.h
        src/FallingController/fallingcontroller.cpp
        src/Items/fallable.cpp
        src/Items/fallable.h
        src/Items/Weapons/bullet.h src/Items/Weapons/bullet.cpp
        src/Items/Weapons/sniper.h src/Items/Weapons/sniper.cpp
        src/Items/Weapons/ball.h src/Items/Weapons/ball.cpp
        src/Items/Weapons/ballhand.h src/Items/Weapons/ballhand.cpp
        src/Items/Weapons/knife.h src/Items/Weapons/knife.cpp
        src/Items/Drugs/drug.h src/Items/Drugs/drug.cpp
        src/Items/Drugs/bandage.h src/Items/Drugs/bandage.cpp
        src/Items/Drugs/medbag.h src/Items/Drugs/medbag.cpp
        src/Items/Drugs/medical.h src/Items/Drugs/medical.cpp
        src/Scenes/Platform.h
        src/Scenes/Platform.cpp
        src/Items/Maps/grass.h src/Items/Maps/grass.cpp
        src/Items/Maps/ice.h src/Items/Maps/ice.cpp
        )
target_link_libraries(${PROJECT_NAME}
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Multimedia
)
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
