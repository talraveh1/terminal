# Windows Terminal background tabs

Milestone 1 built and installed successfully with VS 2026 on 2026-10-23. The background-tab patch is unchanged, and the repository's v145 build setting is restored. Five headless checks of the installed command-line parser passed. Launcher changes and window/input tests remain deferred. Brooke owns the independent review.

## Build and installation evidence

The Release x64 Dev build completed on its first VS 2026 attempt in **12 minutes 15 seconds**, including dependency restore; MSBuild reported **12 minutes 7.80 seconds, 132 warnings and zero errors**. It ran at BelowNormal priority with `/m:6`, `/nr:false`, `CL_MPCount=1` and `VCPKG_MAX_CONCURRENCY=6`. The wrapper and evidence are retained locally in ignored `bg-artifacts/build-vs18.ps1`, `build-vs18-1-result.json`, `build-vs18-1-full.log` and `build-vs18-1.binlog`.

The unsigned artifact is `bg-artifacts/packages/CascadiaPackage_0.0.1.0_x64_Test/CascadiaPackage_0.0.1.0_x64.msix`. It was extracted and registered from `D:\forks\terminal\bg-artifacts\loose\AppxManifest.xml`. The installed identity is **WindowsTerminalDev_0.0.1.0_x64__8wekyb3d8bbwe**, version **0.0.1.0**. Its launcher is `D:\forks\terminal\bg-artifacts\loose\wtd.exe`; its registered alias is `C:\Users\trax7\AppData\Local\Microsoft\WindowsApps\wtd.exe`. Keep the loose directory in place while installed.

The ordinary executable help path can display a message box, so it was not invoked. Instead, a finite `pythonw.exe` process under the Dev package identity activated only `TerminalApp.CommandlineArgs` from the installed `TerminalApp.dll`. The checks confirmed ordinary new-tab parsing, `--background new-tab`, `-w 0 --background`, rejection of `--background split-pane`, and help containing `--background`. Evidence is in `bg-artifacts/check-parser.py`, `parser-result.json` and `parser-help.txt`; the process exited. No Terminal window or shell payload was started. This verifies parser acceptance, not window-focus behavior.

## Earlier build evidence

The earlier VS 2022 attempt failed in the Settings Editor XAML compiler with `WMC9999: Value cannot be null. Parameter name: name`, followed by the packaging error `MSB4181`. Its logs and timing records remain in `bg-artifacts/build-1*` and `bg-artifacts/build-2*`. Those attempts produced no Dev installation. Use VS 2026 for this branch.

## Source and branch

