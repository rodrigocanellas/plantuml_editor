#ifndef UMLEDITOR_H
#define UMLEDITOR_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QString>
#include <QTextDocument>

#include "findDialog.h"
#include "umlDisplayer.h"

namespace Ui {
class UmlEditor;
}

class UmlEditor : public QMainWindow
{
  Q_OBJECT

public:
  explicit UmlEditor(const QString& p_jarPath, QWidget* parent = nullptr);
  ~UmlEditor();

  int getId() const;

  void closeEvent(QCloseEvent* event) override;

  void showEvent(QShowEvent* ev) override;

private slots:
  void on_btnOpen_clicked();

  void on_btnSave_clicked();

  void on_btnDraw_clicked();

  void on_txtPlant_textChanged();

  void on_btnMain_clicked();

  void on_btnRename_clicked();

  void on_btnSearch_clicked();

  void on_btnSearchAgain_clicked();

signals:
  void title_changed(int id, const QString& p_title);

  void editor_closed(int id);

private:
  Ui::UmlEditor* ui;

  QString m_jarPath;

  QString m_filePath;

  UmlDisplayer* m_displayer;

  //  bool m_textChanged;

  int m_id;

  FindDialog* m_findDialog = nullptr;

  QTextDocument::FindFlags m_findFlags;

  QString m_textToFind;
  QString m_textReplaceFor;

  static int m_counter;
};

#endif // UMLEDITOR_H
