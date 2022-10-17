cd..
cd..

xcopy .\AR41Engine\Include\*.h .\Engine\Include\ /d /s /y /i
xcopy .\AR41Engine\Include\*.inl .\Engine\Include\ /d /s /y /i
xcopy .\AR41Engine\Include\*.hpp .\Engine\Include\ /d /s /y /i
xcopy .\AR41Engine\Bin\*.* .\Engine\Bin\ /d /s /y /i /exclude:Exclude.txt
xcopy .\AR41Engine\Bin\*.* .\AR41Editor\Bin\ /d /s /y /i /exclude:Exclude.txt
xcopy .\AR41Engine\Bin\*.* .\Client2D\Bin\ /d /s /y /i /exclude:Exclude.txt

:: Create Component Folder If does not exists
if not exist .\AR41Editor\Include\GameObject ( mkdir .\AR41Editor\Include\GameObject )
if not exist .\AR41Editor\Include\Component ( mkdir .\AR41Editor\Include\Component )
if not exist .\AR41Editor\Include\Animation2D ( mkdir .\AR41Editor\Include\Animation2D )
if not exist .\AR41Editor\Bin\Scene ( mkdir .\AR41Editor\Bin\Scene )
if not exist .\AR41Editor\Bin\Animation2D ( mkdir .\AR41Editor\Bin\Animation2D )