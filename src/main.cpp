#include "application.h"
#include "data/settings.h"
#include "widgets/mainwindow.h"
#include "tools/git2wrapper.h"
#include "git2cpp/initializer.h"
#include <QString>
#include <QMessageBox>


int main(int argc, char** argv)
{
    auto_git_initializer;

    CApplication app(argc, argv);

    try
    {
        CGit2 git(app.settings());

        CMainWindow w(git, app.settings());
        w.show();

        return app.exec();
    }
    catch(std::exception& ex)
    {
        QMessageBox::warning(nullptr, QObject::tr("Error"),
                             QObject::tr("Exception occured: %1").arg(ex.what()));
    }

    return 1;
}
