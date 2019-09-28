#include "LeftFrame.hpp"
#include "LeftFrameLogic.hpp"
#include "ui_LeftFrame.h"


#include <custom-widgets/MDListItemWidget.hpp>
#include <configure.h>
#include <Logger.hpp>
#include <QPainter>
#include <QListView>


using namespace PROGRAM_NAMESPACE;


/*
 *   M E N U   I T E M   D E L E G A T E
 */

MenuItemDelegate::MenuItemDelegate(QObject *parent) : QStyledItemDelegate(parent) { }

MenuItemDelegate::~MenuItemDelegate() { }

void MenuItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {

    QVariant data = index.data();

    if (data.canConvert<MenuItem>()) {

        auto&& styleOption = QStyleOptionViewItem(option);
        const MenuItem& item = data.value<MenuItem>();

        MDListItemWidget widget;

        if (styleOption.state & QStyle::State_Selected)
            widget.setIsActive(true);

        if (styleOption.state & QStyle::State_MouseOver)
            widget.setIsHover(true);

        painter->setBackgroundMode(Qt::BGMode::TransparentMode);
        widget.resize(option.rect.width(), option.rect.height());
        widget.setText(item.getLabel());
        widget.setObjectName("MDLIW_"+item.getKey());
        painter->save();
        painter->translate(option.rect.topLeft());
        widget.render(painter, QPoint(), QRegion(), QWidget::RenderFlags(QWidget::DrawChildren));
        painter->restore();

    } else
        QStyledItemDelegate::paint(painter, option, index);

}

QSize MenuItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {

    Q_UNUSED(index);
    int width = option.rect.width();
    int height = 48; // guardare stylesheet: 46 (minheight) + 2 (border bottom)
    return QSize(width, height);

}



/*
 * M E N U   I T E M   M O D E L
 */

MenuItemModel::MenuItemModel(QObject* parent) : QAbstractListModel(parent), items() { }

MenuItemModel::~MenuItemModel() { }

int MenuItemModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent)
    return items.size();
}

QVariant MenuItemModel::data(const QModelIndex& index, int role) const {

    // questo mi serve perche' altrimenti devo ritornare l'errore solo per la fase di rendering
    if (role == Qt::DisplayRole) {
        const MenuItem& bean = items.at(index.row());
        return QVariant::fromValue(bean);
    } else
        return QVariant();
}

bool MenuItemModel::addItem(const MenuItem& item) {

    int size = this->rowCount();
    this->beginInsertRows(QModelIndex(), size, size);
    items.append(item);
    this->endInsertRows();

    return true;
}



/*
 * L E F T   F R A M E   L O G I C
 */

LeftFrameLogic::LeftFrameLogic() {
    traceEnter;
    traceExit;
}

LeftFrameLogic::~LeftFrameLogic() {

    traceEnter;
    traceExit;

}



/*
 * L E F T   F R A M E
 */

LeftFrame::LeftFrame(QWidget *parent) :
    QFrame(parent), ui(new Ui::LeftFrame), dPtr(new LeftFrameLogic()) {

    traceEnter;
    dPtr->qPtr = this;
    dPtr->model = new MenuItemModel(this);
    dPtr->delegate = new MenuItemDelegate(this);

    this->setupUi();
    this->setupSignalsAndSlots();

    ui->lvMenu->setModel(dPtr->model);
    ui->lvMenu->setItemDelegate(dPtr->delegate);

    traceExit;

}

LeftFrame::~LeftFrame() {

    traceEnter;
    delete ui;
    delete dPtr;
    traceExit;

}

void LeftFrame::setupUi() {

    traceEnter;
    ui->setupUi(this);

    dPtr->model->addItem(MenuItem("ItemAlerts", "Alerts"));
    dPtr->model->addItem(MenuItem("ItemIO", "IO"));
    dPtr->model->addItem(MenuItem("ItemMotion", "Motion"));
    dPtr->model->addItem(MenuItem("ItemFileExplorer", "FileExplorer"));

    traceExit;

}

void LeftFrame::setupSignalsAndSlots() {

    traceEnter;

    connect(ui->lvMenu, &QListView::clicked, [&](const QModelIndex& index) {

        QVariant data = index.data();
        if (data.canConvert<MenuItem>()) {

            const MenuItem& menuItem = data.value<MenuItem>();
            QString key = menuItem.getKey();
            PageItem item = PageItem::INVALID_PAGE;

            if (key.compare("ItemAlerts", Qt::CaseInsensitive) == 0)
                item = PageItem::ALERT_PAGE;
            else if (key.compare("ItemIO", Qt::CaseInsensitive) == 0)
                item = PageItem::IO_PAGE;
            else if (key.compare("ItemMotion", Qt::CaseInsensitive) == 0)
                item = PageItem::MOTION_PAGE;
            else if (key.compare("ItemFileExplorer", Qt::CaseInsensitive) == 0)
                item = PageItem::FILE_EXPLORER_PAGE;

            if (item != PageItem::INVALID_PAGE)
                emit pageItemChanged(item);
        }
    });

    traceExit;

}

void LeftFrame::setPageItem(PageItem i) {

    traceEnter;
    if (i == PageItem::INVALID_PAGE)
        return;
    ui->lvMenu->setCurrentIndex(dPtr->model->index(static_cast<int>(i)));
    traceExit;

}
