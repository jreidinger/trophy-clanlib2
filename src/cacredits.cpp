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

    text.push_back("");

    text.push_back("- Programming -");
    text.push_back("");
    text.push_back("Andrew Mustun");
    text.push_back("andrew@mustun.com");
    text.push_back("Matthieu Lecesne");
    text.push_back("matthieulecesne@yahoo.fr");
    text.push_back("Colin Pitrat");
    text.push_back("colin.pitrat@gmail.com");
    text.push_back("");
    text.push_back("");
    text.push_back("- Graphics -");
    text.push_back("");
    text.push_back("Andrew Mustun");
    text.push_back("andrew@mustun.com");
    text.push_back("Colin Pitrat");
    text.push_back("colin.pitrat@gmail.com");
    text.push_back("");
    text.push_back("");
    text.push_back("- Additional Tracks -");
    text.push_back("");
    text.push_back("Volker Schatz (Downtown)");
    text.push_back("Oliver Luethje (Zigzag, Snake, Rally)");
    text.push_back("");
    text.push_back("");
    text.push_back("- Beta Testing / Feedback -");
    text.push_back("");
    text.push_back("Martin Mustun, Michael Mustun,");
    text.push_back("Chris Bruengger, Markus Krieter");
    text.push_back("Armijn Hemel, Guus Sliepen, Alfred de Wijn");
    text.push_back("Kenneth Gangstoe, Martin Starch Soerensen");
    text.push_back("");
    text.push_back("");
    text.push_back("- Graphics Library ClanLib -");
    text.push_back("");
    text.push_back("ClanSoft");
    text.push_back("http://www.clanlib.org");
    text.push_back("");
    text.push_back("---");
    text.push_back("");
    text.push_back("Many thanks to all these people and everyone");
    text.push_back("else who contributed something to Trophy!");
    text.push_back("");
    text.push_back("");     
}

void CACredits::setText(const std::vector<std::string>& newText)
{
    text=newText;
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

    int textHeight = 26 * text.size();

    // Scrolling text:
    //
    CL_Rect crAll( 0,0, CA_APP->width, CA_APP->height );
    CL_Rect crArea( left + 32, top+32, right-32, bottom-32 );

    CA_RES->advanceAnimation( &scrollPos, textHeight-(bottom-top-64), 32.0, CAResources::Revolving );

    CA_APP->graphicContext->set_cliprect( crArea );
    //CL_Display::fill_rect( left,top, right,bottom, 0.0,0.0,0.0, 0.2 );
    for( unsigned int l=0; l<text.size(); ++l ) 
    {
        const std::string item = text[l];
        CA_RES->font_normal_14_white.draw_text ( *CA_APP->graphicContext, (left+32+right-32)/2, (int)(top+32+26-scrollPos+l*26), item );
    }
    CA_APP->graphicContext->set_cliprect( crAll );
}



/** Called on key released.
*/
void
CACredits::on_key_released( const CL_InputEvent &key, const CL_InputState & state ) 
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
