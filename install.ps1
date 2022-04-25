#Requires -RunAsAdministrator

function Test-Administrator  
{  
    [OutputType([bool])]
    param()
    process {
        [Security.Principal.WindowsPrincipal]$user = [Security.Principal.WindowsIdentity]::GetCurrent();
        return $user.IsInRole([Security.Principal.WindowsBuiltinRole]::Administrator);
    }
}


if (-not (Test-Administrator)) {
    Write-Host "This script requires elevated privileges to run."
    exit 1
}

# Kill OVRService service
Stop-Process -Name OVRService -Force

# Detect oculus install directory
oculus_dir=$(Get-ChildItem -Path "C:\Program Files\Oculus" | Where-Object {$_.Name -eq "Oculus"} | Select-Object -ExpandProperty Name)

# Backup OculusDash.exe
if (Test-Path -Path "C:\Program Files\Oculus\OculusDash.exe") {
    $backup_dir = "C:\Program Files\Oculus\OculusDash.exe.backup"
    if (!(Test-Path $backup_dir)) {
        New-Item -Path $backup_dir
    }
    Copy-Item -Path "C:\Program Files\Oculus\OculusDash.exe" -Destination $backup_dir
}

# Check if new OculusDash.exe exists in the same directory as the script
if (Test-Path -Path "OculusDash.exe") {
    Copy-Item -Path "OculusDash.exe" -Destination "C:\Program Files\Oculus"
} else {
    Write-Host "OculusDash.exe not found in the same directory as the script. Please copy it to the Oculus directory manually."
}

# Wait for any key press
Write-Host 'Press any key to continue...';
$null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown');

# Start OVRService service
Start-Service "OVRService"
