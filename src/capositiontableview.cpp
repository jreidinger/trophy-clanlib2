#include "calabel.h"
#include "player.h"
#include "capositiontableview.h"
#include "catrophy.h"
#include <string>
#include <sstream>



/** Constructor.
    \param race true: lap results / false: total results
*/
CAPositionTableView::CAPositionTableView()
        : CADialog() {
    resize( 600, 340 );
    label = new CALabel( "", CAWidget::Left, CA_RES.font_normal_11_white );
}



/** Destructor.
*/
CAPositionTableView::~CAPositionTableView() {
    delete label;
}


/** Builds the menu screen.
*/
void
CAPositionTableView::buildScreen() {
    CADialog::buildScreen();

    int y;                         // Y-Pos of text line
    int pl;                        // Counter for players
    int rank;                      // Rank of current player
    int points;                    // Points of current player
    std::string timeStr;           // Time string


    for( pl=0; pl<CA_RACEMAXPLAYERS; ++pl ) {

		rank = CA_APP->m_RacePlayer[pl]->getRaceRank();
		points = CA_APP->m_RacePlayer[pl]->getRacePoints();
		TrophyMath::timeToString( timeStr, CA_APP->m_RacePlayer[pl]->getRaceTime() );

        if( rank!=0 )
        {
            y = top + 32 + rank*16;

            // Rank:
            //
            std::ostringstream ossRank;
            ossRank << rank << ".";
            CA_RES.font_normal_11_white.draw_text( *CA_APP->graphicContext, left + 40, y, ossRank.str() );

            // Player name:
            //
            CA_RES.font_normal_11_white.draw_text( *CA_APP->graphicContext, left + 64, y, CA_APP->m_RacePlayer[pl]->getName() );

            // Race time:
            //
            label->setText( timeStr );
            label->move( left + 240, y );
            label->display();

            // Player points:
            //
            std::ostringstream ossPoint;
            ossPoint << points;
            label->setText( ossPoint.str() );
            label->move( left + 360, y );
            label->display();

            // Money:
            //
            std::ostringstream ossMoney;
            ossMoney << "$" << CA_APP->m_RacePlayer[pl]->getRaceMoney();
            label->setText( ossMoney.str() );
            label->move( left + 420, y );
            label->display();

            // Car name:
            //
            CA_RES.font_normal_11_white.draw_text( *CA_APP->graphicContext, left + 500, y, CA_APP->m_RacePlayer[pl]->getCar()->name );
        }
    }

}



/** Called on button releases
*/
void
CAPositionTableView::on_key_released (const CL_InputEvent &key) 
{
    switch( key.id ) {
    case CL_KEY_ENTER:
        done = true;
        break;

    case CL_KEY_ESCAPE:
        done = true;
        cancel = true;
        break;

    default:
        break;
    }
}

// EOF
