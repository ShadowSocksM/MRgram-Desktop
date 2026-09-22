#include "settings/sections/settings_yukigram.h"
#include "yukigram/options_database.h"

#include "settings/settings_common_session.h"

#include "base/options.h"
#include "core/application.h"
#include "lang/lang_keys.h"
#include "yukigram/lang.h"
#include "settings/settings_builder.h"
#include "settings/sections/settings_main.h"
#include "ui/boxes/confirm_box.h"
#include "ui/ui_utility.h"
#include "ui/toast/toast.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/popup_menu.h"
#include "ui/wrap/vertical_layout.h"
#include "window/section_widget.h"
#include "window/window_session_controller.h"
#include "styles/style_boxes.h"
#include "styles/style_menu_icons.h"
#include "styles/style_layers.h"
#include "styles/style_settings.h"

#include <QtGui/QGuiApplication>

namespace Settings {

namespace {

using namespace Builder;

QString qs_sv(std::string_view v) {
	return QString::fromStdString(std::string(v));
};

void ConfirmRestart(Window::SessionController *controller) {
	controller->show(Ui::MakeConfirmBox({
		.text = tr::lng_settings_need_restart(),
		.confirmed = [] { Core::Restart(); },
		.confirmText = tr::lng_settings_restart_now(),
		.cancelText = tr::lng_settings_restart_later(),
	}));
}

void InputBox(
	not_null<Ui::GenericBox*> box,
	Window::SessionController *controller,
	rpl::producer<QString> title,
	QString description,
	rpl::producer<QString> validHint,
	QString current,
	QString defaultValue,
	std::function<QString(QString)> maybeUpdate,
	Ui::InputField::Mode mode,
	bool restartRequired
) {
	const auto content = box->verticalLayout();

	if (!description.isEmpty()) {
		content->add(object_ptr<Ui::FlatLabel>(content, description), st::markdownLinkFieldPadding);
	}

	const auto placeholder = validHint ? validHint : rktr("input/placeholder");
	const auto valueInput = content->add(
		object_ptr<Ui::InputField>(content, st::defaultInputField, mode, placeholder, current),
		st::markdownLinkFieldPadding);

	const auto submit = [=] {
		const auto value = valueInput->getTextWithTags().text;
		if (const auto reason = maybeUpdate(value); !reason.isEmpty()) {
			valueInput->showError();
			controller->showToast(reason);
			return;
		}
		if (restartRequired) {
			ConfirmRestart(controller);
		}
		box->closeBox();
	};

	valueInput->submits() | rpl::on_next([=] { submit(); }, valueInput->lifetime());

	box->setTitle(title);
	box->addLeftButton(rktr("input/reset"), [=] { valueInput->setText(defaultValue); });
	box->addButton(tr::lng_settings_apply(), submit);
	box->addButton(tr::lng_cancel(), [=] { box->closeBox(); });

	content->resizeToWidth(st::boxWidth);
	content->moveToLeft(0, 0);
	box->setWidth(st::boxWidth);

	box->setFocusCallback([=] { valueInput->setFocusFast(); });
}

void InputInt(
	Window::SessionController *controller,
	std::string_view key,
	const Yukigram::Options::Description<int> &d,
	std::shared_ptr<rpl::variable<int>> v
) {
	controller->show(Box(
		InputBox,
		controller,
		rktr(u"opt/%1/name"_q.arg(qs_sv(key))),
		ktr(u"opt/%1/desc"_q.arg(qs_sv(key))),
		d.validHint(),
		QString::number(v->current()),
		QString::number(d.defaultValue),
		[=] (QString s) {
			bool ok = false;
			int value = s.toInt(&ok);
			if (!ok) {
				return ktr("input/integer");
			}
			const auto reason = d.checkValid(value);
			if (reason.isEmpty()) {
				*v = value;
			}
			return reason;
		},
		Ui::InputField::Mode::SingleLine,
		d.restartRequired
	));
}

void InputString(
	Window::SessionController *controller,
	std::string_view key,
	const Yukigram::Options::Description<QString> &d,
	std::shared_ptr<rpl::variable<QString>> v
) {
	controller->show(Box(
		InputBox,
		controller,
		rktr(u"opt/%1/name"_q.arg(qs_sv(key))),
		ktr(u"opt/%1/desc"_q.arg(qs_sv(key))),
		d.validHint(),
		v->current(),
		d.defaultValue,
		[=] (QString s) {
			const auto reason = d.checkValid(s);
			if (reason.isEmpty()) {
				*v = s;
			}
			return reason;
		},
		Ui::InputField::Mode::MultiLine,
		d.restartRequired
	));
}

template<typename T>
Ui::SettingsButton *BuildButton(SectionBuilder &builder, Yukigram::Options::Option<T> &o) {
	const auto key = o.key;
	const auto d = o.d;
	auto v = o.v;
	auto c = builder.controller();
	const auto icon = d.mirrorExperimental ? &st::menuIconExperimental : nullptr;

	SectionBuilder::ButtonArgs args = {
		.id = QString("yukigram/%1").arg(qs_sv(key)),
		.title = rktr(u"opt/%1/name"_q.arg(qs_sv(key))),
		.st = icon ? nullptr : &st::settingsButtonNoIcon,
		.icon = { icon },
		.keywords = std::move(d.keywords),
	};

	if constexpr (std::is_same_v<T, bool>) {
		args.toggled = rpl::single(v->current());
	} else if constexpr (std::is_same_v<T, int>) {
		args.label = v->value() | rpl::map([](int i) { return QString::number(i); });
		args.onClick = [=] { InputInt(c, key, d, v); };
	} else if constexpr (std::is_same_v<T, QString>) {
		args.label = v->value();
		args.onClick = [=] { InputString(c, key, d, v); };
	}

	return builder.addButton(std::move(args));
}

void BuildDescription(SectionBuilder &builder, std::string_view key) {
	const auto descKey = u"opt/%1/desc"_q.arg(qs_sv(key));
	if (!ktr(descKey).isEmpty()) {
		builder.addSkip(st::settingsCheckboxesSkip);
		builder.addDividerText(rktr(descKey));
		builder.addSkip(st::settingsCheckboxesSkip);
	}
}

template<typename T>
void BuildRow(SectionBuilder &builder, Yukigram::Options::Option<T> &o) {
	auto button = BuildButton(builder, o);
	const auto controller = builder.controller();
	if (!controller) {
		return;
	}

	const auto link = u"tg://settings/yukigram/%1"_q.arg(qs_sv(o.key));
	const auto menu = button->lifetime().template make_state<base::unique_qptr<Ui::PopupMenu>>();
	button->events(
	) | rpl::filter([](not_null<QEvent*> e) {
		return e->type() == QEvent::ContextMenu;
	}) | rpl::on_next([=](not_null<QEvent*> e) {
		*menu = base::make_unique_q<Ui::PopupMenu>(button, st::popupMenuWithIcons);
		(*menu)->addAction(ktr("settings/yukigram/deep-link/copy"), [=] {
			TextUtilities::SetClipboardText({ link });
			controller->showToast(ktr("settings/yukigram/deep-link/copy/done"));
		}, &st::menuIconCopy);
		(*menu)->popup(QCursor::pos());
		e->accept();
	}, button->lifetime());

	if constexpr (std::is_same_v<T, bool>) {
		auto v = o.v;
		button->toggledValue(
		) | rpl::filter([=](bool toggled) {
			return toggled != v->current();
		}) | rpl::on_next([=, d = o.d](bool toggled) {
			*v = toggled;
			if (d.restartRequired) {
				ConfirmRestart(controller);
			}
		}, button->lifetime());

		BuildDescription(builder, o.key);
	}
}

const std::string_view Categories[] = {
	"interface",
	"info",
	"dialogs",
	"chat",
	"style",
	"messages",
	"reactions",
	"compose",
	"picker",
	"system",
};

void BuildYukigramSectionContent(SectionBuilder &builder) {
	bool isFirstNamedSection = true;
	auto &byCategory = Yukigram::Options::ByCategory();
	for (const auto &category : Categories) {
		const auto &options = byCategory[category];
		if (!options.empty()) {
			builder.addSkip();
			if (isFirstNamedSection) {
				isFirstNamedSection = false;
			} else {
				builder.addDivider();
				builder.addSkip();
			}
			builder.addSubsectionTitle({
				.id = u"yukigram/%1"_q.arg(qs_sv(category)),
				.title = rktr(u"optgroup/%1"_q.arg(qs_sv(category))),
				.keywords = { u"yukigram"_q, qs_sv(category) },
			});
		}
		for (const auto &key : options) {
			v::match(Yukigram::Options::Options()[key], [&](Yukigram::Options::Option<bool> &o) {
				BuildRow(builder, o);
			}, [&](Yukigram::Options::Option<int> &o) {
				BuildRow(builder, o);
			}, [&](Yukigram::Options::Option<QString> &o) {
				BuildRow(builder, o);
			});
		}
	}
}

class Yukigram : public Section<Yukigram> {
public:
	Yukigram(
		QWidget *parent,
		not_null<Window::SessionController*> controller);

	[[nodiscard]] rpl::producer<QString> title() override;

private:
	void setupContent();

};

const auto kMeta = BuildHelper({
	.id = Yukigram::Id(),
	.parentId = MainId(),
	.title = &tr::lng_settings_yukigram,
	.icon = &st::menuIconSettings,
}, [](SectionBuilder &builder) {
	BuildYukigramSectionContent(builder);
});

const SectionBuildMethod kYukigramSection = kMeta.build;

Yukigram::Yukigram(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	setupContent();
}

rpl::producer<QString> Yukigram::title() {
	return rktr("settings/yukigram/title");
}

void Yukigram::setupContent() {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
	build(content, kYukigramSection);
	Ui::ResizeFitChild(this, content);
}

} // namespace

Type YukigramId() {
	return Yukigram::Id();
}

} // namespace Settings



