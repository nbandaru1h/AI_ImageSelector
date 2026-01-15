#include "mainwindow.h"

#include <QAction>
#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QResizeEvent>
#include <QRegularExpression>
#include <QStatusBar>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QSize>
#include <QStyle>
#include <QUrl>
#include <QDesktopServices>
#include <QToolBar>

#include <algorithm>

// ------------------------------------------------------------
// Styling
// ------------------------------------------------------------
void MainWindow::styleButton(QPushButton *button)
{
    button->setStyleSheet(
        "QPushButton {"
        "   background-color: #003366;"
        "   border: 1px solid #4169e1;"
        "   border-radius: 5px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   padding: 8px 16px;"
        "   color: white;"
        "}"
        "QPushButton:hover {"
        "   background-color: #4682b4;"
        "   border: 1px solid #315c8a;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #315c8a;"
        "   border: 1px solid #25485e;"
        "}"
    );
}

void MainWindow::setMainWindowStyle()
{
    this->setStyleSheet("background-color: #FAFAFA; color: black;");
}

void MainWindow::updateToggleYoloButtonStyle()
{
    // Re-integrated: Toggle button status color
    if (!toggleYoloButton) return;

    if (showYoloBoundingBoxes) {
        toggleYoloButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #1f7a1f;"
            "   border: 1px solid #155315;"
            "   border-radius: 5px;"
            "   font-size: 14px;"
            "   font-weight: bold;"
            "   padding: 8px 16px;"
            "   color: white;"
            "}"
            "QPushButton:hover { background-color: #2b9a2b; }"
            "QPushButton:pressed { background-color: #155315; }"
        );
    } else {
        toggleYoloButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #6b6b6b;"
            "   border: 1px solid #4f4f4f;"
            "   border-radius: 5px;"
            "   font-size: 14px;"
            "   font-weight: bold;"
            "   padding: 8px 16px;"
            "   color: white;"
            "}"
            "QPushButton:hover { background-color: #7a7a7a; }"
            "QPushButton:pressed { background-color: #4f4f4f; }"
        );
    }
}

