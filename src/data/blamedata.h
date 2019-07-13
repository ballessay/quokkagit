#ifndef BLAMEDATA_H
#define BLAMEDATA_H

#include <QString>
#include <vector>

namespace quokkagit
{
    struct SBlameData
    {
        QString oid;
        QString signature;
        std::size_t linenumber;
        QString line;
    };

    using tvBlameData = std::vector<SBlameData>;
}

#endif // BLAMEDATA_H
