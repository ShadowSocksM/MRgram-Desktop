#include "yukigram/plugins/plugin_runtime.h"
#include "yukigram/plugins/plugin_manager.h"
#include "core/application.h"
#include "window/window_controller.h"
#include "window/window_session_controller.h"

#include <algorithm>
#include <QtCore/QDebug>
#include <vector>

namespace Yukigram::Plugins {
namespace {

std::vector<RuntimePlugin> ActivePlugins;


void ExecutePluginAction(const PluginInfo &plugin) {
        if (plugin.action == u"show_toast"_q) {
                if (const auto window = Core::App().activeWindow()) {
                        if (const auto controller = window->sessionController()) {
                                controller->showToast(u"Yukigram Plugin: "_q + plugin.name);
                        }
                }
}

}
} // namespace

bool StartPluginRuntime(const QString &id) {
        const auto plugin = FindPlugin(id);
        if (!plugin) return false;

        const auto i = std::find_if(
                ActivePlugins.begin(),
                ActivePlugins.end(),
                [&](const RuntimePlugin &plugin) { return plugin.id == id; });

        if (i != ActivePlugins.end()) {
                i->active = true;
                qDebug() << "[Yukigram Plugin Runtime] START:" << id;
                ExecutePluginAction(*plugin);
                return true;
        }

        qDebug() << "[Yukigram Plugin Runtime] START:" << id;
        ExecutePluginAction(*plugin);

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
                qDebug() << "[Yukigram Plugin Runtime] STOP:" << id;
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