// ------------------------------------------------------------
// Constructor / Destructor
// ------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Class colors (stable palette)
    classColors[0] = QColor("#FF6347");
    classColors[1] = QColor("#9400D3");
    classColors[2] = QColor("#32CD32");
    classColors[3] = QColor("#FFD700");
    classColors[4] = QColor("#FF4500");
    classColors[5] = QColor("#00CED1");
    classColors[6] = QColor("#A0522D");
    classColors[7] = QColor("#FF1493");
    classColors[8] = QColor("#4682b4");
    classColors[9] = QColor("#2E8B57");
    classColors[10] = QColor("#DC143C");
    classColors[11] = QColor("#8A2BE2");
    classColors[12] = QColor("#7FFF00");
    classColors[13] = QColor("#D2691E");
    classColors[14] = QColor("#FFB6C1");
    classColors[15] = QColor("#800080");
    classColors[16] = QColor("#C71585");
    classColors[17] = QColor("#FF69B4");
    classColors[18] = QColor("#5F9EA0");
    classColors[19] = QColor("#FFDAB9");
    classColors[20] = QColor("#FF8C00");
    classColors[21] = QColor("#20B2AA");
    classColors[22] = QColor("#B22222");
    classColors[23] = QColor("#4682B4");
    classColors[24] = QColor("#008080");

    setMainWindowStyle();

    // Title
    titleLabel = new QLabel("AI Image Suite", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #003366; }");

    // Image view
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Info label (center below image)
    infoLabel = new QLabel(this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("QLabel { color: black; background-color: transparent; }");

    // Folder timestamp label (based on first image modified time)
    dateTimeLabel = new QLabel(this);
    dateTimeLabel->setAlignment(Qt::AlignCenter);
    dateTimeLabel->setStyleSheet("QLabel { color: #003366; background-color: transparent; font-weight: bold; }");
    dateTimeLabel->setText("");

    // Tagging hint label
    taggingHintLabel = new QLabel(this);
    taggingHintLabel->setAlignment(Qt::AlignLeft);
    taggingHintLabel->setStyleSheet("QLabel { color: #003366; background-color: transparent; }");

    // Last saved label
    lastSavedLabel = new QLabel(this);
    lastSavedLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    lastSavedLabel->setStyleSheet("QLabel { color: black; background-color: transparent; }");

    // Navigation buttons
    leftButton = new QPushButton("<", this);
    rightButton = new QPushButton(">", this);
    styleButton(leftButton);
    styleButton(rightButton);
    leftButton->setFixedSize(60, 40);
    rightButton->setFixedSize(60, 40);

    // Slider (re-integrated)
    imageSlider = new QSlider(Qt::Horizontal, this);
    imageSlider->setObjectName("imageSlider");
    imageSlider->setRange(0, 0);
    imageSlider->setSingleStep(1);
    imageSlider->setPageStep(1);
    imageSlider->setTickPosition(QSlider::TicksBelow);

    // Bottom-right index (re-integrated)
    indexLabel = new QLabel("0 / 0", this);
    indexLabel->setObjectName("indexLabel");
    statusBar()->addPermanentWidget(indexLabel, 0);

    // Buttons
    configureTaggingButton = new QPushButton("Configure Image Tagging", this);

    addButton = new QPushButton("Add", this);
    removeButton = new QPushButton("Remove", this);
    saveButton = new QPushButton("Save Lists", this);
    clearButton = new QPushButton("Clear Category", this);

    deleteButton = new QPushButton("Delete", this);
    moveButton = new QPushButton("Move", this);
    copyButton = new QPushButton("Copy", this);

    toggleYoloButton = new QPushButton("Toggle YOLO Bounding Boxes", this);
    loadNamesButton = new QPushButton("Load Class Names", this);

    for (QPushButton *b : {configureTaggingButton, addButton, removeButton, saveButton, clearButton,
                           deleteButton, moveButton, copyButton, toggleYoloButton, loadNamesButton}) {
        styleButton(b);
        b->setFixedHeight(40);
    }

    // Category tabs
    categoryTabs = new QTabWidget(this);
    categoryTabs->setFixedWidth(320);

    // Default category
    ensureDefaultCategory();
    rebuildCategoryTabs();
    updateTaggingHintLabel();

    // --- Layouts ---
    // Navigation row under image
    QHBoxLayout *navRow = new QHBoxLayout();
    navRow->addStretch();
    navRow->addWidget(leftButton);
    navRow->addWidget(rightButton);
    navRow->addStretch();

    QVBoxLayout *imageCol = new QVBoxLayout();
    imageCol->addWidget(imageLabel, 1);
    imageCol->addWidget(imageSlider);
    imageCol->addLayout(navRow);
    imageCol->addWidget(infoLabel);
    imageCol->addWidget(dateTimeLabel);

    // Right panel
    QVBoxLayout *rightPanel = new QVBoxLayout();
    rightPanel->addWidget(configureTaggingButton);
    rightPanel->addWidget(taggingHintLabel);
    rightPanel->addWidget(categoryTabs, 1);

    // List controls (tagging lists)
    QGroupBox *listGroup = new QGroupBox("List", this);
    QVBoxLayout *listLayout = new QVBoxLayout();
    listLayout->addWidget(addButton);
    listLayout->addWidget(removeButton);
    listLayout->addWidget(saveButton);
    listLayout->addWidget(clearButton);
    listGroup->setLayout(listLayout);
    rightPanel->addWidget(listGroup);
    // (removed duplicate List group)
rightPanel->addWidget(lastSavedLabel);

    // Spacers: fixed left gap, expandable right
    QSpacerItem *leftSpacer = new QSpacerItem(200, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
    QSpacerItem *rightSpacer = new QSpacerItem(50, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout *centerRow = new QHBoxLayout();
    centerRow->addItem(leftSpacer);
    centerRow->addLayout(imageCol, 2);
    centerRow->addLayout(rightPanel, 0);
    centerRow->addItem(rightSpacer);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(centerRow);

    QWidget *container = new QWidget(this);
    container->setLayout(mainLayout);
    setCentralWidget(container);

    setMinimumSize(1100, 650);

    // Menu
    QMenuBar *mb = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("File", mb);
    QAction *openDir = new QAction("Open Directory", this);
    connect(openDir, &QAction::triggered, this, &MainWindow::openImageDirectory);
    fileMenu->addAction(openDir);
    mb->addMenu(fileMenu);
    setMenuBar(mb);

    // ------------------------------------------------------------
    // Top toolbar (organized by function)
    // ------------------------------------------------------------
    QToolBar *topBar = new QToolBar("Toolbar", this);
    topBar->setMovable(false);
    topBar->setIconSize(QSize(18, 18));
    addToolBar(Qt::TopToolBarArea, topBar);

    auto makeTbBtn = [this](const QString &text, QStyle::StandardPixmap icon, const QString &tip) {
        QPushButton *b = new QPushButton(text, this);
        b->setIcon(style()->standardIcon(icon));
        b->setToolTip(tip);
        b->setFixedHeight(32);
        // Use same palette as other buttons but slightly tighter
        b->setStyleSheet(
            "QPushButton { background-color: #003366; border: 1px solid #4169e1; border-radius: 5px; "
            "font-size: 13px; font-weight: bold; padding: 6px 12px; color: white; }"
            "QPushButton:hover { background-color: #4682b4; border: 1px solid #315c8a; }"
            "QPushButton:pressed { background-color: #315c8a; border: 1px solid #25485e; }"
        );
        return b;
    };

    // File
    QPushButton *tbOpenDir = makeTbBtn("Load Image Directory", QStyle::SP_DirOpenIcon, "Select a new image directory");
    topBar->addWidget(tbOpenDir);

    QPushButton *tbOpenFolder = makeTbBtn("Open Current Directory", QStyle::SP_DirIcon, "Open the current image folder in your file explorer");
    topBar->addWidget(tbOpenFolder);

    topBar->addSeparator();

    // Navigate
    QPushButton *tbPrev = makeTbBtn("Prev Dir", QStyle::SP_ArrowBack, "Previous directory (sibling folder)");
    topBar->addWidget(tbPrev);

    QPushButton *tbNext = makeTbBtn("Next Dir", QStyle::SP_ArrowForward, "Next directory (sibling folder)");
    topBar->addWidget(tbNext);

    topBar->addSeparator();

    // Actions
    QPushButton *tbCopy = makeTbBtn("Copy", QStyle::SP_DialogSaveButton, "Copy selected images (per-category destinations)");
    topBar->addWidget(tbCopy);

    QPushButton *tbMove = makeTbBtn("Move", QStyle::SP_ArrowRight, "Move selected images (per-category destinations)");
    topBar->addWidget(tbMove);

    QPushButton *tbDelete = makeTbBtn("Delete", QStyle::SP_TrashIcon, "Delete selected images (implemented as move to chosen folders)");
    topBar->addWidget(tbDelete);

    topBar->addSeparator();

    // YOLO
    QPushButton *tbYoloToggle = new QPushButton("YOLO", this);
    tbYoloToggle->setToolTip("Toggle YOLO bounding boxes");
    tbYoloToggle->setFixedHeight(32);
    topBar->addWidget(tbYoloToggle);

    QPushButton *tbLoadNames = makeTbBtn("Load Names", QStyle::SP_FileIcon, "Load .names file for class labels");
    topBar->addWidget(tbLoadNames);

    auto syncTbYoloToggle = [this, tbYoloToggle]() {
        if (showYoloBoundingBoxes) {
            tbYoloToggle->setStyleSheet(
                "QPushButton { background-color: #1f7a1f; border: 1px solid #155315; border-radius: 5px; "
                "font-size: 13px; font-weight: bold; padding: 6px 12px; color: white; }"
                "QPushButton:hover { background-color: #2b9a2b; }"
                "QPushButton:pressed { background-color: #155315; }"
            );
        } else {
            tbYoloToggle->setStyleSheet(
                "QPushButton { background-color: #6b6b6b; border: 1px solid #4f4f4f; border-radius: 5px; "
                "font-size: 13px; font-weight: bold; padding: 6px 12px; color: white; }"
                "QPushButton:hover { background-color: #7a7a7a; }"
                "QPushButton:pressed { background-color: #4f4f4f; }"
            );
        }
    };
    syncTbYoloToggle();

    // Toolbar connections
    connect(tbOpenDir, &QPushButton::clicked, this, &MainWindow::openImageDirectory);
    connect(tbOpenFolder, &QPushButton::clicked, this, &MainWindow::openCurrentImageFolderInExplorer);

    connect(tbPrev, &QPushButton::clicked, this, &MainWindow::showPreviousDirectory);
    connect(tbNext, &QPushButton::clicked, this, &MainWindow::showNextDirectory);

    connect(tbCopy, &QPushButton::clicked, this, &MainWindow::copySelectedImages);
    connect(tbMove, &QPushButton::clicked, this, &MainWindow::moveSelectedImage);
    connect(tbDelete, &QPushButton::clicked, this, &MainWindow::deleteSelectedImage);

    connect(tbYoloToggle, &QPushButton::clicked, this, [this, syncTbYoloToggle]() {
        toggleYoloBoundingBoxes();
        syncTbYoloToggle();
    });
    connect(tbLoadNames, &QPushButton::clicked, this, &MainWindow::on_loadNamesFileButton_clicked);

    mb->setStyleSheet("QMenuBar { background-color: #003366; color: white; }"
                      "QMenuBar::item { color: white; }"
                      "QMenuBar::item:selected { background-color: #4682b4; }"
                      "QMenu::item:selected { background-color: #00509E; }");

    // Connections
    connect(leftButton, &QPushButton::clicked, this, &MainWindow::showPreviousImage);
    connect(rightButton, &QPushButton::clicked, this, &MainWindow::showNextImage);

    connect(imageSlider, &QSlider::valueChanged, this, [this](int v){
        if (imageList.isEmpty()) return;
        v = std::clamp(v, 0, imageList.size()-1);
        currentImageIndex = v;
        updateImage();
    });

    connect(configureTaggingButton, &QPushButton::clicked, this, &MainWindow::openTaggingConfig);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addImageToList);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeImageFromList);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveImageList);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearImageList);

    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteSelectedImage);
    connect(moveButton, &QPushButton::clicked, this, &MainWindow::moveSelectedImage);
    connect(copyButton, &QPushButton::clicked, this, &MainWindow::copySelectedImages);

    connect(toggleYoloButton, &QPushButton::clicked, this, &MainWindow::toggleYoloBoundingBoxes);
    connect(loadNamesButton, &QPushButton::clicked, this, &MainWindow::on_loadNamesFileButton_clicked);

    // Logging
    const QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    const QString logName = QString("log_%1.txt").arg(timestamp);
    logFile.setFileName(logName);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        logStream.setDevice(&logFile);
        logActivity("Application started.");
    }

    // Startup: prompt for folder
    loadImagesFromDirectory();
    updateImage();

    // initial YOLO toggle style
    updateToggleYoloButtonStyle();

    setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
    if (logFile.isOpen()) {
        logActivity("Application closed.");
        logFile.close();
    }
}

