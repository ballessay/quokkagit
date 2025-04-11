#ifndef APPLICATION_H
#define APPLICATION_H

#include "data/settings.h"
#include <QApplication>


class CApplication : public QApplication
{
public:
    CApplication(int& argc, char** argv);
    ~CApplication() override;

    turtlegit::SSettings& settings() { return m_settings; }
    const turtlegit::SSettings& settings() const { return m_settings; }

public slots:
    void OnSettingsChanged();

private:
    void SetFont();
    void SetStyle();

    turtlegit::SSettings m_settings;
};

#endif // APPLICATION_H
