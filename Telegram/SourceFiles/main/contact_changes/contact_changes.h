#pragma once

#include <QString>
#include <QDateTime>
#include <QVector>

namespace Main::ContactChanges {

enum class ChangeType {
    Name,
    Username,
    Photo,
};

struct Entry {
    quint64 userId = 0;
    ChangeType type = ChangeType::Name;
    QString oldValue;
    QString newValue;
    QDateTime detectedAt;
    bool isNew = true;
};

struct Snapshot {
    quint64 userId = 0;
    QString name;
    QString username;
    quint64 photoId = 0;
};

using History = QVector<Entry>;

} // namespace Main::ContactChanges
