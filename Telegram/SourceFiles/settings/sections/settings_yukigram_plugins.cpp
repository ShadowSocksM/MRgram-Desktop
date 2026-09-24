#include "settings/sections/settings_yukigram_plugins.h"

#include "settings/settings_builder.h"
#include "settings/settings_common_session.h"
#include "settings/settings_notifications_common.h"
#include "lang/lang_keys.h"
#include "core/file_utilities.h"
#include "yukigram/plugins/plugin_manager.h"
#include "settings/sections/settings_yukigram.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/checkbox.h"
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

class YukigramPluginDetails : public AbstractSection {
public:
        YukigramPluginDetails(
                QWidget *parent,
                not_null<Window::SessionController*> controller,
                const QString &pluginId);

        [[nodiscard]] Type id() const override;
        [[nodiscard]] rpl::producer<QString> title() override;

        [[nodiscard]] static Type Id(const QString &pluginId);

private:
        void setupContent();

        QString _pluginId;
};

struct YukigramPluginDetailsFactory final : AbstractSectionFactory {
        explicit YukigramPluginDetailsFactory(QString pluginId)
        : pluginId(std::move(pluginId)) {
        }

        object_ptr<AbstractSection> create(
                not_null<QWidget*> parent,
                not_null<Window::SessionController*> controller,
                not_null<Ui::ScrollArea*> scroll,
                rpl::producer<Container> containerValue
        ) const final override {
                return object_ptr<YukigramPluginDetails>(
                        parent,
                        controller,
                        pluginId);
        }

        const QString pluginId;
};
YukigramPluginDetails::YukigramPluginDetails(
        QWidget *parent,
        not_null<Window::SessionController*> controller,
        const QString &pluginId)
: AbstractSection(parent, controller)
, _pluginId(pluginId) {
        setupContent();
}

Type YukigramPluginDetails::Id(const QString &pluginId) {
        return std::make_shared<YukigramPluginDetailsFactory>(pluginId);
}

Type YukigramPluginDetails::id() const {
        return Id(_pluginId);
}

rpl::producer<QString> YukigramPluginDetails::title() {
        const auto plugin = Yukigram::Plugins::FindPlugin(_pluginId);
        return rpl::single(plugin ? plugin->name : u"Plugin Details"_q);
}
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
                        const auto pluginId = plugin.id;
                        const auto enabledValue = std::make_shared<rpl::variable<bool>>(plugin.enabled);
                        const auto [button, toggleButton, checkView] = SetupSplitToggle(
                                pluginsList,
                                rpl::single(plugin.name),
                                nullptr,
                                plugin.enabled,
                                enabledValue->value() | rpl::map([](bool enabled) {
                                        return enabled ? u"Enabled"_q : u"Disabled"_q;
                                }));

                        button->setClickedCallback([=] {
                                showOther(YukigramPluginDetails::Id(pluginId));
                        });

                        toggleButton->clicks(
                        ) | rpl::on_next([=] {
                                const auto enabled = !checkView->checked();
                                if (Yukigram::Plugins::SetPluginEnabled(pluginId, enabled)) {
                                        checkView->setChecked(enabled, anim::type::normal);
                                        *enabledValue = enabled;
                                }
                        }, toggleButton->lifetime());
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

void YukigramPluginDetails::setupContent() {
        const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
        const auto plugin = Yukigram::Plugins::FindPlugin(_pluginId);

        if (plugin) {
                AddButtonWithLabel(
                        content,
                        rpl::single(u"Name"_q),
                        rpl::single(plugin->name),
                        st::settingsButtonNoIcon);

                AddButtonWithLabel(
                        content,
                        rpl::single(u"Version"_q),
                        rpl::single(plugin->version),
                        st::settingsButtonNoIcon);

                AddButtonWithLabel(
                        content,
                        rpl::single(u"Author"_q),
                        rpl::single(plugin->author),
                        st::settingsButtonNoIcon);

                AddButtonWithLabel(
                        content,
                        rpl::single(u"Description"_q),
                        rpl::single(plugin->description),
                        st::settingsButtonNoIcon);
        }

        content->add(object_ptr<Ui::SettingsButton>(
                content,
                rpl::single(u"Uninstall Plugin"_q),
                st::settingsAttentionButton
        ))->setClickedCallback([=] {
                if (Yukigram::Plugins::UninstallPlugin(_pluginId)) {
                        showOther(YukigramPlugins::Id());
                }
        });
        Ui::ResizeFitChild(this, content);
}
} // namespace

Type YukigramPluginsId() {
        return YukigramPlugins::Id();
}

} // namespace Settings
































