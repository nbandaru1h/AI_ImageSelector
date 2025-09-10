#!/usr/bin/env bash
set -euo pipefail

APP="AI_ImageSelector"

# Paths
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
REPO_ROOT="$( cd "${SCRIPT_DIR}/.." && pwd )"
SRC_DIR="${REPO_ROOT}/src"
BUILD_DIR="${REPO_ROOT}/build"
APPDIR="${REPO_ROOT}/${APP}.AppDir"
ASSETS_DIR="${REPO_ROOT}/assets"
ICON_SRC="${ASSETS_DIR}/AI_ImageSelector.png"
OUT_DIR="${REPO_ROOT}/appimage"
mkdir -p "${BUILD_DIR}" "${OUT_DIR}"

# Build (Release)
cmake -S "${SRC_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release
cmake --build "${BUILD_DIR}" --config Release -j

BIN="${BUILD_DIR}/${APP}"
[[ -x "${BIN}" ]] || { echo "❌ Built binary not found at ${BIN}"; exit 1; }

# Get linuxdeploy & plugin
ARCH="$(uname -m)"
if [[ "$ARCH" == "x86_64" ]]; then
  LINUXDEPLOY_URL="https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage"
  QTPLUGIN_URL="https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage"
elif [[ "$ARCH" == "aarch64" ]]; then
  LINUXDEPLOY_URL="https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-aarch64.AppImage"
  QTPLUGIN_URL="https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-aarch64.AppImage"
else
  echo "❌ Unsupported architecture: ${ARCH}"; exit 1
fi
cd "${REPO_ROOT}"
LINUXDEPLOY="$(basename "$LINUXDEPLOY_URL")"
QTPLUGIN="$(basename "$QTPLUGIN_URL")"
[[ -f "$LINUXDEPLOY" ]] || wget -O "$LINUXDEPLOY" "$LINUXDEPLOY_URL"
[[ -f "$QTPLUGIN" ]]    || wget -O "$QTPLUGIN"    "$QTPLUGIN_URL"
chmod +x "$LINUXDEPLOY" "$QTPLUGIN"

# AppDir layout
rm -rf "${APPDIR}"
mkdir -p "${APPDIR}/usr/bin" "${APPDIR}/usr/share/icons/hicolor/256x256/apps"
cp "${BIN}" "${APPDIR}/usr/bin/${APP}"
cat > "${APPDIR}/${APP}.desktop" <<EOF
[Desktop Entry]
Type=Application
Name=${APP}
Exec=${APP}
Icon=ai_image_selector
Terminal=false
Categories=Graphics;Utility;
EOF
# 256x256 icon (required)
command -v convert >/dev/null 2>&1 || { echo "Installing ImageMagick..."; sudo apt-get update -y && sudo apt-get install -y imagemagick; }
convert "${ICON_SRC}" -resize 256x256 -gravity center -background none -extent 256x256 \
  "${APPDIR}/usr/share/icons/hicolor/256x256/apps/ai_image_selector.png"

# Build AppImage
"./${LINUXDEPLOY}" --appdir "${APPDIR}" \
  -e "${BIN}" \
  -d "${APPDIR}/${APP}.desktop}" \
  -i "${APPDIR}/usr/share/icons/hicolor/256x256/apps/ai_image_selector.png" \
  --plugin qt
"./${LINUXDEPLOY}" --appdir "${APPDIR}" --output appimage

# Move to appimage/ with arch suffix
OUT_NAME="${APP}-$(date +%Y%m%d)-${ARCH}.AppImage"
mv ${APP}-*.AppImage "${OUT_DIR}/${OUT_NAME}"
echo "✅ AppImage: ${OUT_DIR}/${OUT_NAME}"

