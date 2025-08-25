#ifndef TSONGLISTMODEL_H
#define TSONGLISTMODEL_H

#include <QAbstractTableModel>
#include "list.hpp" // To understand TLinkedList
#include "TSong.h"  // We will need to create this file later for the TSong class

// Our custom model must inherit from QAbstractTableModel
class TSongListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    // The constructor takes a pointer to the student's linked list.
    // This is how the model knows where to get its data.
    explicit TSongListModel(TLinkedList<TSong*>* songList, QObject* parent = nullptr);

    // --- Core Functions (Required by QAbstractTableModel) ---

    // Returns the number of rows (songs) in the table.
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    // Returns the number of columns (Title, Artist, etc.).
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    // Fetches the data for a specific cell.
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Fetches the header text for each column.
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	// Refreshes the entire model (e.g., after data changes).
	void RefreshModel();

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;


private:
    // A pointer to the student's actual data structure. The model does NOT own this.
    TLinkedList<TSong*>* llSongs;
};

#endif // TSONGLISTMODEL_H