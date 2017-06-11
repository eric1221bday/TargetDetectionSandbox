# TargetDetectionSandbox

Requires OpenCV 3 and CMake

## Installation

```bash
git clone https://github.com/eric1221bday/TargetDetectionSandbox.git
cd TargetDetectionSandbox
mkdir build
cd build
cmake ..
make
```

## Usage
```bash
cd TargetDetectionSandbox/build
./ConcentricDetector # opens webcam and tries to detect any concentric circles
./BlobDetector Path/To/Video # opens specified video and segments base on pink hue
```