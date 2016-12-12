#include <QGuiApplication>
#include "solver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Solver solver;

    return app.exec();
}
