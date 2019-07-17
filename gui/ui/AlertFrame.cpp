#include "AlertFrame.hpp"
#include "AlertFrameLogic.hpp"
#include "ui_AlertFrame.h"

#include <algorithm>

#include "MotionFrame.hpp"
#include <QLineEdit>

#include <Logger.hpp>
#include <QPainter>
#include <QStyleOptionViewItemV4>
#include <QStylePainter>

#include <custom-widgets/ErrorItemWidget.hpp>
#include <devices/DeviceFactory.hpp>


using namespace PROGRAM_NAMESPACE;


/*
 *  A L E R T   F R A M E   L O G I C
 */

AlertFrameLogic::AlertFrameLogic() {

    traceEnter;
    errorManager = DeviceFactoryInstance.getErrorManager();
    traceExit;

}

AlertFrameLogic::~AlertFrameLogic() {

    traceEnter;
    traceExit;
}

void AlertFrameLogic::updateModel(const QList<Error>& updateErrors) {

    model->updateErrors(updateErrors);

}



/*
 * A L E R T   I T E M   M O D E L
 */

AlertItemModel::AlertItemModel(QObject* parent) : QAbstractListModel(parent), errors() { }

AlertItemModel::~AlertItemModel() { }

int AlertItemModel::rowCount(const QModelIndex& parent) const {
    return errors.size();
}

QVariant AlertItemModel::data(const QModelIndex& index, int role) const {

    // questo mi serve perche' altrimenti devo ritornare l'errore solo per la fase di rendering
    if (role == Qt::DisplayRole) {
        const Error& bean = errors.at(index.row());
        return QVariant::fromValue(bean);
    } else
        return QVariant();

}

bool AlertItemModel::removeRows(int row, int count, const QModelIndex& parent) {

    beginRemoveRows(parent, row, row + count -1);
    errors.removeAt(row);
    endRemoveRows();
    return true;

}

bool AlertItemModel::addError(const Error& err) {

    if (this->errors.contains(err))
        return false;

    int size = this->rowCount();
    this->beginInsertRows(QModelIndex(), size, size);
    errors.append(err);
    this->endInsertRows();

    return true;

}

void AlertItemModel::updateErrors(const QList<Error> refreshList) {

    const QList<Error>& currentErrorList = this->errors;
    QList<Error> errors2Remove;

    bool modelChanged = false;

    /* NOTE NIC 15/07/2019 - rimuovo gli errori non piu' presenti oppure presenti ma con stato diverso
     * per rimuovere gli errori, non posso rimuoverli direttamente dalla lista perche' altrimenti
     * ho problemi con gli indici; allora li inserisco temporaneamente in una lista degli errori
     * da rimuovere
     */
    for (auto&& oldErr: currentErrorList) {
        if (!refreshList.contains(oldErr))
            errors2Remove.append(oldErr);
        else {
            int index = refreshList.indexOf(oldErr);
            Error newError = refreshList.at(index);
            if (newError.getErrorType() != oldErr.getErrorType())
                errors2Remove.append(oldErr);
        }
    }

    // qui rimuovo effettivamente gli errori
    for (auto&& err: errors2Remove) {
        this->removeRow(currentErrorList.indexOf(err));
        modelChanged = true;
    }

    // e aggiungo quelli nuovi
    for (auto&& error: refreshList)
        if (this->addError(error))
            modelChanged = true;

    if (modelChanged) {
        /* ordine priorita'
         * 1. tipo errore piu alto
         * 2. codice device inferiore
         * 3. codice errore inferiore
         */
        std::sort(errors.begin(), errors.end(), [](const Error& l, const Error& r) -> bool {

            // confronto prima il tipo di codice
            if (l.getErrorType() > r.getErrorType())
                return true;
            if (l.getErrorType() < r.getErrorType())
                return false;

            // se il tipo di codice e' uguale, confronto il device
            if (static_cast<int>(l.getDeviceKey()) < static_cast<int>(r.getDeviceKey()))
                return true;
            if (static_cast<int>(l.getDeviceKey()) > static_cast<int>(r.getDeviceKey()))
                return false;

            // se il device e' uguale, confronto il codice dell'errore
            return l.getErrorId() < r.getErrorId();
        });
    }

}

