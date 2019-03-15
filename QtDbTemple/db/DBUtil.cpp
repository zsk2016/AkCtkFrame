#include "DBUtil.h"
#include "ConnectionPool.h"
#include <functional>

DBUtil::DebugMode DBUtil::m_debugMode = DBUtil::DEBUG_ON;

void DBUtil::setDebugMode(DebugMode mode)
{
    m_debugMode = mode;
}

long long DBUtil::insert(const QString &sql, const QVariantMap &params)
{
    long long id = -1;
    executeSql(sql, params, [&id](QSqlQuery *query){
        id = query->lastInsertId().toLongLong();
    });
    return id;
}

bool DBUtil::update(const QString &sql, const QVariantMap &params)
{
    bool result = false;
    executeSql(sql, params, [&result](QSqlQuery *query){
        result = query->lastError().type() == QSqlError::NoError;
    });
    return result;
}

bool DBUtil::deleted(const QString &sql, const QVariantMap &params)
{
    bool result = false;
    executeSql(sql, params, [&result](QSqlQuery *query){
        result = query->lastError().type() == QSqlError::NoError;
    });
    return result;
}

bool DBUtil::deletedByForeignKeys(const QString &sql, const QVariantMap &params)
{
    bool result = false;
    executeSql(sql, params, [&result](QSqlQuery *query){
        result = query->lastError().type() == QSqlError::NoError;
    });
    return result;
}

QVariantMap DBUtil::selectMap(const QString &sql, const QVariantMap &params)
{
    return selectMaps(sql, params).value(0);
}

QList<QVariantMap> DBUtil::selectMaps(const QString &sql, const QVariantMap &params)
{
    QList<QVariantMap> result;
    executeSql(sql, params, [&result](QSqlQuery *query){
        result = queryToMaps(query);
    });
    return result;
}

int DBUtil::selectInt(const QString &sql, const QVariantMap &params)
{
    return selectVariant(sql, params).toInt();
}

qint64 DBUtil::selectInt64(const QString &sql, const QVariantMap &params)
{
    return selectVariant(sql, params).toLongLong();
}

QString DBUtil::selectString(const QString &sql, const QVariantMap &params)
{
    return selectVariant(sql, params).toString();
}

QStringList DBUtil::selectStrings(const QString &sql, const QVariantMap &params)
{
    QStringList result;
    executeSql(sql, params, [&result](QSqlQuery *query){
        while(query->next())
        {
            result.append(query->value(0).toString());
        }
    });
    return result;
}

QDate DBUtil::selectData(const QString &sql, const QVariantMap &params)
{
    return selectVariant(sql, params).toDate();
}

QDateTime DBUtil::selectDateTime(const QString &sql, const QVariantMap &params)
{
    return selectVariant(sql, params).toDateTime();
}

QVariant DBUtil::selectVariant(const QString &sql, const QVariantMap &params)
{
    QVariant result;
    executeSql(sql, params, [&result](QSqlQuery *query){
        while(query->next())
        {
            result = query->value(0);
        }
    });
    return result;
}

void DBUtil::executeSql(const QString &sql, const QVariantMap &params, std::function<void(QSqlQuery *query)> fn, bool hasForeignKeys)
{
    ConnectionGuard guard;
    QSqlQuery query = guard.query();
    if(hasForeignKeys)
            query.prepare("PRAGMA foreign_keys = ON");
    query.prepare(sql);
    bindValues(&query, params);
    if(query.exec())
    {
        fn(&query);
    }
    debug(query, params);
}

QStringList DBUtil::getFiledNames(const QSqlQuery &query)
{
    QSqlRecord record = query.record();
    QStringList names;
    int count = record.count();
    for(int i=0; i<count; ++i)
    {
        names<<record.fieldName(i);
    }
    return names;
}

void DBUtil::bindValues(QSqlQuery *query, const QVariantMap &params)
{
    for(QVariantMap::const_iterator i= params.constBegin(); i!= params.constEnd(); ++i)
    {
        query->bindValue(":" + i.key(), i.value());
    }
}

QList<QVariantMap> DBUtil::queryToMaps(QSqlQuery *query)
{
    QList<QVariantMap> rowMaps;
    QStringList fieldNames = getFiledNames(*query);
    while (query->next())
    {
        QVariantMap rowMap;
        foreach (const QString &fieldName, fieldNames) {
            rowMap.insert(fieldName, query->value(fieldName));
        }
        rowMaps.append(rowMap);
    }
    return rowMaps;
}

void DBUtil::debug(const QSqlQuery &query, const QVariantMap &params)
{
    if(m_debugMode == DEBUG_ON)
    {
        if(query.lastError().type() != QSqlError::NoError)
        {
            qDebug()<<"SQL Error: "<<query.lastError().text().trimmed();
        }
        qDebug()<<"SQL Query : "<<query.lastQuery();
    }
    if(params.size() > 0)
    {
        qDebug()<<"SQL params : "<<params;
    }
}
