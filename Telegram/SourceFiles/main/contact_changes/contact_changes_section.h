#pragma once

#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QVector>
#include <QTimer>
#include "ui/userpic_view.h"

#include "window/section_widget.h"
#include "window/section_memento.h"


namespace Main::ContactChanges {

class SectionMemento;

class Widget final : public Window::SectionWidget {
public:
    Widget(
        QWidget *parent,
        not_null<Window::SessionController*> controller);

    bool floatPlayerHandleWheelEvent(QEvent *e) override { return false; }
    QRect floatPlayerAvailableRect() override { return rect(); }

    bool hasTopBarShadow() const override {
        return true;
    }

    bool showInternal(
        not_null<Window::SectionMemento*> memento,
        const Window::SectionShow &params) override;

    std::shared_ptr<Window::SectionMemento> createMemento() override;

    auto createIdentityMemento()
        -> std::shared_ptr<Window::SectionMemento> override;

protected:
    void resizeEvent(QResizeEvent *e) override;

private:
    void setupContent();
    QScrollArea *_historyScroll = nullptr;
    struct AvatarItem {
        quint64 userId = 0;
        QPushButton *label = nullptr;
        Ui::PeerUserpicView view;
    };
    QVector<AvatarItem> _avatars;
    QTimer *_avatarRefreshTimer = nullptr;
};

class SectionMemento final : public Window::SectionMemento {
public:
    object_ptr<Window::SectionWidget> createWidget(
        QWidget *parent,
        not_null<Window::SessionController*> controller,
        Window::Column column,
        const QRect &geometry) override;
};

} // namespace Main::ContactChanges
