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
};

void Init();
bool LoadPlugin(const QString &path);
const std::vector<PluginInfo> &LoadedPlugins();
rpl::producer<> PluginsChanged();

} // namespace Yukigram::Plugins







