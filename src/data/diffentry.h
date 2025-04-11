#ifndef DIFFDATA_H
#define DIFFDATA_H

#include <git2/oid.h>

#include <QByteArray>
#include <QString>


namespace turtlegit
{
    struct SFile
    {
        git_oid oid;
        QString path;
        QString id;
    };

    struct SDelta
    {
        QString status;
        QString display;
        SFile oldFile;
        SFile newFile;
    };

    struct SDiffEntry
    {
        QByteArray oldData;
        QByteArray newData;
        QString oldFilename;
        QString newFilename;
        QString oldHash;
        QString newHash;
    };
}

#endif // DIFFDATA_H
