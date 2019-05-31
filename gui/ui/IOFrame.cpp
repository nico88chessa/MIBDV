#include "IOFrame.hpp"
#include "IOFrameLogic.hpp"
#include "ui_IOFrame.h"

#include <Settings.hpp>
#include <DigitalInputValue.hpp>
#include <DigitalOutputValue.hpp>
#include <AnalogInputValue.hpp>
#include <gui/custom-widgets/MDSwitchLine.hpp>
#include <gui/custom-widgets/MDDoubleSpinBox.hpp>
#include <gui/ui/DialogAlert.hpp>

#include <DeviceFactory.hpp>

#include <Logger.hpp>


using namespace PROGRAM_NAMESPACE;

/*******************************************
 *       I O   F R A M E   L O G I C
 *******************************************/

IOFrameLogic::IOFrameLogic(QObject* parent) : QObject (parent) {

    traceEnter;
    traceExit;

}

IOFrameLogic::~IOFrameLogic() {

    traceEnter;
    traceExit;

}

void IOFrameLogic::initialize() {

    traceEnter;
    this->ioManager = DeviceFactoryInstance.instanceIOManager();
    this->ioSignaler = DeviceFactoryInstance.getIOSignaler();

    if (!this->ioManager->isConnected())
        this->ioManager->connect();

    traceExit;

}

void IOFrameLogic::setDigitalOutput(IOType type) {

    traceEnter;
    bool ok = this->ioManager->setDigitalOutput(type);

    if (!ok) {
        DialogAlert diag;
        diag.setupLabels("Error", tr("Impossibile impostare l'uscita digitale."));
        diag.exec();
    }

    traceExit;

}

void IOFrameLogic::unsetDigitalOutput(IOType type) {

    traceEnter;
    bool ok = this->ioManager->unsetDigitalOutput(type);

    if (!ok) {
        DialogAlert diag;
        diag.setupLabels("Error", tr("Impossibile impostare l'uscita digitale."));
        diag.exec();
    }

    traceExit;

}


/*
 * I O   F R A M E
 */

int IOFrame::getRowMinHeight() const { return rowMinHeight; }

void IOFrame::setRowMinHeight(int value) { rowMinHeight = value; }

IOFrame::IOFrame(QWidget *parent) :
    QFrame(parent), ui(new Ui::IOFrame), dPtr(new IOFrameLogic(parent)) {

    traceEnter;

    auto&& s = Settings::instance();
    dPtr->qPtr = this;
    dPtr->initialize();

    digitalInputs = s.getDigitalInputs();
    digitalOutputs = s.getDigitalOutputs();
    analogInputs = s.getAnalogInputs();

    this->setupUi();
    this->setupSignalsAndSlots();

    traceExit;

}

IOFrame::~IOFrame() {

    traceEnter;
    delete dPtr;
    delete ui;
    traceExit;

}

void IOFrame::setupUi() {

    traceEnter;

    ui->setupUi(this);

    // tab IO digitali
    QWidget* tabDigitalIO = ui->tabDigitalIO;
    QGridLayout* glDigitalIO = ui->glDigitalIOContents;

    const int colDigitalInput = 0;
    const int colDigitalOutput = 1;

    // ingressi digitali
    auto spacerDigitalIO = ui->vsDigitalIO;
    glDigitalIO->removeItem(spacerDigitalIO);

    int startDIRow = 0;
    for (auto&& digitalInput : digitalInputs) {

        QCheckBox* switchLine = new QCheckBox(tabDigitalIO);
        switchLine->setObjectName(digitalInput.getName());
        switchLine->setProperty("isDigitalIO", true);
        switchLine->setText(digitalInput.getName());
        switchLine->setEnabled(false);
        glDigitalIO->addWidget(switchLine, ++startDIRow, colDigitalInput);

    }

    // uscite digitali
    int startDORow = 0;
    for (auto&& digitalOutput : digitalOutputs) {

        MDSwitchLine* switchLine = new MDSwitchLine(tabDigitalIO);
        switchLine->setObjectName(digitalOutput.getName());
        switchLine->setText(digitalOutput.getName());
        glDigitalIO->addWidget(switchLine, ++startDORow, colDigitalOutput);

    }

    int maxStartRow = qMax(startDIRow, startDORow);
    glDigitalIO->addItem(spacerDigitalIO, ++maxStartRow, colDigitalInput);

    // tab IO analogici
    QWidget* tabAnalogIO = ui->tabAnalogIO;
    QGridLayout* glAnalogIO = ui->glAnalogIOContents;

    // ingressi analogici
    const int colAnalogInput = 0;

    auto spacerAi = ui->vsAnalogInput;
    int startRow = 0;
    glAnalogIO->removeItem(spacerAi);

    for (auto&& analogInput : analogInputs) {

        MDDoubleSpinBox* spinBox = new MDDoubleSpinBox(tabAnalogIO);
        spinBox->setObjectName(analogInput.getName());
        spinBox->setDecimals(3);
        spinBox->setRange(IOFRAME_DSB_MIN_VALUE, IOFRAME_DSB_MAX_VALUE);
        spinBox->setEnabled(false);
        QString label = analogInput.getName();
        if (!analogInput.getUnit().isEmpty())
            label = label.append(" [%1]").arg(analogInput.getUnit());

        spinBox->setLabel(label);
        glAnalogIO->addWidget(spinBox, ++startRow, colAnalogInput);

    }

    glAnalogIO->addItem(spacerAi, ++startRow, colAnalogInput);

    this->updateUI();

    traceExit;

}

