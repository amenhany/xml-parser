#include "gui.h"

#include "xml_editor/graph.hpp"
#include "xml_editor/io.hpp"
#include "xml_editor/sna.hpp"
#include "xml_editor/tree.hpp"
#include "xml_editor/user.hpp"
#include "xml_editor/util.hpp"
#include "xml_editor/visualization.hpp"
#include "xml_editor/xml.hpp"

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>
#include <sstream>
#include <streambuf>

namespace xml_editor::gui {

int run_gui(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QWidget window;
  window.setWindowTitle("XML Editor");
  window.resize(900, 700);

  // Layouts
  QVBoxLayout *mainLayout = new QVBoxLayout(&window);
  QHBoxLayout *fileLayout = new QHBoxLayout();
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  QHBoxLayout *graphLayout = new QHBoxLayout();

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

  // Graph/Network buttons
  QPushButton *mostActiveButton = new QPushButton("Most Active");
  QPushButton *mostInfluencerButton = new QPushButton("Most Influencer");
  QPushButton *mutualButton = new QPushButton("Mutual Followers");
  QPushButton *suggestButton = new QPushButton("Suggest Users");
  QPushButton *searchWordButton = new QPushButton("Search Word");
  QPushButton *searchTopicButton = new QPushButton("Search Topic");
  QPushButton *drawGraphButton = new QPushButton("Draw Graph");

  graphLayout->addWidget(mostActiveButton);
  graphLayout->addWidget(mostInfluencerButton);
  graphLayout->addWidget(mutualButton);
  graphLayout->addWidget(suggestButton);
  graphLayout->addWidget(searchWordButton);
  graphLayout->addWidget(searchTopicButton);
  graphLayout->addWidget(drawGraphButton);
  mainLayout->addLayout(graphLayout);

  // Output area
  QTextEdit *outputArea = new QTextEdit();
  outputArea->setReadOnly(true);
  mainLayout->addWidget(outputArea);

  // Save output
  QPushButton *saveButton = new QPushButton("Save Output");
  mainLayout->addWidget(saveButton);

  // Variable to track the last operation type for the save dialog
  static QString lastOpType = "xml";

  // --- Helpers

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

  auto buildGraph = [&]() -> std::unique_ptr<Graph> {
    QString xmlText = getInputText();
    if (xmlText.isEmpty())
      return nullptr;

    Tree *tree = xml::parse(xmlText.toStdString());
    if (!tree)
      return nullptr;

    return std::make_unique<Graph>(tree);
  };

  // --- Connections for CLI-like XML operations ---

  QObject::connect(browseButton, &QPushButton::clicked, [&]() {
    QString fileName = QFileDialog::getOpenFileName(
        &window, "Open XML File", "",
        "XML Files (*.xml);;Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty()) {
      filePathEdit->setText(fileName);
      QFile file(fileName);
      if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        inputArea->setPlainText(in.readAll());
        file.close();
      }
    }
  });

  QObject::connect(validateButton, &QPushButton::clicked, [&]() {
    QString xmlText = getInputText();
    if (xmlText.isEmpty()) {
      outputArea->setText("No XML content.");
      return;
    }
    lastOpType = "xml";

    bool valid = xml::is_valid(xmlText.toStdString());
    if (valid) {
      QMessageBox::information(&window, "Validation", "XML is valid!");
      outputArea->setPlainText(xmlText);
    } else {
      QString errorMessage = QString("XML has %1 errors. Click OK to see the "
                                     "fixed version in the output area.")
                                 .arg(xml::get_error_count());
      QString detailedErrors;
      for (auto &err : xml::get_errors()) {
        detailedErrors += QString("Line %1: %2\n")
                              .arg(err.line)
                              .arg(QString::fromStdString(err.message));
      }
      QMessageBox::warning(&window, "XML Errors Found",
                           errorMessage + "\n\n" + detailedErrors);

      // Set output area to ONLY the fixed XML content so it can be saved
      // directly.
      outputArea->setPlainText(QString::fromStdString(xml::get_fixed_XML()));
    }
  });

