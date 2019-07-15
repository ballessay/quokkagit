#ifndef KDIFF3_H
#define KDIFF3_H

#include "difftool.h"

class CKdiff3 : public CDiffTool
{
public:
    CKdiff3(const quokkagit::SDiffSettings& settings,
            const quokkagit::SDiffEntry& entry,
            int index);

private:
    QStringList args() const override;
};

#endif // KDIFF3_H
