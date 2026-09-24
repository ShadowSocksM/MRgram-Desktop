#pragma once

#include <QtCore/QString>

namespace Yukigram::Plugins {

inline constexpr int kPluginApiVersion = 1;

struct PluginApi {
        int version = kPluginApiVersion;
        QString yukigramVersion;
};

} // namespace Yukigram::Plugins
