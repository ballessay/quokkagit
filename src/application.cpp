#include "application.h"
#include <QFile>
#include <QTextStream>

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
}


QString CApplication::InitialRepoPath() const
{
  if(arguments().size() > 1) {
    return arguments().at(1);
  } else {
    return ".";
  }
}
