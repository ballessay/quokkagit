#include "mainwindow.h"
#include "git2wrapper.h"
#include "git2cpp/initializer.h"
#include <QApplication>
#include <QSplashScreen>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

namespace
{
  QString InitialPath()
  {
    if(qApp->arguments().size() > 1) {
      return qApp->arguments().at(1);
    } else {
      return qApp->applicationDirPath();
    }
  }
}


int main(int argc, char** argv)
{
  auto_git_initializer;

  QApplication app(argc, argv);

  app.setOrganizationName("ballessay");
  app.setOrganizationDomain("ballessay.de");
  app.setApplicationName("qtgit");


  QFile stylesheet( ":/QTDark.stylesheet");
  stylesheet.open(QFile::ReadOnly | QFile::Text);
  QTextStream ts(&stylesheet);

  app.setStyleSheet(ts.readAll());

  QPixmap pixmap(":/quokka.jpg");
  QSplashScreen splash(pixmap);
  splash.show();
  app.processEvents();

  try
  {
    splash.showMessage("Init git...");
    CGit2Wrapper git(InitialPath());

    CMainWindow w(git);
    w.show();

    splash.showMessage("Init GUI...");

    splash.finish(&w);

    return app.exec();
  }
  catch(std::exception& ex)
  {
    QMessageBox::warning(nullptr, QObject::tr("Error"),
                         QObject::tr("Exception occured: %1").arg(ex.what()));
  }

  return 1;
}
