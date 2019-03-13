#ifndef MDFILEEXPLORERWIDGETLOGIC_HPP
#define MDFILEEXPLORERWIDGETLOGIC_HPP

#include <QString>
#include <QFileSystemModel>

class MDFileExplorerWidget;

class MDFileExplorerWidgetLogic {
public:
    using Ptr = MDFileExplorerWidgetLogic*;
    using ConstPtr = const MDFileExplorerWidgetLogic*;

    friend class MDFileExplorerWidget;

private:
    MDFileExplorerWidget* qPtr;
    QFileSystemModel model;

public:
    MDFileExplorerWidgetLogic(const QString& rootPath = "");
    void setPath(const QString& path);
    QString currentPath() const;

};

#endif // MDFILEEXPLORERWIDGETLOGIC_HPP
