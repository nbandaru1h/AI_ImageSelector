#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QVector>
#include <QRect>
#include <QImage>
#include <QGroupBox>
#include <QSlider>
#include <QStringList>
#include <QMap>
#include <QColor>

struct BoundingBox {
    QRect rect;
    QString label;
    float confidence;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    // Refresh current image (and YOLO boxes if toggled)
    void updateImage();

private slots:
    void showPreviousImage();
    void showNextImage();
    void addImageToList();
    void removeImageFromList();
    void saveImageList();
    void clearImageList();
    void deleteSelectedImage();
    void moveSelectedImage();
    void openImageDirectory();
    void copySelectedImages();
    void toggleYoloBoundingBoxes();
    void on_loadNamesFileButton_clicked();

private:
    // === YOLO / annotations ===
    QMap<int, QColor> classColors;
    QImage currentImage;

    struct Annotation {
        QRect boundingBox;
        QString className;
        float confidence;
    };
    QVector<Annotation> currentAnnotations;
    QStringList classNames;

    QString getClassName(int classId);
    void loadClassNames(const QString& namesFilePath);

    // === File / logging / list management ===
    void loadImagesFromDirectory();
    void logActivity(const QString &message);

    // === Bounding boxes display helpers ===
    void displayBoundingBoxes();
    void hideBoundingBoxes();
    void loadYOLOAnnotations(const QString &imagePath);
    void displayBoundingBoxes(const QImage &image);

    QVector<BoundingBox> getAnnotationsForCurrentImage();
    QVector<BoundingBox> boundingBoxes;

    // === UI widgets ===
    QListWidget *imageListWidget;
    QLabel *imageLabel;
    QLabel *titleLabel;
    QLabel *infoLabel;
    QLabel *lastSavedLabel;
    QPushButton *leftButton;
    QPushButton *rightButton;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *saveButton;
    QPushButton *clearButton;
    QPushButton *deleteButton;
    QPushButton *moveButton;
    QPushButton *copyButton;
    QPushButton *toggleYoloButton;
    QPushButton *loadNamesButton;
    QGroupBox *listGroupBox;
    QGroupBox *actionGroupBox;
    QGroupBox *yoloGroupBox;

    // Slider under image
    QSlider *imageSlider = nullptr;

    QStringList imageList;
    QStringList selectedImagePaths;
    QString savedFilePath;

    QDir directory;
    int currentImageIndex;

    QFile logFile;
    QTextStream logStream;
    QAction *loadNamesAction;

    // Layouts
    QVBoxLayout *actionButtonLayout;

    // Styling
    void styleButton(QPushButton *button);
    void setMainWindowStyle();

    // Bounding box toggle state
    bool showYoloBoundingBoxes = false;
};

#endif // MAINWINDOW_H