- Checkout: `D:\forks\terminal`, branch `bg-tabs-1.25`.
- Stable base: tag `v1.25.2733.0`, commit `dc8ae365847d41f62d1ebca93815bb00959bc12f`.
- Upstream: [PR #20734](https://github.com/microsoft/terminal/pull/20734), NathanSweet, commit `c3edbcb12f276dfde3b3c0b6679b10b21b3b21ab`.
- Local cherry-pick: `beba5f596787599eb628d65cbf5beb874286581d`, 12 files, 200 insertions and 20 deletions. Cherry-picked with `-x`; no conflicts and no manual source edits. The subsequent documentation commit is separate.
- `git range-diff` shows only the added cherry-pick provenance and an unchanged context line in `TerminalPage.h` that differs between main and stable. A whitespace check with `core.whitespace=cr-at-eol` passes; the ordinary check treats the repository's committed CRLF line endings as trailing whitespace.
- Local tag `bg-tabs-base` records the base for the next rebase. The branch is published to the user fork. Remote `origin` is `https://github.com/talraveh1/terminal.git`; remote `upstream` is `https://github.com/microsoft/terminal.git`. Fetch stable tags from `upstream` and push this branch only to `origin` using `git push origin bg-tabs-1.25`. A future rebase rewrites published history: publish it under a fresh fork branch unless a history-replacing push has separately been authorized.

## Upstream review and local decision

The PR remains OPEN with CHANGES_REQUESTED. Read using `gh pr view --comments`, the reviews and comments REST endpoints, and GraphQL reviewThreads; the latter contained one unresolved thread and no further page.

The [maintainer's review](https://github.com/microsoft/terminal/pull/20734#pullrequestreview-5338384163) questions relying on an agent to add the flag, proposes avoiding `cmd /c`, and raises the project's AI contribution policy. A later discussion questions adding the feature without prior feature-request agreement. Our intended caller is a controlled launcher that would add the option deterministically; these comments do not specify a code correction needed for that use. They do matter to upstream acceptance, which is not assured.

The only [inline request](https://github.com/microsoft/terminal/pull/20734#discussion_r4119857946) is the spelling bot's request for a comma after `Otherwise` in `TerminalPage.cpp`. The issue-comment report also suggests removing unused `ADDREF` from spelling expectations. Neither changes runtime behavior. Apply neither in milestone 0: preserve the PR patch as a recognizable cherry-pick. A later cleanup can carry the punctuation change separately if wanted; there is no requested functional fix to apply here.

Upstream's [AI policy](https://github.com/microsoft/terminal/blob/main/AGENTS.md) permits non-public local experiments but restricts AI-authored public comments. The draft below stays local; it is not an instruction to post it verbatim.

## Required toolchain

This 1.25 build requires **Visual Studio Community 2026 18.11.1** at `C:\Program Files\Microsoft Visual Studio\18\Community`, with the repository's **v145** toolset (MSVC 14.51.36231). Use VS 18 explicitly when VS 2022 is also installed. Keep the repository's v145 dependency triplet unchanged.

Installed prerequisites are Desktop development with C++, WinUI application development, C++ UWP tools, UWP support, MSIX Packaging Tools, .NET Framework 4.8 SDK and the required .NET Framework targeting packs. Windows SDK **10.0.26100.0** and PowerShell 7 are installed. Developer Mode is on; unsigned loose-package registration requires no new signing certificate. Direct installation of an MSIX would require a trusted signature instead.

## Rebase onto the next stable release

Use PowerShell 7. Replace the example new tag with the exact new stable release and independently verify its commit before rebasing. Start with a clean worktree. Keep the old branch until the new binary has passed qualification. These commands are instructions for a future update, not actions performed during preparation.

```powershell
$env:LIFETIME='ATTACHED'; Set-Location D:\forks\terminal
$env:LIFETIME='ATTACHED'; git status --short
$env:LIFETIME='ATTACHED'; git fetch upstream --tags
$env:LIFETIME='ATTACHED'; $bgNewTag = 'v1.NEXT.RELEASE.0'
$env:LIFETIME='ATTACHED'; git rev-parse "$bgNewTag^{commit}"
$env:LIFETIME='ATTACHED'; git branch bg-tabs-before-next-stable bg-tabs-1.25
$env:LIFETIME='ATTACHED'; git rebase --onto $bgNewTag bg-tabs-base bg-tabs-1.25
```

Choose a fresh backup-branch name on later updates. If conflicts occur, preserve the new stable release's surrounding behavior while porting the background flag, command forwarding, non-selected tab layout/startup and show-window suppression together. Stage only resolved files and use `git rebase --continue`; use `git rebase --abort` to return to the original branch. If upstream already includes the feature, compare it and drop the duplicate patch rather than applying it twice. After the rebase succeeds, inspect the diff/range-diff against the saved branch, update this guide's base and run the focused build/tests and later window/input qualification. Only after accepting the result move `bg-tabs-base` to the verified new tag with `git tag -f bg-tabs-base $bgNewTag`. Keep documentation and any later functional adjustments in separate commits. Do not push to microsoft/terminal.

## Build and package

The following reproduces the successful build's restore sequence and MSBuild arguments. An initial build downloads dependencies and can take substantial CPU, disk space and time; warn the user before starting and wait for it to finish. Do not run a stress test or a windowed test as part of this step. Use x64 Release optimization with **Dev branding**; `Configuration=Release` does not mean `WindowsTerminalBranding=Release`. Use fresh log names when preserving an earlier build's evidence.

`tools/OpenConsole.psm1` requires PowerShell 7. `Set-MsBuildDevEnvironment` selects VS with x86/x64 C++ tools, loads its developer shell into the current process, and can download the VSSetup PowerShell module into this checkout. `Invoke-OpenConsoleBuild` restores the solution and global NuGet dependencies, then forwards its arguments to MSBuild. It does not check each restore's exit code before continuing, so inspect restore output as well as the final build result.

```powershell
$env:LIFETIME='ATTACHED'; Set-Location D:\forks\terminal
$env:LIFETIME='ATTACHED'; [Diagnostics.Process]::GetCurrentProcess().PriorityClass = 'BelowNormal'
$env:LIFETIME='ATTACHED'; $env:MSBUILDDISABLENODEREUSE = '1'; $env:CL_MPCount = '1'; $env:VCPKG_MAX_CONCURRENCY = '6'
$env:LIFETIME='ATTACHED'; git submodule update --init --recursive
$env:LIFETIME='ATTACHED'; Import-Module 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\Microsoft.VisualStudio.DevShell.dll'
$env:LIFETIME='ATTACHED'; Enter-VsDevShell -VsInstallPath 'C:\Program Files\Microsoft Visual Studio\18\Community' -SkipAutomaticLocation -DevCmdArguments '-arch=x64 -host_arch=x64'
$env:LIFETIME='ATTACHED'; & .\dep\nuget\nuget.exe restore .\OpenConsole.slnx -NonInteractive -DisableParallelProcessing -MSBuildPath 'C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64'
$env:LIFETIME='ATTACHED'; if ($LASTEXITCODE -ne 0) { throw 'Solution restore failed' }
$env:LIFETIME='ATTACHED'; & .\dep\nuget\nuget.exe restore .\dep\nuget\packages.config -NonInteractive -DisableParallelProcessing -MSBuildPath 'C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64'
$env:LIFETIME='ATTACHED'; if ($LASTEXITCODE -ne 0) { throw 'Global dependency restore failed' }
$env:LIFETIME='ATTACHED'; & 'C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe' .\OpenConsole.slnx '/t:Terminal\CascadiaPackage' '/p:Configuration=Release' '/p:Platform=x64' '/p:WindowsTerminalBranding=Dev' '/p:AppxPackageSigningEnabled=false' '/p:AppxBundle=Never' '/p:AppxSymbolPackageEnabled=false' '/p:GenerateAppxPackageOnBuild=true' '/p:AppxPackageDir=D:\forks\terminal\bg-artifacts\packages\' '/m:6' '/nr:false' '/p:CL_MPCount=1' '/bl:D:\forks\terminal\bg-artifacts\bg-build.binlog' '/verbosity:minimal' '/fl' '/flp:logfile=D:\forks\terminal\bg-artifacts\bg-build.log;verbosity=normal'
$env:LIFETIME='ATTACHED'; if ($LASTEXITCODE -ne 0) { throw 'Terminal build failed; do not install' }
```

The target and unsigned-MSIX-to-loose-layout procedure follow [doc/building.md](doc/building.md). Run the build process at BelowNormal priority and limit project concurrency to six with compiler parallelism set to one; do not run another build concurrently. A solution-wide build can omit `/t:Terminal\CascadiaPackage`, but that also brings in unrelated tools and tests. The PR's `CommandlineTest::TestBackground` suite was not run; the installed parser checks above provide the bounded verification for milestone 1.

## Install side by side

The Dev manifest uses package name **WindowsTerminalDev**, display name **Windows Terminal Dev**, and execution alias **wtd.exe**. Stable remains **Microsoft.WindowsTerminal** with **wt.exe**. Dev also uses its own settings. Do not replace the stable manifest, change the default terminal, or change the stable settings. Existing-window routing is separated by branding: the destination must be an existing **Dev** window, not the installed stable window. A first Dev window must therefore be created in a later authorized window-testing stage; `--background` is not proof of a hidden first-window launch.

Inspect the generated package before registration. Run in the same PowerShell 7 developer environment. The artifact folder below must be new; stop rather than overwrite an existing loose deployment. Keep it on disk while registered.

```powershell
$env:LIFETIME='ATTACHED'; $bgPackages = @(Get-ChildItem D:\forks\terminal\bg-artifacts\packages -Recurse -File -Filter '*.msix')
$env:LIFETIME='ATTACHED'; if ($bgPackages.Count -ne 1) { throw 'Select the one newly built x64 Dev MSIX explicitly' }
$env:LIFETIME='ATTACHED'; $bgLoose = 'D:\forks\terminal\bg-artifacts\loose'
$env:LIFETIME='ATTACHED'; if (Test-Path -LiteralPath $bgLoose) { throw 'Loose deployment directory already exists; choose a fresh directory' }
$env:LIFETIME='ATTACHED'; & 'C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x64\makeappx.exe' unpack /p $bgPackages[0].FullName /d $bgLoose
$env:LIFETIME='ATTACHED'; if ($LASTEXITCODE -ne 0) { throw 'MSIX extraction failed' }
$env:LIFETIME='ATTACHED'; $bgManifest = Join-Path $bgLoose 'AppxManifest.xml'; [xml]$bgXml = Get-Content -LiteralPath $bgManifest
$env:LIFETIME='ATTACHED'; if ($bgXml.Package.Identity.Name -ne 'WindowsTerminalDev') { throw 'Refusing a non-Dev package' }
$env:LIFETIME='ATTACHED'; $bgXml.Package.Identity; $bgXml.Package.Dependencies; Select-String -LiteralPath $bgManifest -Pattern 'wtd.exe'
$env:LIFETIME='ATTACHED'; if (Get-AppxPackage -Name WindowsTerminalDev) { throw 'A Dev package already exists; preserve its data and identify its owner first' }
$env:LIFETIME='ATTACHED'; Add-AppxPackage -Register $bgManifest -ErrorAction Stop
$env:LIFETIME='ATTACHED'; Get-AppxPackage -Name WindowsTerminalDev | Select-Object Name,Version,PackageFullName,InstallLocation
```

If an existing Dev package is listed, stop to preserve its settings and identify its owner before replacing it; do not force shutdown or uninstall it blindly. Dependencies must satisfy the generated manifest. This machine currently has x64 `Microsoft.VCLibs.140.00` 14.0.33519.0, `Microsoft.VCLibs.140.00.UWPDesktop` 14.0.33728.0 and `Microsoft.UI.Xaml.2.8` 8.2511.26001.0. If registration reports a missing/newer framework, use the matching signed x64 dependency from the build's `Dependencies\x64` output via `Add-AppxPackage -Path <exact dependency file>` before retrying; don't substitute another architecture. Registration itself does not launch Terminal.

The selected route uses an unsigned MSIX only as a transport to a loose layout. **Do not pass that unsigned MSIX directly to `Add-AppxPackage -Path`.** Direct MSIX installation is a different route requiring signing and machine trust; certificate installation and any Developer Mode changes belong to the user. The present route needs neither a new certificate nor a machine-setting change.

## Later launcher and window qualification

Reuse the existing recorder and workers from `D:\agents`, branch `cli/wt-tab-quiet`, commit `31682357`: `Shared/skills/cli/references/wt-quiet-tab.md`, `Shared/skills/cli/scripts/probe-terminal-tabs.py`, `probe-terminal-select.ps1` and `probe-terminal-helper.ps1`. They were read, not run or changed in milestone 0. Preserve the recorded baseline results.

The current recorder discovers `wt` through PATH and creates ordinary new-tab commands; running it unchanged would test stable again. At the later integration milestone, adapt that same recorder to accept the explicit Dev `wtd.exe` path and put global `--background` before `new-tab` for the patched cases. Retain the idle gate, physical-input cancellation, foreground/focus event recording, exact 240-character input comparison, payload-start evidence, minimized-state check and awaited cleanup. Require one existing Dev window in both minimized and behind-window conditions, with no selected-tab change, restore, focus theft or diverted keys. The patch accepts only new-tab actions with this flag; other actions are rejected. Its background startup layout includes a 30-second retry limit, so prove the payload actually starts while minimized rather than just proving a tab appears.

Every test-created window must be offscreen, absent from the taskbar and unable to take focus; the existing harness must be checked against all of these constraints before use. If it cannot meet them, obtain the user's explicit permission before running it. No visible window or probe was started during preparation.

## Remove the development installation

Close only the Dev window and its runs first, after preserving any work. Uninstall the exact Dev identity for the current user; never use a wildcard matching stable. Removal also removes that package's settings/data, so copy any Dev data the user wants to retain first.

```powershell
$env:LIFETIME='ATTACHED'; $bgDev = @(Get-AppxPackage -Name WindowsTerminalDev)
$env:LIFETIME='ATTACHED'; $bgDev | Select-Object Name,PackageFullName,InstallLocation
$env:LIFETIME='ATTACHED'; if ($bgDev.Count -ne 1 -or $bgDev[0].Name -ne 'WindowsTerminalDev') { throw 'Expected exactly one Dev package; inspect before removal' }
$env:LIFETIME='ATTACHED'; Remove-AppxPackage -Package $bgDev[0].PackageFullName -ErrorAction Stop
$env:LIFETIME='ATTACHED'; Get-AppxPackage -Name WindowsTerminalDev
```

After confirming removal, the user may delete the exact loose-layout/artifact directory under `D:\forks\terminal\bg-artifacts`; retain the source branch and binary log if wanted. No certificate needs removal for this route, and Developer Mode was pre-existing, so do not turn it off as cleanup. The stable package and its settings are independent.

## Private draft comment, not posted

Thank you for considering this feature. We launch agent runs through a controlled launcher and would like their tabs to open in an existing background Terminal window without stealing focus, restoring the window, or interrupting typing. Our launcher can reliably add `--background`, so this use does not depend on an agent remembering to include it. Would you consider merging #20734 once the contribution-policy concerns and remaining review feedback are resolved?
