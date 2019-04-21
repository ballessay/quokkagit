#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class CApplication : public QApplication
{
public:
  CApplication(int& argc, char** argv);

  QString InitialRepoPath() const;
};

#endif // APPLICATION_H
