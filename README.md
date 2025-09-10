# AI_ImageSelector
A Qt application for efficient image management and annotation workflows, designed to simplify the process for data annotators.

---

## Features & Workflow
1. **Choose** an image directory on startup.
2. **Navigate** images using left/right arrow keys or on-screen buttons.
3. **Add** an image (click **Add** or press **A**) → image path appears in the right-side panel.
4. **Remove** an image path if added by mistake (select and click **Remove**).
5. **Save** the list of selected file paths (e.g., `move_to_train.txt`, `move_to_val.txt`).
6. **Perform actions** (only after saving a list):
   - **Delete** → moves selected images to a `deleted_images/` folder in the parent directory.
   - **Move** → prompts the user to select a destination folder (e.g., `Train/`).
   - **Copy** → same as Move but copies instead of moving.
7. **YOLO Integration**:
   - **Choose Names File** → select a `.names` file.
   - **Toggle Bounding Boxes** → view/hide YOLO annotations (class + confidence).

---

## Installation & Usage

### Clone the Repository
```bash
git clone https://github.com/nbandaru1h/AI_ImageSelector.git
cd AI_ImageSelector

```
BUILD (LINUX)

```bash
mkdir build && cd build
cmake ..
make
./AI_ImageSelector

```
------------------------------------------------------------------------------------------------------------------------------------------------------
Upcoming Updates:
1. Windows Executable
2. Support for coco format, X-AnyLabeling format in addition to darknet. 
------------------------------------------------------------------------------------------------------------------------------------------------------

## AUTHOR

NIRALI BANDARU
