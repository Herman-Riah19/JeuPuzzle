#ifndef PUZZLE_H
#define PUZZLE_H

#include <QWidget>
#include <QPoint>
#include <QVector>
#include <QPixmap>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
QT_END_NAMESPACE

class Puzzle: public QWidget
{
    Q_OBJECT
public:
    explicit Puzzle(int size = 0, QWidget *parent = nullptr);
    void clear ();

    int longPiece () const;
    int longImage () const;

signals:
    void puzzleComplet ();

private:
    struct Piece {
        QPixmap pixmap;
        QRect rectangle;
        QPoint location;
    };

    int findPiece (const QRect &pieceRect ) const;
    const QRect targetSquare (const QPoint &pos);

    QVector<Piece> pieces;
    QRect rectangleBlanc;

    int inPlace;
    int imageSize;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // PUZZLE_H
