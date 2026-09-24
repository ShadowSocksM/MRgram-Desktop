#pragma once

#include <QtCore/QString>
#include "rpl/rpl.h"
#include <vector>

namespace Yukigram::Plugins {

struct PluginInfo {
        QString id;
        QString name;
        QString version;
        QString author;
        QString description;
        QString path;
        QString action;
        int apiVersion = 1;
        bool enabled = true;
};

void Init();
bool LoadPlugin(const QString &path);
const std::vector<PluginInfo> &LoadedPlugins();
const PluginInfo *FindPlugin(const QString &id);
bool SetPluginEnabled(const QString &id, bool enabled);
bool UninstallPlugin(const QString &id);
rpl::producer<> PluginsChanged();

} // namespace Yukigram::Plugins












