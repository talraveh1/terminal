# Windows Terminal background tabs

Milestone 1 stopped after two build attempts on 2026-10-23; no installable Terminal package was produced. All seven requested VS components are installed. No Dev package was registered, no Terminal window was opened, and the agents launcher and stable Terminal settings were unchanged. Brooke owns the independent review. The milestone 0 inventory below is historical; build and installation commands remain instructions, not evidence of a successful installation.

## Milestone 1 build result

The checkout/remotes guide update was pushed to the user's fork as `732eb9ed0`. The build ran in PowerShell 7 using the VS 2022 developer environment, the two NuGet restores from `Invoke-OpenConsoleBuild`, and MSBuild on `OpenConsole.slnx` targeting `Terminal\CascadiaPackage`. Both attempts ran at BelowNormal priority with `/m:6`, `/nr:false`, `/p:CL_MPCount=1`, Release x64, Dev branding and unsigned MSIX packaging. Sampled compiler/MSBuild processes were BelowNormal. No simultaneous second build was started, and no build process remained afterward.

Attempt 1 took **97.568 seconds** including restore. Dependency setup failed because `dep/vcpkg-overlay-triplets/x64-windows-static.cmake` pinned `VCPKG_PLATFORM_TOOLSET v145`, unavailable in VS 2022. The single-line setup correction changes that x64 triplet to **v143**; the PR's code is unchanged. Attempt 2 took **355.818 seconds** including cached restore (**350.53 seconds** reported by MSBuild). Dependency builds succeeded, but the Settings Editor XAML compilation failed:

```text
C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\XamlCompiler\Microsoft.Windows.UI.Xaml.Common.targets(354,5):
Xaml Internal Error error WMC9999: Value cannot be null.
Parameter name: name
Project: src\cascadia\TerminalSettingsEditor\Microsoft.Terminal.Settings.Editor.vcxproj
Target: MarkupCompilePass1
```

The subsequent `Microsoft.DesktopBridge.targets(499,5)` error `MSB4181: The "MSBuild" task returned false but did not log an error` is a consequence, not the first failure. Final MSBuild totals were **5 warnings, 2 errors**. The cause of the XAML compiler's null value is not established. The two-attempt limit was reached; no third build or installation was attempted. Completing this build with the current setup may require further toolchain investigation.

Local evidence under `D:\forks\terminal\bg-artifacts`: `build-1.binlog`, `build-2.binlog`, `build-1-result.json`, `build-2-result.json`, and `build-2-full.log` (diagnostic text replayed from the second binary log). The `.log` PowerShell transcripts record wrapper output only; use the `.binlog` files for authoritative build output. Build artifacts and logs are excluded locally through `.git/info/exclude`, not committed. `build.ps1` retains the exact second-attempt commands. The Dev executable/alias and `--background` acceptance could not be verified because no package was built. The ordinary `--help` path calls `MessageBoxW`, so it must not be used for a no-window check. A parser-only check would be needed after a successful build.

The installed stable package remained `Microsoft.WindowsTerminal_1.25.2733.0_x64__8wekyb3d8bbwe`. Its settings SHA-256 before and after this work was `F9FD2A0B6D77F497D6B149C0B0E01DA5AC51888634EA2DEC9109185F9C459986`.

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

## Historical milestone 0 toolchain inventory and user steps

Read-only evidence: `vswhere -all -products '*' -format json`, instance `e6e6f067`, plus the installer's `state.json`, `state.packages.json` and `catalog.json` under `C:\ProgramData\Microsoft\VisualStudio\Packages\_Instances\e6e6f067`. Catalog English labels below match this installed VS release. The inventory was also compared with this checkout's `.vsconfig`; no installer was launched.

Present: Visual Studio Community 2022 **17.14.42** (`17.14.37806.7`); Desktop development with C++ (`Microsoft.VisualStudio.Workload.NativeDesktop`); MSVC v143 x64/x86 (`Microsoft.VisualStudio.Component.VC.Tools.x86.x64`, compiler directory `14.44.35207`); Windows 11 SDK **10.0.26100.0** (`Microsoft.VisualStudio.Component.Windows11SDK.26100`); MSBuild; NuGet; PowerShell **7.6.7**. Standalone .NET SDKs **10.0.401/10.0.402** and .NET 8 runtime/Windows Desktop runtime **8.0.32** are present, although the VS component records for .NET SDK and .NET 8 runtime are absent.

