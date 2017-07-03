#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_browse_filename_clicked();

    void on_pushButton_browse_save_dir_clicked();

    void on_comboBox_projection_currentIndexChanged(const QString &projection);

    void on_pushButton_generate_clicked();

    void on_lineEdit_filename_textChanged(const QString &arg1);

    void on_comboBox_show_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
