#include "umlEditor.h"
#include "ui_umlEditor.h"

#include <cstdio>
#include <type_traits>

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextStream>

#include "events.h"
#include <tenacitas.lib/async.h>
#include <tenacitas.lib/number.h>

using namespace tenacitas;
using namespace tenacitas::plantuml_editor;

int UmlEditor::m_counter = 0;

inline std::string tmp_file() {
  std::string _file_name{"tmp" + number::id().str() + ".plantuml"};
  mkstemp(std::remove_const_t<char *>(_file_name.c_str()));
  return _file_name;
}

UmlEditor::UmlEditor(const QString &p_jarPath, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::UmlEditor), m_jarPath(p_jarPath),
      m_filePath(tmp_file().c_str()), m_displayer(nullptr)
      //  , m_textChanged(false)
      ,
      m_id(m_counter++) {
  ui->setupUi(this);
  this->setWindowTitle("Editor - " + m_filePath);
  async::dispatch(evt::title_changed{m_id, m_filePath.toStdString()});
  ui->btnRename->setVisible(false);
  ui->btnSearch->setVisible(false);
  ui->btnDraw->setVisible(false);
  ui->btnSave->setVisible(false);
  ui->btnSearchAgain->setVisible(false);
  ui->line1->setVisible(false);
  ui->line2->setVisible(false);
}

UmlEditor::~UmlEditor() { delete ui; }

int UmlEditor::getId() const { return m_id; }

void UmlEditor::on_btnOpen_clicked() {
  m_filePath =
      QFileDialog::getOpenFileName(this, tr("Choose file to open"), "./",
                                   tr("PlanutUML Files (*.plantuml)"));
  async::dispatch(evt::title_changed{m_id, m_filePath.toStdString()});

  QFile file(m_filePath);
  file.open(QFile::ReadOnly | QFile::Text);
  ui->txtPlant->setPlainText(file.readAll());

  //  QFile file(m_filePath);
  //  file.open(QIODevice::Text | QIODevice::ReadOnly);
  //  QString content;
  //  while (!file.atEnd()) {
  //    content.append(file.readLine());
  //  }
  //  ui->txtPlant->setText(content);
  file.close();
  this->setWindowTitle("Editor - " + m_filePath);
  ui->btnRename->setVisible(true);
  ui->btnSearch->setVisible(true);
  ui->btnDraw->setVisible(true);
  ui->btnSave->setVisible(true);
  ui->line1->setVisible(false);
  ui->line2->setVisible(false);

  //  emit title_changed(m_id, this->windowTitle());
  async::dispatch(evt::title_changed{m_id, m_filePath.toStdString()});
  //  m_textChanged = true;
}

void UmlEditor::on_btnSave_clicked() {
  if (m_filePath.isEmpty()) {
    m_filePath =
        QFileDialog::getSaveFileName(this, tr("Enter new file name"), "./",
                                     tr("PlanutUML Files (*.plantuml)"));
  }

  if (!m_filePath.isEmpty()) {
    async::dispatch(evt::title_changed{m_id, m_filePath.toStdString()});
    ui->btnRename->setVisible(true);
    QFile file(m_filePath);
    if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
      QTextStream stream(&file);
      stream << ui->txtPlant->toPlainText();
      file.flush();
      file.close();
    }
    this->setWindowTitle("Editor - " + m_filePath);
    //    emit title_changed(m_id, this->windowTitle());
    //    m_textChanged = true;
  }
}

