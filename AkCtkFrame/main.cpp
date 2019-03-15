#include <ctkPluginFrameworkFactory.h>
#include <ctkPluginFramework.h>
#include <ctkPluginFrameworkLauncher.h>
#include <ctkPluginException.h>
#include <ctkPluginContext.h>
#include <QApplication>
#include <QSettings>
#include <QDirIterator>
#include <QDir>
#include <QDebug>




#define USE_TRY_CATCH 0

bool startPluginByName(QSharedPointer<ctkPluginFramework> framework, const QString &name)
{
	QSharedPointer<ctkPlugin> plugin;
	QList<QSharedPointer<ctkPlugin>> pluginList = framework->getPluginContext()->getPlugins();

	for (int i = 0; i < pluginList.count(); ++i)
	{
		plugin = pluginList.at(i);
		if (plugin->getSymbolicName() == name)
		{
			try
			{
				plugin->start(ctkPlugin::START_TRANSIENT);
			}
			catch (const ctkException& e)
			{
				qCritical() << e.what();
				return false;
			}

			return true;
		}
	}

	return false;
}

bool installPluginByPath(QSharedPointer<ctkPluginFramework> framework, const QString &path)
{
	QString currentFile;
	QStringList libFilter;
	libFilter << "*.dll" << "*.so" << "*.dylib";
	QDirIterator dirIter(path, libFilter, QDir::Files, QDirIterator::Subdirectories);
	while (dirIter.hasNext())
	{
		try
		{
			currentFile = dirIter.next();
			qDebug() << "Find a library:" << currentFile;
			QSharedPointer<ctkPlugin> plugin = framework->getPluginContext()->installPlugin(QUrl::fromLocalFile(currentFile));
			qDebug() << "Install plugin:" << plugin->getSymbolicName();
		}
		catch (const ctkException& e)
		{
			qCritical() << e.what() << currentFile;
		}
	}

	return true;
}

bool expandLibraryPath(const QString &path)
{
	QStringList libFilter;
	libFilter << "*.dll" << "*.so" << "*.dylib";
	QDirIterator dirIter(path, libFilter, QDir::Files, QDirIterator::Subdirectories);
	while (dirIter.hasNext())
	{
		QString fileLocation = dirIter.next();

		QFileInfo fileInfo(fileLocation);;
		qApp->addLibraryPath(fileInfo.absolutePath());
		ctkPluginFrameworkLauncher::addSearchPath(QDir::toNativeSeparators(fileInfo.absolutePath()));
	}

	return true;
}

QString parseEnv(QString str)
{
	int pos = 0;
	QRegExp rx("%([^%]+)%"); // Match env var between two '%'
	rx.setMinimal(true);
	while ((pos = rx.indexIn(str, pos)) != -1)
	{
		// Replace env var
		QString capture = rx.cap(1);
		QString replacement = getenv(capture.toLatin1());
		str.replace("%" + capture + "%", replacement);

		// Skip env var + two '%'
		pos += rx.matchedLength() + 2;
	}
	return str;
}

QString parsePath(QString str)
{
	return parseEnv(str).replace("\\\\", "\\").replace("\\", "/");
}

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	qApp->setOrganizationName("ak");
	qApp->setOrganizationDomain("ak.com");
	qApp->setApplicationName("aks");

	QStringList pluginPathList;

	{ // initialize pluginPathList
#ifdef _DEBUG
		QString batosCorePath = parsePath("%OSGI_DIR%/bin/plugins/debug");
#else
		QString batosCorePath = parsePath("%OSGI_DIR%/bin/plugins/release");
#endif
		pluginPathList << batosCorePath;

		// additional paths specified by command line.
		for (int i = 1; i < argc; ++i)
		{
			QString path = argv[i];
			if (QDir(path).exists())
			{
				pluginPathList << path;
			}
		}

		QString localPluginPath = qApp->applicationDirPath() + QString("/plugins");
		pluginPathList << localPluginPath;
	}

	ctkProperties fwProps;

	fwProps.insert(ctkPluginConstants::FRAMEWORK_STORAGE_CLEAN,
		ctkPluginConstants::FRAMEWORK_STORAGE_CLEAN_ONFIRSTINIT);
#if 0
	fwProps.insert(ctkPluginFrameworkLauncher::PROP_CONFIG_AREA,
		QCoreApplication::applicationDirPath() + +"/" + QCoreApplication::applicationName() + "_ctkpluginfw");
#endif

	ctkPluginFrameworkFactory fwFactory(fwProps);
	QSharedPointer<ctkPluginFramework> framework = fwFactory.getFramework();

#if USE_TRY_CATCH
	try
	{
		framework->init();
	}
	catch (const ctkPluginException& exc)
	{
		qCritical() << "Failed to initialize the plug-in framework:" << exc;
		exit(1);
	}
#else
	framework->init();
#endif

	// expand library path
	foreach(QString path, pluginPathList)
	{
		if (!path.isEmpty()) expandLibraryPath(path);
	}

	// install plugins
	foreach(QString path, pluginPathList)
	{
		if (!path.isEmpty()) installPluginByPath(framework, path);
	}

	framework->start();

	// start plugins
	startPluginByName(framework, "org.commontk.eventadmin");
	startPluginByName(framework, "ak.CtkMainWindow");
	/*startPluginByName(framework, "QtDbTemple");
	startPluginByName(framework, "NetManager");*/
	

	qApp->setQuitOnLastWindowClosed(true);

#if USE_TRY_CATCH
	try
	{
		return app.exec();
	}
	catch (ctkException e)
	{
		qCritical() << "ctkPluginException " << e.message();
	}
	return -1;
#else
	return app.exec();
#endif
}
