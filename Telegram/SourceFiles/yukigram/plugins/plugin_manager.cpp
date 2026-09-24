#include "yukigram/plugins/plugin_manager.h"
#include "yukigram/plugins/plugin_api.h"
#include "yukigram/plugins/plugin_runtime.h"

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

QString PluginStatesPath() {
        return cWorkingDir() + u"tdata/plugins/plugin_states.json"_q;
}

QJsonObject LoadPluginStates() {
        QFile file(PluginStatesPath());
        if (!file.open(QIODevice::ReadOnly)) {
                return {};
        }

        QJsonParseError error;
        const auto document = QJsonDocument::fromJson(file.readAll(), &error);
        if (error.error != QJsonParseError::NoError || !document.isObject()) {
                return {};
        }
        return document.object();
}

void SavePluginStates(const QJsonObject &states) {
        QFile file(PluginStatesPath());
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                return;
        }
        file.write(QJsonDocument(states).toJson(QJsonDocument::Indented));
}
} // namespace

void Init() {
        const auto pluginsPath = cWorkingDir() + u"tdata/plugins/"_q;
        QDir().mkpath(pluginsPath);
        const auto directory = QDir(pluginsPath);
        const auto files = directory.entryList({
                u"*.yukiplugin"_q,
        }, QDir::Files);

        for (const auto &file : files) {
                LoadPlugin(directory.filePath(file), false);
        }

        qDebug() << "[Yukigram Plugins] Plugin Manager initialized:"
                << LoadedPluginsList.size() << "plugin(s) loaded.";
}


bool LoadPlugin(const QString &path, bool startRuntime) {
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
        const auto action = object.value(u"action"_q).toString();
        const auto apiVersion = object.value(u"api_version"_q).toInt(kPluginApiVersion);

        if (apiVersion != kPluginApiVersion) {
                qWarning() << "[Yukigram Plugins] Unsupported API version:" << apiVersion << "for" << name;
                return false;
        }

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

        const auto states = LoadPluginStates();
        const auto enabled = states.contains(id) ? states.value(id).toBool(true) : true;

        const auto info = PluginInfo{
                id,
                name,
                version,
                author,
                description,
                installedPath,
                action,
                apiVersion,
                enabled,
        };

        if (existing != LoadedPluginsList.end()) {
                *existing = info;
        } else {
                LoadedPluginsList.push_back(info);
        }
        if (enabled && startRuntime) {
                StartPluginRuntime(id);
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

const PluginInfo *FindPlugin(const QString &id) {
        const auto i = std::find_if(
                LoadedPluginsList.begin(),
                LoadedPluginsList.end(),
                [&](const PluginInfo &plugin) { return plugin.id == id; });
        return (i != LoadedPluginsList.end()) ? &*i : nullptr;
}


bool UninstallPlugin(const QString &id) {
        const auto i = std::find_if(
                LoadedPluginsList.begin(),
                LoadedPluginsList.end(),
                [&](const PluginInfo &plugin) { return plugin.id == id; });
        if (i == LoadedPluginsList.end()) {
                return false;
        }

        const auto path = i->path;
        if (!path.isEmpty() && QFile::exists(path) && !QFile::remove(path)) {
                return false;
        }

        auto states = LoadPluginStates();
        states.remove(id);
        SavePluginStates(states);

        StopPluginRuntime(id);
        LoadedPluginsList.erase(i);
        PluginsChangedStream.fire({});
        return true;
}
bool SetPluginEnabled(const QString &id, bool enabled) {
        const auto i = std::find_if(
                LoadedPluginsList.begin(),
                LoadedPluginsList.end(),
                [&](const PluginInfo &plugin) { return plugin.id == id; });
        if (i == LoadedPluginsList.end()) {
                return false;
        }
        if (i->enabled == enabled) {
                return true;
        }
        i->enabled = enabled;
        auto states = LoadPluginStates();
        states.insert(id, enabled);
        SavePluginStates(states);

        if (enabled) {
                StartPluginRuntime(id);
        } else {
                StopPluginRuntime(id);
        }
        
        return true;
}


} // namespace Yukigram::Plugins














































