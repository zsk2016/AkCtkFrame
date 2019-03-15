#pragma once

#include <QMap>
#include <QList>
#include <QtSql>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVariantMap>
#include <functional>

class DBUtil
{
public:
    enum DebugMode
    {
        DEBUG_OFF,
        DEBUG_ON
    };

    static void setDebugMode(DebugMode mode);
    static long long insert(const QString &sql, const QVariantMap &params = QVariantMap());
    static bool update(const QString &sql, const QVariantMap &params = QVariantMap());
    static bool deleted(const QString &sql, const QVariantMap &params = QVariantMap());
    static bool deletedByForeignKeys(const QString &sql, const QVariantMap &params = QVariantMap());
    static QVariantMap selectMap(const QString &sql, const QVariantMap &params = QVariantMap());
    static QList<QVariantMap> selectMaps(const QString &sql, const QVariantMap &params = QVariantMap());

    static int selectInt(const QString &sql, const QVariantMap &params = QVariantMap());
    static qint64 selectInt64(const QString &sql, const QVariantMap &params = QVariantMap());
    static QString selectString(const QString &sql, const QVariantMap &params = QVariantMap());
    static QStringList selectStrings(const QString &sql, const QVariantMap &params = QVariantMap());
    static QDate selectData(const QString &sql, const QVariantMap &params = QVariantMap());
    static QDateTime selectDateTime(const QString &sql, const QVariantMap &params = QVariantMap());
    static QVariant selectVariant(const QString &sql, const QVariantMap &params = QVariantMap());

    template<typename T>
    static T selectBean(T mapToBean(const QVariantMap &rowMap), const QString &sql, const QVariantMap &params = QVariantMap())
    {
        return mapToBean(selectMap(sql, params));
    }

    template<typename T>
    static QList<T> selectBeans(T mapToBean(const QVariantMap &rowMap), const QString &sql, const QVariantMap &params = QVariantMap())
    {
        QList<T> beans;
        foreach (const QVariantMap row, selectMaps(sql, params))
        {
            beans.append(mapToBean(row));
        }
        return beans;
    }



private:
    static void executeSql(const QString &sql, const QVariantMap &params, std::function<void(QSqlQuery *query)> fn, bool hasForeignKeys = false);
    static QStringList getFiledNames(const QSqlQuery &query);
    static void bindValues(QSqlQuery *query, const QVariantMap &params);
    static QList<QVariantMap> queryToMaps(QSqlQuery *query);
    static void debug(const QSqlQuery &query, const QVariantMap &params);
    static DebugMode m_debugMode;
};
