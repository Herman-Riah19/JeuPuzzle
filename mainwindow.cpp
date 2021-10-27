#include "mainwindow.h"
#include "piecemodel.h"
#include "puzzle.h"

#include <QtWidgets>
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    Menus ();
    Widgets ();

    model = new PieceModel (puzzleWidget->longPiece (), this);
    piecesList->setModel (model);

    setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));
    setWindowTitle (tr ("Mon merveuilleu Puzzle"));
}

void MainWindow::loadImage(const QString &path)
{
    QPixmap image;
    if (!image.load (path)) {
        QMessageBox::warning (this, tr ("Ouvrir une Image"), tr ("On ne peut pas ouvrir cet image."));
        return;
    }
    puzzle_Image = image;
    initPuzzle ();
}

void MainWindow::openImage()
{
    const QString direction = QStandardPaths::standardLocations (QStandardPaths::PicturesLocation).value (0, QDir::homePath ());
    QFileDialog dial (this, tr ("Ouvrir une Image"), direction);
    dial.setAcceptMode (QFileDialog::AcceptOpen);
    dial.setFileMode (QFileDialog::ExistingFile);

    QStringList mimeTypesFilters;
    for (const QByteArray &mimeName : QImageReader::supportedMimeTypes ()) {
        mimeTypesFilters.append (mimeName);
    }
    mimeTypesFilters.sort ();

    dial.setMimeTypeFilters (mimeTypesFilters);
    dial.selectMimeTypeFilter ("image/jpeg");
    if (dial.exec () == QDialog::Accepted)
        loadImage (dial.selectedFiles ().constFirst ());
}

void MainWindow::initPuzzle()
{
    int longueur = qMin (puzzle_Image.width (), puzzle_Image.height ());
    //copy puzzle image
    puzzle_Image = puzzle_Image.copy ((puzzle_Image.width () - longueur)/2,
                                      (puzzle_Image.height () - longueur)/2,
                                      longueur, longueur).scaled (puzzleWidget->longImage (),
                                                                  puzzleWidget->longImage (),
                                                                  Qt::IgnoreAspectRatio,
                                                                  Qt::SmoothTransformation);
    model->ajouteLesPieces (puzzle_Image);
    puzzleWidget->clear ();
}

void MainWindow::setCompleted()
{
    QMessageBox::information(this, tr("Puzzle Completé"),
                                   tr("Félicitation! vous avez terminer le puzzle!\n"
                                      "Clique OK pour recommencer"),
                                   QMessageBox::Ok);

    initPuzzle();
}

void MainWindow::Menus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&Fichier"));

    QAction *openAction = fileMenu->addAction(tr("&Ouvrir..."), this, &MainWindow::openImage);
    openAction->setShortcuts(QKeySequence::Open);

    QAction *exitAction = fileMenu->addAction(tr("Fermer"), qApp, &QCoreApplication::quit);
    exitAction->setShortcuts(QKeySequence::Quit);

    QMenu *gameMenu = menuBar()->addMenu(tr("&Jouer"));

    gameMenu->addAction(tr("&Recommencer"), this, &MainWindow::initPuzzle);
}

void MainWindow::Widgets()
{
    QFrame *frame = new QFrame;
    QHBoxLayout *layout = new QHBoxLayout(frame);
    puzzleWidget = new Puzzle (400);
    piecesList = new QListView;

    piecesList->setDragEnabled (true);
    piecesList->setViewMode (QListView::IconMode);
    piecesList->setIconSize(QSize(puzzleWidget->longPiece () - 20, puzzleWidget->longPiece () - 20));
    piecesList->setGridSize(QSize(puzzleWidget->longPiece (), puzzleWidget->longPiece ()));
    piecesList->setSpacing(20);
    piecesList->setMovement(QListView::Snap);
    piecesList->setAcceptDrops(true);
    piecesList->setDropIndicatorShown(true);

    PieceModel *autreModel = new PieceModel (puzzleWidget->longPiece (), this);
    piecesList->setModel (autreModel);

    connect(puzzleWidget, &Puzzle::puzzleComplet,
                 this, &MainWindow::setCompleted, Qt::QueuedConnection);

    layout->addWidget(piecesList);
    layout->addWidget(puzzleWidget);
    setCentralWidget(frame);
}
