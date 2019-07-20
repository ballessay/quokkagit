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

    OnSettingsChanged();
}


CApplication::~CApplication()
{
}


void CApplication::OnSettingsChanged()
{
    SetStyle();

    SetFont();
}


void CApplication::SetFont()
{
    setFont(m_settings.font);
}


void CApplication::SetStyle()
{
    if (quokkagit::SStyleSettings::Dark == m_settings.style.id)
    {
        QFile stylesheet(":/QTDark.stylesheet");
        if (stylesheet.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream ts(&stylesheet);

            setStyleSheet(ts.readAll());
        }
    }
    else
    {
        setStyleSheet(QString());
    }
}
