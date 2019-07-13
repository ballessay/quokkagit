#include "application.h"
#include <QFile>
#include <QTextStream>
#include <QFont>

CApplication::CApplication(int& argc, char** argv)
    : QApplication (argc, argv)
{
    setOrganizationName("ballessay");
    setOrganizationDomain("ballessay.de");
    setApplicationName("quokkagit");

    QFile stylesheet( ":/QTDark.stylesheet");
    if (stylesheet.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream ts(&stylesheet);

        setStyleSheet(ts.readAll());
    }

    QFont monoFont("Hack", 8);
    setFont(monoFont);
}


QString CApplication::InitialRepoPath() const
{
    if(arguments().size() > 1) {
        return arguments().at(1);
    } else {
        return ".";
    }
}
