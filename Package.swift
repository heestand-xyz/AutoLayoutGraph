// swift-tools-version: 5.9

import PackageDescription

let package = Package(
    name: "AutoLayoutGraph",
    platforms: [
        .iOS(.v16),
        .macOS(.v13),
        .visionOS(.v1)
    ],
    products: [
        .library(
            name: "AutoLayoutGraph",
            targets: ["AutoLayoutGraph"]),
    ],
    targets: [
        .target(
            name: "AutoLayoutGraph"),
    ],
    cxxLanguageStandard: .cxx20
)
