BEGIN { print "" }

#- [Basic Programming Concepts](#basic-programming-concepts)
#   + [Event Driven Programming](#event-driven-programming)
#   + [Three Runtime Modes of MiniGUI](#three-runtime-modes-of-minigui)
#- [Hello, world!](#hello-world)
#   + [Header Files](#header-files)
#   + [Entrance of the Program](#entrance-of-the-program)
#   + [Joining a Layer in MiniGUI-Processes](#joining-a-layer-in-minigui-processes)
#   + [Creating and Showing Main Window](#creating-and-showing-main-window)
#   + [Window Procedure Function](#window-procedure-function)
#   + [Screen Output](#screen-output)
#   + [Exit of the Program](#exit-of-the-program)
#- [Compiling, Linking and Running](#compiling-linking-and-running)
#- [Writing Autotools Scripts for MiniGUI Application](#writing-autotools-scripts-for-minigui-application)

function get_anchor(title)
{
    gsub(/-/, "_", title)
    gsub(/ /, "TMD", title)
    gsub(/\W/, "", title)
    gsub(/TMD/, "-", title)
    anchor = tolower(title)
    return anchor
}

{
    if (match ($0, /^(#+) (.*)/, fields)) {
        level = fields[1]
        title = fields[2]

        anchor = get_anchor(title)
        if (length(anchor) == 0) {
            print "BAD TITLE: " title
            next
        }

        switch (length(level)) {
        case 1:
            break;
        case 2:
            print "- [" title "](#" anchor ")"
            break;
        case 3:
            print "   + [" title "](#" anchor ")"
            break;
        case 4:
            print "      * [" title "](#" anchor ")"
            break;
        case 5:
            print "         - [" title "](#" anchor ")"
            break;
        default:
            break;
        }
    }
}

END { print "" }
