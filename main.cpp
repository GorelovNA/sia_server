#include <QCoreApplication>
#include <sia_server.h>

int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    paths.append("sqldrivers");
    QCoreApplication::setLibraryPaths(paths);



    QCoreApplication a(argc, argv);

    sia_server SERV;

    return a.exec();
}
