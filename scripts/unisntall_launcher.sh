#!/usr/bin/env bash
set -euo pipefail

APP_ID="ai-image-selector"
APP_NAME="AI Image Selector"

DESKTOP_FILE="${HOME}/.local/share/applications/${APP_ID}.desktop"
APPIMAGE_DST="${HOME}/Applications/${APP_ID}.AppImage"
ICON_BASE="${HOME}/.local/share/icons/hicolor"

rm -f "${DESKTOP_FILE}" || true
rm -f "${APPIMAGE_DST}" || true

for SZ in 16 24 32 48 64 128 256 512; do
  rm -f "${ICON_BASE}/${SZ}x${SZ}/apps/${APP_ID}.png" || true
done

update-desktop-database "${HOME}/.local/share/applications" || true
gtk-update-icon-cache -f -t "${ICON_BASE}" || true

echo "🧹 Uninstalled “${APP_NAME}” from your launcher."

