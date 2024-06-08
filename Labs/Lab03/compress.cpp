#include <iostream>
#include <sstream>
#include <string>

using namespace std;

string compress(string input);
string paddingCheck(string prev, string curr, string tmpString, string output, int count);
string uncompress(string input);

int main(int argc, char *argv[])
{
    string input = "93 93 93 93 92 91 91 94 94 94 94 94 95 95 95 73 73 73 73 73 73 73";
    cout << "    Original: " << input << endl;

    string compressedInput = compress(input);
    cout << "  Compressed: " << compressedInput << endl;
    cout << "Uncompressed: " << uncompress(compressedInput) << endl;

    return 0;
}

//Compress inputed info
string compress(string input)
{
    stringstream ss(input);
    string prev, curr;
    string output, tmpString;
    int count = 1;
    
    ss >> prev;
    tmpString = prev + " ";

    while (ss >> curr)
    {
        if (curr == prev)
        {
            tmpString = tmpString + curr + " ";
            count++;
        }
        else
        {
            output = paddingCheck(prev, curr, tmpString, output, count);
            prev = curr;
            count = 1;
            tmpString = curr + " ";
        }
    }
    output = paddingCheck(prev, curr, tmpString, output, count);

    return output;
}

//Adds 0 padding when hex value only 1 character in length
string paddingCheck(string prev, string curr, string tmpString, string output, int count)
{
    if (count < 4)
        output += tmpString;
    else
    {
        string countString = to_string(count);
        int padCount = 2 - countString.length();
        output += "ff " + prev + " " + countString.insert(0, padCount, '0') + " ";
    }

    return output;
}

//Uncompreses compressed input
string uncompress(string input)
{
    stringstream ss(input);
    string ffCheck, num, count;
    string output;

    while (ss >> ffCheck)
    {
        if (ffCheck == "ff")
        {
            ss >> num >> dec >> count;

            for (int i = 0; i < stoi(count); i++)
                output += num + " ";

        }
        else
        {
            stringstream tempss(ffCheck);
            tempss >> ffCheck;
            output += ffCheck + " ";
        }
    }
    return output;
}
