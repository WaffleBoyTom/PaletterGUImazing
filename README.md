# PaletterGUImazing
Palettize your favorite image and apply that palette to another image, or export that palette to use it for something else 
! All of that inside a GUI !!!

To build in `PaletterGUImazing` and run:

```bash
cmake -H. -Bbuild
cmake --build .
```

# oh yeah
Shabbu Dabbu : Paletter GUImazing is the work of Ethan Da Broski and Tiny Tom !

# we need to do this
- [ ] Worker threads
    - [x] BallerTask
    - [x] QuantizeTask
    - [x] RemapTask
    - [ ] ImageProcessor as task manager
- [x] Logging
    - [x] SickLogger
    - [x] Logger singleton class with Logger::tellBoss and connect to LogViewer, call this from everywhere
    - [ ] It should be possible the hide the log viewer and/or to set a verbosity level 
    - [ ] Some sort of progress bar to show the user that we are cooking. Some image or something.
- [ ] UI
    - [ ] Fix ImageViewer hierarchy
    - [ ] Fix layout issues
    - [x] Sick File Line Edit for all file choosers and writers !
- [ ] GPU
    - [x] Use to apply palette to second image
    - [ ] Use to run K-Means for palette generation
    - [ ] Metal support
    - [ ] Compute backend abstraction layer
- [ ] Application method
- [ ] Color sorting methods
- [ ] Export
    - [x] JSON palette string
    - [ ] The image with the palette applied
- [x] What do we do with SickViewer ? That was meant to be a base class for image_viewer because right now we have weird branching...
# BUGS
- [x] Clicking Process Image to generate Palette seems to put focus in the left Sick File Line Edit like wtf >?!
- [x] "Apply Palette" is still applying to the filtered image
- [x] Logger scroll does not jump to bottom properly.
# creedentials
IF THIS DOES NOT COMPILE TOM YOU IDIOT
OPEN QTCREATOR AND LOAD THE PROJECT BY CLICKING THE CMAKELIST FILE
QTCREATOR WILL JUST COMPILE THAT SHIT FOR YOU
WE HATE GUIS BUT DAMN THIS SHIT DID THE HOLD ME NOW
CUZ I WAS 6 FEET FROM THE EDGE AND I WAS THINKING
MAYBE SIX FEET AINT SO FAR DOWN
...
