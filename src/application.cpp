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

    m_settings.Load();

    QFile stylesheet(":/QTDark.stylesheet");
    if (stylesheet.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream ts(&stylesheet);

        setStyleSheet(ts.readAll());
    }

    setFont(m_settings.font);
}


CApplication::~CApplication()
{
}


QString CApplication::InitialRepoPath() const
{
    if(arguments().size() > 1) {
        return arguments().at(1);
    } else {
        return ".";
    }
}
