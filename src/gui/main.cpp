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
// #include "xml_editor/xml.hpp"
// #include "xml_editor/io.hpp"
// #include "xml_editor/tree.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("XML GUI Skeleton");
    window.resize(800, 600);

    // Layouts
    QVBoxLayout *mainLayout = new QVBoxLayout(&window);
    QHBoxLayout *fileLayout = new QHBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Input area
    QTextEdit *inputArea = new QTextEdit();
    inputArea->setPlaceholderText("Enter XML content here or choose a file...");
    mainLayout->addWidget(inputArea);

    // Browse button & file path display
    QPushButton *browseButton = new QPushButton("Browse File");
    QLineEdit *filePathEdit = new QLineEdit();
    fileLayout->addWidget(browseButton);
    fileLayout->addWidget(filePathEdit);
    mainLayout->addLayout(fileLayout);

    // Operation buttons (placeholders)
    QPushButton *op1Button = new QPushButton("Validate");
    QPushButton *op2Button = new QPushButton("Format");
    QPushButton *op3Button = new QPushButton("To JSON");
    QPushButton *op4Button = new QPushButton("Manify");
    QPushButton *op5Button = new QPushButton("Compress");
    QPushButton *op6Button = new QPushButton("Decompress");
    buttonLayout->addWidget(op1Button);
    buttonLayout->addWidget(op2Button);
    buttonLayout->addWidget(op3Button);
    buttonLayout->addWidget(op4Button);
    buttonLayout->addWidget(op5Button);
    buttonLayout->addWidget(op6Button);
    mainLayout->addLayout(buttonLayout);

    // Output area
    QTextEdit *outputArea = new QTextEdit();
    outputArea->setReadOnly(true);
    mainLayout->addWidget(outputArea);

    // Save output
    QPushButton *saveButton = new QPushButton("Save Output");
    mainLayout->addWidget(saveButton);

    // --- Connections ---

    // Browse file
    QObject::connect(browseButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Select File", "");
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

    // Detect whether input is a file path or raw XML
    auto getInputType = [&]() -> QString {
        QString text = inputArea->toPlainText().trimmed();
        QFileInfo f(text);
        if (f.exists() && f.isFile())
            return "file";
        else
            return "xml";
    };

    // Operation buttons (placeholder functionality)
    QObject::connect(op1Button, &QPushButton::clicked, [&]() {
        QString type = getInputType();
        outputArea->setText("Operation 1 executed on " + type + " input.\n(This is just a placeholder.)");
    });

    QObject::connect(op2Button, &QPushButton::clicked, [&]() {
        QString type = getInputType();
        outputArea->setText("Operation 2 executed on " + type + " input.\n(This is just a placeholder.)");
    });

    // Save output
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
