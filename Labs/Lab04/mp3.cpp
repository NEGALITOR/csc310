#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <string>
#include <set>

using namespace std;

class Song {
  public:
    string title;
    int time;
    int track;      // Primary key
    bool operator < (Song another) const { return title < another.title;}
    friend ostream & operator<<(ostream& out, const Song& l);
};

//this operator writes out the song in the correct format
//This has been changed by me so that works with 60 instead of 100
ostream & operator << (ostream& out, Song & l) {     
  cout << "\t\t" << l.track << ".  " << l.title << ": " << l.time/60 << ":";
  if ((l.time % 60) < 10) cout << "0";
  cout << l.time % 60 << endl;
  return out;
}

class Album {
  public :
    mutable map<int, Song> songs;
    string name;  // Album's name
    string artist;
    string genre;
    mutable int time;     // Total time of all songs on album
    mutable int nsongs;   // Total number of songs on album

    bool operator < (Album another) const { return name < another.name;}
    bool operator == (string albumName) const { return name == albumName;}
    friend std::ostream& operator<<(std::ostream& out, const Album& al);
};

class Artist {
  public :
    string name; // Artist's name
    set<Album> albums; // Artist's Albums
    int time;    // Total time of all songs on all albums by this artist
    int nsongs;  // Total number of songs on all albums by this artist

    bool operator < (Artist another) const { return name < another.name;}
    bool operator == (string artistName) const { return name == artistName;}
    friend std::ostream& operator<<(std::ostream& out, const Artist& ar);
};

void initializeArtist(fstream &inFile, vector<Artist> &artistVec);
string removeUnderscore(string &input);
bool checkArtist(vector<Artist> artistVec, string artistName);
bool checkAlbum(vector<Artist> artistVec, string albumName);
void addAlbum(vector<Artist> &artistVec, Album album, string artistName);
void addSong(vector<Artist> &artistVec, Song song, string artistName);
void print(vector<Artist> artistVec);

int main(int argc, char *argv[])
{
  //Takes care of missing argument
  if (argc < 2)
  {
    cerr << "mp3: missing filename 'mp3'" << endl;
    exit(0);
  }

  fstream inFile(argv[1], ios::in);
  vector<Artist> artistVec;
  
  initializeArtist(inFile, artistVec);

  sort(artistVec.begin(), artistVec.end());
  print(artistVec);
  inFile.close();

  return 0;
}

//Creates and inserts all artists, albums, and songs into artistVec
void initializeArtist(fstream &inFile, vector<Artist> &artistVec)
{
  
  string songTitle, min, sec, artistName, albumTitle, genre, trackNum;
  
  Artist artist;
  Album album;
  Song song;

  while (inFile >> songTitle)
  {

    //feed in line
    song.title = removeUnderscore(songTitle);

    getline(inFile, min, ':');
    getline(inFile, sec, ' ');
    song.time = 60*stoi(min) + stoi(sec);

    inFile >> artistName;
    artistName = removeUnderscore(artistName);

    inFile >> albumTitle;
    albumTitle = removeUnderscore(albumTitle);

    inFile >> genre;

    inFile >> trackNum;
    song.track = stoi(trackNum);

    //If the artist doesn't exist, make one and push to vector
    if (!checkArtist(artistVec, artistName))
    {
      artist.name = artistName;
      artist.time = 0;
      artist.nsongs = 0;
      artistVec.push_back(artist);
    }

    //If album does not exist and artist does, create album under artist
    //If album does exist, put the song in designated album
    if (!checkAlbum(artistVec, albumTitle) && checkArtist(artistVec, artistName))
    {
      album.songs[song.track] = song;
      album.name = albumTitle;
      album.artist = artistName;
      album.genre = genre;
      album.time = song.time;
      album.nsongs = 1;

      addAlbum(artistVec, album, artistName);

    }
    else
      addSong(artistVec, song, albumTitle);
    
  }
}

//Removes underscore from string
string removeUnderscore(string &input)
{
  replace(input.begin(), input.end(), '_', ' ');
  return input;
}

//Checks within the vector for given artist name
bool checkArtist(vector<Artist> artistVec, string artistName)
{
  vector<Artist> :: iterator artistIter;

  for (artistIter = artistVec.begin(); artistIter != artistVec.end(); ++artistIter)
    if (artistIter->name == artistName) return true;
  return false;
}

//Checks for title of Album in vector -> set
bool checkAlbum(vector<Artist> artistVec, string albumTitle)
{
  vector<Artist> :: iterator artistIter;
  set<Album> :: iterator albumIter;

  for (artistIter = artistVec.begin(); artistIter != artistVec.end(); ++artistIter)
  {
    for (albumIter = artistIter->albums.begin(); albumIter != artistIter->albums.end(); albumIter++)
      if (albumIter->name == albumTitle) return true;
  }
  return false;
}

//Adds album to the designated artist in vector
void addAlbum(vector<Artist> &artistVec, Album album, string artistName)
{
  vector<Artist> :: iterator artistIter;

  for (artistIter = artistVec.begin(); artistIter != artistVec.end(); ++artistIter)
  {
    if (artistIter->name == artistName)
    {
      artistIter->albums.insert(album);
      artistIter->nsongs += 1;
      artistIter->time += album.time;
    }
  }
}

//Adds song to an album by going through Artist vector -> Album set -> songs[]
void addSong(vector<Artist> &artistVec, Song song, string artistName)
{
  vector<Artist> :: iterator artistIter;
  set<Album> :: iterator albumIter;

  for (artistIter = artistVec.begin(); artistIter != artistVec.end(); ++artistIter)
  {
    for(albumIter = artistIter->albums.begin(); albumIter != artistIter->albums.end(); albumIter++)
    {
      if(albumIter->name == artistName)
      {
        //Album Update
        albumIter->songs[song.track] = song;
        albumIter->time += song.time;
        albumIter->nsongs += 1;

        //Artist update
        artistIter->time += song.time;
        artistIter->nsongs += 1;
      } 
    }
  } 
}

//prints everything by iterating through artists, albums and songs
void print(vector<Artist> artistVec)
{
  vector<Artist> :: iterator artistIter;
  set<Album> :: iterator albumIter;
  map<int, Song> :: iterator songIter;

  for (artistIter = artistVec.begin(); artistIter != artistVec.end(); ++artistIter)
  {
    cout << artistIter->name << ": " << artistIter->nsongs << ", " << (artistIter->time)/60 << ":";
    if (artistIter->time%60 < 10) cout << "0";
    cout << (artistIter->time) % 60 << endl;

    for(albumIter = artistIter->albums.begin(); albumIter != artistIter->albums.end(); albumIter++)
    {
      cout << "\t" << albumIter->name << ": " << albumIter->nsongs << ", " << (albumIter->time)/60 << ":";
      if (albumIter->time%60 < 10) cout << "0";
      cout << (albumIter->time)%60 << endl;

      for (songIter = (albumIter->songs).begin(); songIter != (albumIter->songs).end(); ++songIter)
      {
        Song song = songIter->second;
        cout << song;
      }
    }
  }

}