#include "yukigram/plugins/plugin_runtime.h"

#include <algorithm>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <vector>

namespace Yukigram::Plugins {
namespace {

std::vector<RuntimePlugin> ActivePlugins;

void WriteRuntimeTest(const QString &text) {
        QFile file(cWorkingDir() + u"tdata/plugins/runtime_test.log"_q);
        if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
                file.write(text.toUtf8());
                file.write("\n");
        }
}

} // namespace

bool StartPluginRuntime(const QString &id) {
        const auto i = std::find_if(
                ActivePlugins.begin(),
                ActivePlugins.end(),
                [&](const RuntimePlugin &plugin) { return plugin.id == id; });

        if (i != ActivePlugins.end()) {
                i->active = true;
                qDebug() << "[Yukigram Plugin Runtime] START:" << id;
                WriteRuntimeTest(u"START: "_q + id);
                return true;
        }

        qDebug() << "[Yukigram Plugin Runtime] START:" << id;
        WriteRuntimeTest(u"START: "_q + id);

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
                WriteRuntimeTest(u"STOP: "_q + id);
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


