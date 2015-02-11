# qtMDI
**************

Qt5 based module for MachineKit, which will run linear programs using MDI commands only

Useful for the huge linear files produced by CAM, which will lock up Axis.

These files are without loops or subroutines and generally consist of G00 and G01
moves only, with curves being made of series of short straight lines

qtMDI is launched from the hal file as a userspace module and sits alongside Axis

All machine control, homing etc is done through Axis as usual

The file to be run, is loaded into qtMDI instead of Axis and run from there

qtMDI reads in the first 25K of the selected file and then loads the file in 5K
increments as it is nearing the end of the currently loaded section

Moves are read line by line from the browser and output via MDI commands

Tested on a 11MB ngc file of nearly 500,000 lines

*********************************************************************************

To build:

Install Qt5  (http://www.qt.io/download-open-source/#)

and set the pathspecs in your ~/.bashrc file along these lines

    export QT5DIR=/YOURPATH/Qt/5.4/gcc
    export QTCREATEDIR=/YOURPATH/Qt/Tools/QtCreator

    PATH=/sbin:/usr/sbin:/bin:/usr/bin:/usr/X11R6/bin:/usr/local/bin:/usr/realtime/bin:/usr/src/linuxcnc-dev-rtai/bin:$QT5DIR/bin:$QTCREATE

    INCLUDE_PATH=/usr/include:/usr/src/linuxcnc-dev-rtai/include:/usr/realtime/include:/usr/include/linuxcnc:$QT5DIR/include:$INCLUDE_PATH

    ENV=$HOME/.bashrc
    USERNAME="YOURUSERNAME"

    export USERNAME ENV PATH INCLUDE_PATH

    export QTVERSION=5.4

    export LD_LIBRARY_PATH=/usr/local/lib:/usr/lib:/usr/src/linuxcnc-dev-rtai/lib:/usr/realtime/lib:$QT5DIR/lib:$QTCREATEDIR/lib:$LD_LIBRAR

    export QT_INSTALL_PLUGINS=$QTDIR/plugins


Go to the git /src directory and open qtMDI.pro

Edit the Qt5 and library paths to match your location for Qt5 headers and libs
(a full copy of all the machinekit headers is included in the local /include
dir, as those without a RIP environment will not have them all)

From git /src directory run

'qmake -makefile'

'make'

'strip qtMDI'

Then copy or symlink the qtMDI executable to a directory in $PATH

At the end of the config main hal file add the line

'loadusr -W qtMDI'

************************************************************************************

To use:

Start the config and the qtMDI window will appear with Axis
Size and place the qtMDI window and its state and position will be
saved and restored at subsequent launches.( Axis won't though!)

Take out of Estop, put Machine On, Home and Touch Off in Axis

---Thereafter all actions whilst running are from qtMDI -------------

When ready to run, go to qtMDI and select File > Open from the menu or press the FileOpen icon

Select the file required (it will be remembered under File > Recent files)

Run by clicking the Run icon or pressing 'R', the plot will be rendered in Axis 

Pause will wait, Step will move one line if in Pause, pressing Stop [Esc] or clicking Run again will abort.

Pause and select the line above the new place to start with the mouse and use right mouse button
for a context menu and select 'Run from Here'

(No account is taken of feed/speed/spindle settings in 'Run From' which remain as they were when paused.
