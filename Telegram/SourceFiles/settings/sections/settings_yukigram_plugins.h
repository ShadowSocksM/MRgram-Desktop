#pragma once

#include "settings/settings_type.h"
#include <QtCore/QString>

namespace Settings {

[[nodiscard]] Type YukigramPluginsId();
[[nodiscard]] Type YukigramPluginDetailsId(const QString &pluginId);

} // namespace Settings


