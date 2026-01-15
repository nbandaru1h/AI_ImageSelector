#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QLabel>
#include <QListWidget>
#include <QMap>
#include <QPushButton>
#include <QSlider>
#include <QTabWidget>
#include <QTextStream>
#include <QVector>

class QKeyEvent;
class QResizeEvent;

struct BoundingBox {
    QRect rect;
    QString label;
    float confidence = 0.0f;
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

private slots:
    // Navigation
    void showPreviousImage();
    void showPreviousDirectory();
    void showNextImage();
    void showNextDirectory();

    // Tagging / lists
    void openTaggingConfig();
    void addImageToList();          // add current image to current category tab
    void removeImageFromList();     // remove selected from current category tab
    void saveImageList();           // save all category lists (first time chooses base dir)
    void clearImageList();          // clear current category tab

    // Actions (operate on selected items across categories)
    void deleteSelectedImage();     // moves to user-selected per-category folders
    void moveSelectedImage();
    void copySelectedImages();

    // YOLO
    void toggleYoloBoundingBoxes();
    void on_loadNamesFileButton_clicked();

    // File menu
    void openImageDirectory();
    void openCurrentImageFolderInExplorer();

private:
    // UI helpers
    void styleButton(QPushButton *button);
    void setMainWindowStyle();
    void updateToggleYoloButtonStyle();

    // Image handling
    void loadImagesFromDirectory();
    bool loadImagesFromDirectoryPath(const QString &dirPath, bool logIt = true);
    void updateImage();
    void logActivity(const QString &message);

    // YOLO helpers
    QString getClassName(int classId) const;
    void loadClassNames(const QString &namesFilePath);
    void loadYOLOAnnotations(const QString &imagePath);
    QImage renderBoundingBoxesOn(const QImage &img) const;

    // Tagging helpers
    void ensureDefaultCategory();
    void rebuildCategoryTabs();
    QString currentCategory() const;
    void updateTaggingHintLabel();
    void updateDirectoryNameLabel();
    void updateFolderDateTimeLabel();
    void addCurrentImageToCategory(const QString &category, bool advanceAfter);

    // Saving lists
    bool ensureSavedListsDir();
    void saveAllCategoryLists(bool silent);
    QString categoryListFilePath(const QString &category) const;

    // Bulk actions
    enum class BulkAction { Copy, Move, Delete };
    bool runBulkAction(BulkAction action);
    bool chooseDestinationsForCategories(const QStringList &categories,
                                        QMap<QString, QString> &outCategoryToDir,
                                        const QString &title,
                                        const QString &actionVerb) const;

    bool applyActionToCategory(const QString &category,
                               const QStringList &paths,
                               BulkAction action,
                               const QString &destDir);

    void pruneMissingFiles();
    QStringList siblingDirectories(QString *outCurrentName = nullptr) const;

private:
    // Data
    QDir directory;
    QStringList imageList;
    int currentImageIndex = 0;

    // YOLO
    bool showYoloBoundingBoxes = false;
    QImage currentImage;
    struct Annotation {
        QRect boundingBox;
        int classId = -1;
        QString className;
        float confidence = 0.0f;
    };
    QVector<Annotation> currentAnnotations;
    QStringList classNames;
    QMap<int, QColor> classColors;

    // Tagging
    QMap<int, QString> keyToCategory;            // Qt::Key_* -> category
    QMap<QString, QStringList> categoryPaths;    // category -> full file paths
    QMap<QString, QListWidget*> categoryWidgets; // category -> list widget
    QString savedListsDir;                       // base dir for list txts

    // UI elements
    QLabel *titleLabel = nullptr;
    QLabel *imageLabel = nullptr;
    QLabel *infoLabel = nullptr;
    QLabel *taggingHintLabel = nullptr;
    QLabel *lastSavedLabel = nullptr;
    QLabel *indexLabel = nullptr;
    QLabel *dirNameLabel = nullptr;
    QLabel *dateTimeLabel = nullptr;

    QPushButton *leftButton = nullptr;
    QPushButton *rightButton = nullptr;

    QSlider *imageSlider = nullptr;

    QPushButton *configureTaggingButton = nullptr;

    QTabWidget *categoryTabs = nullptr;

    // Group buttons
    QPushButton *addButton = nullptr;
    QPushButton *removeButton = nullptr;
    QPushButton *saveButton = nullptr;
    QPushButton *clearButton = nullptr;

    QPushButton *deleteButton = nullptr;
    QPushButton *moveButton = nullptr;
    QPushButton *copyButton = nullptr;

    QPushButton *toggleYoloButton = nullptr;
    QPushButton *loadNamesButton = nullptr;

    // Logging
    QFile logFile;
    QTextStream logStream;
};

#endif // MAINWINDOW_H
