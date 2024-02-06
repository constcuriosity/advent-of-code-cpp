

if (-not (Test-Path env:SHARPMAKE_BIN))
{
    Write-Host -ForegroundColor Red "The SHARPMAKE_BIN environment variable is not set!"
    Write-Host "Clone and look at the README for the sharpmake_bin repository."

    pause
}
elseif (-not (Test-Path $env:SHARPMAKE_BIN))
{
    Write-Host -ForegroundColor Red "The SHARPMAKE_BIN environment variable does not point to a valid location!"
    Write-Host "Re-run the setup script in the sharpmake_bin repository."

    pause
}
else
{
    $app = Join-Path -Path $env:SHARPMAKE_BIN -ChildPath "Sharpmake.Application.exe"

    Write-Host "Executing Sharpmake from: " $app

    if (-not (Test-Path $app))
    {
        Write-Host -ForegroundColor Red "Sharpmake.Application.exe is not found in the SHARPMAKE_BIN location!"
        Write-Host "Re-run the setup script in the sharpmake_bin repository"

        pause
    }
    else
    {
        Start-Process -FilePath $app -ArgumentList "/sources(@'sharpmake\main.sharpmake.cs') /debug /generateDebugSolution"

        Write-Host "Generating projects and solutions..."
        Timeout /T 10
    }
}
