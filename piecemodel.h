#ifndef PIECEMODEL_H
#define PIECEMODEL_H

#include <QPoint>
#include <QAbstractListModel>
#include <QPixmap>
#include <QStringList>
#include <QVector>

class QMimeData;

class PieceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PieceModel(int size, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    Qt::DropActions supportedDropActions() const override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void ajoutePiece (const QPixmap &pixmap, const QPoint &location);
    void ajouteLesPieces (const QPixmap &pixmaps);

private:
    QVector<QPixmap> m_pixmaps;
    QVector<QPoint> m_locations;
    int nbrPieces;
};

#endif // PIECEMODEL_H
