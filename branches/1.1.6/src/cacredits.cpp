#include "cacredits.h"
#include "caresources.h"
#include "caimagemanipulation.h"
#include "catrophy.h"


/** Constructor.
*/
CACredits::CACredits()
        : CADialog() {
    resize( 600, 300 );

    title = "C R E D I T S";

    scrollPos = 0.0;
    text.setList( "~"
                  "- Graphics & Programming -~"
                  "~"
                  "Andrew Mustun~"
                  "andrew@mustun.com~"
                  "Colin Pitrat~"
                  "colin.pitrat@gmail.com"
                  "~"
                  "~"
                  "- Additional Tracks -~"
                  "~"
                  "Volker Schatz (Downtown)~"
				  "Oliver Luethje (Zigzag, Snake, Rally)"
                  "~"
                  "~"
                  "- Beta Testing / Feedback -~"
                  "~"
                  "Martin Mustun, Michael Mustun,~"
                  "Chris Bruengger, Markus Krieter~"
                  "Armijn Hemel, Guus Sliepen, Alfred de Wijn~"
                  "Kenneth Gangstoe, Martin Starch Soerensen~"
                  "~"
                  "~"
                  "- Graphics Library ClanLib -~"
                  "~"
                  "ClanSoft~"
                  "http://www.clanlib.org~"
                  "~"
                  "---~"
                  "~"
                  "Many thanks to all these people and everyone~"
                  "else who contributed something to Trophy!~"
                  "~"
                  "~" );
}



/** Destructor.
*/
CACredits::~CACredits() {}



/** Builds the screen.
*/
void
CACredits::buildScreen() 
{
    CADialog::buildScreen();

    int textHeight = 26 * text.count();

    // Scrolling text:
    //
    CL_Rect crAll( 0,0, CA_APP->width, CA_APP->height );
    CL_Rect crArea( left + 32, top+32, right-32, bottom-32 );

    CA_RES->advanceAnimation( &scrollPos, textHeight-(bottom-top-64), 32.0, CAResources::Revolving );

    CL_Display::set_cliprect( crArea );
    //CL_Display::fill_rect( left,top, right,bottom, 0.0,0.0,0.0, 0.2 );
    for( int l=0; l<text.count(); ++l ) 
    {
        const char* item = text.getItem( l );
        CA_RES->font_normal_14_white->set_alignment(origin_top_center, 0, 0);
        CA_RES->font_normal_14_white->draw ( (left+32+right-32)/2, (int)(top+32+26-scrollPos+l*26), item );
    }
    CL_Display::set_cliprect( crAll );
}



/** Called on key released.
*/
void
CACredits::on_key_released( const CL_InputEvent &key ) 
{
    switch( key.id ) {

        // Cancel (ESC):
        //
    case CL_KEY_ESCAPE:
        cancel = true;
        done = true;
        break;

        // Activate:
        //
    case CL_KEY_ENTER:
    case CL_KEY_SPACE:
        done = true;
        break;

    default:
        break;
    }
}

// EOF
