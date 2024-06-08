#include <fstream>
#include <iomanip>
#include <iostream>
#include <cstring>

using namespace std;

typedef char String[25];
struct BookRec //87 bytes
{
    unsigned int isbn; //4
    String name; //25
    String author; //25
    int onhand; //4
    float price; //4
    String type; //25
};

void readIn(BookRec br);
void printRecords(BookRec br);
void stringParser(BookRec &br, char str[], long &negTemp);

int main()
{
    BookRec br;
    system("clear");
    readIn(br);
    printRecords(br);

    return 0;
}

/*----------------------------------------------------------------------------------------------------------------------
Function: readIn()
In:     1) BookRec record

Out:    Checks if file exists. If not print error and quit program
        Takes a line and parses through the string via stringParser() and places in BookRec
        Checks if isbn is valid/in order and checks of onhand and price is valid
        Write to ouput file
------------------------------------------------------------------------------------------------------------------------*/
void readIn(BookRec br)
{
    fstream inFile("library.dat", ios::in);
    fstream outBin("library.out", ios::out | ios::binary);

    if (!inFile)
    {
        cerr << "Missing library.dat file";
        exit(0);
    }

    bool flagged = false;
    unsigned int previous = 0;
    long negTemp;
    int lineNumber = 0;

    while (inFile)
    {
        lineNumber++;
        flagged = false;
        char temp[87];

        if (inFile)
        {
            inFile.getline(temp, sizeof(BookRec), '\n');
            if (inFile.eof()) break;
        }

        stringParser(br, temp, negTemp);
        
        if (negTemp < 0) 
        {
            cerr << "> Illegal isbn number encountered on line " << lineNumber << " of data file - record ignored." << endl;
            
            flagged = true;
        }
        br.isbn = (unsigned int) negTemp;
        
        if (br.onhand < 0 || br.price < 0) 
        {
            cerr << "> Negative amount onhand on line " << lineNumber << " of data file - record ignored." << endl;
            cerr << "   " << setw(10) << setfill('0') << br.isbn
                << setw(25) << setfill(' ') << br.name
                << setw(25) << br.author
                << setw(3) << br.onhand
                << setw(6) << br.price
                << setw(10) << br.type<<endl;
            flagged = true;
        }
        
        if (flagged) continue;

        if (br.isbn < previous)
        {
            cerr << "> Isbn number out of sequence on line " << lineNumber << " of data file" << endl;
            cerr << "   " << setw(10) << setfill('0') << br.isbn
                << setw(25) << setfill(' ') << br.name
                << setw(25) << br.author
                << setw(3) << br.onhand
                << setw(6) << br.price
                << setw(10) << br.type<<endl;
        }
        previous = br.isbn;

        outBin.write((char *) &br, sizeof(BookRec));
        
    }

    inFile.close();
    outBin.close();
    
}

/*----------------------------------------------------------------------------------------------------------------------
Function: printRecords()
In:     1) Bookrec record

Out:    Opens library file as read/binary
        Reads in and prints to console in formatted style
------------------------------------------------------------------------------------------------------------------------*/
void printRecords(BookRec br)
{
    

    cout << "-----------------------------------------------------------------------------------" << endl;
    cout << "| Book Records" << setw(69) << setfill(' ') << "|" <<endl;
    cout << "-----------------------------------------------------------------------------------" << endl;

    fstream bInFile("library.out", ios::in | ios::binary);
    while(bInFile.read((char *) &br, sizeof(br)))
    {
        cout << "| " << setw(10) << setfill('0') << br.isbn
	      << setw(25) << setfill(' ') << br.name
	      << setw(25) << br.author
	      << setw(3) << br.onhand
	      << setw(6) << br.price
	      << setw(10) << br.type << " |" << endl;
    }
    bInFile.close();
     cout << "-----------------------------------------------------------------------------------" << endl;
}

/*----------------------------------------------------------------------------------------------------------------------
Function: stringParser()
In:     1) BookRec record
        2) String taken from line of input file
        3) Temporary negative isbn

Out:    Parses through string based on delimiter with a pointer
        Places values into BookRec
------------------------------------------------------------------------------------------------------------------------*/
void stringParser(BookRec &br, char str[], long &negTemp)
{
    String arr[6];

    char *ptr;
    ptr = strtok(str, "|");
    int i = 0;

    while (ptr != NULL)
    {
        strcpy(arr[i], ptr);
        i++;
        ptr = strtok(NULL, "|");
    }

    negTemp = stol(arr[0]);
	strcpy(br.name, arr[1]);
	strcpy(br.author, arr[2]);
	br.onhand = stoi(arr[3]);
	br.price = stof(arr[4]);
	strcpy(br.type, arr[5]);
    
}