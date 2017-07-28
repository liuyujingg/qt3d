!include( ../manual.pri ) {
    error( "Couldn't find the manual.pri file!" )
}

QT += 3dcore 3drender 3dinput 3dquick qml quick 3dquickextras

SOURCES += \
    main.cpp

OTHER_FILES += \
    main.qml \
    DefaultSceneEntity.qml

RESOURCES += \
    skinned-mesh.qrc \
    ../../../examples/qt3d/exampleresources/cubemaps.qrc \
    ../../../examples/qt3d/exampleresources/envmaps.qrc \
    ../../../examples/qt3d/exampleresources/gltf.qrc
