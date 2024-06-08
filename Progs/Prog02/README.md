# The JTar Software

## Goal
---
The goal of this program is to be able to compact files and directories into a single tar file. This allows for easier transferring of data from one location to another without worring about individual files being left behind. We should be able to compress files into a tar file and also be able to view all the files within and uncompact it.

## Plans For File.cpp
---
File.cpp will be utilized in a way that information can be stored and reattained within the tarFile. This will be done via an outFile and inFile which will by the size of a File, write grab certain information including dirFlags, recordSize, fileName, protection, timeStamp. These are then used to restore information for each file when unpacking the tarFile.

## Input and Output
---
### - jtar.cpp
Input
```
Any user defined file or directory
```
Output
```
A user defined tar file
```


## Algorithm
---
### - jtar.cpp
```
Function: tarCompilation()

IN:     1) argc
        2) argv

OUT:    Takes in the argType through errorCheck()
        Builds a map of months through buildMap()
        Utilizes a switch to perform actions based on argType provided

        Case 1:
            For each file provided, checks if the file provided is a directory and erases '/' at the 
                end for easier utilization
            Pushes to the tempFileVect for futher processing
            Passes each element through fileToVector() in order to recurse through directories and 
                place each file in initFileVect (does not include . and ..)
            Gets all information from each file (timestamps, size, and protection) and adds the 
                sizes of all the files to calculate tar file's size (excludes dirs)
                and pushes all the files into the finalFileVect
            Creates tar file based on name provided and writes all the files' contents into the tar file

        Case 2:
            Gets name of each file from tar file and outputs to console

        Case 3:
            Unpacks tar file based on File struct size
            Checks if directory exists and if does checks for failure
            Runs commands on command line to restore directores, file permissions, file access/
                modification times, and data (if not a dir)
            
        Case 4:
            Prints help menu

```
```
Function: fileToVector()
IN:     1) File
        2) vector<fileInfo>
OUT:
        Puts files into vector
        If file is a directory recurs through the files within it and add it to the vector
```
```
Function: stringParser()
IN:     1) string
OUT:
        Return bool if file is directory or not
```
```
Function: readInMapOut()
IN:     N/A
OUT:
        Builds the months map and returns it
```
```
Function: transactFile()
IN:     1) string
        2) map<string, string>
OUT:
        Gets the time formatted to be read in to touch -t
```
```
Function: addRecord()
IN:     1) string
OUT:
        Gets size of the file
```
```
Function: changeOnHand()
IN:     1) string
OUT:
        Gets protection value of file via hex
```
```
Function: changePrice()
IN:     1) argc
        2) argv
OUT:
        Checks for errors in the arguments for -cf, -tf, -xf, --help
        If argc is only the executable name, fail
        Checks the argument with a switch
        Returns a value corresponding to the argument type if no errors appear
```
---