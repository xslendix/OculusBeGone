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
$ovr = Get-Process OVRServiceLauncher -ErrorAction SilentlyContinue
if ($ovr) {
    Write-Host "Stopping OVRService"
    Stop-Process -Name OVRServiceLauncher -Force
}

# Backup OculusDash.exe
if (Test-Path -Path "C:\Program Files\Oculus\OculusDash.exe") {
    $backup_file = "C:\Program Files\Oculus\OculusDash.exe.backup"
    if (!(Test-Path $backup_file)) {
        Write-Host "Backing up existing OculusDash.exe"
        New-Item -Path $backup_file
        Copy-Item -Path "C:\Program Files\Oculus\OculusDash.exe" -Destination $backup_file
    }
}

# Check if new OculusDash.exe exists in the same directory as the script
if (Test-Path -Path "OculusDash.exe") {
    Write-Host "Copying new OculusDash.exe"
    Copy-Item -Path "OculusDash.exe" -Destination "C:\Program Files\Oculus"
} else {
    Write-Host "OculusDash.exe not found in the same directory as the script. Please copy it to the Oculus directory manually."

    # Wait for any key press
    Write-Host 'Press any key to continue...';
    $null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown');
}

# Start OVRService service
if ($ovr) {
    Write-Host "Starting OVRService"
    Start-Process OVRServiceLauncher -ArgumentList -start
}

# Wait for any key press
Write-Host 'Successfully installed! Press any key to continue...';
$null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown');
