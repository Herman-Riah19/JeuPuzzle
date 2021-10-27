#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

class PieceModel;
class Puzzle;
class QListView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void openImage ();
    void loadImage (const QString &path);
    void initPuzzle ();

private slots:
    void setCompleted ();

private:
    void Menus ();
    void Widgets ();

    QListView *piecesList;
    QPixmap puzzle_Image;
    PieceModel *model;
    Puzzle *puzzleWidget;
};

#endif // MAINWINDOW_H
