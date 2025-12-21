#include "xml_editor/gui_runner.hpp"
#include "xml_editor/io.hpp"
#include "xml_editor/xml.hpp"
#include "xml_editor/cli.hpp"
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <iostream>
#include <sstream>
#include <streambuf>

int main(int argc, char* argv[]) {
    if (argc == 1 || std::strcmp(argv[1], "--gui") == 0) {
        return run_gui(argc, argv); // launch GUI
    }

    // Otherwise → CLI mode
    xml_editor::cli::run_cli(argc, argv);
    return 0;
}




int run_gui(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("XML Editor");
    window.resize(800, 600);

    // Layouts
    QVBoxLayout *mainLayout = new QVBoxLayout(&window);
    QHBoxLayout *fileLayout = new QHBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Input area
    QTextEdit *inputArea = new QTextEdit();
    inputArea->setPlaceholderText("Enter XML content here or select a file...");
    mainLayout->addWidget(inputArea);

    // Browse file
    QPushButton *browseButton = new QPushButton("Browse File");
    QLineEdit *filePathEdit = new QLineEdit();
    fileLayout->addWidget(browseButton);
    fileLayout->addWidget(filePathEdit);
    mainLayout->addLayout(fileLayout);

    // Operation buttons
    QPushButton *validateButton = new QPushButton("Validate");
    QPushButton *formatButton = new QPushButton("Format");
    QPushButton *jsonButton = new QPushButton("To JSON");
    QPushButton *minifyButton = new QPushButton("Minify");
    QPushButton *compressButton = new QPushButton("Compress");
    QPushButton *decompressButton = new QPushButton("Decompress");
    buttonLayout->addWidget(validateButton);
    buttonLayout->addWidget(formatButton);
    buttonLayout->addWidget(jsonButton);
    buttonLayout->addWidget(minifyButton);
    buttonLayout->addWidget(compressButton);
    buttonLayout->addWidget(decompressButton);
    mainLayout->addLayout(buttonLayout);

    // Output area
    QTextEdit *outputArea = new QTextEdit();
    outputArea->setReadOnly(true);
    mainLayout->addWidget(outputArea);

    // Save output
    QPushButton *saveButton = new QPushButton("Save Output");
    mainLayout->addWidget(saveButton);

    // --- Connections ---

    QObject::connect(browseButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Select File", "", "XML Files (*.xml);;All Files (*)");
        if (!fileName.isEmpty()) {
            filePathEdit->setText(fileName);
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                inputArea->setText(in.readAll());
                file.close();
            }
        }
    });

    auto getInputText = [&]() -> QString {
        QString text = inputArea->toPlainText().trimmed();
        QFileInfo f(text);
        if (f.exists() && f.isFile()) {
            QFile file(text);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                QString content = in.readAll();
                file.close();
                return content;
            }
        }
        return text;
    };

    QObject::connect(validateButton, &QPushButton::clicked, [&]() {
        QString xmlText = getInputText();
        if (xmlText.isEmpty()) {
            outputArea->setText("Please enter XML content or select a file.");
            return;
        }

        bool valid = xml_editor::xml::is_valid(xmlText.toStdString());
        if (valid) {
            outputArea->setText("XML is valid!");
        } else {
            std::ostringstream oss;
            oss << "XML has errors (" << xml_editor::xml::get_error_count() << "):\n";
            for (auto &err : xml_editor::xml::get_errors()) {
                oss << "Line " << err.line << ": " << err.message << "\n";
            }
            oss << "\nFixed XML:\n" << xml_editor::xml::get_fixed_XML();
            outputArea->setText(QString::fromStdString(oss.str()));
        }
    });

    QObject::connect(formatButton, &QPushButton::clicked, [&]() {
        QString xmlText = getInputText();
        if (xmlText.isEmpty()) {
            outputArea->setText("Please enter XML content or select a file.");
            return;
        }
        auto tree = xml_editor::xml::parse(xmlText.toStdString());
        QString formatted = QString::fromStdString(xml_editor::xml::format(tree));
        outputArea->setText(formatted);
    });

    QObject::connect(jsonButton, &QPushButton::clicked, [&]() {
        QString xmlText = getInputText();
        if (xmlText.isEmpty()) {
            outputArea->setText("Please enter XML content or select a file.");
            return;
        }
        auto tree = xml_editor::xml::parse(xmlText.toStdString());
        QString jsonStr = QString::fromStdString(xml_editor::xml::to_json(tree));
        outputArea->setText(jsonStr);
    });

    QObject::connect(minifyButton, &QPushButton::clicked, [&]() {
        QString xmlText = getInputText();
        if (xmlText.isEmpty()) {
            outputArea->setText("Please enter XML content or select a file.");
            return;
        }
        QString minified = QString::fromStdString(xml_editor::xml::minify(xmlText.toStdString()));
        outputArea->setText(minified);
    });

    QObject::connect(compressButton, &QPushButton::clicked, [&]() {
        QString xmlText = getInputText();
        if (xmlText.isEmpty()) {
            outputArea->setText("Please enter XML content or select a file.");
            return;
        }
        QString compressed = QString::fromStdString(xml_editor::xml::compress(xmlText.toStdString()));
        outputArea->setText(compressed);
    });

    QObject::connect(decompressButton, &QPushButton::clicked, [&]() {
        QString xmlText = getInputText();
        if (xmlText.isEmpty()) {
            outputArea->setText("Please enter XML content or select a file.");
            return;
        }
        QString decompressed = QString::fromStdString(xml_editor::xml::decompress(xmlText.toStdString()));
        outputArea->setText(decompressed);
    });

    QObject::connect(saveButton, &QPushButton::clicked, [&]() {
        QString saveFile = QFileDialog::getSaveFileName(&window, "Save Output", "", "Text Files (*.txt);;All Files (*)");
        if (!saveFile.isEmpty()) {
            QFile file(saveFile);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << outputArea->toPlainText();
                file.close();
                QMessageBox::information(&window, "Saved", "Output saved successfully!");
            }
        }
    });

    window.show();
    return app.exec();
}
