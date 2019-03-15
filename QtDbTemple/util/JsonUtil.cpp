#include "JsonUtil.h"
#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QRegularExpression>

JsonUtil::JsonUtil(const QString &jsonFilePath)
{
    QFile file(jsonFilePath);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
        rootObject = jsonDocument.object();
    }
    else
    {
        qDebug()<<__FUNCTION__<<__LINE__<<QString("connot open the file: %1").arg(jsonFilePath);
    }

}

int JsonUtil::getInt(const QString &path, int def) const
{
    return getJsonValue(path).toInt(def);
}

bool JsonUtil::getBool(const QString &path, bool def) const
{
    return getJsonValue(path).toBool(def);
}

QString JsonUtil::getString(const QString &path, const QString &def) const
{
    return getJsonValue(path).toString().trimmed();
}

QStringList JsonUtil::getStringList(const QString &path) const
{
    QStringList result;
    QJsonArray array = getJsonValue(path).toArray();
    for(QJsonArray::const_iterator iter = array.begin(); iter != array.end(); ++iter)
    {
        QJsonValue value = *iter;
        result<<value.toString().trimmed();
    }

    return result;
}

QJsonValue JsonUtil::getJsonValue(const QString &path) const
{
    QStringList tokens = path.split(QRegularExpression("\\."));
    int size = tokens.size();
    QJsonObject object = rootObject;
    for(int i=0; i<size-1; i++)
    {
        if(object.isEmpty())
        {
            return QJsonValue();
        }
        object = object.value(tokens.at(i)).toObject();
    }
    return object.value(tokens.last());
}
