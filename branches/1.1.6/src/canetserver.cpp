#include <stdio.h>

#include "canetserver.h"
#include "catrophy.h"

/** Constructor.
*/
CANetServer::CANetServer() {
    //netGame = 0;
}



/** Destructor.
*/
CANetServer::~CANetServer() {
    /*
    if( netGame ) {
      delete netGame;
      netGame = 0;
}
    */
}



/** Creates a new network game.
*/
bool
CANetServer::createGame() {
    /*
    try {
      netGame = CL_Network::create_game( "trophy", CA_APP->serverPort );
}
    catch(CL_Error err) {
      std::cout << "Could not create server: " << err.message.c_str() << std::endl;
      return false;
}
    */
    return true;
}



/** Keep up the server if there was one created with createGame().
*/
void
CANetServer::keepUp() {
    /*
    if( netGame ) {
      static float start_time = CL_System::get_time() / 1000.0f;
       float cur_time = CL_System::get_time() / 1000.0f;
       if( cur_time - start_time > 0.5 ) { // every 0.25 sec
         start_time = cur_time;
        
         // check network events (new computers, etc.):

         // check for joining computers
         //  give full access to channel zero.
         //  send id to user
         //
         const CL_NetComputer *computer = netGame->receive_computer_join();
         if( computer != NULL ) {
           std::cout << "Computer joined." << std::endl;
           netGame->set_access( 0, computer );
           
           int id = CA_APP->addNetPlayer();
           if( id!=-1 ) {
             char string[255];
             sprintf( string, "id~%d", id );
             CL_NetMessage msg;
             msg.data = string;
             netGame->send( 0, computer, msg );
           }
         }
         
         // Receive and forward messages:
         //
         try {
          CL_NetMessage msg = netGame->receive(0);
          //std::cout << "Server received: " << msg.data.c_str() << std::endl;
        
           netGame->send( 0,
                         netGame->get_all(),
                         msg );    
        }
        catch (...)
        {
        }
      }
}
    */
}


// EOF
