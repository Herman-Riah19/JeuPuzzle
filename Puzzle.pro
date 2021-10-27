#-------------------------------------------------
#
# Project created by QtCreator 2019-11-01T17:04:03
#
#-------------------------------------------------
QT      += widgets
requires (qtConfig (filedialog))

TARGET = Puzzle
TEMPLATE = app

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        piecemodel.cpp \
        puzzle.cpp

HEADERS += \
    mainwindow.h \
    piecemodel.h \
    puzzle.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

TRANSLATIONS = puzzle_en.ts puzzle_mg.ts
