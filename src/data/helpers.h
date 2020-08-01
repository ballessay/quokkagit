#ifndef HELPERS_H
#define HELPERS_H

struct git_oid;
class QString;


namespace quokkagit
{
    namespace helpers
    {
        git_oid OidFrom(const QString& text);

        QString QStringFrom(const git_oid& oid);
    }
}

#endif // HELPERS_H
