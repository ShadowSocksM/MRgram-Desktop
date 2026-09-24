#pragma once

#include <QtCore/QString>

namespace Yukigram::Plugins {

struct RuntimePlugin {
        QString id;
        bool active = false;
};

bool StartPluginRuntime(const QString &id);
void StopPluginRuntime(const QString &id);
bool IsPluginRuntimeActive(const QString &id);

} // namespace Yukigram::Plugins
