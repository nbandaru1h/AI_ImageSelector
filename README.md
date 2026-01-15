# AI_ImageSuite
A Qt application for efficient image management and annotation workflows, designed to simplify the process for data annotators and AI Engineers.

---

## Features & Workflow

Navigation
1. **Choose** an image directory on startup or **Load Image Directory** using the button at the top in the toolbar.
2. **Navigate** images using left/right arrow keys or on-screen buttons below image. Navigate to previous image directory or next image directory using **prev dir** or **next dir** with ease.

Image Categorization
1. **Configure Image Tagging** - use this button to set up key-category pairs where upon the press of the key, current image is added to the respective category's list. 
2. **Add** an image (click **Add** or press **A**) → image path appears in the right-side panel.
3. **Remove** an image path if added by mistake (select and click **Remove**). Multiple selections allowed.
4. **Save** the list of selected file paths (all lists are saved to selected directory and named after their categories).

Image Filtering
1. **Perform actions** (only after saving a list):
   - **Delete** → moves selected images to a `deleted_images/` folder in the parent directory.
   - **Move** → prompts the user to select a destination folder (e.g., `Train/`).
   - **Copy** → same as Move but copies instead of moving.
2. **YOLO Integration**:
   - **Load Names** → select a `.names` file.
   - **YOLO** → view/hide YOLO annotations (class + confidence).

---

## Installation & Usage

### Clone the Repository
```bash
git clone https://github.com/nbandaru1h/AI_ImageSuite.git
cd AI_ImageSuite

```
# INSTALL (LINUX) with APP IMAGE

```bash
./scripts/install_launcher.sh

```
(Search for the app "AI Image Suite" from your launch menu)

# UNINSTALL (LINUX)

```bash
./scripts/uninstall_launcher.sh
```

# BUILD FROM SOURCE

```bash
git clone https://github.com/nbandaru1h/AI_ImageSuite.git
cd AI_ImageSuite
rm -rf build && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
./AI_ImageSuite
```

------------------------------------------------------------------------------------------------------------------------------------------------------
Upcoming Updates:
1. Windows Executable
2. Support for coco format, X-AnyLabeling format in addition to darknet. 
------------------------------------------------------------------------------------------------------------------------------------------------------

## AUTHOR

NIRALI BANDARU
