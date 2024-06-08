#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include <map>
#include "file.h"

using namespace std;

struct fileInfo
{
    string fileName;
    bool isDir = false;
};

void tarCompilation(int argc, char *argv[]);
void fileToVector(fileInfo file, vector<fileInfo> &vect);
bool checkDir(string fileName);
string getTime(string fileName, map<string, string> months);
map<string, string> buildMap();
string getSize(string fileName);
string getProtect(string fileName);
int errorCheck(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    tarCompilation(argc, argv);
    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------
IN:     1) argc
        2) argv

OUT:    Takes in the argType through errorCheck()
        Builds a map of months through buildMap()
        Utilizes a switch to perform actions based on argType provided

        Case 1:
            For each file provided, checks if the file provided is a directory and erases '/' at the end for easier utilization
            Pushes to the tempFileVect for futher processing
            Passes each element through fileToVector() in order to recurse through directories and place each file in initFileVect (does not include . and ..)
            Gets all information from each file (timestamps, size, and protection) and adds the sizes of all the files to calculate tar file's size (excludes dirs)
                and pushes all the files into the finalFileVect
            Creates tar file based on name provided and writes all the files' contents into the tar file

        Case 2:
            Gets name of each file from tar file and outputs to console

        Case 3:
            Unpacks tar file based on File struct size
            Checks if directory exists and if does checks for failure
            Runs commands on command line to restore directores, file permissions, file access/modification times, and data (if not a dir)
            
        Case 4:
            Prints help menu

