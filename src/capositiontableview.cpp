#include <stdio.h>
#include "calabel.h"
#include "player.h"
#include "capositiontableview.h"
#include "catrophy.h"


/** Constructor.
    \param race true: lap results / false: total results
*/
CAPositionTableView::CAPositionTableView( bool race )
        : CADialog() {
    resize( 600, 340 );
    this->race = race;
    label = new CALabel( "", CAWidget::Left, CA_RES->font_normal_11_white );
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
    char rankStr[5];               // Position string
    int points;                    // Points of current player
    char pointStr[16];             // Points string
    char moneyStr[32];             // Money - there's no limit for money
    char timeStr[16];              // Time string
    //bool rankTaken[CA_MAXPLAYERS]; // Rank already taken by another player -> move down

    //for( pl=0; pl<CA_MAXPLAYERS; ++pl ) {
    //  rankTaken[pl] = false;
    //}

    for( pl=0; pl<CA_MAXPLAYERS; ++pl ) {

        if(race) {
            rank = CA_APP->player[pl]->getRaceRank();
            points = CA_APP->player[pl]->getRacePoints();
            CAMath::timeToString( timeStr, CA_APP->player[pl]->getRaceTime() );
        } else {
            rank = CA_APP->player[pl]->getTotalRank();
            points = CA_APP->player[pl]->getTotalPoints();
            strcpy( timeStr, "" );
        }

        // Rank already taken -> move to the next free below.
        //while( rankTaken[rank-1] && rank-1<CA_MAXPLAYERS ) ++rank;

        //rankTaken[rank-1] = true;

        if( rank!=0 ) {
            y = top + 32 + rank*16;

            // Rank:
            //
            sprintf( rankStr, "%d.", rank );
            CA_RES->font_normal_11_white->set_alignment(origin_top_right, 0, 0);
            CA_RES->font_normal_11_white->draw( left + 40, y, rankStr );

            // Player name:
            //
            CA_RES->font_normal_11_white->set_alignment(origin_top_left, 0, 0);
            CA_RES->font_normal_11_white->draw( left + 64, y, CA_APP->player[pl]->getName() );

            // Race time:
            //
            label->setText( timeStr );
            label->move( left + 240, y );
            label->display();

            // Player points:
            //
            sprintf( pointStr, "%d", points );
            label->setText( pointStr );
            label->move( left + 360, y );
            label->display();

            // Money:
            //
            sprintf( moneyStr, "$%d", CA_APP->player[pl]->getMoney() );
            label->setText( moneyStr );
            label->move( left + 420, y );
            label->display();

            // Car name:
            //
            CA_RES->font_normal_11_white->set_alignment(origin_top_left, 0, 0);
            CA_RES->font_normal_11_white->draw( left + 500, y, CA_APP->player[pl]->getCarType()->name );
            /*label->setText( CA_APP->player[pl]->getCarType().name );
            label->move( left + 420, y );
            label->display();*/


            /*
            sprintf( line,
                     "%s %s %s %s %s",
                     rankStr,
                     CA_APP->player[pl]->getName().get_string(),
                     timeStr, pointStr, moneyStr );

            cout << line << endl;
            */
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