  QObject::connect(formatButton, &QPushButton::clicked, [&]() {
    QString xmlText = getInputText();
    if (xmlText.isEmpty()) {
      outputArea->setText("No XML content.");
      return;
    }
    if (!xml::is_valid(xmlText.toStdString())) {
      outputArea->setText("XML is invalid.");
      return;
    }
    lastOpType = "xml";
    auto tree = xml::parse(xmlText.toStdString());
    if (tree) {
      QString formatted = QString::fromStdString(xml::format(tree));
      outputArea->setText(formatted);
      delete tree; // Assuming tree needs to be freed if not handled by library
    } else {
      outputArea->setText("Error parsing XML.");
    }
  });

  QObject::connect(jsonButton, &QPushButton::clicked, [&]() {
    QString xmlText = getInputText();
    if (xmlText.isEmpty()) {
      outputArea->setText("No XML content.");
      return;
    }
    if (!xml::is_valid(xmlText.toStdString())) {
      outputArea->setText("XML is invalid.");
      return;
    }
    lastOpType = "json";
    auto tree = xml::parse(xmlText.toStdString());
    if (tree) {
      QString jsonStr = QString::fromStdString(xml::to_json(tree));
      outputArea->setText(jsonStr);
      delete tree;
    } else {
      outputArea->setText("Error parsing XML.");
    }
  });

  QObject::connect(minifyButton, &QPushButton::clicked, [&]() {
    QString xmlText = getInputText();
    if (xmlText.isEmpty()) {
      outputArea->setText("No XML content.");
      return;
    }
    if (!xml::is_valid(xmlText.toStdString())) {
      outputArea->setText("XML is invalid.");
      return;
    }
    lastOpType = "xml";
    outputArea->setText(
        QString::fromStdString(xml::minify(xmlText.toStdString())));
  });

  QObject::connect(compressButton, &QPushButton::clicked, [&]() {
    QString xmlText = getInputText();
    if (xmlText.isEmpty()) {
      outputArea->setText("No XML content.");
      return;
    }
    if (!xml::is_valid(xmlText.toStdString())) {
      outputArea->setText("XML is invalid.");
      return;
    }
    lastOpType = "comp";
    outputArea->setText(QString::fromStdString(
        xml::compress(xml::minify(xmlText.toStdString()))));
  });

  QObject::connect(decompressButton, &QPushButton::clicked, [&]() {
    QString xmlText = getInputText();
    if (xmlText.isEmpty()) {
      outputArea->setText("No XML content.");
      return;
    }
    lastOpType = "xml";
    try {
      std::string decompressed = xml::decompress(xmlText.toStdString());
      outputArea->setText(QString::fromStdString(decompressed));
    } catch (const std::exception &e) {
      QMessageBox::critical(&window, "Decompression Error",
                            QString("Failed to decompress: %1").arg(e.what()));
      outputArea->setText(QString("Error: %1").arg(e.what()));
    }
  });

