#pragma once

#include "ctkPluginContext.h"
#include <QSharedPointer>
#include <QWidget>
#include <QString>
#include <QVector>
#include <QJsonValue>
#include <QJsonObject>
#include <functional>
#include <QVariantMap>
#include <QDebug>

namespace McsEvent {
static const char* EVENT_PANEL_MANAGER_UPDATE_ALL = "ak/panel_manager/update_all";
static const char* EVENT_PANEL_MANAGER_OPEN_PANEL = "ak/panel_manager/open_panel";
}

namespace ak {

enum ButtonState {
  StateInvisible = -2,
  StateUnknown = -1,
  StateNormal = 0,
  StateMinimize = 1,
  StateMaximize,
  StateChecked,
  StateDisabled,
};

struct McsPanelServiceState;

struct ButtonInfoPrivate;
struct ButtonInfo {
  ButtonInfo() :
    getState(nullptr),
    getName(nullptr),
    clicked(nullptr),
    state(),
    d(nullptr)
  {
    newName = "";
    newModeSource = "";
    checked = false;
    disabled = false;
    visible = true;
  }
  QString id;
  QString newName;
  QString newModeSource;
  bool checked;
  bool disabled;
  bool visible;
  
  std::function<ButtonState(ButtonInfo &buttonInfo, McsPanelServiceState &state)> getState;
  std::function<QString(ButtonInfo &buttonInfo, McsPanelServiceState &state)> getName;
  std::function<bool(ButtonInfo &buttonInfo, McsPanelServiceState &state)> clicked;

  QJsonObject state;
  ButtonInfoPrivate *d;
};
enum MenuInfoOperator {
    MenuInfoOperatorCommander = 1 << 0,
    MenuInfoOperatorRadar = 1 << 1,
    MenuInfoOperatorInfrared = 1 << 2,
    MenuInfoOperatorMagnetic = 1 << 3,
    MenuInfoOperatorAll = MenuInfoOperatorCommander | MenuInfoOperatorRadar | MenuInfoOperatorInfrared | MenuInfoOperatorMagnetic
};
struct MenuInfo {
  MenuInfo() {
    checked = false;
    disabled = false;
    hasChild = false;
    hasBeforeChild = false;
    hasAfterChild = false;
    order = 0;
  }
  QString parentId;
  QString id;
  QString beforeId;
  QString afterId;
  QString name;
  QString source;
  QString iconURL;
  QString eventId;
  QString panelId;
  bool hasChild;
  bool hasBeforeChild;
  bool hasAfterChild;
  int group;
  int order;
  int finalOrder;
  bool checked;
  bool disabled;
  int operators;
};

struct McsPanelServiceState {
  QMap<QString, QJsonObject> panels;
  QMap<QString, ButtonInfo> buttons;
  QMap<QString, MenuInfo> menus;
  QMap<QString, QJsonObject> widgets;
  QJsonObject global;
  std::function<void(const QJsonObject&)> update;
  QString parentId;
  int filterValue;
};

typedef QWidget* (*CreateWidgetConstructorType)();

struct PanelInfo {
  QString id;
  QString title;
  QString iconURL;
  int width = 1; /* 1 or 4*/
  int minPos = 2; /* 0 - 4*/
  int maxPos = 4; /* 0 - 4 [minOffest, maxOffset]*/
  int increment = -1; /* 1 or -1 */
  bool hasTitleBar = true;
  bool hasClose = true;
  bool hasMinimize = true;
  bool autoClose = true;
  bool autoMinimize = true;
  CreateWidgetConstructorType createWidget;
  QJsonObject args;
};

struct DialogInfo {
  QString id;
  QString title;
  QString iconURL;
  int width;
  int height;
  bool hasClose = true;
  bool hasMinimize = false;
  bool hasResize = false;
  CreateWidgetConstructorType createDialog;
  QJsonObject args;
};

typedef QWidget* (*CreateQWidgetConstructorType)(QWidget* parent);
struct WidgetInfo {
  QString id;
  CreateQWidgetConstructorType createWidget;
};

typedef QSharedPointer<QVector<ak::ButtonInfo>> ButtonInfoList;
typedef QSharedPointer<QVector<ak::MenuInfo>> MenuInfoList;
typedef QSharedPointer<QVector<ak::DialogInfo>> DialogInfoList;
typedef QSharedPointer<QVector<ak::PanelInfo>> PanelInfoList;
typedef QSharedPointer<QVector<ak::WidgetInfo>> WidgetInfoList;

class McsPanelService
{
protected:
  McsPanelService() {}
public:
  virtual ~McsPanelService() {}
  /* ?????????????????????????????? ???????????????????????????????? ????????????¦Å???????????????????? */
  virtual void setServiceState(McsPanelServiceState* state) {
  }
  virtual ButtonInfoList getButtonList() {
    return ButtonInfoList(nullptr);
  };
  virtual MenuInfoList getMenuList() {
    return MenuInfoList(nullptr);
  };
  virtual PanelInfoList getPanelList() {
    return PanelInfoList(nullptr);
  };
  virtual DialogInfoList getDialogList()
  {
    return DialogInfoList(nullptr);
  }
  virtual WidgetInfoList getWidgetList() {
    return WidgetInfoList(nullptr);
  }

  static inline void openPanel(ctkPluginContext* context, QString panelId, const QJsonObject &args) {
    ctkDictionary dict;
    dict["args"] = args;
    dict["panelId"] = panelId;
  }

};

class AkPluginOperateInterface
{
protected:
	AkPluginOperateInterface() {}
public:
	virtual QList<QVariantMap> getSql(QString tableName, QString sqlStr) { return QList<QVariantMap>(); }
};

}

template <>
Q_INLINE_TEMPLATE ak::ButtonInfo &QMap<QString, ak::ButtonInfo>::operator[](const QString &akey)
{
  detach();
  Node *n = d->findNode(akey);
  if (!n) {
    qDebug() << "New button id:" << akey;
    return *insert(akey, ak::ButtonInfo());
  }
  return n->value;
}

//Q_DECLARE_INTERFACE(ak::McsPanelService, "ak.main.McsPanelService")
Q_DECLARE_INTERFACE(ak::AkPluginOperateInterface, "ak.AkPluginOperateInterface")
