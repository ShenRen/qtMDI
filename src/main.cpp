#include "qtMDI.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    qtMDI w(argc, argv);
    w.show();
    return a.exec();
}
