#ifndef JSON_UTIL_H
#define JSON_UTIL_H

#include <QJsonObject>

class JsonUtil
{
public:
    JsonUtil(const QString &jsonFilePath);
    int getInt(const QString &path, int def = 0) const;
    bool getBool(const QString &path, bool def = false) const;
    QString getString(const QString &path, const QString &def = QString()) const;
    QStringList getStringList(const QString &path) const;
    QJsonValue getJsonValue(const QString &path) const;

private:
    QJsonObject rootObject;
};

#endif
