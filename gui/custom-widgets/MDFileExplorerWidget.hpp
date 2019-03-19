#ifndef MDFILEEXPLORERWIDGET_HPP
#define MDFILEEXPLORERWIDGET_HPP

#include <QFileSystemModel>
#include <QListView>
#include <QString>


class MDFileExplorerWidgetLogic;

class MDFileExplorerWidget : public QListView {
    Q_OBJECT

public:
    using Ptr = MDFileExplorerWidget*;
    using ConstPtr = const MDFileExplorerWidget*;

    friend class MDFileExplorerWidgetLogic;

private:
    MDFileExplorerWidgetLogic* dPtr;
    bool isTapAndHoldEventInProgress;

public:
    explicit MDFileExplorerWidget(QWidget* parent = Q_NULLPTR);
    ~MDFileExplorerWidget();

public slots:
    void setPath(const QString& path);

private:
    void setupSignalsAndSlots();
    bool event(QEvent* event);

signals:
    void currentFolderSignal(const QString& folderPath);
    void currentItemSignal(const QString& path);

    void itemsPathSignal(const QStringList& items);
    void changeFolderRequestSignal(const QString& folderPath);


    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent* event);
};



#endif // MDFILEEXPLORERWIDGET_HPP
