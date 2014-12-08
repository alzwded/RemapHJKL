; IMPORTANT INFO ABOUT GETTING STARTED: Lines that start with a
; semicolon, such as this one, are comments.  They are not executed.

; This script has a special filename and path because it is automatically
; launched when you run the program directly.  Also, any text file whose
; name ends in .ahk is associated with the program, which means that it
; can be launched simply by double-clicking it.  You can have as many .ahk
; files as you want, located in any folder.  You can also run more than
; one ahk file simultaneously and each will get its own tray icon.

; SAMPLE HOTKEYS: Below are two sample hotkeys.  The first is Win+Z and it
; launches a web site in the default browser.  The second is Control+Alt+N
; and it launches a new Notepad window (or activates an existing one).  To
; try out these hotkeys, run AutoHotkey again, which will load this file.

#z::Run www.autohotkey.com

MyState := false

ListVars

^!3::
MyState := !MyState
return

*h::
if (MyState) {
    Send {Blind}{Left down}
} else {
    Send {Blind}{h down}
}
return

*h up::
if (MyState) {
    Send {Blind}{Left up}
} else {
    Send {Blind}{h up}
}
return

*j::
if (MyState) {
    Send {Blind}{Down down}
} else {
    Send {Blind}{j down}
}
return

*j up::
if (MyState) {
    Send {Blind}{Down up}
} else {
    Send {Blind}{j up}
}
return

*k::
if (MyState) {
    Send {Blind}{Up down}
} else {
    Send {Blind}{k down}
}
return

*k up::
if (MyState) {
    Send {Blind}{Up up}
} else {
    Send {Blind}{k up}
}
return

*l::
if (MyState) {
    Send {Blind}{Right down}
} else {
    Send {Blind}{l down}
}
return

*l up::
if (MyState) {
    Send {Blind}{Right up}
} else {
    Send {Blind}{l up}
}
return

; Note: From now on whenever you run AutoHotkey directly, this script
; will be loaded.  So feel free to customize it to suit your needs.

; Please read the QUICK-START TUTORIAL near the top of the help file.
; It explains how to perform common automation tasks such as sending
; keystrokes and mouse clicks.  It also explains more about hotkeys.