----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void tarCompilation(int argc, char *argv[])
{
    int argType = errorCheck(argc, argv);
    struct fileInfo fileInfoStruct;
    vector <fileInfo> tempFileVect, initFileVect;
    vector <File> finalFileVect;
    vector <fileInfo> :: iterator fIIter;
    vector <File> :: iterator fileIter;
    File fileFile;
    int size, tarSize;
    string tarName, time, fileSize, protect;
    map<string, string> months = buildMap();
    
    switch (argType)
    {
        //Performs -cf argument
        case 1:
        {
            size = argc - 3;
            tarSize = 0;

            //Formats and puts in all provided files into temFileVect
            for (int i = 3; i < argc; i++)
            {
                fileInfoStruct.fileName = argv[i];
                fileInfoStruct.isDir = checkDir(fileInfoStruct.fileName);

                if (fileInfoStruct.isDir) fileInfoStruct.fileName.erase(fileInfoStruct.fileName.size() - 1);

                tempFileVect.push_back(fileInfoStruct);
            }
            
            //Places files from directories into initFileVect recusively
            for (fIIter = tempFileVect.begin(); fIIter < tempFileVect.end(); fIIter++)
            {
                fileInfoStruct = *fIIter;
                fileToVector(fileInfoStruct, initFileVect);
            }
            
            //Gets all information from each file (timestamps, size, and protection) and adds the sizes of all the files to calculate tar file's size (excludes dirs)
            //and pushes all the files into the finalFileVect
            for (fIIter = initFileVect.begin(); fIIter < initFileVect.end(); fIIter++)
            {
                fileInfoStruct = *fIIter;
                time = getTime(fileInfoStruct.fileName, months);
                fileSize = getSize(fileInfoStruct.fileName);
                protect = getProtect(fileInfoStruct.fileName);

                File file(fileInfoStruct.fileName.c_str(), protect.c_str(), fileSize.c_str(), time.c_str());
                
                if (fileInfoStruct.isDir)
                {
                    file.flagAsDir();
                    int recSize = file.recordSize();

                    tarSize = tarSize + recSize;
                }
                else
                {
                    int recSize = file.recordSize();
                    tarSize = tarSize + recSize + stoi(fileSize);
                }
                finalFileVect.push_back(file);
            }
            

            //Creates tar file based on name provided and writes all the files' contents into the tar file
            tarName = argv[2];
            fstream outFile(tarName, ios::out | ios::binary);

            string tarSizeString = to_string(tarSize);

            outFile.write((char *) tarSizeString.c_str(), sizeof(char) * tarSizeString.size());

            for (fileIter = finalFileVect.begin(); fileIter < finalFileVect.end(); fileIter++)
            {
                fileFile = *fileIter;
                outFile.write((char *) &fileFile, sizeof(File));

                if(fileFile.isADir() == false)
                {
                    fstream inFile(fileFile.getName().c_str(), ios::in);

                    size = stoi(fileFile.getSize());
                    char buffer[size];
                    inFile.read(buffer, size);

                    outFile.write((char*) &buffer, size);
                    inFile.close();

                }
            }
            outFile.close();
            break;
        }

        //Performs -tf argument
        case 2:
        {
            fstream inFile(argv[2], ios::in | ios::binary);

            inFile >> tarSize;

            while (inFile.read((char *) &fileFile, sizeof(File)))
            {
                cout << fileFile.getName() << endl;

                if (fileFile.isADir() == false)
                {
                    int bytes = stoi(fileFile.getSize());
                    inFile.seekg(bytes, ios::cur);
                }
            }

            break;
        }

        //Performs -xf argument
        case 3:
        {
            fstream inFile(argv[2], ios::in | ios::binary);
            string cmd;

            inFile >> tarSize;

            while (inFile.read((char *) &fileFile, sizeof(File)))
            {
                string tarName = fileFile.getName();

                if (fileFile.isADir())
                {
                    fstream check(tarName.c_str(), ios::in);

                    if (check.fail() == true)
                    {
                        //makes a directory under the tar given
                        cmd = "mkdir ";
                        cmd += tarName;
                        system(cmd.c_str());

                        //restore permissions
                        cmd = "chmod ";
                        cmd += fileFile.getPmode() + " ";
                        cmd += tarName;
                        system(cmd.c_str());

                        //restore time
                        cmd = "touch -t ";
                        cmd += fileFile.getStamp() + " ";
                        cmd += tarName;
                        system(cmd.c_str());
                        
                    }
                }
                else
                {
                    size = stoi(fileFile.getSize());
                    char buffer[size];
                    inFile.read((char *) &buffer, size);

                    fstream newFile(tarName.c_str(), ios::out);

                    newFile.write((char *) &buffer, sizeof(char) * size);

                    //restore permissions
                    cmd = "chmod ";
                    cmd += fileFile.getPmode() + " ";
                    cmd += tarName;
                    system(cmd.c_str());

                    //restore time
                    cmd = "touch -t ";
                    cmd += fileFile.getStamp() + " ";
                    cmd += tarName;
                    system(cmd.c_str());
                }
            }
            break;
        }

        //Prints --help prompt        
        case 4:
            cout << "\'jtar\' saves many files together into a single tape or disk archive, and"
                << "can restore individual files from the archive.\n\n"
                << "Usage: tar [OPTION]... [FILE]...\n\n"
                << "Option Examples:\n"
                << "  tar -cf archive.tar foo bar  # Create archive.tar from files foo and bar.\n"
                << "  tar -tf archive.tar          # List all files in archive.tar verbosely.\n"
                << "  tar -xf archive.tar          # Extract all files from archive.tar." << endl;   
                    
            exit(0);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------
IN:     1) File
        2) vector<fileInfo>
OUT:
        Puts files into vector
        If file is a directory recurs through the files within it and add it to the vector
--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fileToVector(fileInfo file, vector<fileInfo> &vect)
{
    struct fileInfo temp;

    vect.push_back(file);

    if (checkDir(file.fileName))
    {
        DIR* accessedDir = opendir(file.fileName.c_str());
        struct dirent *entPtr;

        while ((entPtr = readdir(accessedDir)) != NULL)
        {
            string dirName = entPtr->d_name;

            if (dirName == "." || dirName == "..") continue;

            temp.fileName = file.fileName + '/' + entPtr->d_name;
            temp.isDir = checkDir(temp.fileName);

            fileToVector(temp, vect);

        }
        closedir(accessedDir);
    }

}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------
IN:     1) string
OUT:
        Return bool if file is directory or not
--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bool checkDir(string fileName)
{
    struct stat checkDir;
    stat(fileName.c_str(), &checkDir);
    if (S_ISDIR(checkDir.st_mode)) return true;
    return false;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------
IN:     1) argc
        2) argv