void UmlEditor::on_btnDraw_clicked() {
  if (ui->txtPlant->document()->isEmpty()) {
    return;
  }
  if (m_displayer == nullptr) {
    m_displayer = new UmlDisplayer(m_jarPath, this);
  }

  if (m_displayer != nullptr) {

    m_displayer->setWindowState(
        (m_displayer->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    m_displayer->raise();
    m_displayer->activateWindow();
    m_displayer->show();

    //    if (m_textChanged) {
    if (m_filePath.isEmpty()) {
      //      m_filePath = std::tmpnam(nullptr);
      //      char tmp_file[] = "/tmp/plantuml_XXXXXX";

      m_filePath = tmp_file().c_str();

      async::dispatch(evt::title_changed{m_id, m_filePath.toStdString()});
      ui->btnRename->setVisible(true);
    }
    on_btnSave_clicked();
    m_displayer->update(m_filePath);
    //      m_textChanged = false;
  }
  //  }
}

void UmlEditor::on_txtPlant_textChanged() {
  ui->btnSave->setVisible(true);
  ui->line1->setVisible(true);

  if (ui->txtPlant->document()->isEmpty()) {
    ui->btnDraw->setVisible(false);
    ui->btnSearch->setVisible(false);
    ui->line2->setVisible(false);
  } else {
    ui->btnDraw->setVisible(true);
    ui->btnSearch->setVisible(true);
    ui->line2->setVisible(true);
  }
}

void UmlEditor::closeEvent(QCloseEvent *event) {

  if (m_displayer != nullptr) {
    m_displayer->close();
  }
  on_btnSave_clicked();
  //  emit editor_closed(m_id);
  async::dispatch(evt::editor_closed{m_id});
  event->accept();
}

void UmlEditor::showEvent(QShowEvent *) { ui->txtPlant->setFocus(); }

void UmlEditor::on_btnMain_clicked() {
  QWidget *parent = this->parentWidget();
  if (parent != nullptr) {
    ui->btnMain->setFocus();
    parent->setWindowState((parent->windowState() & ~Qt::WindowMinimized) |
                           Qt::WindowActive);

    //    parent->setFocus();
    parent->raise();
    parent->activateWindow();
    //    parent->show();
  }
}

void UmlEditor::on_btnRename_clicked() {
  if (!m_filePath.isEmpty()) {
    m_filePath =
        QFileDialog::getSaveFileName(this, tr("Enter new file name"), "./",
                                     tr("PlanutUML Files (*.plantuml)"));
    async::dispatch(evt::title_changed{m_id, m_filePath.toStdString()});
  }

  if (!m_filePath.isEmpty()) {
    QFile file(m_filePath);
    if (file.open(QIODevice::Text | QIODevice::ReadWrite)) {
      QTextStream stream(&file);
      stream << ui->txtPlant->toPlainText();
      file.flush();
      file.close();
    }
    this->setWindowTitle("Editor - " + m_filePath);
    //    emit title_changed(m_id, this->windowTitle());
    async::dispatch(evt::title_changed{m_id, m_filePath.toStdString()});

    //    m_textChanged = true;
  }
}

void UmlEditor::on_btnSearch_clicked() {
  if (m_findDialog == nullptr) {
    m_findDialog = new FindDialog(this);
  }

  ui->btnSearchAgain->setVisible(false);
  m_findDialog->exec();

  m_textToFind.clear();
  m_textReplaceFor.clear();

  m_textToFind = m_findDialog->getTextToFind();

  if (!m_textToFind.isEmpty()) {
    ui->btnSearchAgain->setVisible(true);

    m_findFlags = QTextDocument::FindFlags();
    if (m_findDialog->backward()) {
      m_findFlags |= QTextDocument::FindBackward;
    }
    if (m_findDialog->caseSensitive()) {
      m_findFlags |= QTextDocument::FindCaseSensitively;
    }
    if (m_findDialog->wholeWord()) {
      m_findFlags |= QTextDocument::FindWholeWords;
    }

    m_textReplaceFor = m_findDialog->getTextReplaceFor();
    on_btnSearchAgain_clicked();
  }
}

void UmlEditor::on_btnSearchAgain_clicked() {
  QTextCursor textFound;

  textFound = ui->txtPlant->document()->find(
      m_textToFind, ui->txtPlant->textCursor(), m_findFlags);

  if (!textFound.isNull()) {
    ui->txtPlant->setTextCursor(textFound);
    //    textFound.movePosition(QTextCursor::WordRight,
    //    QTextCursor::KeepAnchor);
    if (!m_textReplaceFor.isEmpty()) {
      textFound.removeSelectedText();
      textFound.insertText(m_textReplaceFor);
    }
  } else {
    QMessageBox::information(this, tr(""), tr("Expression not found."));
  }
}
