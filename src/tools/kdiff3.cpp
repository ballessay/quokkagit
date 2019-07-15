#include "kdiff3.h"
#include "data/diffentry.h"


CKdiff3::CKdiff3(const quokkagit::SDiffSettings& settings,
                 const quokkagit::SDiffEntry& entry,
                 int index)
    : CDiffTool(settings,
                entry,
                index)
{
}


QStringList CKdiff3::args() const
{
    QStringList args;
    args << "--L1"
         << m_entry.oldFilename + "@" +m_entry.oldHash
         << "--L2"
         << m_entry.newFilename + "@" + m_entry.newHash
         << m_old->fileName()
         << m_new->fileName();
    return args;
}
