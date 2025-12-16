#include "mainwindow.h"
#include "xml_editor/cli.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    if (argc > 1) {
        xml_editor::cli::run_cli(argc, argv);
        return 0;
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
