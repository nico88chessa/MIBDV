#ifndef JSON_UTILS_HPP
#define JSON_UTILS_HPP

#include <QFile>
#include <QByteArray>

class JsonUtils {

public:

    static QByteArray readFilterHeaderFromJsonFile(const QString& path) {

        QFile file(path);

        if (!file.exists())
            return QByteArray();

        if (!file.open(QFile::ReadOnly | QFile::Text))
            return QByteArray();

        QByteArray header;
        header.append(file.readLine());
        header.append(file.readLine());
        header.append(file.readLine());
        QString lastLine = file.readLine();
        lastLine = lastLine.left(lastLine.size()-2);
        header.append(lastLine);
        header.append("}");

        file.close();
        return header;

    }

};

#endif // JSON_UTILS_HPP
