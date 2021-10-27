#include "piecemodel.h"

#include <QIcon>
#include <QMimeData>
#include <QRandomGenerator>

PieceModel::PieceModel(int size, QObject *parent):
    QAbstractListModel (parent), nbrPieces (size)
{
}

int PieceModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid () ? 0 : m_pixmaps.size ();
}

QVariant PieceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid ())
        return QVariant ();

    if (role == Qt::DecorationRole)
        return QIcon (m_pixmaps.value (index.row ()).scaled (nbrPieces, nbrPieces, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else if (role == Qt::UserRole)
        return m_pixmaps.value (index.row ());
    else if (role == Qt::UserRole + 1)
        return m_locations.value (index.row ());

    return QVariant ();
}

QStringList PieceModel::mimeTypes() const
{
    QStringList types;
        types << "image/x-image-piece" ;
    return types;
}

QMimeData *PieceModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mime = new QMimeData ();
    QByteArray encodeur;
    QDataStream stream (&encodeur, QIODevice::WriteOnly);

    for (const QModelIndex &index : indexes) {
        if (index.isValid ()) {
            QPixmap pixmap = qvariant_cast<QPixmap> (data (index, Qt::UserRole));
            QPoint location = data (index, Qt::UserRole+1).toPoint ();

            stream << pixmap << location;
        }
    }

    mime->setData ("image/x-image-piece", encodeur);
    return mime;
}

bool PieceModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    //test d'abord les parametres s'ils sont pretes à l'insertion
    if (!data->hasFormat ("image/x-image-piece")) return false;

    if (action == Qt::IgnoreAction) return true;

    if (column > 0) return false;

    //prendre une ligne
    int ligne;
    if (!parent.isValid ()) {
        if (row < 0)
            ligne = m_pixmaps.size ();
        else
            ligne = qMin (row, m_pixmaps.size ());
    } else {
        ligne = parent.row ();
    }

    //on prend l'encodeur mime et on le lit
    QByteArray encodeur = data->data ("image/x-image-piece");
    QDataStream stream (&encodeur, QIODevice::ReadOnly);

    //on insert les image dans n pixmap et point
    while (!stream.atEnd ()) {
        QPixmap pixmap; QPoint location;
        stream >> pixmap >> location;

        beginInsertRows (QModelIndex (), ligne, ligne);
            m_pixmaps.insert (ligne, pixmap);
            m_locations.insert (ligne, location);
        endInsertRows ();

        ++ligne;
    }
    return true;
}

Qt::DropActions PieceModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

bool PieceModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid ())
        return false;

    if (row >= m_pixmaps.size () || row + count <= 0)
        return false;

    int beginRow = qMax (0, row);
    int endRow = qMin (row + count - 1, m_pixmaps.size () - 1);
    beginRemoveRows (parent, beginRow, endRow);
        while (beginRow <= endRow) {
            m_pixmaps.removeAt (beginRow);
            m_locations.removeAt (beginRow);
            ++beginRow;
        }
    endRemoveRows ();

    return true;
}

Qt::ItemFlags PieceModel::flags(const QModelIndex &index) const
{
    if (index.isValid ())
        return (QAbstractListModel::flags (index) | Qt::ItemIsDragEnabled);
    return Qt::ItemIsDropEnabled;
}

void PieceModel::ajoutePiece(const QPixmap &pixmap, const QPoint &location)
{
    int ligne;
    if (QRandomGenerator::global ()->bounded (2) == 1)
        ligne = 0;
    else
        ligne = m_pixmaps.size ();

    beginInsertRows (QModelIndex (), ligne, ligne);
        m_pixmaps.insert (ligne, pixmap);
        m_locations.insert (ligne, location);
    endInsertRows ();
}

void PieceModel::ajouteLesPieces(const QPixmap &pixmaps)
{
    //on vide d'abord les donnees
    if (!m_pixmaps.isEmpty ()) {
        beginRemoveRows (QModelIndex(), 0, m_pixmaps.size () - 1);
            m_pixmaps.clear ();
            m_locations.clear ();
        endRemoveRows ();
    }

    //completons les pieces
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            QPixmap piece = pixmaps.copy (j*nbrPieces, i*nbrPieces, nbrPieces, nbrPieces);
            ajoutePiece (piece, QPoint (j, i));
        }
    }
}
