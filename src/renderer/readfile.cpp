#include <readfile.hpp>

#include <fstream>
#include <QFile>

std::string readFile(const std::string& filename){
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if(!file){
        throw std::runtime_error("Err: readFile: Failed to open file for reading");
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string content;
    content.resize(size);
    
    if(!file.read(&content[0], size)){
        throw std::runtime_error("Err: readFile: Failed to read file content");
    }

    return content;
}

std::string readQFile(const QString& path){
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        throw std::runtime_error("Err: readQFile: Failed to open shader: " + path.toStdString());
    }

    QByteArray source = file.readAll();

    return source.toStdString();
}

