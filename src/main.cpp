#include "application.h"
#include "widgets/mainwindow.h"
#include "tools/git2wrapper.h"
#include "git2cpp/initializer.h"
#include <QSplashScreen>
#include <QString>
#include <QMessageBox>


int main(int argc, char** argv)
{
  auto_git_initializer;

  CApplication app(argc, argv);

  QPixmap pixmap(":/quokka.jpg");
  QSplashScreen splash(pixmap);
  splash.show();
  app.processEvents();

  try
  {
    splash.showMessage("Init git...");
    CGit2Wrapper git(app.InitialRepoPath());
    git.Initialize();

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
