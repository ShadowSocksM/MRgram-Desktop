#include "yukigram/plugins/plugin_runtime.h"

#include <algorithm>
#include <vector>

namespace Yukigram::Plugins {
namespace {

std::vector<RuntimePlugin> ActivePlugins;

} // namespace

bool StartPluginRuntime(const QString &id) {
        const auto i = std::find_if(
                ActivePlugins.begin(),
                ActivePlugins.end(),
                [&](const RuntimePlugin &plugin) { return plugin.id == id; });

        if (i != ActivePlugins.end()) {
                i->active = true;
                return true;
        }

        ActivePlugins.push_back(RuntimePlugin{
                id,
                true,
        });
        return true;
}

void StopPluginRuntime(const QString &id) {
        const auto i = std::find_if(
                ActivePlugins.begin(),
                ActivePlugins.end(),
                [&](const RuntimePlugin &plugin) { return plugin.id == id; });

        if (i != ActivePlugins.end()) {
                i->active = false;
        }
}

bool IsPluginRuntimeActive(const QString &id) {
        const auto i = std::find_if(
                ActivePlugins.begin(),
                ActivePlugins.end(),
                [&](const RuntimePlugin &plugin) { return plugin.id == id; });

        return (i != ActivePlugins.end()) && i->active;
}

} // namespace Yukigram::Plugins
