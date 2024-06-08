#include <sstream>
#include <iostream>
#include <map>

using namespace std;

int formatCal(string date);

int main()
{
    string date = "February 14, 2023";
    cout << formatCal(date) << endl;
    return 0;
}

int formatCal(string date)
{
    map<string, string> months = {{"January","1"},
                              {"February","2"},
                              {"March","3"}, 
                              {"April","4"}, 
                              {"May","5"}, 
                              {"June","6"}, 
                              {"July","7"}, 
                              {"August","8"}, 
                              {"September","9"}, 
                              {"October","10"}, 
                              {"November","11"}, 
                              {"December","12"}};

    stringstream ss(date);
    string month;
    string day, year;
    
    getline(ss, month, ' ');
    getline(ss, day, ',');
    ss >> year;   

    return stoi(months[month] + day + year);
}