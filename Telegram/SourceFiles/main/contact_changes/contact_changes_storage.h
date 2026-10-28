#pragma once

#include "main/contact_changes/contact_changes.h"

namespace Main::ContactChanges {

History LoadHistory(quint64 accountId);
QVector<Snapshot> LoadSnapshots(quint64 accountId);
bool SaveSnapshots(quint64 accountId, const QVector<Snapshot> &snapshots);
bool SaveHistory(quint64 accountId, const History &history);

} // namespace Main::ContactChanges
