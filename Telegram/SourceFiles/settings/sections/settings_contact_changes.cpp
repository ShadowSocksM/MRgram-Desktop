#include "settings/sections/settings_contact_changes.h"

#include "main/contact_changes/contact_changes.h"
#include "main/contact_changes/contact_changes_storage.h"
#include "main/main_session.h"
#include "ui/widgets/labels.h"
#include "ui/wrap/vertical_layout.h"
#include "window/window_session_controller.h"
#include "styles/style_settings.h"
#include "styles/style_boxes.h"

#include <algorithm>

namespace Settings {
namespace {

class ContactChanges final : public Section<ContactChanges> {
public:
    ContactChanges(
        QWidget *parent,
        not_null<Window::SessionController*> controller)
    : Section(parent, controller) {
        const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
        const auto history = Main::ContactChanges::LoadHistory(
            controller->session().uniqueId());

        const auto addLine = [&](const QString &value) {
            content->add(object_ptr<Ui::FlatLabel>(
                content,
                rpl::single(value),
                st::changePhoneDescription));
        };

        addLine(u"Contact Changes"_q);
        addLine(u"Recorded events: "_q + QString::number(history.size()));

        for (auto i = history.size(); i > 0; --i) {
            const auto &entry = history[i - 1];
            const auto type = (entry.type == Main::ContactChanges::ChangeType::Name)
                ? u"Name"_q
                : (entry.type == Main::ContactChanges::ChangeType::Username)
                ? u"Username"_q
                : u"Photo"_q;

            addLine(u"------------------------"_q);
            addLine(type + u" | User ID: "_q
                + QString::number(entry.userId));
            addLine(u"Old: "_q + entry.oldValue);
            addLine(u"New: "_q + entry.newValue);
            addLine(entry.detectedAt.toLocalTime().toString(
                u"yyyy/MM/dd HH:mm:ss"_q));
            if (entry.isNew) {
                addLine(u"NEW"_q);
            }
        }

        Ui::ResizeFitChild(this, content);
    }

    [[nodiscard]] rpl::producer<QString> title() override {
        return rpl::single(u"Contact Changes"_q);
    }
};

} // namespace

Type ContactChangesId() {
    return ContactChanges::Id();
}

} // namespace Settings
