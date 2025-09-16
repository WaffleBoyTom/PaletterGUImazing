# PaletterGUImazing
Palettize your favorite image and apply that palette to another image, or export that palette to use it for something else ! All of that inside a GUI !!!
Shabu Dabu : Paletter GUImazing is the work of Ethan Da Broski and Tiny Tom

cmake --build . 
in build/Deslktop

# TODO
- CUDA:
  - Use to run K-Means for palette generation
  - Use to apply palette to second image
- Add Modes to apply:
  - Added dropdown but does nothing right now.
  - Applying should work on CPU even when we have CUDA and right now, no worko whatsoever...
- color sorting options
- Export:
  - Json ?
  - Export the image with the palette applied
- put quantizer on a worker thread :)

...
