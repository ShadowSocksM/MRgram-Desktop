#include "settings/sections/settings_yukigram_plugins.h"

#include "settings/settings_builder.h"
#include "settings/settings_common_session.h"
#include "lang/lang_keys.h"
#include "core/file_utilities.h"
#include "yukigram/plugins/plugin_manager.h"
#include "settings/sections/settings_yukigram.h"
#include "ui/wrap/vertical_layout.h"
#include "window/window_session_controller.h"
#include "styles/style_menu_icons.h"
#include "styles/style_settings.h"

namespace Settings {
namespace {

using namespace Builder;

class YukigramPlugins : public Section<YukigramPlugins> {
public:
        YukigramPlugins(
                QWidget *parent,
                not_null<Window::SessionController*> controller);

        [[nodiscard]] rpl::producer<QString> title() override;

private:
        void setupContent();
};

const auto kMeta = BuildHelper({
        .id = YukigramPlugins::Id(),
        .parentId = YukigramId(),
        .title = &tr::lng_settings_yukigram,
        .icon = &st::menuIconSettings,
}, [](SectionBuilder &builder) {
        builder.addSubsectionTitle(rpl::single(u"Plugin Manager"_q));
        builder.addButton({
                .id = u"yukigram/plugins/install"_q,
                .title = rpl::single(u"Install Plugin"_q),
                .icon = { &st::menuIconSettings },
                .onClick = [parent = QPointer<QWidget>(builder.container())] {
                        FileDialog::GetOpenPath(
                                parent,
                                u"Install Plugin"_q,
                                u"Yukigram Plugin (*.yukiplugin);;"_q + FileDialog::AllFilesFilter(),
                                [](FileDialog::OpenResult &&result) {
                                        if (result.paths.isEmpty()) {
                                                return;
                                        }
                                        Yukigram::Plugins::LoadPlugin(result.paths.front());
                                });
                },
                .keywords = { u"plugin"_q, u"install"_q },
        });
});

const SectionBuildMethod kPluginsSection = kMeta.build;

YukigramPlugins::YukigramPlugins(
        QWidget *parent,
        not_null<Window::SessionController*> controller)
: Section(parent, controller) {
        setupContent();
}

rpl::producer<QString> YukigramPlugins::title() {
        return rpl::single(u"Plugins"_q);
}

void YukigramPlugins::setupContent() {
        const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
        build(content, kPluginsSection);
        const auto pluginsList = content->add(
                object_ptr<Ui::VerticalLayout>(content));
        const auto refreshPlugins = [=] {
                pluginsList->clear();
                for (const auto &plugin : Yukigram::Plugins::LoadedPlugins()) {
                        AddButtonWithLabel(
                                pluginsList,
                                rpl::single(plugin.name),
                                rpl::single(plugin.version),
                                st::settingsButtonNoIcon);
                }
        };

        refreshPlugins();
        Yukigram::Plugins::PluginsChanged().start(
                [=] { refreshPlugins(); },
                [](auto&&) {},
                [] {},
                content->lifetime());
        Ui::ResizeFitChild(this, content);
}

} // namespace

Type YukigramPluginsId() {
        return YukigramPlugins::Id();
}

} // namespace Settings


















