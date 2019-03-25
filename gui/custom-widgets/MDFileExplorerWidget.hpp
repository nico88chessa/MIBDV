#ifndef MDFILEEXPLORERWIDGET_HPP
#define MDFILEEXPLORERWIDGET_HPP

#include <QListView>

#include <model/FilterFileExplorerModel.hpp>

class MDFileExplorerWidget : public QListView {
    Q_OBJECT

public:
    using Ptr = MDFileExplorerWidget*;
    using ConstPtr = const MDFileExplorerWidget*;

private:
    bool isTapAndHoldEventInProgress;
    FilterFileExplorerModel::Ptr model;

public:
    explicit MDFileExplorerWidget(QWidget* parent = Q_NULLPTR);
    ~MDFileExplorerWidget();

public slots:
    void setPath(const QString& path);
    void clearSelection();
    void removeCurrentItem();
    void renameCurrentItem();

private:
    void setupSignalsAndSlots();
    bool event(QEvent* event);

signals:
    void currentFolderSignal(const QString& folderPath);
    void currentItemSignal(const QString& path);

    void itemsPathSignal(const QStringList& items);
    void changeFolderRequestSignal(const QString& folderPath);

    void selectionClearSignal();

protected:
    void mousePressEvent(QMouseEvent* event);

};



#endif // MDFILEEXPLORERWIDGET_HPP
