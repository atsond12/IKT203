#include "TSongListModel.h"


// We'll use an enum to make column management easier and more readable.
enum EQColumnName{
    Title = 0,
    Artist,
    Album,
    Year,
    Favorite,
    Info,
    Count // Helper to get the total number of columns
};

TSongListModel::TSongListModel(TLinkedList<TSong*>* songList, QObject* parent)
    : QAbstractTableModel(parent), llSongs(songList)
{
}

int TSongListModel::rowCount(const QModelIndex& parent) const
{
    // The number of rows is simply the number of items in the linked list.
    return llSongs ? llSongs->GetSize() : 0;
}

int TSongListModel::columnCount(const QModelIndex& parent) const
{
    // We have a fixed number of columns.
    return EQColumnName::Count;
}

QVariant TSongListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // We only care about horizontal headers (the column titles).
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }

    switch (section) {
    case EQColumnName::Title:    return "Title";
    case EQColumnName::Artist:   return "Artist";
    case EQColumnName::Album:    return "Album";
    case EQColumnName::Year:     return "Year";
    case EQColumnName::Favorite: return "Favorite";
    case EQColumnName::Info:     return "Info";
    default:              return QVariant();
    }
}

QVariant TSongListModel::data(const QModelIndex& index, int role) const
{
    // --- 1. Basic Validity Checks ---
    // Ensure the index is valid and the song list pointer exists.
    if (!index.isValid() || !llSongs) {
        return QVariant();
    }

    // Check if the requested row is within the bounds of the list.
    if (index.row() >= llSongs->GetSize()) {
        return QVariant();
    }

    // --- 2. Get the Song Object ---
    // Retrieve the pointer to the correct TSong object from the linked list.
    TSong* song = llSongs->GetAtIndex(index.row());
    if (!song) {
        return QVariant();
    }

    // --- 3. Handle Different Data Roles ---
    // The view asks for data for different "roles". We primarily care about
    // what to display (DisplayRole) and the state of our checkbox (CheckStateRole).

    if (role == Qt::DisplayRole) {
        // The view wants text to display. We switch on the column to decide which piece of data to return.
        switch (index.column()) {
        case EQColumnName::Title:   return song->title;
        case EQColumnName::Artist:  return song->artist;
        case EQColumnName::Album:   return song->album;
        case EQColumnName::Year:    return song->year;
        case EQColumnName::Info:    return song->info;
            // Note: We don't return text for the 'Favorite' column, as it will be a checkbox.
        default:
            return QVariant();
        }
    }
    else if (role == Qt::CheckStateRole) {
        // The view wants to know if a checkbox should be drawn, and if it's checked.
        // We only do this for the "Favorite" column.
        if (index.column() == EQColumnName::Favorite) {
            return song->isFavorite ? Qt::Checked : Qt::Unchecked;
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
        // The view is asking how to align the item. We only want to
        // specify a non-default alignment for the Favorite column.
        if (index.column() == EQColumnName::Favorite) {
            // Return the alignment flag to center horizontally and vertically.
            return QVariant(Qt::AlignCenter);
        }
    }

    // --- 4. Default Case ---
    // For any other roles we don't handle, we return an empty QVariant.
    return QVariant();
}

void TSongListModel::RefreshModel()
{
    // Notify any attached views that the data has changed.
    beginResetModel();
    endResetModel();
}

Qt::ItemFlags TSongListModel::flags(const QModelIndex& index) const
{
    // Get the default flags from the base class implementation.
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

    // If the column is the "Favorite" column, add the checkable flag.
    if (index.column() == EQColumnName::Favorite) {
        return defaultFlags | Qt::ItemIsUserCheckable;
    }

    return defaultFlags;
}

bool TSongListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    // We only handle changes to the check state of the "Favorite" column.
    if (!index.isValid() || role != Qt::CheckStateRole || index.column() != EQColumnName::Favorite) {
        return false;
    }

    // Get the song object for the given row.
    TSong* song = llSongs->GetAtIndex(index.row());
    if (!song) {
        return false;
    }

    // Update the actual data in our TSong object.
    song->isFavorite = (value.toInt() == Qt::Checked);

    // CRUCIAL: Emit the dataChanged signal.
    // This tells the view that the data has changed and it needs to redraw the item.
    emit dataChanged(index, index, { role });

    // Return true to indicate the change was successfully handled.
    return true;
}