// ------------------------------------------------------------
// Events
// ------------------------------------------------------------
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    const int k = event->key();

    if (k == Qt::Key_Right) { showNextImage(); return; }
    if (k == Qt::Key_Left)  { showPreviousImage(); return; }

    // Re-integrated: 'a' adds to current category + advances
    if (k == Qt::Key_A) {
        addCurrentImageToCategory(currentCategory(), true);
        return;
    }

    // Tagging keys: add to mapped category + advance
    if (keyToCategory.contains(k)) {
        addCurrentImageToCategory(keyToCategory.value(k), true);
        return;
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateImage();
}

// ------------------------------------------------------------
// Directory / navigation
// ------------------------------------------------------------
void MainWindow::loadImagesFromDirectory()
{
    const QString dirPath = QFileDialog::getExistingDirectory(this, "Select Directory", "");
    if (dirPath.isEmpty()) return;

    loadImagesFromDirectoryPath(dirPath, true);
}

bool MainWindow::loadImagesFromDirectoryPath(const QString &dirPath, bool logIt)
{
    if (dirPath.isEmpty()) return false;

    directory.setPath(dirPath);

    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.JPG" << "*.JPEG" << "*.PNG";

    imageList = directory.entryList(filters, QDir::Files, QDir::Name);
    currentImageIndex = 0;

    imageSlider->setRange(0, std::max(0, imageList.size() - 1));
    imageSlider->blockSignals(true);
    imageSlider->setValue(currentImageIndex);
    imageSlider->blockSignals(false);

    indexLabel->setText(imageList.isEmpty() ? "0 / 0" : QString("1 / %1").arg(imageList.size()));

    updateDirectoryNameLabel();

    updateFolderDateTimeLabel();

    // Keep category tabs as-is, but ensure view refresh
    updateImage();

    if (logIt) logActivity("Loaded images from directory: " + dirPath);
    return true;
}

