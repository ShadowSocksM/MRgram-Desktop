#include "yukigram/plugins/plugin_manager.h"

#include <algorithm>

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

namespace Yukigram::Plugins {

namespace {
std::vector<PluginInfo> LoadedPluginsList;
rpl::event_stream<> PluginsChangedStream;
} // namespace

void Init() {
        const auto pluginsPath = cWorkingDir() + u"tdata/plugins/"_q;
        QDir().mkpath(pluginsPath);
        const auto directory = QDir(pluginsPath);
        const auto files = directory.entryList({
                u"*.yukiplugin"_q,
        }, QDir::Files);

        for (const auto &file : files) {
                LoadPlugin(directory.filePath(file));
        }

        qDebug() << "[Yukigram Plugins] Plugin Manager initialized:"
                << LoadedPluginsList.size() << "plugin(s) loaded.";
}


bool LoadPlugin(const QString &path) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
                return false;
        }

        QJsonParseError error;
        const auto document = QJsonDocument::fromJson(file.readAll(), &error);
        if (error.error != QJsonParseError::NoError || !document.isObject()) {
                return false;
        }

        const auto object = document.object();
        const auto id = object.value(u"id"_q).toString();
        const auto name = object.value(u"name"_q).toString();
        const auto version = object.value(u"version"_q).toString();
        const auto author = object.value(u"author"_q).toString();
        const auto description = object.value(u"description"_q).toString();

        if (id.isEmpty() || name.isEmpty() || version.isEmpty()) {
                return false;
        }

        const auto pluginsPath = cWorkingDir() + u"tdata/plugins/"_q;
        QDir().mkpath(pluginsPath);
        const auto sourcePath = QFileInfo(path).absoluteFilePath();
        const auto installedPath = QFileInfo(
                QDir(pluginsPath).filePath(QFileInfo(path).fileName())
        ).absoluteFilePath();

        if (sourcePath != installedPath) {
                if (QFile::exists(installedPath)) {
                        QFile::remove(installedPath);
                }
                if (!QFile::copy(sourcePath, installedPath)) {
                        return false;
                }
        }
        const auto existing = std::find_if(
                LoadedPluginsList.begin(),
                LoadedPluginsList.end(),
                [&](const PluginInfo &plugin) { return plugin.id == id; });

        const auto info = PluginInfo{
                id,
                name,
                version,
                author,
                description,
        };

        if (existing != LoadedPluginsList.end()) {
                *existing = info;
        } else {
                LoadedPluginsList.push_back(info);
        }
        PluginsChangedStream.fire({});
        qDebug() << "[Yukigram Plugins] Loaded:" << name << version << author;
        return true;
}

rpl::producer<> PluginsChanged() {
        return PluginsChangedStream.events();
}

const std::vector<PluginInfo> &LoadedPlugins() {
        return LoadedPluginsList;
}
} // namespace Yukigram::Plugins





















