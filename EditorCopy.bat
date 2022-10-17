cd..
cd..
xcopy .\AR41Editor\Include\GameObject\*.* .\Client2D\Include\GameObject\ /d /s /y /i
xcopy .\AR41Editor\Include\Scene\*.* .\Client2D\Include\Scene\ /d /s /y /i /exclude:EditorExclude.txt
xcopy .\AR41Editor\Include\UI\*.* .\Client2D\Include\UI\ /d /s /y /i /exclude:EditorExclude.txt
xcopy .\AR41Editor\Include\Setting\*.* .\Client2D\Include\Setting\ /d /s /y /i /exclude:EditorExclude.txt

xcopy .\AR41Editor\Bin\*.* .\Client2D\Bin\ /d /s /y /i /exclude:Exclude.txt

:: Create Component Folder If does not exists
if not exist .\Client2D\Include\GameObject ( mkdir .\Client2D\Include\GameObject )
if not exist .\Client2D\Include\Component ( mkdir .\Client2D\Include\Component )