QStringList MainWindow::siblingDirectories(QString *outCurrentName) const
{
    // Return sibling directory NAMES under the parent of the current image directory,
    // sorted by name (QDir::Name). Also returns the current directory name.
    const QString curAbs = directory.absolutePath();
    if (curAbs.isEmpty()) return {};

    // Canonical path helps avoid mismatch due to symlinks/trailing slashes.
    const QString curCanonical = QFileInfo(curAbs).canonicalFilePath().isEmpty()
                                     ? QDir(curAbs).absolutePath()
                                     : QFileInfo(curAbs).canonicalFilePath();

    QFileInfo curInfo(curCanonical);
    const QString curName = curInfo.fileName();
    if (outCurrentName) *outCurrentName = curName;

    QDir parentDir = curInfo.dir(); // parent of current directory
    if (!parentDir.exists()) return {};

    // List sibling directories (names only)
    QStringList dirs = parentDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    return dirs;
}

void MainWindow::showPreviousDirectory()
{
    QString curName;
    const QStringList dirs = siblingDirectories(&curName);
    if (dirs.isEmpty() || curName.isEmpty()) return;

    int idx = dirs.indexOf(curName);
    if (idx < 0) {
        // If the name doesn't match, try matching by canonical path basename
        const QString base = QFileInfo(directory.absolutePath()).fileName();
        idx = dirs.indexOf(base);
    }
    if (idx <= 0) return;

    const QString curAbs = directory.absolutePath();
    const QString curCanonical = QFileInfo(curAbs).canonicalFilePath().isEmpty()
                                     ? QDir(curAbs).absolutePath()
                                     : QFileInfo(curAbs).canonicalFilePath();
    QDir parentDir = QFileInfo(curCanonical).dir(); // parent of current directory

    const QString target = parentDir.filePath(dirs[idx - 1]);
    if (!loadImagesFromDirectoryPath(target, true)) return;

    pruneMissingFiles();
    rebuildCategoryTabs();
    updateImage();
}

void MainWindow::showNextDirectory()
{
    QString curName;
    const QStringList dirs = siblingDirectories(&curName);
    if (dirs.isEmpty() || curName.isEmpty()) return;

    int idx = dirs.indexOf(curName);
    if (idx < 0) {
        const QString base = QFileInfo(directory.absolutePath()).fileName();
        idx = dirs.indexOf(base);
    }
    if (idx < 0 || idx >= dirs.size() - 1) return;

    const QString curAbs = directory.absolutePath();
    const QString curCanonical = QFileInfo(curAbs).canonicalFilePath().isEmpty()
                                     ? QDir(curAbs).absolutePath()
                                     : QFileInfo(curAbs).canonicalFilePath();
    QDir parentDir = QFileInfo(curCanonical).dir(); // parent of current directory

    const QString target = parentDir.filePath(dirs[idx + 1]);
    if (!loadImagesFromDirectoryPath(target, true)) return;

    pruneMissingFiles();
    rebuildCategoryTabs();
    updateImage();
}

void MainWindow::openImageDirectory()
{
    loadImagesFromDirectory();
    pruneMissingFiles();
    rebuildCategoryTabs();
    updateImage();
}void MainWindow::showPreviousImage()
{
    if (imageList.isEmpty()) return;

    if (currentImageIndex > 0) {
        --currentImageIndex;
        imageSlider->blockSignals(true);
        imageSlider->setValue(currentImageIndex);
        imageSlider->blockSignals(false);
        updateImage();
        logActivity("Navigated to previous image.");
    }
}

void MainWindow::showNextImage()
{
    if (imageList.isEmpty()) return;

    if (currentImageIndex < imageList.size() - 1) {
        ++currentImageIndex;
        imageSlider->blockSignals(true);
        imageSlider->setValue(currentImageIndex);
        imageSlider->blockSignals(false);
        updateImage();
        logActivity("Navigated to next image.");
    }
}

// ------------------------------------------------------------
// Image refresh (draw + info + counters)
// ------------------------------------------------------------
void MainWindow::updateImage()
{
    if (imageList.isEmpty()) {
        imageLabel->setText("No images loaded.");
        infoLabel->clear();
        indexLabel->setText("0 / 0");
        return;
    }

    currentImageIndex = std::clamp(currentImageIndex, 0, imageList.size()-1);

    const QString imagePath = directory.filePath(imageList.at(currentImageIndex));

    currentImage = QImage(imagePath);
    if (currentImage.isNull()) {
        imageLabel->setText("Failed to load image.");
        return;
    }

    loadYOLOAnnotations(imagePath);

    QImage display = currentImage;
    if (showYoloBoundingBoxes) {
        display = renderBoundingBoxesOn(display);
    }

    QPixmap pix = QPixmap::fromImage(display);
    pix = pix.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(pix);

    const QFileInfo fi(imagePath);
    const QString folderAbs = directory.absolutePath();
    const QString folderBase = QFileInfo(folderAbs).fileName().isEmpty() ? folderAbs : QFileInfo(folderAbs).fileName();
    infoLabel->setText(QString("%1\nFolder: %2\n%3 x %4")
                           .arg(fi.fileName())
                           .arg(folderBase)
                           .arg(currentImage.width())
                           .arg(currentImage.height()));

    indexLabel->setText(QString("%1 / %2").arg(currentImageIndex + 1).arg(imageList.size()));
}

