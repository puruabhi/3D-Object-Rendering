#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globalvariables.h"
#include "object_3d.h"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <time.h>

void setupDefaults(Ui::MainWindow *ui);

void setViewForObject(Object_3D &obj, Ui::MainWindow *ui);

void setLightForObject(Object_3D &obj, Ui::MainWindow *ui);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupDefaults(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void setupDefaults(Ui::MainWindow *ui)
{
    ui->comboBox_obj_type->addItems({QString::fromStdString(FACE_TYPE),
                                     QString::fromStdString(EDGE_TYPE)});
    ui->comboBox_projection->addItems({QString::fromStdString(PARALLEL),
                                       QString::fromStdString(PERSPECTIVE)});
    ui->comboBox_show->addItems({QString::fromStdString(FACE),
                                QString::fromStdString(EDGE),
                                QString::fromStdString(VERTEX)});
    ui->spinBox_observer->setVisible(false);
    ui->label_observer->setVisible(false);
    ui->pushButton_generate->setDisabled(true);
}

void MainWindow::on_pushButton_browse_filename_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                NULL,
                "OBJ files (*.obj)");
    if(filename!="")
    {
        ui->lineEdit_filename->setText(filename);
    }
}

void MainWindow::on_pushButton_browse_save_dir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
                this,
                tr("Open Directory"),
                NULL,
                QFileDialog::ShowDirsOnly);
    if(dir!="")
    {
        ui->lineEdit_save->setText(dir);
    }
}

void MainWindow::on_comboBox_projection_currentIndexChanged(const QString &projection)
{
    if(projection == QString::fromStdString(PERSPECTIVE))
    {
        ui->spinBox_observer->setVisible(true);
        ui->label_observer->setVisible(true);
    }
    else
    {
        ui->spinBox_observer->setVisible(false);
        ui->label_observer->setVisible(false);
    }
}

void MainWindow::on_pushButton_generate_clicked()
{
    clock_t start = clock();
    ui->pushButton_generate->setText("Generating...");
    ui->pushButton_generate->setDisabled(true);
    //Setting Object from File tab
    QString filename = ui->lineEdit_filename->text();
    std::string filename_str = filename.toStdString();
    Object_3D obj(filename_str);

    QString objectType = ui->comboBox_obj_type->currentText();
    obj.setType(objectType.toStdString());

    QString saveLocation = ui->lineEdit_save->text();

    //Setting Object from View tab
    setViewForObject(obj, ui);

    //Setting Object from Light tab
    setLightForObject(obj, ui);

    QString show = ui->comboBox_show->currentText();

    std::string ObjectFilename = filename_str.substr(filename_str.find_last_of("\\/"));

    std::string saveFilename = saveLocation.toStdString()
            +ObjectFilename.substr(0,ObjectFilename.find_last_of("."))
            +".svg";
    obj.generateSvg(saveFilename,show.toStdString());
    clock_t end = clock();
    std::cout<<(double)(end - start)/CLOCKS_PER_SEC<<std::endl;

    QMessageBox::information(this,"Generated","SVG file is generated");
    ui->pushButton_generate->setText("Generate");
    ui->pushButton_generate->setDisabled(false);
}

void MainWindow::on_lineEdit_filename_textChanged(const QString &filename)
{
    std::string filename_str = filename.toStdString();
    filename_str = filename_str.substr(0, filename_str.find_last_of("\\/"));
    ui->lineEdit_save->setText(QString::fromStdString(filename_str));

    if(filename == NULL)
        ui->pushButton_generate->setDisabled(true);
    else
        ui->pushButton_generate->setDisabled(false);
}

void setViewForObject(Object_3D &obj, Ui::MainWindow *ui)
{
    int scale = ui->spinBox_scale->value();
    QString rotations = ui->lineEdit_rotations->text();
    QString projection = ui->comboBox_projection->currentText();
    //QString show = ui->comboBox_show->currentText();
    int observerPosition = ui->spinBox_observer->value();
    double strokeWidth = ui->doubleSpinBox_stroke_width->value();
    double strokeOpacity = ui->doubleSpinBox_stroke_opacity->value();
    bool backFacingPolygon = ui->checkBox_back_face->isChecked();

    ObjectView objectView(scale,rotations.toStdString(),projection.toStdString(),
                          strokeWidth,strokeOpacity,backFacingPolygon);
    objectView.setObserverPosition(observerPosition);
    //objectView.setShow(show.toStdString());
    obj.setObjectView(objectView);

}

void setLightForObject(Object_3D &obj, Ui::MainWindow *ui)
{
    Eigen::Vector3d lightPosition(ui->doubleSpinBox_light_position_x->value(),
                           ui->doubleSpinBox_light_position_y->value(),
                           ui->doubleSpinBox_light_position_z->value());
    Eigen::Vector3d lightAmbient(ui->doubleSpinBox_ambient_r->value(),
                                 ui->doubleSpinBox_ambient_g->value(),
                                 ui->doubleSpinBox_ambient_b->value());
    Eigen::Vector3d lightDiffuse(ui->doubleSpinBox_diffuse_r->value(),
                                 ui->doubleSpinBox_diffuse_g->value(),
                                 ui->doubleSpinBox_diffuse_b->value());
    Eigen::Vector3d lightSpecular(ui->doubleSpinBox_specular_r->value(),
                                 ui->doubleSpinBox_specular_g->value(),
                                 ui->doubleSpinBox_specular_b->value());
    Light light(lightPosition,lightAmbient,lightDiffuse,lightSpecular);

    obj.setLight(light);
}

void MainWindow::on_comboBox_show_currentIndexChanged(const QString &show)
{
    if(show == QString::fromStdString(FACE))
    {
        ui->checkBox_back_face->setVisible(true);
    }
    else
    {
        ui->checkBox_back_face->setVisible(false);
    }
}
