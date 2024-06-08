#include <fstream>
#include <cstdlib>
#include <cstring>         
#include <iostream>
using namespace std;
#define RECSIZ 1024           
#define NUMREC 128            
#define FILNAME "password.dat"

#define NUMBITS (RECSIZ * NUMREC * 8)

// The following function must be provided
//unsigned int hashIt(char *P) ;
unsigned int hashIt( const string& key );

int main(int argc, char *argv[]) {
   unsigned char updBuff ;
   fstream updStream(FILNAME, ios::in | ios::out | ios::binary) ;

   if (!updStream) {
      cerr << "Unable to open " << FILNAME << endl ;
      exit(1) ;
   }


   if (argc < 2) {
      cerr << "No new password given" << endl ;
      exit(1) ;
   }

   unsigned int hashNum = hashIt(argv[1]) % NUMBITS ;
   unsigned int hashBytpos = hashNum/8 ;	// byte position
   unsigned int hashBitpos = hashNum%8 ;	// bit position within byte
   int bitMask = 1<<hashBitpos ;		// bit mask

   updStream.seekg(hashBytpos, ios::beg) ;
   updStream.read((char *) &updBuff, 1) ;
   if (bitMask & updBuff)
      cout << "Password " << argv[1] << " may have been used" << endl ;
   else {
      cout << "Password " << argv[1] << " has not been used" << endl ;
      updStream.seekp(hashBytpos, ios::beg) ;
      updBuff |= bitMask ;
      updStream.write((char *) &updBuff, 1) ;
      if (!updStream)
         cout << "Write failed" << endl ;
   }

   updStream.close() ;
   return 0;
}

unsigned int hashIt( const string& key )
{
            unsigned int hashVal = 0;

            for( int i = 0; i < key.length( ); i++ )
                hashVal += key[ i ];

            return hashVal;
}