// ------------------------------------------------------------
// YOLO helpers
// ------------------------------------------------------------
QString MainWindow::getClassName(int classId) const
{
    if (classId >= 0 && classId < classNames.size()) return classNames.at(classId);
    return QString("Class %1").arg(classId);
}

void MainWindow::loadClassNames(const QString &namesFilePath)
{
    classNames.clear();
    QFile f(namesFilePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Load Names", "Failed to open .names file.");
        return;
    }
    QTextStream in(&f);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (!line.isEmpty()) classNames << line;
    }
    f.close();
    logActivity("Loaded class names: " + namesFilePath);
}

void MainWindow::on_loadNamesFileButton_clicked()
{
    const QString file = QFileDialog::getOpenFileName(this, "Select .names file", "", "Names (*.names);;All (*)");
    if (file.isEmpty()) return;
    loadClassNames(file);
    updateImage();
}

void MainWindow::loadYOLOAnnotations(const QString &imagePath)
{
    currentAnnotations.clear();

    QFileInfo fi(imagePath);
    const QString txtPath = fi.dir().filePath(fi.completeBaseName() + ".txt");
    QFile f(txtPath);
    if (!f.exists()) return;
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&f);
    const int W = currentImage.width();
    const int H = currentImage.height();

    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        const QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.size() < 5) continue;

        bool ok0=false, ok1=false, ok2=false, ok3=false, ok4=false, ok5=true;
        int cls = parts[0].toInt(&ok0);
        double xc = parts[1].toDouble(&ok1);
        double yc = parts[2].toDouble(&ok2);
        double ww = parts[3].toDouble(&ok3);
        double hh = parts[4].toDouble(&ok4);
        float conf = 0.0f;
        if (parts.size() >= 6) conf = parts[5].toFloat(&ok5);

        if (!(ok0 && ok1 && ok2 && ok3 && ok4 && ok5)) continue;

        const int x = int((xc - ww/2.0) * W);
        const int y = int((yc - hh/2.0) * H);
        const int w = int(ww * W);
        const int h = int(hh * H);

        Annotation a;
        a.classId = cls;
        a.className = getClassName(cls);
        a.confidence = conf;
        a.boundingBox = QRect(x, y, w, h);
        currentAnnotations.push_back(a);
    }

    f.close();
}

QImage MainWindow::renderBoundingBoxesOn(const QImage &img) const
{
    QImage out = img.copy();
    QPainter p(&out);
    p.setRenderHint(QPainter::Antialiasing, true);

    for (const auto &a : currentAnnotations) {
        QColor c = classColors.contains(a.classId) ? classColors.value(a.classId) : QColor("#00FF00");
        QPen pen(c, 3);
        p.setPen(pen);
        p.drawRect(a.boundingBox);

        QString label = a.className;
        if (a.confidence > 0.0f) label += QString(" (%1)").arg(a.confidence, 0, 'f', 2);

        QFont font = p.font();
        font.setBold(true);
        font.setPointSize(10);
        p.setFont(font);

        const QRect r = a.boundingBox.adjusted(0, -22, 0, 0);
        p.fillRect(QRect(r.left(), r.top(), std::max(60, r.width()), 20), QColor(0, 0, 0, 140));
        p.setPen(Qt::white);
        p.drawText(QRect(r.left()+4, r.top(), std::max(60, r.width()), 20), Qt::AlignVCenter, label);
    }

    p.end();
    return out;
}

void MainWindow::toggleYoloBoundingBoxes()
{
    showYoloBoundingBoxes = !showYoloBoundingBoxes;
    updateToggleYoloButtonStyle();
    updateImage();
    logActivity(QString("YOLO bounding boxes %1").arg(showYoloBoundingBoxes ? "ON" : "OFF"));
}

// ------------------------------------------------------------
// Tagging helpers
// ------------------------------------------------------------
void MainWindow::ensureDefaultCategory()
{
    const QString def = "Uncategorized";
    if (!categoryPaths.contains(def)) categoryPaths[def] = QStringList();
}

QString MainWindow::currentCategory() const
{
    if (!categoryTabs || categoryTabs->count() == 0) return "Uncategorized";
    return categoryTabs->tabText(categoryTabs->currentIndex());
}

void MainWindow::rebuildCategoryTabs()
{
    categoryTabs->clear();
    categoryWidgets.clear();

    QStringList cats = categoryPaths.keys();
    cats.removeAll("Uncategorized");
    std::sort(cats.begin(), cats.end(), [](const QString &a, const QString &b){
        return a.toLower() < b.toLower();
    });
    cats.prepend("Uncategorized");

    for (const QString &cat : cats) {
        QListWidget *w = new QListWidget(this);
        w->setSelectionMode(QAbstractItemView::ExtendedSelection);
        w->setStyleSheet("QListWidget { color: black; background-color: white; }");

        for (const QString &p : categoryPaths.value(cat)) w->addItem(p);

        categoryWidgets[cat] = w;
        categoryTabs->addTab(w, cat);
    }
}

void MainWindow::updateTaggingHintLabel()
{
    QStringList parts;
    for (auto it = keyToCategory.constBegin(); it != keyToCategory.constEnd(); ++it) {
        QString keyText = QKeySequence(it.key()).toString(QKeySequence::NativeText);
        if (keyText.isEmpty()) continue;
        parts << QString("%1=%2").arg(keyText, it.value());
    }
    parts.sort();
    QString hint = "Tag keys: " + parts.join("   ");
    hint += "   |   A = add to current tab";
    taggingHintLabel->setText(hint);
}

