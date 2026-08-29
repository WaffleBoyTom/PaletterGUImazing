# PaletterGUImazing
Palettize your favorite image and apply that palette to another image, or export that palette to use it for something else 
! All of that inside a GUI !!!

To build in `PaletterGUImazing`:

```bash
cmake -H. -Bbuild
cmake --build .
```

# oh yeah
Shabbu Dabbu : Paletter GUImazing is the work of Ethan Da Broski and Tiny Tom !

# days since Ethan's commits gave Tom an aneurysm: 0

# this is what still needs to be done
## Nerd
- [x] K-Means in CUDA
- [x] K-Means in Metal.
## Logging
- [ ] "CUDA" and "Metal" aren't severities. Add a separate "tag" string argument for context. Color could be specified in a map. The severity is the fallback color.
- [ ] BUG: Metal logs do not have a severity.
## Async
- [ ] Clean up use of QtConcurrent since there is some duplication going on. Could add a "busy handler" for the cursor.
## UI
- [ ] Source pane and destination pane should definitely share more functionality...
- [ ] Add an "Export Image" button for saving the quantized image.
- [x] BUG: File path not populated on DnD.
- [ ] BUG: Inspector doesn't persist across viewer.
- [x] BUG: Resolution text does not repaint on image load.
- [ ] BUG: Inspector does not work with circles.
- [ ] BUG: Resolution text should always be visible and aligned with the image.
# Release
- [ ] Bundle Qt.
- [ ] Bundle Metal.
- [ ] Make a release build.
- [ ] Test.

# creedentials
IF THIS DOES NOT COMPILE TOM YOU IDIOT
OPEN QTCREATOR AND LOAD THE PROJECT BY CLICKING THE CMAKELIST FILE
QTCREATOR WILL JUST COMPILE THAT SHIT FOR YOU
WE HATE GUIS BUT DAMN THIS SHIT DID THE HOLD ME NOW
CUZ I WAS 6 FEET FROM THE EDGE AND I WAS THINKING
MAYBE SIX FEET AINT SO FAR DOWN
...