  QObject::connect(saveButton, &QPushButton::clicked, [&]() {
    QString filter;
    QString defaultExt;
    if (lastOpType == "comp") {
      filter = "Compressed Files (*.comp);;All Files (*)";
      defaultExt = ".comp";
    } else if (lastOpType == "json") {
      filter = "JSON Files (*.json);;All Files (*)";
      defaultExt = ".json";
    } else {
      filter = "XML Files (*.xml);;All Files (*)";
      defaultExt = ".xml";
    }

    QString saveFile =
        QFileDialog::getSaveFileName(&window, "Save Output", "", filter);
    if (!saveFile.isEmpty()) {
      if (!saveFile.contains('.'))
        saveFile += defaultExt;
      QFile file(saveFile);
      if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << outputArea->toPlainText();
        file.close();
        QMessageBox::information(&window, "Saved",
                                 "Output saved successfully!");
      }
    }
  });

  // --- Graph / Network buttons using ready-made sna functions ---

  QObject::connect(mostActiveButton, &QPushButton::clicked, [&]() {
    auto graphPtr = buildGraph();
    if (!graphPtr) {
      outputArea->setText("Invalid XML input.");
      return;
    }

    auto user = sna::most_active(*graphPtr);
    if (!user) {
      outputArea->setText("No users found.");
      return;
    }

    outputArea->setText(QString("Most Active User:\nName: %1\nID: %2")
                            .arg(QString::fromStdString(user->get_name()))
                            .arg(QString::fromStdString(user->get_id())));
  });

  QObject::connect(mostInfluencerButton, &QPushButton::clicked, [&]() {
    auto graphPtr = buildGraph();
    if (!graphPtr) {
      outputArea->setText("Invalid XML input.");
      return;
    }

    auto user = sna::most_influencer(*graphPtr);
    if (!user) {
      outputArea->setText("No users found.");
      return;
    }

    outputArea->setText(QString("Most Influencer User:\nName: %1\nID: %2")
                            .arg(QString::fromStdString(user->get_name()))
                            .arg(QString::fromStdString(user->get_id())));
  });

  QObject::connect(mutualButton, &QPushButton::clicked, [&]() {
    bool ok;
    QString idsText = QInputDialog::getText(
        &window, "Mutual Followers",
        "Enter IDs (comma-separated):", QLineEdit::Normal, "", &ok);
    if (!ok || idsText.isEmpty())
      return;

    std::vector<std::string> ids;
    for (const auto &s : idsText.split(",", Qt::SkipEmptyParts))
      ids.push_back(s.trimmed().toStdString());

    auto graphPtr = buildGraph();
    if (!graphPtr)
      return;

    auto mutualUsers = sna::get_mutual(*graphPtr, ids);

    QString out = "Mutual Followers:\n";
    for (const auto *u : mutualUsers)
      out += QString("%1 (%2)\n")
                 .arg(QString::fromStdString(u->get_name()))
                 .arg(QString::fromStdString(u->get_id()));

    outputArea->setText(out);
  });

  QObject::connect(suggestButton, &QPushButton::clicked, [&]() {
    bool ok;
    QString id = QInputDialog::getText(&window, "Suggest Users",
                                       "User ID:", QLineEdit::Normal, "", &ok);
    if (!ok || id.isEmpty())
      return;

    auto graphPtr = buildGraph();
    if (!graphPtr)
      return;

    auto suggestions = sna::get_suggestions(*graphPtr, id.toStdString());

    QString out = "Suggested Users:\n";
    for (const auto *u : suggestions)
      out += QString("%1 (%2)\n")
                 .arg(QString::fromStdString(u->get_name()))
                 .arg(QString::fromStdString(u->get_id()));

    outputArea->setText(out);
  });

  QObject::connect(searchWordButton, &QPushButton::clicked, [&]() {
    bool ok;
    QString word = QInputDialog::getText(&window, "Search Word",
                                         "Word:", QLineEdit::Normal, "", &ok);
    if (!ok || word.isEmpty())
      return;

    auto graphPtr = buildGraph();
    if (!graphPtr)
      return;

    auto posts = sna::search_by_word(*graphPtr, word.toStdString());

    QString out = "Posts containing \"" + word + "\":\n\n";
    for (const auto *p : posts)
      out += QString("[%1]: %2\n\n")
                 .arg(QString::fromStdString(p->get_author_id()))
                 .arg(QString::fromStdString(p->get_text()));

    outputArea->setText(out);
  });

  QObject::connect(searchTopicButton, &QPushButton::clicked, [&]() {
    bool ok;
    QString topic = QInputDialog::getText(&window, "Search Topic",
                                          "Topic:", QLineEdit::Normal, "", &ok);
    if (!ok || topic.isEmpty())
      return;

    auto graphPtr = buildGraph();
    if (!graphPtr)
      return;

    auto posts = sna::search_by_topic(*graphPtr, topic.toStdString());

    QString out = "Posts under topic \"" + topic + "\":\n\n";
    for (const auto *p : posts)
      out += QString("[%1]: %2\n\n")
                 .arg(QString::fromStdString(p->get_author_id()))
                 .arg(QString::fromStdString(p->get_text()));

    outputArea->setText(out);
  });

  QObject::connect(drawGraphButton, &QPushButton::clicked, [&]() {
    auto graphPtr = buildGraph();
    if (!graphPtr) {
      outputArea->setText("Invalid XML input.");
      return;
    }

    QString saveFile = QFileDialog::getSaveFileName(
        &window, "Save Graph As", "", "Images (*.png *.jpg *.svg)");
    if (saveFile.isEmpty())
      return;

    try {
      draw::run_graphviz(graphPtr->to_dot(), saveFile.toStdString());

      outputArea->setText("Graph saved to: " + saveFile);
    } catch (const std::exception &e) {
      outputArea->setText("Error: " + QString::fromStdString(e.what()));
    }
  });

  window.show();
  return app.exec();
}
} // namespace xml_editor::gui