void MainWindow::addCurrentImageToCategory(const QString &category, bool advanceAfter)
{
    if (imageList.isEmpty()) return;

    const QString cat = category.isEmpty() ? "Uncategorized" : category;
    if (!categoryPaths.contains(cat)) categoryPaths[cat] = QStringList();

    const QString imagePath = directory.filePath(imageList.at(currentImageIndex));
    if (!categoryPaths[cat].contains(imagePath)) {
        categoryPaths[cat].append(imagePath);

        if (categoryWidgets.contains(cat)) {
            categoryWidgets[cat]->addItem(imagePath);
        } else {
            rebuildCategoryTabs();
        }

        logActivity(QString("Tagged: %1 -> %2").arg(imagePath, cat));
    }

    if (advanceAfter) showNextImage();
}

void MainWindow::addImageToList()
{
    addCurrentImageToCategory(currentCategory(), false);
    setFocus();
}

void MainWindow::removeImageFromList()
{
    const QString cat = currentCategory();
    if (!categoryWidgets.contains(cat)) return;

    QListWidget *w = categoryWidgets.value(cat);
    const QList<QListWidgetItem*> sel = w->selectedItems();
    if (sel.isEmpty()) return;

    // Collect selected row indices and remove from bottom to top to avoid pointer invalidation.
    QVector<int> rows;
    rows.reserve(sel.size());
    for (QListWidgetItem *it : sel) {
        const int row = w->row(it);
        if (row >= 0) rows.push_back(row);
    }
    std::sort(rows.begin(), rows.end());
    rows.erase(std::unique(rows.begin(), rows.end()), rows.end());

    for (int i = rows.size() - 1; i >= 0; --i) {
        const int row = rows[i];
        QListWidgetItem *item = w->item(row);
        if (!item) continue;

        const QString path = item->text(); // capture before deletion
        categoryPaths[cat].removeAll(path);

        delete w->takeItem(row);

        logActivity(QString("Removed from '%1': %2").arg(cat, path));
    }

    setFocus();
}

void MainWindow::clearImageList()
{
    const QString cat = currentCategory();
    const auto reply = QMessageBox::question(
        this, "Clear Category",
        QString("Clear all items in category '%1'?").arg(cat),
        QMessageBox::Yes | QMessageBox::No
    );
    if (reply != QMessageBox::Yes) return;

    categoryPaths[cat].clear();
    if (categoryWidgets.contains(cat)) categoryWidgets[cat]->clear();
    logActivity("Cleared category: " + cat);
    setFocus();
}

// ------------------------------------------------------------
// Saving per-category lists
// ------------------------------------------------------------
bool MainWindow::ensureSavedListsDir()
{
    if (!savedListsDir.isEmpty()) return true;

    const QString dir = QFileDialog::getExistingDirectory(this, "Choose folder to save category lists", "");
    if (dir.isEmpty()) return false;
    savedListsDir = dir;
    return true;
}

QString MainWindow::categoryListFilePath(const QString &category) const
{
    return QDir(savedListsDir).filePath(QString("%1_list.txt").arg(category));
}

void MainWindow::saveAllCategoryLists(bool silent)
{
    if (!ensureSavedListsDir()) return;

    for (auto it = categoryPaths.constBegin(); it != categoryPaths.constEnd(); ++it) {
        QFile f(categoryListFilePath(it.key()));
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) continue;
        QTextStream out(&f);
        for (const QString &p : it.value()) out << p << "\n";
        f.close();
    }

    const QString ts = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    lastSavedLabel->setText("Last saved lists at: " + ts);
    logActivity("Saved category lists to: " + savedListsDir);

    if (!silent) QMessageBox::information(this, "Saved", "All category lists saved.");
}

void MainWindow::saveImageList()
{
    saveAllCategoryLists(false);
    setFocus();
}

// ------------------------------------------------------------
// Tagging config dialog
// ------------------------------------------------------------
static bool isSingleKeyValid(const QString &s)
{
    const QString t = s.trimmed();
    if (t.size() != 1) return false;
    const QChar c = t.at(0);
    return c.isLetterOrNumber();
}

static int keyCodeFromSingleChar(const QString &s)
{
    const QString t = s.trimmed();
    if (t.size() != 1) return 0;
    const QChar c = t.at(0);

    if (c.isDigit()) {
        return int(Qt::Key_0) + (c.unicode() - QChar('0').unicode());
    }
    if (c.isLetter()) {
        const QChar u = c.toUpper();
        return int(Qt::Key_A) + (u.unicode() - QChar('A').unicode());
    }
    return 0;
}

