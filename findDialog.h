#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
  Q_OBJECT

public:
  explicit FindDialog(QWidget* parent = nullptr);
  ~FindDialog();

  const QString& getTextToFind() const;
  const QString& getTextReplaceFor() const;

  bool forward() const;
  bool backward() const;
  bool wholeWord() const;
  bool wordPart() const;
  bool caseSensitive() const;
  bool nonCaseSensitive() const;

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

private:
  Ui::FindDialog* ui;

  QString m_textToFind;
  QString m_textReplaceFor;
};

#endif // FINDDIALOG_H
