# Arduino Uno Auto-Uploader Script
$ErrorActionPreference = "Stop"

Write-Host "========================================================" -ForegroundColor Cyan
Write-Host "    Arduino Uno 5-in-1 Firmware Uploader (COM3)" -ForegroundColor Cyan
Write-Host "========================================================" -ForegroundColor Cyan
Write-Host ""

$cliPath = "C:\Program Files\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
$sketchPath = "$PSScriptRoot\all_in_one"

if (-not (Test-Path $cliPath)) {
    Write-Host "[Error] arduino-cli not found at: $cliPath" -ForegroundColor Red
    Read-Host "Press Enter to exit..."
    exit 1
}

# Find Uno Port
Write-Host "Searching for connected Arduino Uno..." -ForegroundColor Yellow
$boardList = & $cliPath board list
$unoLine = $boardList | Where-Object { $_ -match "COM\d+" -and $_ -match "Arduino" }

$targetPort = "COM3"
if ($unoLine -and ($unoLine -match "(COM\d+)")) {
    $targetPort = $matches[1]
}

Write-Host "-> Target Port: $targetPort" -ForegroundColor Green
Write-Host "-> Uploading 5-in-1 firmware to $targetPort..." -ForegroundColor Yellow
Write-Host "   (Note: Please CLOSE Arduino IDE Serial Monitor before uploading!)" -ForegroundColor Gray
Write-Host ""

try {
    & $cliPath upload -p $targetPort --fqbn arduino:avr:uno $sketchPath
    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "========================================================" -ForegroundColor Green
        Write-Host " [SUCCESS] Firmware uploaded successfully!" -ForegroundColor Green
        Write-Host " Now open index.html in your browser and enjoy!" -ForegroundColor Green
        Write-Host "========================================================" -ForegroundColor Green
    } else {
        throw "Upload failed with exit code $LASTEXITCODE"
    }
} catch {
    Write-Host ""
    Write-Host "========================================================" -ForegroundColor Red
    Write-Host " [FAILED] Upload failed: Access Denied / Port Busy" -ForegroundColor Red
    Write-Host " Solution: Please CLOSE the Arduino IDE Serial Monitor window" -ForegroundColor Yellow
    Write-Host "           or DISCONNECT Web Serial in your browser first." -ForegroundColor Yellow
    Write-Host "========================================================" -ForegroundColor Red
}

Write-Host ""
Read-Host "Press Enter to exit..."
