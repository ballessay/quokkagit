#ifndef APPLICATION_H
#define APPLICATION_H

#include "data/settings.h"
#include <QApplication>

class CApplication : public QApplication
{
public:
    CApplication(int& argc, char** argv);
    ~CApplication() override;

    quokkagit::SSettings& settings() { return m_settings; }
    const quokkagit::SSettings& settings() const { return m_settings; }

private:
    quokkagit::SSettings m_settings;
};

#endif // APPLICATION_H
