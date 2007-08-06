#include <stdio.h>

#include "canetclient.h"
#include "catrophy.h"


/** Constructor.
*/
CANetClient::CANetClient( CAPlayer* player ) {
    //netGame = 0;
    //this->player = player;
}



/** Destructor.
*/
CANetClient::~CANetClient() {
    /*if( netGame ) {
      delete netGame;
      netGame = 0;
}*/
}



/** Joins a network game with given IP (CA_APP->serverIp)
    on given port (CA_APP->serverPort).
  \return true on success otherwise false
*/
bool
CANetClient::joinGame() {
    bool ret = false;

    /*
    try {
      CL_Network::find_game_at( "trophy", CA_APP->serverIp, CA_APP->serverPort );
      netGame = CL_Network::receive_game_found( 5000 );
      ret = true;
}
    catch( CL_Error err ) {
      std::cout << "Fatal client error: " << err.message.c_str() << std::endl;
      ret = false;
}
    */

    return ret;
}



/** Sends current data to the server.
*/
void
CANetClient::keepUp() {
    /*
    if( netGame ) {
      static float start_time = CL_System::get_time() / 1000.0f;
       float cur_time = CL_System::get_time() / 1000.0f;
       if( cur_time - start_time > 2.0 ) {// every 0.25 sec
         start_time = cur_time;
       
         // Send data from this player to server:
         //
         char string[256];
         sprintf( string,
                  "%d~%d~%d~%d",
                  player->getId(),
                  (int)player->getX(),
                  (int)player->getY(),
                  (int)player->getDirection() );
         
         CL_NetMessage msg;
         msg.data = string;
         netGame->send( 0, netGame->get_all(), msg );
         
         
         // Receive data for other players:
         //
        try {
          CL_NetMessage msg = netGame->receive( 0 );
          //std::cout << "Server said: " << msg.data.c_str() << std::endl;
        
          CAStringList slist( msg.data.c_str() );
        
          // Server tells us id:
          //
          if( !strcmp( slist.getItem( 0 ), "id" )  ) {
            player->setId( atoi( slist.getItem( 1 ) ) );
          }
        
          // Other player does something:
          //
          else {  
            int id = atoi( slist.getItem( 0 ) );
            int x = atoi( slist.getItem( 1 ) );
            int y = atoi( slist.getItem( 2 ) );
            int dir = atoi( slist.getItem( 3 ) );
          
            if( id!=player->getId() ) {
               CA_APP->controlNetPlayer( id, x, y, dir );
             }
          }
        }
        catch (...) { }
      }
}
    */
}

// EOF

