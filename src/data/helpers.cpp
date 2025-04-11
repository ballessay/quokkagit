#include "helpers.h"
#include "git2cpp/id_to_str.h"
#include <QString>


git_oid turtlegit::helpers::OidFrom(const QString& text)
{
    return git::str_to_id(text.toUtf8().constData());
}


QString turtlegit::helpers::QStringFrom(const git_oid& oid)
{
    return git::id_to_str(oid).c_str();
}