void IOFrame::setupSignalsAndSlots() {

    traceEnter;

    connect(dPtr->ioSignaler.data(), &IOSignaler::statusSignal, [&](auto a, auto b, auto c) {
        QMetaObject::invokeMethod(this, "updateDigitalIOStatus", Qt::QueuedConnection,
                                  Q_ARG(const mibdv::DigitalInputStatus&, a),
                                  Q_ARG(const mibdv::DigitalOutputStatus&, b),
                                  Q_ARG(const mibdv::AnalogInputStatus&, c));
    });

    auto&& digitalIOSA = ui->saDigitalIOContents;

    for (auto&& digitalOutput : digitalOutputs) {

        QString widgetName = digitalOutput.getName();
        auto widget = digitalIOSA->findChild<MDSwitchLine*>(widgetName, Qt::FindDirectChildrenOnly);
        if (widget==nullptr)
            continue;

        IOType type = digitalOutput.getElementType();
        connect(widget, &MDSwitchLine::toggled, [this, type](bool value){
            value ? dPtr->setDigitalOutput(type) : dPtr->unsetDigitalOutput(type);
        });

    }

    traceExit;

}

void IOFrame::paintEvent(QPaintEvent* event) {

    QFrame::paintEvent(event);

    QGridLayout* glDigitalIO = ui->glDigitalIOContents;
    for (int i=0; i<glDigitalIO->rowCount(); ++i)
        glDigitalIO->setRowMinimumHeight(i, rowMinHeight);

    QGridLayout* glAnalogIO = ui->glAnalogIOContents;
    for (int i=0; i<glAnalogIO->rowCount(); ++i)
        glAnalogIO->setRowMinimumHeight(i, rowMinHeight);

}

void IOFrame::updateUI() {

    traceEnter;

    auto&& glDigitalIO = ui->saDigitalIOContents;
    auto&& glAnalogIO = ui->saAnalogIOContents;

    for (auto&& digitalInput: digitalInputStatus) {

        auto&& widget = glDigitalIO->findChild<QCheckBox*>(digitalInput.getName(), Qt::FindDirectChildrenOnly);
        if (widget)
            widget->setChecked(digitalInput.getValue());

    }

    for (auto&& digitalOutput: digitalOutputStatus) {

        auto&& widget = glDigitalIO->findChild<MDSwitchLine*>(digitalOutput.getName(), Qt::FindDirectChildrenOnly);
        if (widget)
            widget->setIsOn(digitalOutput.getValue());

    }

    for (auto&& analogInput: analogInputStatus) {

        auto&& widget = glAnalogIO->findChild<MDDoubleSpinBox*>(analogInput.getName(), Qt::FindDirectChildrenOnly);
        if (widget)
            widget->setValue(analogInput.getValue());

    }

    traceExit;

}

void IOFrame::updateDigitalIOStatus(const DigitalInputStatus& iStatus,
                                    const DigitalOutputStatus& oStatus,
                                    const AnalogInputStatus& aiStatus) {

    traceEnter;
    this->digitalInputStatus = iStatus;
    this->digitalOutputStatus = oStatus;
    this->analogInputStatus = aiStatus;
    this->updateUI();
    traceExit;

}