void MainWindow::openTaggingConfig()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Configure Image Tagging (Key -> Category)");

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QLabel *desc = new QLabel(
        "Add up to 30 mappings. Key must be a single letter or digit (no multi-key like \"ab\").\n"
        "Pressing the key will tag the current image into that category and advance.",
        &dlg
    );
    layout->addWidget(desc);

    QTableWidget *table = new QTableWidget(&dlg);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"Key", "Category"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    layout->addWidget(table);

    QVector<QPair<int, QString>> existing;
    existing.reserve(keyToCategory.size());
    for (auto it = keyToCategory.constBegin(); it != keyToCategory.constEnd(); ++it)
        existing.push_back({it.key(), it.value()});

    std::sort(existing.begin(), existing.end(), [](const auto &a, const auto &b){ return a.first < b.first; });

    table->setRowCount(std::min(30, int(existing.size())));
    for (int r = 0; r < table->rowCount(); ++r) {
        const QString keyText = QKeySequence(existing[r].first).toString(QKeySequence::NativeText);
        table->setItem(r, 0, new QTableWidgetItem(keyText));
        table->setItem(r, 1, new QTableWidgetItem(existing[r].second));
    }

    QHBoxLayout *rowBtns = new QHBoxLayout();
    QPushButton *addRow = new QPushButton("Add Row", &dlg);
    QPushButton *rmRow = new QPushButton("Remove Row", &dlg);
    rowBtns->addWidget(addRow);
    rowBtns->addWidget(rmRow);
    rowBtns->addStretch();
    layout->addLayout(rowBtns);

    connect(addRow, &QPushButton::clicked, &dlg, [table](){
        if (table->rowCount() >= 30) return;
        table->insertRow(table->rowCount());
    });
    connect(rmRow, &QPushButton::clicked, &dlg, [table](){
        const auto ranges = table->selectedRanges();
        if (ranges.isEmpty()) return;
        for (int i = ranges.size()-1; i >= 0; --i) {
            for (int r = ranges[i].bottomRow(); r >= ranges[i].topRow(); --r)
                table->removeRow(r);
        }
    });

    QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addWidget(bb);
    connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    QMap<int, QString> newMap;
    QStringList newCats;

    for (int r = 0; r < table->rowCount(); ++r) {
        const QString keyText = table->item(r, 0) ? table->item(r, 0)->text().trimmed() : "";
        const QString catText = table->item(r, 1) ? table->item(r, 1)->text().trimmed() : "";

        if (keyText.isEmpty() && catText.isEmpty()) continue;

        if (!isSingleKeyValid(keyText)) {
            QMessageBox::warning(this, "Invalid Key",
                                 QString("Row %1: Key must be a single letter or digit.").arg(r+1));
            return;
        }
        if (catText.isEmpty()) {
            QMessageBox::warning(this, "Invalid Category",
                                 QString("Row %1: Category cannot be empty.").arg(r+1));
            return;
        }

        const int keyCode = keyCodeFromSingleChar(keyText);
        if (keyCode == 0) {
            QMessageBox::warning(this, "Invalid Key",
                                 QString("Row %1: Unable to interpret key.").arg(r+1));
            return;
        }

        if (newMap.contains(keyCode)) {
            QMessageBox::warning(this, "Duplicate Key",
                                 QString("Key '%1' is used more than once.").arg(keyText));
            return;
        }

        newMap[keyCode] = catText;
        if (!newCats.contains(catText)) newCats << catText;
    }

    keyToCategory = newMap;

    for (const QString &c : newCats) {
        if (!categoryPaths.contains(c)) categoryPaths[c] = QStringList();
    }

    rebuildCategoryTabs();
    updateTaggingHintLabel();
    logActivity("Updated tagging configuration.");
    setFocus();
}

// ------------------------------------------------------------
// Bulk actions
// ------------------------------------------------------------
bool MainWindow::chooseDestinationsForCategories(const QStringList &categories,
                                                QMap<QString, QString> &outCategoryToDir,
                                                const QString &title,
                                                const QString &actionVerb) const
{
    outCategoryToDir.clear();

    for (const QString &cat : categories) {
        const QString prompt = QString("%1 destination folder for category '%2'").arg(actionVerb, cat);
        const QString dir = QFileDialog::getExistingDirectory(const_cast<MainWindow*>(this), title + " - " + prompt, "");
        if (dir.isEmpty()) return false;
        outCategoryToDir[cat] = dir;
    }
    return true;
}

bool MainWindow::applyActionToCategory(const QString &category,
                                      const QStringList &paths,
                                      BulkAction action,
                                      const QString &destDir)
{
    if (paths.isEmpty()) return true;

    QDir d(destDir);
    if (!d.exists()) {
        if (!d.mkpath(".")) {
            QMessageBox::warning(this, "Folder", "Failed to create destination folder:\n" + destDir);
            return false;
        }
    }

    for (const QString &src : paths) {
        QFileInfo fi(src);
        if (!fi.exists()) continue;

        const QString dst = d.filePath(fi.fileName());
        const QString srcTxt = fi.dir().filePath(fi.completeBaseName() + ".txt");
        const QString dstTxt = d.filePath(fi.completeBaseName() + ".txt");

        auto doCopy = [&](const QString &a, const QString &b){
            if (QFile::exists(b)) QFile::remove(b);
            return QFile::copy(a, b);
        };
        auto doMove = [&](const QString &a, const QString &b){
            if (QFile::exists(b)) QFile::remove(b);
            return QFile::rename(a, b);
        };

        bool okImg = true;
        bool okAnn = true;

        if (action == BulkAction::Copy) {
            okImg = doCopy(src, dst);
            if (QFile::exists(srcTxt)) okAnn = doCopy(srcTxt, dstTxt);
        } else {
            okImg = doMove(src, dst);
            if (QFile::exists(srcTxt)) okAnn = doMove(srcTxt, dstTxt);
        }

        if (!okImg) {
            QMessageBox::warning(this, "File Operation", "Failed on:\n" + src);
            return false;
        }
        if (!okAnn) logActivity("Warning: failed annotation op for " + src);

        logActivity(QString("%1: %2 -> %3 (cat=%4)")
                        .arg(action == BulkAction::Copy ? "COPY" : "MOVE",
                             src, destDir, category));
    }

    return true;
}

