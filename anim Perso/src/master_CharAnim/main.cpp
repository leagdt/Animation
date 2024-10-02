#include "CharAnimViewer.h"

using namespace std;



#ifndef NO_MAIN


int main( int argc, char **argv )
{
    // si on lance le programme avec la commmande suivante : 
    // ./bin/master_CharAnim mg on lancera la version avec le motion graphe 
    // sinon on lance la version avec la machine à état
    if (argc > 1) {
        std::string firstArg = argv[1];
        CharAnimViewer v(firstArg == "mg");
        //Viewer v;
    //    ViewerBasic v;
        v.run();
        return 0;
    } 
    CharAnimViewer v;
    //Viewer v;
//    ViewerBasic v;
    v.run();
    return 0;
}

#endif
