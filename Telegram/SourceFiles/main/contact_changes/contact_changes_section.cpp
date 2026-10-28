#include <QHBoxLayout>
#include <QPixmap>
#include "data/data_peer.h"
#include "ui/userpic_view.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include "main/contact_changes/contact_changes_section.h"

#include "window/window_session_controller.h"
#include "main/contact_changes/contact_changes_storage.h"
#include "main/main_session.h"
#include "data/data_user.h"
#include "data/data_photo.h"
#include "api/api_peer_photo.h"
#include "apiwrap.h"
#include "data/data_session.h"
#include "data/data_changes.h"

namespace Main::ContactChanges {

Widget::Widget(
    QWidget *parent,
    not_null<Window::SessionController*> controller)
: Window::SectionWidget(parent, controller) {
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(QStringLiteral("#172638")));
    setPalette(palette);
    setupContent();
}

void Widget::setupContent() {
    auto *title = new QLabel(QStringLiteral("Contact Changes - SushiGram"), this);
    title->setGeometry(24, 16, 450, 48);
    title->setStyleSheet(QStringLiteral("color: #70B7FF; font-size: 22px; font-weight: bold;"));
    title->setObjectName(QStringLiteral("contactChangesTitle"));
    title->show();
    auto *clearButton = new QPushButton(QStringLiteral("Clear History"), this);
    clearButton->setGeometry(qMax(0, width() - 155), 22, 135, 36);
    title->setGeometry(24, 16, qMax(0, width() - 190), 48);
    clearButton->setStyleSheet(QStringLiteral(
        "QPushButton { background: #9B3445; color: white; border: none;"
        "border-radius: 7px; font-weight: bold; padding: 6px; }"
        "QPushButton:hover { background: #BD4256; }"
    ));
    connect(clearButton, &QPushButton::clicked, this, [=] {
        const auto answer = QMessageBox::question(
            this,
            QStringLiteral("Clear History"),
            QStringLiteral("Delete all Contact Changes history? New changes will still be recorded."),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No);
        if (answer != QMessageBox::Yes) return;
        if (!controller()->session().clearContactChangesHistory()) {
            QMessageBox::warning(this, QStringLiteral("Clear History"),
                QStringLiteral("Could not clear history."));
            return;
        }
        _avatars.clear();
        _historyScroll->takeWidget()->deleteLater();
        auto *empty = new QLabel(QStringLiteral("No contact changes yet."), this);
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet(QStringLiteral("color: #AFC5DC; background: #172638;"));
        _historyScroll->setWidget(empty);
    });
    clearButton->setObjectName(QStringLiteral("contactChangesClear"));
    clearButton->show();

    _historyScroll = new QScrollArea(this);
    _historyScroll->setWidgetResizable(true);
    _historyScroll->setFrameShape(QFrame::NoFrame);
    _historyScroll->setStyleSheet(QStringLiteral(
        "QScrollArea { background: #172638; border: none; }"
        "QScrollBar:vertical { background: #172638; width: 9px; }"
        "QScrollBar::handle:vertical { background: #46627E; border-radius: 4px; min-height: 25px; }"
    ));

    auto *content = new QWidget();
    content->setStyleSheet(QStringLiteral("background-color: #172638;"));
    auto *layout = new QVBoxLayout(content);
    layout->setContentsMargins(16, 12, 16, 12);
    layout->setSpacing(8);

    const auto history = ContactChanges::LoadHistory(controller()->session().uniqueId());
    for (auto i = history.size(); i > 0; --i) {
        const auto &entry = history[i - 1];
        QString contactName = QStringLiteral("User ID: ") + QString::number(entry.userId);
        if (const auto user = controller()->session().data().userLoaded(UserId(entry.userId))) {
            contactName = user->name();
        }
        QString kind;
        switch (entry.type) {
        case ChangeType::Name: kind = QStringLiteral("Name changed"); break;
        case ChangeType::Username: kind = QStringLiteral("Username changed"); break;
        case ChangeType::Photo: kind = QStringLiteral("Profile photo changed"); break;
        }
        auto *item = new QLabel(
            kind + QStringLiteral("\n") + contactName
            + QStringLiteral("\n") + entry.oldValue
            + QStringLiteral("  ->  ") + entry.newValue
            + QStringLiteral("\n") + entry.detectedAt.toLocalTime().toString(QStringLiteral("yyyy/MM/dd HH:mm")),
            content);
        item->setWordWrap(true);
        item->setMinimumHeight(90);
        item->setStyleSheet(QStringLiteral(
            "QLabel { background: #22364D; color: #E5EFFA;"
            "border-radius: 10px; padding: 12px; font-size: 13px; }"
        ));
        auto *card = new QWidget(content);
        auto *cardLayout = new QHBoxLayout(card);
        cardLayout->setContentsMargins(0, 0, 0, 0);
        cardLayout->setSpacing(10);

        auto *avatar = new QPushButton(card);
        avatar->setFixedSize(48, 48);
        avatar->setFlat(true);
        avatar->setCursor(Qt::PointingHandCursor);
        avatar->setStyleSheet(QStringLiteral("QPushButton { border: none; background: transparent; padding: 0; }"));

        AvatarItem avatarItem;
        avatarItem.userId = entry.userId;
        avatarItem.label = avatar;
        if (const auto user = controller()->session().data().userLoaded(UserId(entry.userId))) {
            user->loadUserpic();
            avatarItem.view = user->createUserpicView();
            avatar->setIcon(QIcon(QPixmap::fromImage(
                PeerData::GenerateUserpicImage(user, avatarItem.view, 48))));
            avatar->setIconSize(QSize(48, 48));
        }
        QObject::connect(avatar, &QPushButton::clicked, this, [this, userId = entry.userId] {
            const auto user = controller()->session().data().userLoaded(UserId(userId));
            if (!user) return;
            const auto id = user->userpicPhotoId();
            if (id && user->owner().photo(id)->date()) {
                controller()->openPhoto(user->owner().photo(id), user);
                return;
            }
            user->updateFullForced();
            user->session().api().peerPhoto().requestUserPhotos(user, 0);
            auto *photoTimer = new QTimer(this);
            photoTimer->setInterval(200);
            auto *attempts = new int(0);
            QObject::connect(photoTimer, &QTimer::timeout, this,
                [this, photoTimer, attempts, userId] {
                    const auto finish = [photoTimer, attempts] {
                        photoTimer->stop();
                        photoTimer->deleteLater();
                        delete attempts;
                    };
                    if (++(*attempts) > 50) {
                        finish();
                        return;
                    }
                    const auto current = controller()->session().data().userLoaded(UserId(userId));
                    if (!current) {
                        finish();
                        return;
                    }
                    const auto photoId = current->userpicPhotoId();
                    if (photoId && current->owner().photo(photoId)->date()) {
                        controller()->openPhoto(current->owner().photo(photoId), current);
                        finish();
                    }
                });
            photoTimer->start();
        });
        _avatars.push_back(std::move(avatarItem));
        cardLayout->addWidget(avatar);
        cardLayout->addWidget(item, 1);
        layout->addWidget(card);
    }
    if (history.isEmpty()) {
        auto *empty = new QLabel(QStringLiteral("No contact changes recorded yet."), content);
        empty->setStyleSheet(QStringLiteral("color: #AFC5D9; padding: 24px;"));
        layout->addWidget(empty);
    }
    layout->addStretch();
    _historyScroll->setWidget(content);
    _historyScroll->setGeometry(0, 72, width(), qMax(0, height() - 72));
    _historyScroll->show();
    _avatarRefreshTimer = new QTimer(this);
    _avatarRefreshTimer->setSingleShot(true);
    _avatarRefreshTimer->setInterval(350);
    QObject::connect(_avatarRefreshTimer, &QTimer::timeout, this, [this] {
        if (!_historyScroll || !isVisible()) return;
        const auto visibleArea = _historyScroll->viewport()->rect();
        for (auto &avatar : _avatars) {
            if (!avatar.label || !avatar.label->isVisible()) continue;
            const auto position = avatar.label->mapTo(
                _historyScroll->viewport(), QPoint(0, 0));
            if (!QRect(position, avatar.label->size()).intersects(visibleArea)) continue;
            if (const auto user = controller()->session().data().userLoaded(
                    UserId(avatar.userId))) {
                avatar.label->setIcon(QIcon(QPixmap::fromImage(
                    PeerData::GenerateUserpicImage(user, avatar.view, 48))));
                avatar.label->setIconSize(QSize(48, 48));
            }
        }
    });
    controller()->session().downloaderTaskFinished() | rpl::on_next([=] {
        if (!_avatarRefreshTimer->isActive()) {
            _avatarRefreshTimer->start();
        }
    }, lifetime());
}

