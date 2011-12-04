#include "calineedit.h"
#include "caresources.h"
#include "catrophy.h"

/** Constructor.
    \param text The initial text of the edit.
    \param x    X-Coordinate (left or right)
    \param x    Y-Coordinate (top)
    \param letters Number of letters (max)
    \param alignment Alignment of text
    \param font Font to use for the text
*/
CALineEdit::CALineEdit( const std::string text,
                        unsigned int letters,
                        Alignment alignment,
                        CL_Font_Sprite font )
        : CAWidget( alignment ) {
    if( CA_APP->debug ) std::cout << "CALineEdit() begin" << std::endl;

    this->text = text;
    this->letters = letters;
    this->font = font;
    cursor = this->text.length();

    resize( font.get_font_metrics().get_max_character_width()*letters, font.get_font_metrics().get_height() );

    if( CA_APP->debug ) std::cout << "CALineEdit() end" << std::endl;
}



/** Destructor.
*/
CALineEdit::~CALineEdit() {}



/** Displays the line edit.
    \param active Display in an active state (cursor visible)
*/
void
CALineEdit::display( bool active ) 
{
    static float frame = 0.0;
    CA_RES->advanceAnimation( &frame, 1, 1.0 );

    switch( alignment ) 
    {
    case Left:
        font.draw_text (*CA_APP->graphicContext, left, top, text );
        if( active && frame>0.5 ) {
            font.draw_text ( *CA_APP->graphicContext, left + font.get_text_size( *CA_APP->graphicContext, text.substr(0, cursor) ).width, top, "|" );
        }
        break;

    case Right:
        font.draw_text (*CA_APP->graphicContext, right, top, text );
        if( active && frame>0.5 ) {
            font.draw_text(*CA_APP->graphicContext, right - font.get_text_size( *CA_APP->graphicContext,text.substr(cursor)).width + 3, top, "|" );
        }
        break;

    case Center:
        // To implement.

        break;
    }
}



/** Handles the key inputs.
*/
void
CALineEdit::handleKey (const CL_InputEvent &key) 
{
    char dummy[256];

    switch( key.id ) 
    {
    case CL_KEY_RIGHT:
        if( CA_APP->sound ) CA_RES->effectMenu->play( 1 );
        if( cursor < text.length() ) ++cursor;
        break;

    case CL_KEY_LEFT:
        if( CA_APP->sound ) CA_RES->effectMenu->play( 1 );
        if( cursor>0 ) --cursor;
        break;

    case CL_KEY_HOME:
        if( CA_APP->sound ) CA_RES->effectMenu->play( 1 );
        cursor=0;
        break;

    case CL_KEY_END:
        if( CA_APP->sound ) CA_RES->effectMenu->play( 1 );
        cursor = text.length();
        break;

    case CL_KEY_BACKSPACE:
        if( cursor > 0 ) 
        {
            if( CA_APP->sound ) CA_RES->effectMenu->play( 1 );
            --cursor;
            sprintf( dummy,
                     "%s%s",
                     text.substr(0,cursor).c_str(),
                     text.substr(cursor+1).c_str() );
            text = dummy;
        }
        break;

    case CL_KEY_DELETE:
        if( cursor < text.length() ) 
        {
            if( CA_APP->sound ) CA_RES->effectMenu->play( 1 );
            sprintf( dummy,
                     "%s%s",
                     text.substr(0,cursor).c_str(),
                     text.substr(cursor+1).c_str() );
            text = dummy;
        }
        break;

    default:
        if( key.str.length() != 0 ) 
        {
            if( text.length() + key.str.length() <= letters ) 
            {
                if( CA_APP->sound ) CA_RES->effectMenu->play( 1 );
                sprintf( dummy,
                        "%s%s%s",
                        text.substr(0,cursor).c_str(),
                        key.str.c_str(),
                        text.substr(cursor).c_str() );
                text = dummy;
                ++cursor;
            }
        }
        break;
    }
}


// EOF
