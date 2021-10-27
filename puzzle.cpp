#include "puzzle.h"

#include <QtWidgets>

Puzzle::Puzzle(int size, QWidget *parent):
    QWidget (parent), imageSize (size)
{
    setAcceptDrops (true);
    setMinimumSize (imageSize, imageSize);
    setMaximumSize (imageSize, imageSize);
}

void Puzzle::clear()
{
    pieces.clear ();
    rectangleBlanc = QRect ();
    inPlace = 0;
    update ();
}

int Puzzle::longPiece() const
{
    return imageSize/5;
}

int Puzzle::longImage() const
{
    return imageSize;
}

int Puzzle::findPiece(const QRect &pieceRect) const
{
    for (int i = 0; i < pieces.size (); ++i) {
        if (pieces.at (i).rectangle == pieceRect)
            return i;
    }
    return -1;
}

const QRect Puzzle::targetSquare(const QPoint &pos)
{
    return QRect (pos / longPiece ()*longPiece (), QSize (longPiece (), longPiece ()));
}

void Puzzle::mousePressEvent(QMouseEvent *event)
{
    QRect rect = targetSquare (event->pos ());
    int indice = findPiece (rect);
    if (indice == -1) return;

    Piece piece = pieces.takeAt (indice);

    if (piece.location == rect.topLeft ()/longPiece ())
        inPlace--;

    update (rect);

    QByteArray encodeur;
    QDataStream stream (&encodeur, QIODevice::WriteOnly);
    stream << piece.pixmap << piece.location;

    QMimeData *mime = new QMimeData;
    mime->setData ("image/x-image-piece", encodeur);

    QDrag *drag = new QDrag(this);
    drag->setMimeData (mime);
    drag->setHotSpot (event->pos () - rect.topLeft ());
    drag->setPixmap (piece.pixmap);

    if (drag->exec (Qt::MoveAction) == Qt::IgnoreAction) {
        pieces.insert (indice, piece);
        update (targetSquare (event->pos ()));

        if (piece.location == QPoint (rect.x ()/longPiece (), rect.y ()/longPiece ()))
            inPlace++;
    }
}

void Puzzle::paintEvent(QPaintEvent *event)
{
    QPainter crayon (this);
    crayon.fillRect (event->rect (), Qt::white);

    if (rectangleBlanc.isValid ()) {
        crayon.setBrush (QColor ("#ffcccc"));
        crayon.setPen (Qt::NoPen);
        crayon.drawRect (rectangleBlanc.adjusted (0, 0, -1, -1));
    }

    for (const Piece &piece : pieces) {
        crayon.drawPixmap (piece.rectangle, piece.pixmap);
    }
}

void Puzzle::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData ()->hasFormat ("image/x-image-piece"))
        event->accept ();
    else
        event->ignore ();
}

void Puzzle::dragMoveEvent(QDragMoveEvent *event)
{
    QRect nouveauRect = rectangleBlanc.united (targetSquare (event->pos ()));

    if (event->mimeData ()->hasFormat ("image/x-image-piece") && findPiece (targetSquare (event->pos ())) == -1) {
        rectangleBlanc = targetSquare (event->pos ());
        event->setDropAction (Qt::MoveAction);
        event->accept ();
    } else {
        rectangleBlanc = QRect ();
        event->ignore ();
    }
    update (nouveauRect);
}

void Puzzle::dragLeaveEvent(QDragLeaveEvent *event)
{
    QRect rect = rectangleBlanc;
    rectangleBlanc = QRect ();
    update (rect);
    event->accept ();
}

void Puzzle::dropEvent(QDropEvent *event)
{
    if (event->mimeData ()->hasFormat ("image/x-image-piece") && findPiece (targetSquare (event->pos ())) == -1) {
        QByteArray encodeur = event->mimeData ()->data ("image/x-image-piece");
        QDataStream stream (&encodeur, QIODevice::ReadOnly);

        Piece piece;
        piece.rectangle = targetSquare (event->pos ());
        stream >> piece.pixmap >> piece.location;
        pieces.append (piece);

        rectangleBlanc = QRect ();
        update (piece.rectangle);

        event->setDropAction (Qt::MoveAction);
        event->accept ();

        if (piece.location == piece.rectangle.topLeft ()/longPiece ()) {
            inPlace++;
            if (inPlace == 25)
                emit puzzleComplet ();
        }
    } else {
        rectangleBlanc = QRect ();
        event->ignore ();
    }
}
