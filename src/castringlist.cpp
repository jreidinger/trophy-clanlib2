#include "castringlist.h"



/** Constructor.
*/
CAStringList::CAStringList( char separator ) {
    this->list = "";
    this->separator = separator;
}



/** Constructor.
*/
CAStringList::CAStringList( const char* list, char separator ) {
    this->list = list;
    this->separator = separator;
}



/** Destructor.
*/
CAStringList::~CAStringList() {}



/** Count.
*/
int
CAStringList::count() {
    int ret=1;

    for( unsigned int i = 0; i < list.length(); ++i ) 
    {
        if( list[i]==separator ) {
            ++ret;
        }
    }

    return ret;
}



/** Returns item number 'which' of the list.
    \param maxLen Max length of the sting to return. Default: 128
*/
const char*
CAStringList::getItem( int which, int maxLen ) {
    unsigned int i = 0;
    int j = 0;
    int found = 0;
    item = "";

    while( i < list.length() && found < which ) 
    {
        if( list[i]==separator ) {
            ++found;
        }
        ++i;
    }

    j = list.find( '~', i );

    if( j == -1 ) 
    {
        j = list.length();
    }

    item = list.substr( i, j-i );

    return item.c_str();
}



// EOF
