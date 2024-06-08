#include <fstream>
#include <iostream>

using namespace std;

void wc(string fileName, bool lineBool, bool wordBool, bool byteBool);

int main(int argc, char *argv[])
{
    string fileName = argv[argc-1];
    bool lineBool = false;
    bool wordBool = false;
    bool byteBool = false;

    if (argc > 2)
    {
        for (int i = 1; i < argc-1; i++)
        {
            //line count
            if ((string) argv[i] == "-l") lineBool = true;

            //word count
            if ((string) argv[i] == "-w") wordBool = true;

            //byte count
            if ((string) argv[i] == "-c") byteBool = true;

        }
    }
    if (lineBool==false && wordBool==false && byteBool==false) lineBool = true, wordBool = true, byteBool = true;
    
    wc(fileName, lineBool, wordBool, byteBool);
}

void wc(string fileName, bool lineBool, bool wordBool, bool byteBool)
{
    fstream inOut(fileName.c_str(), ios::in | ios::out);
    int lineCount = 0;
    int wordCount = 1;
    int byteCount = 0;
    char ch;

    if (!inOut) { cout << "File does not exist!" << endl; return;}

    inOut.seekg(0, ios::end);
    int fileLength = inOut.tellg();
    if (fileLength == 0) 
    {
        int lineCount = 0;
        int wordCount = 0;
        int byteCount = 0;
        cout << lineCount << " " << wordCount << " " << byteCount << " " << fileName << endl;
        return;
    }
    
    inOut.seekg(0);
    while (inOut.get(ch)) //!inOut.eof()
    {        
        if (ch == ' ')
        {
            wordCount++;
        }
        if (ch == '\n')
        {
            lineCount++;
            wordCount++;
        }

        byteCount++;
    }

    if (inOut.eof())
    {
        inOut.seekg(-1, ios::end);
        inOut.get(ch);
        if (ch == '\n') wordCount--;
    }

    inOut.close();

    string lineCountOutput = to_string(lineCount) + " ";
    string wordCountOutput = to_string(wordCount) + " ";
    string byteCountOutput = to_string(byteCount) + " ";

    if (lineBool == false) lineCountOutput = "";
    if (wordBool == false) wordCountOutput = "";
    if (byteBool == false) byteCountOutput = "";

    cout << lineCountOutput << wordCountOutput <<  byteCountOutput << fileName << endl;
}