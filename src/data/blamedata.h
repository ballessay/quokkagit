#ifndef BLAMEDATA_H
#define BLAMEDATA_H

#include <QString>
#include <vector>


namespace turtlegit
{
    struct SBlameData
    {
        QString hash;
        QString signature;
        std::size_t line;
        QString data;
        QString origPath;
    };

    using BlameData = std::vector<SBlameData>;
}

#endif // BLAMEDATA_H
