#!/bin/sh
#\
exec wish "$0"

set qmc(status) "Status: waiting for input"
set qmc(output) ""

#proc run_calc { action } {
proc run_calc { } {
  global qmc

#   set qmc(status) "Status: calculating..."

    if {$qmc(opt_browser) > 0} {
      exec "qmc" -G $qmc(opt_time) $qmc(opt_av_ast) -s $qmc(equation) > ~/.qmc_gui_tmp
      exec "konqueror" "~/.qmc_gui_tmp" &
    } else {
      exec "qmc" $qmc(opt_table) $qmc(opt_time) $qmc(opt_av_ast) -s $qmc(equation) > ~/.qmc_gui_tmp
      set fp [open "~/.qmc_gui_tmp" r]
      set qmc(output) [read $fp]
      close $fp
      .output_frame.output_text insert 1.0 "$qmc(output)\n"
    }

#  set qmc(status) "Status: waiting for input"
}

proc run_about  { } {
  global qmc

   exec "qmc" "--version" > ~/.qmc_gui_tmp
   set fp [open "~/.qmc_gui_tmp" r]
   set qmc(output) [read $fp]
   close $fp
  .output_frame.output_text insert 1.0 "$qmc(output)\n"

}

proc run_desc  { } {
  global qmc

   exec "qmc" "-d" | "head" "-n" "9" > ~/.qmc_gui_tmp
   set fp [open "~/.qmc_gui_tmp" r]
   set qmc(output) [read $fp]
   close $fp
  .output_frame.output_text insert 1.0 "$qmc(output)\n"

}

#frames
frame .menu_frame -borderwidth 1 -relief raised
frame .equation_frame -borderwidth 1 -relief raised
frame .options_frame -borderwidth 1 -relief raised
frame .output_frame -borderwidth 1 -relief raised
frame .action_frame -borderwidth 1 -relief raised
frame .status_frame -borderwidth 1 -relief raised

#pack frames
pack .menu_frame    -side top -fill x
pack .equation_frame -side top -fill x
pack .options_frame -side top -fill x
pack .output_frame -side top -fill x
pack .action_frame   -side top -fill x
pack .status_frame   -side top -fill x

button .calc_button -text Calculate  -command { run_calc}
button .quit_button -text Quit -command exit

bind .calc_button <Alt-1> run_calc

pack .calc_button -in .action_frame -side left -fill x -expand yes
pack .quit_button -in .action_frame -side right -fill x -expand yes


menubutton .file_menu -text File -menu .file_menu.file_menu_dropdown

menu .file_menu.file_menu_dropdown
.file_menu.file_menu_dropdown add command -label "Syntax Help" -command run_desc
.file_menu.file_menu_dropdown add command -label "About qmc" -command run_about

.file_menu.file_menu_dropdown add separator
.file_menu.file_menu_dropdown add command -label "Quit" -command exit

pack .file_menu -in .menu_frame -side left

#equation input
label .equation_frame.equation_label -text "Q="
entry .equation_frame.equation_entry_field -textvariable qmc(equation)

pack .equation_frame.equation_label -side left
pack .equation_frame.equation_entry_field -side right -fill x -expand yes


checkbutton .opt_table_box -text "output as table" -variable qmc(opt_table) -onvalue "-g" -offvalue "-D"
checkbutton .opt_time_box -text "enable timer" -variable qmc(opt_time) -onvalue "-T" -offvalue "-D"
checkbutton .opt_av_ast_box  -text "avoid astericks" -variable qmc(opt_av_ast) -onvalue "-a" -offvalue "-D"
checkbutton .opt_browser_box -text "show result in konqueror" -variable qmc(opt_browser)

grid .opt_table_box  -in .options_frame -row 0 -column 0 -sticky sw
grid .opt_time_box  -in .options_frame -row 0 -column 1 -sticky w
grid .opt_av_ast_box  -in .options_frame -row 1 -column 0 -sticky sw
grid .opt_browser_box  -in .options_frame -row 1 -column 1 -sticky w


text .output_frame.output_text -width 40 -height 20 -relief sunken -bd 2 -yscrollcommand ".output_frame.scroll set"
scrollbar .output_frame.scroll -command ".output_frame.output_text yview"


pack .output_frame.output_text -side left -fill x -expand yes
pack .output_frame.scroll -side right -fill y


label .status_frame.status_label -textvariable qmc(status)
pack .status_frame.status_label -side left -fill x -fill y


#change title
wm title . "qmc-gui"

