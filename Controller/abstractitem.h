#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QString>

class AbstractItem
{
public:
    virtual QString data() = 0;
    virtual ~AbstractItem() {}
};

#endif // ABSTRACTITEM_H
