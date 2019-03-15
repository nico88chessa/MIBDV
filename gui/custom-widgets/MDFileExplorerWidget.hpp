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

public:
    explicit MDFileExplorerWidget(QWidget* parent = Q_NULLPTR);
    ~MDFileExplorerWidget();

public slots:
    void setPath(const QString& path);

private:
    void setupSignalsAndSlots();
    bool event(QEvent* event);

signals:
    void currentSubFolderSignal(const QString& folderPath);
    void currentItemPathListSignal(const QStringList& items);

};

#endif // MDFILEEXPLORERWIDGET_HPP
