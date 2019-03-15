#ifndef SQLS_H
#define SQLS_H

#include "util/Singleton.h"

#include <QHash>
#include <QString>

class Sqls
{
protected:
    Sqls();
    static Sqls *createInstance();
public:
    ~Sqls();
    static Sqls *getInstance();
    QString getSql(const QString &sql_namespace, const QString &sql_id);

private:
    QHash<QString, QString> sqls;

    friend class SqlsPrivate;
};

#endif //SQLS_H
