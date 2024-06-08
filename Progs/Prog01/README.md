# Binary Book Database

## Goal
---
The purpose of this program is to be able to utilize binary files to read and edit to a file. It feeds in data from a text file and manipulates it based on transactions. The program utilizes binary files which allow for compact storage on disk compared to a normal text file. 

## Input and Output
---
### - create.cpp
Input
```
library.dat - Input text file with all the data for the BookRecs
```
Output
```
library.out - Output binary file of all the BookRecs fed in by library.dat
```
### - update.cpp
Input
```
library.out - Input binary file that comes from create.cpp and a copy is created for manipulation

fileCopy.out - The copy of the library.out binary which becomes utilized for transactions and map offsets

transact.out - Binary that contains all operations that need to be made on the copy file
```
Output
```
update.out - The final binary file that is outputted once all transactions are finished and from fileCopy.out transferred over

ERROR - Outputs any errors to a text file that may occur during the transaction phase
```

## Algorithm
---
### - create.cpp
```
Function: readIn()
In:     1) BookRec record

Out:    Checks if file exists. If not print error and quit program
        Takes a line and parses through the string via stringParser() and places in BookRec
        Checks if isbn is valid/in order and checks of onhand and price is valid
        Write to ouput file
```
```
Function: printRecords()
In:     1) Bookrec record

Out:    Opens library file as read/binary
        Reads in and prints to console in formatted style
```
```
Function: stringParser()
In:     1) BookRec record
        2) String taken from line of input file
        3) Temporary negative isbn

Out:    Parses through string based on delimiter with a pointer
        Places values into BookRec
```

### - update.cpp
```
Function: readInMapOut()
In:     1) Map of books with an unsigned int isbn as the key and an unsigned int byte offset
        2) BookRec record
        3) File name to read/write output file

Out:    Opens copied file and reads in intital file byte offsets of each BookRec to map
```
```
Function: transactFile()
In:     1) Map of books with an unsigned int isbn as the key and an unsigned int byte offset
        2) BookRec record
        3) File name to read/binary in both the transact file and output file
        4) File name to pass thorugh to makeUpdatedRecords().

Out:    Performs transactions (add, delete, changeOnHand, changePrice) to the copy file based on the transaction file.
        Creates a map of the locations of valid entries not marked for deletion
        Writes them into the output file via makeUpdatedRecords()
```
```
Function: addRecord()
In:     1) BookRec record
        2) Map of books with an unsigned int isbn as the key and an unsigned int byte offset
        3) Current transaction line
        4/5) the fstreams opened in transactFile() of both the copyFile and errorFile

Out:    Checks if isbn exists in file. If so, returns error of duplicate key
        Seeks to end of file, gets byteOffset, inputs to map, and writes to copied file
```
```
Function: changeOnHand()
In:     1) BookRec record
        2) Map of books with an unsigned int isbn as the key and an unsigned int byte offset
        3) Current transaction line
        4/5) the fstreams opened in transactFile() of both the copyFile and errorFile

Out:    Checks if isbn exists in file. If not, returns error of missing key
        Creates a temporary BookRec, seek in copyFile to isbn byteOffset and read into temp BookRec
        Add temp Bookrec.onhand to passed through BookRec.onhand
        Checks if onhand is negative after adding. If so, print error and set passed through onhand to 0
        Seek to byte offset of BookRec and write to copyFile
```
```
Function: changePrice()
In:     1) BookRec record
        2) Map of books with an unsigned int isbn as the key and an unsigned int byte offset
        3) Current transaction line
        4/5) the fstreams opened in transactFile() of both the copyFile and errorFile

Out:    Checks if isbn exists in file. If so, returns error of missing key
        Seek to isbn offset and write to copyFile
```
```
Function: makeUpdatedRecords()
In:     1) BookRec record
        2) Map of books with an unsigned int isbn as the key and an unsigned int byte offset
        3) the fstreams opened in transactFile() of both the copyFile and errorFile
        4) File name to write/binary in output file

Out:    Prints out to console in formatted way.
        Places items into output file by utilizing map to get offsets from copyfile that arent marked for deletion
```
---