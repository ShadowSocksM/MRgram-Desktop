#include "yukigram/plugins/plugin_runtime.h"
#include "yukigram/plugins/plugin_manager.h"
#include "core/application.h"
#include "window/window_controller.h"
#include "window/window_session_controller.h"

#include <algorithm>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <vector>

namespace Yukigram::Plugins {
namespace {

std::vector<RuntimePlugin> ActivePlugins;


void ExecutePluginAction(const PluginInfo &plugin) {
        if (plugin.action == u"show_toast"_q) {
                if (const auto window = Core::App().activeWindow()) {
                        if (const auto controller = window->sessionController()) {
                                controller->showToast(u"MRgram Plugin: "_q + plugin.name);
                        }
                }
}

}
} // namespace

void StartEnabledPlugins() {
	static bool started = false;
	if (started) {
		return;
	}
	started = true;
	for (const auto &plugin : LoadedPlugins()) {
		if (plugin.enabled) {
			StartPluginRuntime(plugin.id);
		}
	}
}

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

bool IsPluginActionActive(const QString &action) {
QFile debugFile(cWorkingDir() + u"tdata/plugins/forward_pro_debug.log"_q);
debugFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
QTextStream debug(&debugFile);

debug << "CHECK action=" << action << "\n";
debug << "LoadedPlugins=" << LoadedPlugins().size() << "\n";

for (const auto &plugin : LoadedPlugins()) {
const auto runtime = IsPluginRuntimeActive(plugin.id);

debug
<< "plugin id=" << plugin.id
<< " action=" << plugin.action
<< " enabled=" << (plugin.enabled ? "true" : "false")
<< " runtime=" << (runtime ? "true" : "false")
<< "\n";

if (plugin.action == action
&& plugin.enabled
&& runtime) {
debug << "RESULT=ACTIVE\n\n";
debug.flush();
return true;
}
}

debug << "RESULT=INACTIVE\n\n";
debug.flush();
return false;
}
bool IsPluginRuntimeActive(const QString &id) {
        const auto i = std::find_if(
                ActivePlugins.begin(),
                ActivePlugins.end(),
                [&](const RuntimePlugin &plugin) { return plugin.id == id; });

        return (i != ActivePlugins.end()) && i->active;
}

} // namespace Yukigram::Plugins