bool MainWindow::runBulkAction(BulkAction action)
{
    saveAllCategoryLists(true);

    QMap<QString, QStringList> selectedByCat;
    for (auto it = categoryWidgets.constBegin(); it != categoryWidgets.constEnd(); ++it) {
        const QString cat = it.key();
        QListWidget *w = it.value();
        QStringList sel;
        for (QListWidgetItem *item : w->selectedItems()) sel << item->text();
        if (!sel.isEmpty()) selectedByCat[cat] = sel;
    }

    const int totalSel = [&](){
        int n=0;
        for (auto it=selectedByCat.begin(); it!=selectedByCat.end(); ++it) n += it.value().size();
        return n;
    }();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        (action == BulkAction::Copy) ? "Copy" : (action == BulkAction::Move) ? "Move" : "Delete",
        QString("Proceed to %1 selected images across categories?\n(Selected: %2)\n\nYou will be asked for a destination folder per category.")
            .arg(action == BulkAction::Copy ? "COPY" : (action == BulkAction::Move ? "MOVE" : "MOVE (Delete)"))
            .arg(totalSel),
        QMessageBox::Yes | QMessageBox::No
    );
    if (reply != QMessageBox::Yes) return false;

    if (selectedByCat.isEmpty()) {
        logActivity("Bulk action requested with no selections.");
        return true;
    }

    const QStringList cats = selectedByCat.keys();
    QMap<QString, QString> catToDir;
    const QString actionVerb = (action == BulkAction::Copy) ? "Choose COPY"
                                : (action == BulkAction::Move) ? "Choose MOVE"
                                : "Choose DELETE (Move)";

    if (!chooseDestinationsForCategories(cats, catToDir, "Select Destination Folders", actionVerb)) {
        logActivity("Bulk action cancelled by user during destination selection.");
        return false;
    }

    for (const QString &cat : cats) {
        if (!applyActionToCategory(cat, selectedByCat.value(cat), action, catToDir.value(cat)))
            return false;
    }

    if (action == BulkAction::Move || action == BulkAction::Delete) {
        for (const QString &cat : cats) {
            const QStringList moved = selectedByCat.value(cat);
            // Qt5/older Qt6 compatibility: QStringList has no removeIf().
            {
                const QStringList current = categoryPaths.value(cat);
                QStringList kept;
                kept.reserve(current.size());
                for (const QString &p : current) {
                    if (!moved.contains(p)) kept << p;
                }
                categoryPaths[cat] = kept;
            }

            if (categoryWidgets.contains(cat)) {
                QListWidget *w = categoryWidgets[cat];
                for (int i = w->count()-1; i >= 0; --i) {
                    if (moved.contains(w->item(i)->text())) delete w->takeItem(i);
                }
            }
        }
        pruneMissingFiles();
        updateImage();
    }

    logActivity("Bulk action completed.");
    QMessageBox::information(this, "Done", "Action completed.");
    return true;
}

void MainWindow::copySelectedImages()  { runBulkAction(BulkAction::Copy); setFocus(); }
void MainWindow::moveSelectedImage()   { runBulkAction(BulkAction::Move); setFocus(); }
void MainWindow::deleteSelectedImage() { runBulkAction(BulkAction::Delete); setFocus(); }

// ------------------------------------------------------------
// Prune missing files
// ------------------------------------------------------------
void MainWindow::pruneMissingFiles()
{
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.JPG" << "*.JPEG" << "*.PNG";
    imageList = directory.entryList(filters, QDir::Files, QDir::Name);
    if (currentImageIndex >= imageList.size()) currentImageIndex = std::max(0, imageList.size()-1);

    imageSlider->setRange(0, std::max(0, imageList.size()-1));
    imageSlider->blockSignals(true);
    imageSlider->setValue(currentImageIndex);
    imageSlider->blockSignals(false);

    for (auto it = categoryPaths.begin(); it != categoryPaths.end(); ++it) {
        QStringList kept;
        kept.reserve(it.value().size());
        for (const QString &p : it.value()) {
            if (QFileInfo::exists(p)) kept << p;
        }
        it.value() = kept;
    }

    updateFolderDateTimeLabel();
}

// ------------------------------------------------------------
// Logging
// ------------------------------------------------------------
void MainWindow::logActivity(const QString &message)
{
    if (!logStream.device()) return;
    logStream << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
              << " - " << message << "\n";
    logStream.flush();
}


// ------------------------------------------------------------
// Open current image folder in file explorer
// ------------------------------------------------------------
void MainWindow::openCurrentImageFolderInExplorer()
{
    const QString folder = directory.absolutePath();
    if (folder.isEmpty()) return;
    QDesktopServices::openUrl(QUrl::fromLocalFile(folder));
}


// ------------------------------------------------------------
// Status bar: show current directory name
// ------------------------------------------------------------
void MainWindow::updateDirectoryNameLabel()
{
    // Folder name is displayed in the centered info label under the image.
}


// ------------------------------------------------------------
// Folder timestamp label: uses modified time of FIRST image in folder
// ------------------------------------------------------------
void MainWindow::updateFolderDateTimeLabel()
{
    if (!dateTimeLabel) return;

    if (imageList.isEmpty()) {
        dateTimeLabel->setText("");
        return;
    }

    const int idx = std::clamp(currentImageIndex, 0, imageList.size() - 1);
    const QString imgPath = directory.filePath(imageList.at(idx));

    QFileInfo fi(imgPath);
    if (!fi.exists()) {
        dateTimeLabel->setText("");
        return;
    }

    const QDateTime dt = fi.lastModified();
    dateTimeLabel->setText(QString("Modified: %1").arg(dt.toString("yyyy-MM-dd HH:mm:ss")));
}
