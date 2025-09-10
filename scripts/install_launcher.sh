#!/usr/bin/env bash
set -euo pipefail

APP_ID="ai-image-selector"
APP_NAME="AI Image Selector"

# Detect repo root
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
REPO_ROOT="$( cd "${SCRIPT_DIR}/.." && pwd )"

# Paths
ICON_SRC="${REPO_ROOT}/assets/AI_ImageSelector.png"
LOCAL_APPIMAGE_DIR="${REPO_ROOT}/appimage"
USER_APPS_DIR="${HOME}/.local/share/applications"
USER_ICON_BASE="${HOME}/.local/share/icons/hicolor"
USER_APPIMAGE_DST="${HOME}/Applications/${APP_ID}.AppImage"
DESKTOP_FILE="${USER_APPS_DIR}/${APP_ID}.desktop"

# Optional: auto-download from GitHub Releases if no local AppImage
GITHUB_REPO="nbandaru1h/AI_ImageSelector"  # <-- set to your repo
USE_RELEASE_DOWNLOAD=true

# Tools
need() { command -v "$1" >/dev/null 2>&1 || { echo "Installing $1..."; sudo apt-get update -y && sudo apt-get install -y "$2"; }; }
need convert imagemagick
need update-desktop-database desktop-file-utils
need gtk-update-icon-cache libgtk-3-bin
need curl curl
need jq jq || true

mkdir -p "${HOME}/Applications" "${USER_APPS_DIR}"

# Arch → filename pattern
ARCH="$(uname -m)"
if [[ "$ARCH" == "x86_64" ]]; then
  PATTERN="AI_ImageSelector*x86_64*.AppImage"
elif [[ "$ARCH" == "aarch64" ]]; then
  PATTERN="AI_ImageSelector*aarch64*.AppImage"
else
  echo "❌ Unsupported architecture: ${ARCH}"; exit 1
fi

# Choose AppImage: prefer repo/appimage/, else latest Release
APPIMAGE_SRC=""
if [[ -d "${LOCAL_APPIMAGE_DIR}" ]]; then
  APPIMAGE_SRC="$(ls -1t ${LOCAL_APPIMAGE_DIR}/${PATTERN} 2>/dev/null | head -n1 || true)"
fi

if [[ -z "${APPIMAGE_SRC}" && "${USE_RELEASE_DOWNLOAD}" == true ]]; then
  echo "ℹ️  No local AppImage found. Checking latest GitHub Release for ${GITHUB_REPO}..."
  if command -v jq >/dev/null 2>&1; then
    API_URL="https://api.github.com/repos/${GITHUB_REPO}/releases/latest"
    DL_URL="$(curl -fsSL "${API_URL}" | jq -r '.assets[]?.browser_download_url' | grep -E "${PATTERN//\*/.*}" | head -n1 || true)"
    if [[ -n "${DL_URL}" ]]; then
      TMP_DL="${HOME}/Downloads/${APP_ID}-$(date +%s).AppImage"
      echo "⬇️  Downloading ${DL_URL}"
      curl -fL "${DL_URL}" -o "${TMP_DL}"
      APPIMAGE_SRC="${TMP_DL}"
    else
      echo "⚠️  Could not find a matching asset in latest release."
    fi
  else
    echo "⚠️  jq not installed; skipping release download."
  fi
fi

[[ -n "${APPIMAGE_SRC}" ]] || { echo "❌ No AppImage found. Place it under appimage/ or publish a Release."; exit 1; }

# Install AppImage
cp -f "${APPIMAGE_SRC}" "${USER_APPIMAGE_DST}"
chmod +x "${USER_APPIMAGE_DST}"
echo "✅ AppImage → ${USER_APPIMAGE_DST}"

# Icons (install multiple sizes into user hicolor theme)
for SZ in 16 24 32 48 64 128 256 512; do
  OUTDIR="${USER_ICON_BASE}/${SZ}x${SZ}/apps"
  mkdir -p "${OUTDIR}"
  convert "${ICON_SRC}" -resize ${SZ}x${SZ} -gravity center -background none -extent ${SZ}x${SZ} \
    "${OUTDIR}/${APP_ID}.png"
done

# Desktop entry
cat > "${DESKTOP_FILE}" <<EOF
[Desktop Entry]
Type=Application
Version=1.0
Name=${APP_NAME}
Comment=Visualize and manage image lists
Exec="${USER_APPIMAGE_DST}" %F
Icon=${APP_ID}
Terminal=false
Categories=Graphics;Utility;
Keywords=Images;Viewer;Annotation;Qt;
StartupNotify=true
EOF
chmod +x "${DESKTOP_FILE}"

# Refresh caches
update-desktop-database "${USER_APPS_DIR}" || true
gtk-update-icon-cache -f -t "${HOME}/.local/share/icons/hicolor" || true
gio set "${DESKTOP_FILE}" metadata::trusted true 2>/dev/null || true

echo
echo "🎉 Installed. Search your launcher for “${APP_NAME}”."
echo "   Desktop file: ${DESKTOP_FILE}"
echo "   AppImage:     ${USER_APPIMAGE_DST}"

