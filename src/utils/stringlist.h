#ifndef STRINGLIST_H
#define STRINGLIST_H

#include <ClanLib/core.h>

#define SL_ITEMSIZE  128

/** A simple string list implemented by strings with given separators.
    It's not fast, not good, but simple.
    @author Andrew Mustun
*/
class StringList 
{
    public:
        StringList( char separator='~' );
        StringList( const char* list, char separator='~' );
        ~StringList();

        /** Sets the string list.
          \param list The new list in format "entry 1~entry2~entry3".
          */
        void   setList( const char* list ) { this->list = list; }
        int    count();
        const char*  getItem( int which, int maxLen=SL_ITEMSIZE );

    private:
        //! The list as one string.
        std::string list;
        //! One item. Which depends on the last action.
        std::string item;
        //! Separator character (default: '~').
        char   separator;
};

#endif
