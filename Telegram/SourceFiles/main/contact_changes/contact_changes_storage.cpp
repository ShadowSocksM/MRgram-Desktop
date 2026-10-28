#include "main/contact_changes/contact_changes_storage.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QStandardPaths>

namespace Main::ContactChanges {
namespace {

QString HistoryPath(quint64 accountId) {
    const auto base = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir().mkpath(base + "/contact_changes");
    return base + "/contact_changes/" + QString::number(accountId) + ".json";
}

} // namespace

History LoadHistory(quint64 accountId) {
    QFile file(HistoryPath(accountId));
    if (!file.open(QIODevice::ReadOnly)) return {};
    const auto doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isArray()) return {};
    History result;
    for (const auto &value : doc.array()) {
        const auto obj = value.toObject();
        Entry entry;
        entry.userId = obj.value("userId").toString().toULongLong();
        entry.type = static_cast<ChangeType>(obj.value("type").toInt());
        entry.oldValue = obj.value("oldValue").toString();
        entry.newValue = obj.value("newValue").toString();
        entry.detectedAt = QDateTime::fromString(obj.value("detectedAt").toString(), Qt::ISODateWithMs);
        entry.isNew = obj.value("isNew").toBool();
        result.push_back(entry);
    }
    return result;
}

bool SaveHistory(quint64 accountId, const History &history) {
    QJsonArray array;
    for (const auto &entry : history) {
        QJsonObject obj;
        obj.insert("userId", QString::number(entry.userId));
        obj.insert("type", static_cast<int>(entry.type));
        obj.insert("oldValue", entry.oldValue);
        obj.insert("newValue", entry.newValue);
        obj.insert("detectedAt", entry.detectedAt.toString(Qt::ISODateWithMs));
        obj.insert("isNew", entry.isNew);
        array.append(obj);
    }
    QSaveFile file(HistoryPath(accountId));
    if (!file.open(QIODevice::WriteOnly)) return false;
    file.write(QJsonDocument(array).toJson(QJsonDocument::Indented));
    return file.commit();
}


QVector<Snapshot> LoadSnapshots(quint64 accountId) {
    QFile file(HistoryPath(accountId) + ".snapshots");
    if (!file.open(QIODevice::ReadOnly)) return {};
    const auto doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isArray()) return {};
    QVector<Snapshot> result;
    for (const auto &value : doc.array()) {
        const auto obj = value.toObject();
        Snapshot snapshot;
        snapshot.userId = obj.value("userId").toString().toULongLong();
        snapshot.name = obj.value("name").toString();
        snapshot.username = obj.value("username").toString();
        snapshot.photoId = obj.value("photoId").toString().toULongLong();
        result.push_back(snapshot);
    }
    return result;
}

bool SaveSnapshots(quint64 accountId, const QVector<Snapshot> &snapshots) {
    QJsonArray array;
    for (const auto &snapshot : snapshots) {
        QJsonObject obj;
        obj.insert("userId", QString::number(snapshot.userId));
        obj.insert("name", snapshot.name);
        obj.insert("username", snapshot.username);
        obj.insert("photoId", QString::number(snapshot.photoId));
        array.append(obj);
    }
    QSaveFile file(HistoryPath(accountId) + ".snapshots");
    if (!file.open(QIODevice::WriteOnly)) return false;
    if (file.write(QJsonDocument(array).toJson(QJsonDocument::Indented)) < 0) return false;
    return file.commit();
}

} // namespace Main::ContactChanges
