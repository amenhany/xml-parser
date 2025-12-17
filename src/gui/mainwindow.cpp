#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xml_editor/xml.hpp"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_browseBtn_clicked() {
  QString fileName = QFileDialog::getOpenFileName(this, "Open XML File", "",
                                                  "XML Files (*.xml)");

  if (fileName.isEmpty())
    return;

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMessageBox::warning(this, "Error", "Cannot open file");
    return;
  }

  QTextStream in(&file);
  ui->inputText->setPlainText(in.readAll());
  file.close();
}

void MainWindow::on_formatBtn_clicked() {
  QString input = ui->inputText->toPlainText();

  if (input.isEmpty()) {
    QMessageBox::warning(this, "Error", "Input XML is empty");
    return;
  }

  // Convert QString → std::string
  std::string xml = input.toStdString();

  // Parse XML into tree
  xml_editor::Tree *tree = xml_editor::xml::parse(xml);

  if (!tree) {
    QMessageBox::warning(this, "Error", "Failed to parse XML");
    return;
  }

  // Format XML
  std::string formatted = xml_editor::xml::format(tree);

  // Show output
  ui->outputText->setPlainText(QString::fromStdString(formatted));

  delete tree;
}

void MainWindow::on_verifyBtn_clicked() {
  QString input = ui->inputText->toPlainText();
  std::string xml = input.toStdString();

  bool valid = xml_editor::xml::is_valid(xml);

  if (valid) {
    ui->outputText->setPlainText("XML is valid ✅");
  } else {
    std::string errors;
    for (const auto &e : xml_editor::xml::get_errors()) {
      errors += "Line " + std::to_string(e.line) + ": " + e.message + "\n";
    }
    ui->outputText->setPlainText(QString::fromStdString(errors));
  }
}

void MainWindow::on_jsonBtn_clicked() {
  QString input = ui->inputText->toPlainText();
  std::string xml = input.toStdString();

  xml_editor::Tree *tree = xml_editor::xml::parse(xml);
  if (!tree) {
    QMessageBox::warning(this, "Error", "Invalid XML");
    return;
  }

  std::string json = xml_editor::xml::to_json(tree);
  ui->outputText->setPlainText(QString::fromStdString(json));

  delete tree;
}
