# Picsmith-CLI-Image-Utility

**Picsmith** is a fast and easy-to-use command-line utility that lets you convert, compress, and resize images with precision, supporting multiple formats like JPG, PNG, WEBP, BMP, TIFF, EXR, and more. Perfect for developers, designers, and anyone looking to automate image processing via the terminal.

---

## Features

* **Image Conversion:** Easily convert images between popular formats (`jpg`, `png`, `webp`, `bmp`, `tiff`, `exr`, `jp2`, and more).
* **Compression:** Reduce file size while maintaining quality. Supports JPG, PNG, and WEBP with adjustable compression levels.
* **Resizing:** Quickly rescale images to predefined resolutions like SD, HD, FHD, 2K, 4K, 8K, or custom dimensions.
* **Cross-Platform:** Works on Windows, macOS, and Linux.
* **CLI Friendly:** Lightweight and scriptable, perfect for automation and batch processing.

---

## Installation

Clone the repository:

```bash
git clone https://github.com/0xhex0rc1st/PicSmith-CLI-Image-Utility.git
cd PicSmith-CLI-Image-Utility/PicSmith/src
```

Compile with C++17 and OpenCV:

```bash
g++ -std=c++17 -o PicSmith PicSmith.cpp `pkg-config --cflags --libs opencv4`
```

> Make sure you have OpenCV installed on your system.

---

## Usage

```bash
./PicSmith -i INPUT [OPTIONS]
```

### Arguments

* `-i INPUT`
  Path to the input image file.

### Options

* `-f FORMAT`
  Output format (`jpg`, `png`, `webp`, `bmp`, `tiff`, `exr`, `jp2`, etc.)

* `-c [VALUE]`
  Compression quality:

  * JPG/WEBP: 1-100 (default: JPG=85, WEBP=80)
  * PNG: 1-9 (1 = max compression, 9 = min, default=6)

* `-o OUTPUT`
  Output directory (defaults to input file location)

* `-r RESOLUTION`
  Rescale image to a predefined resolution:

  ```
  SD, SD_640x480, SD_800x600
  HD, HD_widescreen, HD+, FHD, 2K, QHD, 4K, 8K
  ```

* `-h, --help`
  Show help message.

---

## Examples

* Convert an image to PNG:

```bash
./PicSmith -i photo.jpg -f png
```

* Compress an image with custom quality:

```bash
./PicSmith -i photo.jpg -c 70
```

* Resize an image to HD:

```bash
./PicSmith -i photo.jpg -r HD
```

* Convert, compress, and resize:

```bash
./PicSmith -i photo.png -f jpg -c 85 -r FHD
```

---

## Predefined Resolutions

| Key            | Resolution (WxH) |
| -------------- | ---------------- |
| SD             | 640x480          |
| SD\_640x480    | 640x480          |
| SD\_800x600    | 800x600          |
| HD             | 1280x720         |
| HD\_widescreen | 1366x768         |
| HD+            | 1600x900         |
| FHD            | 1920x1080        |
| 2K / QHD       | 2560x1440        |
| 4K             | 3840x2160        |
| 8K             | 7680x4320        |

---

## Contributing

Contributions are welcome! Please fork the repository and submit pull requests.

* Bug reports and feature requests: open an issue.
* Code contributions: submit PRs with clean, well-documented code.

---

## License

MIT License © 2025 \[0xhex0rc1st]
