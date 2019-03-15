#include "Sqls.h"
#include "util/JsonUtil.h"
#include "util/ConfigUtil.h"

#include <QFile>
#include <QDebug>
#include <QXmlInputSource>
#include <QXmlAttributes>
#include <QXmlParseException>
#include <QXmlDefaultHandler>

namespace {
    const char *const SQL_ID                = "id";
    const char *const SQL_REF_ID            = "refId";
    const char *const SQL_TAGNAME_SQL       = "sql";
    const char *const SQL_TAGNAME_SQLS      = "sqls";
    const char *const SQL_TAGNAME_DEFINE    = "define";
    const char *const SQL_TAGNAME_INCLUDE   = "include";
    const char *const SQL_NAMESPACE         = "namespace";
}

class SqlsPrivate : public QXmlDefaultHandler
{
public:
    SqlsPrivate(Sqls *ctx);
    static QString buildKey(const QString &sql_namespace, const QString &id);

protected:
    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool characters(const QString &ch);
    bool fatalError(const QXmlParseException &exception);

private:
    QHash<QString, QString> defines;
    QString sql_namespace;
    QString current_text;
    QString current_sql_id;
    QString current_define_id;
    QString current_ref_id;
    Sqls *ctx;
};

SqlsPrivate::SqlsPrivate(Sqls *ctx)
{
    this->ctx = ctx;
    QStringList sql_files = ConfigUtil::getInstance()->getDatabaseSqlFiles();
    foreach (QString file_name, sql_files)
    {
        QString filepath = QStringLiteral(":/resource/sql/%1").arg(file_name);
        QFile file(filepath);
        QXmlInputSource inputSource(&file);
        QXmlSimpleReader reader;
        reader.setContentHandler(this);
        reader.setErrorHandler(this);
        reader.parse(inputSource);
        defines.clear();
    }
}

QString SqlsPrivate::buildKey(const QString &sql_namespace, const QString &id)
{
    return sql_namespace + ":" + id;
}

bool SqlsPrivate::startElement(const QString &namespaceURI, const QString &localName, const QString &q_name, const QXmlAttributes &atts)
{
    Q_UNUSED(namespaceURI)
    Q_UNUSED(localName)

    if(SQL_TAGNAME_SQL == q_name)
    {
        current_sql_id = atts.value(SQL_ID);
        current_text = "";
    }
    else if(SQL_TAGNAME_INCLUDE == q_name)
    {
        current_ref_id = atts.value(SQL_REF_ID);
    }
    else if(SQL_TAGNAME_DEFINE == q_name)
    {
        current_define_id = atts.value(SQL_ID);
        current_text = "";
    }
    else if(SQL_TAGNAME_SQLS == q_name)
    {
        sql_namespace = atts.value(SQL_NAMESPACE);
    }
    return true;
}

bool SqlsPrivate::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    Q_UNUSED(namespaceURI)
    Q_UNUSED(localName)

    if(SQL_TAGNAME_SQL == qName)
    {
        ctx->sqls.insert(buildKey(sql_namespace, current_sql_id), current_text.simplified());
        current_text = "";
    }
    else if(SQL_TAGNAME_INCLUDE == qName)
    {
        QString def = defines.value(buildKey(sql_namespace, current_ref_id));
        QString def_key = buildKey(sql_namespace, current_ref_id);
        if(!def.isEmpty())
        {
            current_text += defines.value(def_key);
        }
        else
        {
            qDebug()<<__FUNCTION__<<__LINE__<<"cannot find define: "<<def_key;
        }
    }
    else if(SQL_TAGNAME_DEFINE == qName)
    {
        defines.insert(buildKey(sql_namespace, current_define_id), current_text.simplified());
    }
    return true;
}

bool SqlsPrivate::characters(const QString &ch)
{
    current_text += ch;
    return true;
}

bool SqlsPrivate::fatalError(const QXmlParseException &exception)
{
    qDebug ()<< __FUNCTION__<<__LINE__<<QString("parse error at line %1, column %2, message: %3")
                .arg(exception.lineNumber()).arg(exception.columnNumber()).arg(exception.message());
    return false;
}

Sqls::Sqls()
{
    SqlsPrivate *data = new SqlsPrivate(this);
    delete data;

    QHashIterator<QString, QString> i(sqls);
      while (i.hasNext()) {
          i.next();
            qDebug() << i.key() << ": " << i.value() << endl;
      }

}

Sqls *Sqls::createInstance()
{
    return new Sqls();
}

Sqls::~Sqls()
{

}

Sqls *Sqls::getInstance()
{
    return Singleton<Sqls>::instance(Sqls::createInstance);
}

QString Sqls::getSql(const QString &sql_namespace, const QString &sql_id)
{
    QString sql = sqls.value(SqlsPrivate::buildKey(sql_namespace, sql_id));
    qDebug()<<sql;
    if(sql.isEmpty())
    {
        qDebug()<<__FUNCTION__<< __LINE__ <<QString("cannot find SQL for %1::%2").arg(sql_namespace).arg(sql_id);
    }
    return sql;
}