1. Open Visual Studio Installer yourself, choose **Modify** on Community 2022, and select **WinUI application development** (`Microsoft.VisualStudio.Workload.Universal`). This is the current installer label for the workload called Universal Windows Platform Development in Terminal's README.
2. In its installation details / Individual components, select **Universal Windows Platform tools** (`Microsoft.VisualStudio.ComponentGroup.UWP.Support`), **C++ (v143) Universal Windows Platform tools** (`Microsoft.VisualStudio.ComponentGroup.UWP.VC`) and **MSIX Packaging Tools** (`Microsoft.VisualStudio.ComponentGroup.MSIX.Packaging`). All are absent. The installer resolves their dependencies, including ARM tool entries its UWP group requires even when our target is x64. `Microsoft.DesktopBridge.props` and the VCLibs extension SDK are currently absent on disk.
3. Include **.NET Framework 4.8 SDK** (`Microsoft.Net.Component.4.8.SDK`) and **.NET Framework 4.7.2 targeting pack** (`Microsoft.Net.Component.4.7.2.TargetingPack`), both absent. The latter is used by solution test projects; **.NET Framework 4.8 targeting pack** (`Microsoft.Net.Component.4.8.TargetingPack`) is also absent and requested by `.vsconfig`. Install it with the workload configuration. No .NET Framework reference-assembly directory exists for these targets today.
4. Keep Windows SDK **26100** selected. No required Windows SDK is missing: both native and packaging project properties target 26100. `.vsconfig` still lists SDK **22621**, which is absent but is not the SDK this source actually targets; importing that file verbatim would install it unnecessarily for this build.
5. Developer Mode is already enabled: `HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\AppModelUnlock\AllowDevelopmentWithoutDevLicense=1`. Leave it as it is. The chosen loose-package registration below requires Developer Mode and requires no signing certificate. Installing an MSIX directly instead requires a valid signature trusted by the machine, even with Developer Mode enabled; that alternative would require the user to create/trust a certificate whose subject matches the manifest publisher. No certificate was created, trusted or selected here. See [Microsoft's deployment guidance](https://learn.microsoft.com/en-us/windows/apps/dev-tools/winapp-cli/security#developer-mode).

The following is the **complete 30-entry difference against `.vsconfig`**, not a claim that all 30 are independent requirements for an x64 Terminal package. IDE tools, managed templates and other architectures account for much of the difference. The three .NET runtime/SDK facts above distinguish VS component registration from software actually available on the machine.

| Missing VS component ID                                                   | Installed-catalog label / relevance                        |
| :------------------------------------------------------------------------ | :--------------------------------------------------------- |
| Microsoft.VisualStudio.Component.SQL.LocalDB.Runtime                      | SQL Server Express 2019 LocalDB; IDE/config extra          |
| Microsoft.VisualStudio.Component.SQL.CLR                                  | CLR data types for SQL Server; IDE/config extra            |
| Microsoft.Net.Component.4.8.SDK                                           | .NET Framework 4.8 SDK                                     |
| Microsoft.Net.Component.4.7.2.TargetingPack                               | .NET Framework 4.7.2 targeting pack; solution tests        |
| Microsoft.NetCore.Component.Runtime.8.0                                   | .NET 8 runtime; standalone 8.0.32 already present          |
| Microsoft.NetCore.Component.SDK                                           | .NET SDK; standalone SDK 10 already present                |
| Microsoft.VisualStudio.Component.AppInsights.Tools                        | Developer Analytics tools; IDE/config extra                |
| Microsoft.Net.Component.4.8.TargetingPack                                 | .NET Framework 4.8 targeting pack                          |
| Microsoft.VisualStudio.Component.DiagnosticTools                          | .NET profiling tools; IDE/config extra                     |
| Microsoft.VisualStudio.Component.ClassDesigner                            | Class Designer; IDE/config extra                           |
| Microsoft.VisualStudio.Component.GraphDocument                            | DGML editor; IDE/config extra                              |
| Microsoft.VisualStudio.Component.CodeMap                                  | Code Map; IDE/config extra                                 |
| Microsoft.VisualStudio.Component.Windows11SDK.22621                       | Windows SDK 22621; actual source uses installed 26100      |
| Microsoft.VisualStudio.ComponentGroup.MSIX.Packaging                      | MSIX Packaging Tools                                       |
| Microsoft.VisualStudio.ComponentGroup.WindowsAppSDK.Cs                    | Windows App SDK C# Templates                               |
| Microsoft.ComponentGroup.Blend                                            | Blend for Visual Studio                                    |
| Microsoft.VisualStudio.ComponentGroup.ArchitectureTools.Native            | Architecture tools for C++                                 |
| Microsoft.Component.NetFX.Native                                          | .NET Native                                                |
| Microsoft.VisualStudio.Component.Graphics                                 | Image and 3D model editors                                 |
| Microsoft.VisualStudio.ComponentGroup.UWP.Xamarin                         | Universal Windows Platform tools for Xamarin               |
| Microsoft.VisualStudio.ComponentGroup.UWP.Support                         | Universal Windows Platform tools                           |
| Microsoft.VisualStudio.Component.VC.Tools.ARM64EC                         | MSVC v143 ARM64EC build tools                              |
| Microsoft.VisualStudio.Component.UWP.VC.ARM64EC                           | C++ UWP v143 ARM64EC support                               |
| Microsoft.VisualStudio.Component.VC.Tools.ARM64                           | MSVC v143 ARM64/ARM64EC build tools                        |
| Microsoft.VisualStudio.Component.UWP.VC.ARM64                             | C++ UWP v143 ARM64/ARM64EC support                         |
| Microsoft.VisualStudio.Component.VC.Tools.ARM                             | MSVC v143 ARM build tools                                  |
| Microsoft.VisualStudio.ComponentGroup.UWP.VC                              | C++ (v143) Universal Windows Platform tools                |
| Microsoft.VisualStudio.ComponentGroup.WindowsAppDevelopment.Prerequisites | .NET WinUI app development prerequisites                   |
| Microsoft.VisualStudio.ComponentGroup.UWP.NetCoreAndStandard              | .NET Native and .NET Standard                              |
| Microsoft.VisualStudio.Workload.Universal                                 | WinUI application development                              |

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

The following is a source-derived procedure, not a successful build transcript. The initial build downloads dependencies and can take substantial CPU, disk space and time; warn the user before starting and wait for it to finish. Do not run a stress test or a windowed test as part of this step. Use x64 Release optimization with **Dev branding**; `Configuration=Release` does not mean `WindowsTerminalBranding=Release`.

`tools/OpenConsole.psm1` requires PowerShell 7. `Set-MsBuildDevEnvironment` selects VS with x86/x64 C++ tools, loads its developer shell into the current process, and can download the VSSetup PowerShell module into this checkout. `Invoke-OpenConsoleBuild` restores the solution and global NuGet dependencies, then forwards its arguments to MSBuild. It does not check each restore's exit code before continuing, so inspect restore output as well as the final build result.

```powershell
$env:LIFETIME='ATTACHED'; Set-Location D:\forks\terminal
$env:LIFETIME='ATTACHED'; git submodule update --init --recursive
$env:LIFETIME='ATTACHED'; Import-Module .\tools\OpenConsole.psm1
$env:LIFETIME='ATTACHED'; Set-MsBuildDevEnvironment
$env:LIFETIME='ATTACHED'; Invoke-OpenConsoleBuild '/t:Terminal\CascadiaPackage' '/p:Configuration=Release' '/p:Platform=x64' '/p:WindowsTerminalBranding=Dev' '/p:AppxPackageSigningEnabled=false' '/p:AppxBundle=Never' '/p:AppxSymbolPackageEnabled=false' '/p:GenerateAppxPackageOnBuild=true' '/p:AppxPackageDir=D:\forks\terminal\bg-artifacts\packages\' '/m:6' '/nr:false' '/p:CL_MPCount=1' '/bl:D:\forks\terminal\bg-build.binlog'
$env:LIFETIME='ATTACHED'; if ($LASTEXITCODE -ne 0) { throw 'Terminal build failed; do not install' }
```

The target and unsigned-MSIX-to-loose-layout procedure follow [doc/building.md](doc/building.md). Run the build process at BelowNormal priority and limit project concurrency to six with compiler parallelism set to one; do not run other build/test floods concurrently. A solution-wide build can omit `/t:Terminal\CascadiaPackage`, but that also brings in unrelated tools and tests. No C++ compilation or test execution occurred in milestone 0, including the PR's `CommandlineTest::TestBackground` cases.

## Install side by side after build approval

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
