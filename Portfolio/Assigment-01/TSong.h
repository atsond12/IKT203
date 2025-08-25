#ifndef TSONG_H
#define TSONG_H

#include <QString> // Using QString is convenient for Qt integration

// A simple struct to hold all the data for a single song.
// This is a plain data object with no Qt inheritance.
struct TSong
{
    QString title;
    QString artist;
    QString album;
    int     year;
    bool    isFavorite;
    QString info;
};

#endif // TSONG_H