bool Widget::showInternal(
    not_null<Window::SectionMemento*> memento,
    const Window::SectionShow &params) {
    return dynamic_cast<SectionMemento*>(memento.get()) != nullptr;
}

std::shared_ptr<Window::SectionMemento> Widget::createMemento() {
    return std::make_shared<SectionMemento>();
}

auto Widget::createIdentityMemento()
    -> std::shared_ptr<Window::SectionMemento> {
    return std::make_shared<SectionMemento>();
}

void Widget::resizeEvent(QResizeEvent *e) {
    Window::SectionWidget::resizeEvent(e);
    if (auto *title = findChild<QLabel*>(QStringLiteral("contactChangesTitle"))) {
        title->setGeometry(16, 16, qMax(0, width() - 190), 48);
    }
    if (auto *clear = findChild<QPushButton*>(QStringLiteral("contactChangesClear"))) {
        clear->setGeometry(qMax(0, width() - 155), 22, 135, 36);
    }
    if (_historyScroll) {
        _historyScroll->setGeometry(0, 72, width(), qMax(0, height() - 72));
    }
}

object_ptr<Window::SectionWidget> SectionMemento::createWidget(
    QWidget *parent,
    not_null<Window::SessionController*> controller,
    Window::Column column,
    const QRect &geometry) {
    if (column == Window::Column::Third) {
        return nullptr;
    }
    auto result = object_ptr<Widget>(parent, controller);
    result->setGeometry(geometry);
    return result;
}

} // namespace Main::ContactChanges
