# This block defines the parameters the script can accept from the command line.
param (
    # The full path to the build directory where the .exe is located
    # Example: F:/IKT203/VisualStudio/Exercises/out/build/x64-debug/Submission-01
    [string]$BuildDir,

    # The path to your vcpkg installation
    [string]$VcpkgRoot = "D:/dev/vcpkg",

    # The vcpkg triplet name (e.g., x64-windows)
    [string]$Triplet = "x64-windows"
)

# --- Script Body ---
Write-Host "Starting deployment for project in $BuildDir..." -ForegroundColor Green

# Define the source paths using the parameters.
# Path to the main Qt DLLs (e.g., .../debug/bin)
$vcpkgBinPath = Join-Path $VcpkgRoot "installed/$Triplet/debug/bin"

# CORRECTED: Path to the platform plugins (e.g., .../debug/Qt6/plugins/platforms)
$vcpkgPluginsPath = Join-Path $VcpkgRoot "installed/$Triplet/debug/Qt6/plugins/platforms"

# The destination path is the build directory's main folder.
$destinationPath = $BuildDir

# Check if the destination directory exists.
if (-not (Test-Path $destinationPath)) {
    Write-Host "Error: Build directory not found at $destinationPath" -ForegroundColor Red
    exit 1
}

# --- Copy Main DLLs ---
# List the essential DLLs your project needs.
$qtDlls = @(
    "Qt6Cored.dll",
    "Qt6Guid.dll",
    "Qt6Widgetsd.dll"
)

Write-Host "Copying main Qt DLLs to $destinationPath"
foreach ($dll in $qtDlls) {
    $sourceDll = Join-Path $vcpkgBinPath $dll
    Copy-Item -Path $sourceDll -Destination $destinationPath -Force
}

# --- Copy Platform Plugin ---
# Create the "platforms" subdirectory if it doesn't exist.
$pluginDestPath = Join-Path $destinationPath "platforms"
if (-not (Test-Path $pluginDestPath)) {
    New-Item -ItemType Directory -Path $pluginDestPath
}

Write-Host "Copying platform plugin to $pluginDestPath"
$sourcePlugin = Join-Path $vcpkgPluginsPath "qwindowsd.dll"
Copy-Item -Path $sourcePlugin -Destination $pluginDestPath -Force

Write-Host "Deployment complete!" -ForegroundColor Green
