#include <iostream>

#include "album.h"

Album::Album(const Album &otherAlbum)
{
    strcpy(UPC, otherAlbum.UPC);
    strcpy(Artist, otherAlbum.Artist);
    strcpy(Title, otherAlbum.Title);
}

Album::Album (String upc, String artist, String title)
{
    strcpy(UPC, upc);
    strcpy(Artist, artist);
    strcpy(Title, title);
}

Album::Album()
{
    strcpy(UPC, "\n");
    strcpy(Artist, "\n");
    strcpy(Title, "\n");
}

Album & Album :: operator = (const Album& otherAlbum)
{
    strcpy(UPC, otherAlbum.UPC);
    strcpy(Artist, otherAlbum.Artist);
    strcpy(Title, otherAlbum.Title);
    return *this;
}

bool operator < (const Album& a, const Album& b)
{
    return strcmp(a.UPC, b.UPC) < 0;
}

istream & operator >> (istream & stream, Album & C)
{
    stream.getline(C.UPC, 50);
    stream.getline(C.Artist, 50);
    stream.getline(C.Title, 50);
    
    return stream;
}

ostream & operator << (ostream & stream, Album & C)
{
    return stream << C.UPC << " | " << C.Artist << " | " << C.Title << " | ";
}

bool operator == (const Album & a, const Album & b)
{
    if(a.UPC == b.UPC) return true;

    return false;
}

string Album::getUPC()
{
    return UPC;
}

string Album::getArtist()
{
    return Artist;
}

string Album::getTitle()
{
    return Title;
}

int Album::recordSize()
{
    return strlen(UPC) + strlen(Artist) + strlen(Title);
}