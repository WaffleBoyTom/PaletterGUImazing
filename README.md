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

# TODO
## Baller library
- [x] BallerTask
- [x] QuantizeTask (now GeneratePaletteTask)
- [x] RemapTask
- [ ] ImageProcessor as task manager
## Logging
- [x] SickLogger
- [x] Logger singleton class with Logger::tellBoss and connect to LogViewer, call this from everywhere
- [x] Some sort of progress bar to show the user that we are cooking. Some image or something.
## UI
- [x] Fix ImageViewer hierarchy
- [x] Fix layout issues
- [x] Sick File Line Edit for all file choosers and writers !
## GPU
- [x] Use to apply palette to second image
- [ ] Use to run K-Means for palette generation
- [x] Metal support
## Features
- [x] Application method
## Export
- [x] JSON palette string
- [ ] The image with the palette applied
- [x] What do we do with SickViewer ? That was meant to be a base class for image_viewer because right now we have weird branching...
# Release
- [ ] Make a release build.
- [ ] Test.
## Bugs
- [x] Clicking Process Image to generate Palette seems to put focus in the left Sick File Line Edit like wtf >?!
- [x] "Apply Palette" is still applying to the filtered image
- [x] Logger scroll does not jump to bottom properly.
- [ ] File path not populated on DnD.
- [ ] Inspector doesn't persist across viewer.
- [ ] Resolution text does not repaint on image load.
- [ ] Inspector does not work with circles.
- [ ] Resolution text should always be visible and aligned with the image.
- [ ] Use SickLogSeverity::Metal.
# creedentials
IF THIS DOES NOT COMPILE TOM YOU IDIOT
OPEN QTCREATOR AND LOAD THE PROJECT BY CLICKING THE CMAKELIST FILE
QTCREATOR WILL JUST COMPILE THAT SHIT FOR YOU
WE HATE GUIS BUT DAMN THIS SHIT DID THE HOLD ME NOW
CUZ I WAS 6 FEET FROM THE EDGE AND I WAS THINKING
MAYBE SIX FEET AINT SO FAR DOWN
...
