#Requires -Version 5.1
Set-StrictMode -Version 'Latest'
$ErrorActionPreference = 'Stop'

$TRAVIS_BRANCH = If ($env:TRAVIS_PULL_REQUEST -eq 'false') { $env:TRAVIS_BRANCH } Else { $env:TRAVIS_PULL_REQUEST_BRANCH }

Function StartProcess([String]$FilePath, [String[]]$ArgumentList='') {
    $Process = New-Object System.Diagnostics.Process
    $Process.StartInfo.FileName = $FilePath
    $Process.StartInfo.Arguments = $ArgumentList
    $Process.StartInfo.UseShellExecute = $false
    $Process.StartInfo.RedirectStandardError = $true
    $Process.StartInfo.RedirectStandardOutput = $true
    $Process.Start() | Out-Null
    $Process.WaitForExit()
    Write-Host $Process.StandardOutput.ReadToEnd().replace("$env:GITHUB_TOKEN", '[SECRET]')
    Write-Host $Process.StandardError.ReadToEnd().replace("$env:GITHUB_TOKEN", '[SECRET]')
    return $Process.ExitCode
}

[string]$version = .\gradlew -s properties | Where { $_ -match '^version: ' } | % { $_ -replace 'version: ', '' }

If ($TRAVIS_BRANCH -eq 'master') {
    [string]$baseVersion = $version.replace('-SNAPSHOT', '')
    Write-Host $baseVersion
    [string]$patchVersion = git tag | Where { $_ -match "$baseVersion" } | % { $_ -replace "$baseVersion.", '' } | Sort-Object | Select-Object -Last 1
    if($patchVersion) {
        ([int]$patchVersion)++
    } else {
        $patchVersion = '0'
    }
    [string]$newVersion = "$baseVersion.$patchVersion"
    (Get-Content 'build.gradle.kts') | Foreach-Object {$_ -replace "[ ]*version[ ]*=[ ]*`"$version`"", "version = `"$newVersion`"" } | Out-File 'build.gradle.kts'
}

./gradlew clean assembleReleaseX86 assembleReleaseX86-64

If ($TRAVIS_BRANCH -eq 'master') {
    StartProcess -FilePath 'git' -ArgumentList 'config', '--global', 'push.default', 'simple'
    StartProcess -FilePath 'git' -ArgumentList 'config', '--global', 'user.name', 'Travis CI'
    StartProcess -FilePath 'git' -ArgumentList 'config', '--global', 'user.email', 'travis@travis-ci.com'
    StartProcess -FilePath 'git' -ArgumentList 'remote', 'add', 'origins', "https://$env:GITHUB_TOKEN@github.com/$env:TRAVIS_REPO_SLUG.git"
    StartProcess -FilePath 'git' -ArgumentList 'add', 'build.gradle.kts'
    StartProcess -FilePath 'git' -ArgumentList 'commit', '-m', "Release $newVersion"

    StartProcess -FilePath 'git' -ArgumentList 'push', 'origins', 'master'
    StartProcess -FilePath 'git' -ArgumentList 'tag', '-f', "$newVersion"
    StartProcess -FilePath 'git' -ArgumentList 'tag', '-f', 'latest'
    StartProcess -FilePath 'git' -ArgumentList 'push', '--tags'
}

If ($TRAVIS_BRANCH -eq 'master' -or $TRAVIS_BRANCH -eq 'develop') {
    ./gradlew publish
}

exit $LastExitCode