void AlertItemModel::clear() {

    this->beginResetModel();
    this->errors.clear();
    this->endResetModel();

}

QList<Error> AlertItemModel::getErrors() const {
    return errors;
}



/*
 *  A L E R T   I T E M   D E L E G A T E
 */

AlertItemDelegate::AlertItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

AlertItemDelegate::~AlertItemDelegate() { }

void AlertItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {

    QVariant data = index.data();

    if (data.canConvert<Error>()) {

        auto&& styleOption = QStyleOptionViewItem(option);

        ErrorItemWidget widget;
        if (styleOption.state & QStyle::State_Selected)
            widget.setProperty(ErrorItemWidget::FOCUS_PROPERTY_NAME, true);

        widget.resize(option.rect.width(), option.rect.height());
        widget.setError(qvariant_cast<Error>(data));
        painter->save();
        painter->translate(option.rect.topLeft());
        widget.render(painter);
        painter->restore();

    } else
        QStyledItemDelegate::paint(painter, option, index);

}

QSize AlertItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {

    int width = option.rect.width();
    int height = 48; // guardare stylesheet: 46 (minheight) + 2 (border bottom)
    return QSize(width, height);

}


/*
 *  A L E R T   F R A M E
 */

AlertFrame::AlertFrame(QWidget *parent) :
    QFrame(parent), ui(new Ui::AlertFrame), dPtr(new AlertFrameLogic()) {

    traceEnter;
    dPtr->qPtr = this;
    this->setupUi();

    dPtr->model = new AlertItemModel(this);
    delegate = new AlertItemDelegate(ui->lvAlerts);

    ui->lvAlerts->setModel(dPtr->model);
    ui->lvAlerts->setItemDelegate(delegate);

    ui->eiwSample->setError(Error(DeviceKey::GALIL_CN, 10, "ciao", ErrorType::ERROR));

    this->setupSignalsAndSlots();

//    constexpr int size = 2000;
//    QString descr = "questo e' un errore grave 1 questo e' un errore grave 2 questo e' un errore grave 3 questo e' un errore grave 4 questo e' un errore grave 5 \
//questo e' un errore grave 6 questo e' un errore grave 7 questo e' un errore grave 8 questo e' un errore grave 9 questo e' un errore grave 10";
//    Error vet[size];
//    for (int i=0; i<size; ++i)
//        switch (i%4) {
//        case ErrorType::INFO: new (&vet[i]) Error(static_cast<int>(DeviceKey::GALIL_CN), i, descr, ErrorType::INFO); break;
//        case ErrorType::WARNING: new (&vet[i]) Error(static_cast<int>(DeviceKey::GALIL_CN), i, descr, ErrorType::WARNING); break;
//        case ErrorType::ERROR: new (&vet[i]) Error(static_cast<int>(DeviceKey::GALIL_CN), i, descr, ErrorType::ERROR); break;
//        case ErrorType::FATAL: new (&vet[i]) Error(static_cast<int>(DeviceKey::GALIL_CN), i, descr, ErrorType::FATAL); break;
//        }

//    for (int i=0; i<size; ++i)
//        dPtr->model->addError(QVariant::fromValue(vet[i]));

    traceExit;

}

AlertFrame::~AlertFrame() {

    traceEnter;
    delete ui;
    delete dPtr;
    traceExit;

}

void AlertFrame::setupUi() {

    traceEnter;
    ui->setupUi(this);
    ui->lvAlerts->setSelectionMode(QListView::NoSelection);
    traceExit;

}

void AlertFrame::setupSignalsAndSlots() {

    traceEnter;

    connect(dPtr->errorManager.data(), &ErrorManager::errorListUpdated, dPtr, &AlertFrameLogic::updateModel);

    traceExit;

}