OUT:
        Builds the months map and returns it
--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
map<string, string> buildMap()
{
    map <string, string> months;
    months["Jan"] = "01";
    months["Feb"] = "02";
    months["Mar"] = "03";
    months["Apr"] = "04";
    months["May"] = "05";
    months["Jun"] = "06";
    months["Jul"] = "07";
    months["Aug"] = "08";
    months["Sep"] = "09";
    months["Oct"] = "10";
    months["Nov"] = "11";
    months["Dec"] = "12";

    return months;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------
IN:     1) string
        2) map<string, string>
OUT:
        Gets the time formatted to be read in to touch -t
--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
string getTime(string fileName, map<string, string> months)
{
    stringstream ss;
    stringstream convertedTime;
    int day, year, hour, min, sec;
    string month;

    struct stat getFileTime;
    stat(fileName.c_str(), &getFileTime);
    time_t accessTime = getFileTime.st_atime;
    string time = ctime(&accessTime);

    ss.str(time);
    ss.ignore(3);
    ss >> month;
    ss >> day;
    ss >> hour;
    ss.ignore(1);
    ss >> min;
    ss.ignore(1);
    ss >> sec;
    ss >> year;

    convertedTime << year
                << setfill('0') << setw(2) << months[month]
                << setfill('0') << setw(2) << day
                << setfill('0') << setw(2) << hour
                << setfill('0') << setw(2) << min << "."
                << setfill('0') << setw(2) << sec;


    return convertedTime.str();
    
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------
IN:     1) string
OUT:
        Gets size of the file
--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
string getSize(string fileName)
{
    struct stat getFileSize;
    stat(fileName.c_str(), &getFileSize);
    return to_string(getFileSize.st_size);
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------
IN:     1) string
OUT:
        Gets protection value of file via hex
--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
string getProtect(string fileName)
{
    stringstream ss;

    struct stat getFileProtect;
    stat(fileName.c_str(), &getFileProtect);

    ss << (getFileProtect.st_mode & S_IRWXU)/64 << (getFileProtect.st_mode & S_IRWXG)/8 << (getFileProtect.st_mode & S_IRWXO)%8;

    return ss.str();   

}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------
IN:     1) argc
        2) argv
OUT:
        Checks for errors in the arguments for -cf, -tf, -xf, --help
        If argc is only the executable name, fail
        Checks the argument with a switch
        Returns a value corresponding to the argument type if no errors appear
--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int errorCheck(int argc, char *argv[])
{
    //If argc is only the executable name, fail
    if (argc <= 1)
    {
        cerr << "Please provide an option. \nTry 'jtar --help' for more info." << endl;
        exit(0);
    }
    
    //Checks the argument with a switch
    switch (argv[1][1])
    {
        struct stat fileCheck;

        //Checks any errors for -cf
        case 'c':
            if (argc < 4)
            {
                cerr << "Invalid argument options. \nTry 'jtar --help' for more info." << endl;
                return -1;
            }

            for (int i = 3; i< argc; i++)
            {
                if (stat(((string) argv[i]).c_str(), &fileCheck) != 0)
                {
                    cerr << argv[i] << " does not exist" << endl;
                    return -1;
                }
            }
            return 1;
        
        //Checks any errors for -tf
        case 't':
            if (argc < 3)
            {
                cerr << "Invalid argument options. \nTry 'jtar --help' for more info." << endl;
                return -1;
            }
            if (stat(((string) argv[2]).c_str(), &fileCheck) != 0)
            {
                cerr << argv[2] << " does not exist" << endl;
                return -1;
            }

            return 2;

        //Checks any errors for -xf
        case 'x':
            if (argc != 3)
            {
                cerr << "Invalid argument options. \nTry 'jtar --help' for more info." << endl;
                return -1;
            }
            if (stat(((string) argv[2]).c_str(), &fileCheck) != 0)
            {
                cerr << argv[2] << " does not exist" << endl;
                return -1;
            }

            return 3;

        //Returns if --help
        case '-':
            return 4;

        //If anything else, fail
        default:
            cerr << "Invalid argument provided. \nTry 'jtar --help' for more info." << endl;
            return -1;

    }
}

