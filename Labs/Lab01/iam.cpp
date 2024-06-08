#include <iostream>

using namespace std;

class Base
{
    public:
        virtual void iam();
};

void Base::iam()
{
    cout << "base" << endl;
}

class One : public Base
{
    public:
        virtual void iam()
        {
            cout << "one" << endl;
        }
        void iam2()
        {
            Base :: iam();
        }
};

class Two : public Base
{
    public:
        virtual void iam()
        {
            cout << "two" << endl;
        }
        void iam2()
        {
            Base :: iam();
        }
};

int main()
{
    One derivedOne;
    Two derivedTwo;

    cout << "derivedOne iam(): "; derivedOne.iam();
    cout << "derivedOne iam2(): "; derivedOne.iam2(); cout << endl;
    cout << "derivedTwo iam(): "; derivedTwo.iam();
    cout << "derivedTwo iam2(): "; derivedTwo.iam2(); cout << endl;

    Base* derivedOnePtr = &derivedOne;
    Base* derivedTwoPtr = &derivedTwo;

    cout << "derivedOne Pointer iam(): "; derivedOnePtr->iam();
    cout << "derivedTwo Pointer iam2(): "; derivedTwoPtr->iam();

}